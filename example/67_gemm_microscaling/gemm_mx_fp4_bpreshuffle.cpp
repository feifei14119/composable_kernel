// SPDX-License-Identifier: MIT
// Copyright (c) 2025, Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cstdlib>

#include "ck/ck.hpp"
#include "ck/library/utility/literals.hpp"
#include "ck/tensor_operation/gpu/device/tensor_layout.hpp"
#include "ck/tensor_operation/gpu/element/unary_element_wise_operation.hpp"
#include "ck/tensor_operation/gpu/device/gemm_specialization.hpp"
#include "ck/tensor_operation/gpu/device/impl/device_gemm_xdl_cshuffle_v3_mx_b_preshuffle.hpp"
#include "ck/library/utility/host_tensor_generator.hpp"
#include "ck/utility/blkgemmpipe_scheduler.hpp"
#include "ck/utility/data_type.hpp"
#include "ck/utility/sequence.hpp"
#include "ck/library/reference_tensor_operation/cpu/reference_mx_gemm.hpp"
#include "ck/library/utility/check_err.hpp"
#include "ck/library/utility/device_memory.hpp"
#include "ck/library/utility/fill.hpp"
#include "ck/library/utility/host_tensor.hpp"

template <ck::index_t... Is>
using S = ck::Sequence<Is...>;

using F4        = ck::f4x2_pk_t;
using F4PK      = ck::f4x2_pk_t;
using F16       = ck::half_t;
using BF16      = ck::bhalf_t;
using F32       = float;
using XDataType = ck::e8m0_bexp_t;
using F8        = ck::e8m0_bexp_t;

using Row = ck::tensor_layout::gemm::RowMajor;
using Col = ck::tensor_layout::gemm::ColumnMajor;

using A0DataType       = F4;
using A1DataType       = XDataType;
using B0DataType       = F4;
using B1DataType       = XDataType;
using AccDataType      = F32;
using DsDataType       = ck::Tuple<>;
using CDataType        = BF16;
using CShuffleDataType = CDataType;

using A0Layout = Row;
using B0Layout = Col;
using CLayout  = Row;

void preShuffleBuffer(const F4* src, F4* dst, int N, int K, int NXdl)
{
    int KPack = 32;
    int NLane = NXdl;
    int KLane = 64 / NLane;

    int K0 = K / (KLane * KPack);
    // K -> K0 KLane KPack
    // N -> N0 NLane
    // N, K -> N0 K0 KLane NLane KPack
    int tempk;
    for(int n = 0; n < N; ++n)
    {
        for(int k = 0; k < K; ++k)
        {
            int n0 = n / NLane;
            int n1 = n % NLane;

            int k0 = k / (KLane * KPack);
            tempk  = k % (KLane * KPack);
            int k1 = tempk / KPack;
            int k2 = tempk % KPack;

            int outputIndex = n0 * KPack * NLane * KLane * K0 + k0 * KPack * NLane * KLane +
                              k1 * KPack * NLane + n1 * KPack + k2;

            dst[outputIndex / 2] = src[(n * K + k) / 2];
        }
    }
}

using PassThrough = ck::tensor_operation::element_wise::PassThrough;

using AElementOp = PassThrough; // elementwise transformation for A matrix
using BElementOp = PassThrough; // elementwise transformation for B matrix
using CElementOp = PassThrough; // elementwise transformation for C matrix

constexpr ck::index_t ScaleBlockSize = 32; // scaling block size

constexpr auto GemmSpec = ck::tensor_operation::device::GemmSpecialization::Default;

// clang-format off
using DeviceOpInstance = ck::tensor_operation::device::DeviceGemmMX_Xdl_CShuffleV3_BPreShuffle<
    A0Layout,    B0Layout,    CLayout,          
    A0DataType,  A1DataType,  B0DataType,   B1DataType,   CDataType,    AccDataType,  CShuffleDataType, 
    AElementOp, BElementOp, CElementOp,  GemmSpec,         
    ScaleBlockSize,   256,   
    128,  128,   128,        // MNK PRER BLK
    32,    32,               // BUFFLER LOAD ELEMENT
    16,    16,               // MFMA
    8,     2,                // MN WAVE REPEATE <- TILE / WAVE PATT
    // k_blk/32, 256/k0,         order                   lds
    S<4, 64, 1>,   S<1, 0, 2>,   S<1, 0, 2>,   2,   32,   32,   0,            
    S<4, 64, 1>,   S<1, 0, 2>,   S<1, 0, 2>,   2,   32,   32,   0,            
    // M RPT, N RPT, <1, M, 1, N> THREAD 
    2,   1,   S<1, 32, 1, 8>,  8,                
    ck::BlockGemmPipelineScheduler::Intrawave, ck::BlockGemmPipelineVersion::v3, A0DataType, B0DataType>;
// clang-format on

int main(int argc, char* argv[])
{
    bool do_verification = true;
    int init_method      = 1;
    bool time_kernel     = false;
    bool flush_cache     = false;

    // GEMM shape
    ck::index_t M = 4096;
    ck::index_t N = 4096;
    ck::index_t K = 4096;

    ck::index_t StrideA = K;
    ck::index_t StrideB = K;
    ck::index_t StrideC = N;

    if(argc == 1)
    {
        // use default case
    }
    else if(argc == 4)
    {
        do_verification = std::stoi(argv[1]);
        init_method     = std::stoi(argv[2]);
        time_kernel     = std::stoi(argv[3]);
    }
    else if(argc == 8)
    {
        do_verification = std::stoi(argv[1]);
        init_method     = std::stoi(argv[2]);
        time_kernel     = std::stoi(argv[3]);

        M = std::stoi(argv[4]);
        N = std::stoi(argv[5]);
        K = std::stoi(argv[6]);

        flush_cache = std::stoi(argv[7]);

        StrideA = K;
        StrideB = K;
        StrideC = N;
    }
    else
    {
        printf("arg1: verification (0=no, 1=yes)\n");
        printf("arg2: initialization (0=no init, 1=integer value, 2=decimal value)\n");
        printf("arg3: time kernel (0=no, 1=yes)\n");
        printf("arg4 to 6: M, N, K\n");
        printf("arg7: flush both I$ and L2$ (0=no, 1=yes)\n");
        exit(0);
    }

    ck::index_t Scale_Stride_AM = (K + ScaleBlockSize - 1) / ScaleBlockSize;
    ck::index_t Scale_Stride_BN = (K + ScaleBlockSize - 1) / ScaleBlockSize;

    auto f_host_tensor_descriptor =
        [](std::size_t row, std::size_t col, std::size_t stride, auto layout) {
            using namespace ck::literals;

            if(std::is_same<decltype(layout), ck::tensor_layout::gemm::RowMajor>::value)
            {
                return HostTensorDescriptor({row, col}, {stride, 1_uz});
            }
            else
            {
                return HostTensorDescriptor({row, col}, {1_uz, stride});
            }
        };

    Tensor<A0DataType> a_m_k(f_host_tensor_descriptor(M, K, StrideA, A0Layout{}));
    Tensor<A1DataType> a_m_k_scale(f_host_tensor_descriptor(
        M, (K + ScaleBlockSize - 1) / ScaleBlockSize, Scale_Stride_AM, A0Layout{}));
    Tensor<B0DataType> b_k_n(f_host_tensor_descriptor(K, N, StrideB, B0Layout{}));
    Tensor<B0DataType> b_preshuffled(f_host_tensor_descriptor(K, N, StrideB, B0Layout{}));
    Tensor<B1DataType> b_k_n_scale(f_host_tensor_descriptor(
        (K + ScaleBlockSize - 1) / ScaleBlockSize, N, Scale_Stride_BN, B0Layout{}));
    Tensor<CDataType> c_m_n_host_result(f_host_tensor_descriptor(M, N, StrideC, CLayout{}));
    Tensor<CDataType> c_m_n_device_result(f_host_tensor_descriptor(M, N, StrideC, CLayout{}));

    std::cout << "a_m_k: " << a_m_k.mDesc << std::endl;
    std::cout << "a_m_k_scale: " << a_m_k_scale.mDesc << std::endl;
    std::cout << "b_k_n: " << b_k_n.mDesc << std::endl;
    std::cout << "b_k_n_scale: " << b_k_n_scale.mDesc << std::endl;
    std::cout << "e_m_n: " << c_m_n_host_result.mDesc << std::endl;

    switch(init_method)
    {
    case 0: break;
    case 1:
        a_m_k.GenerateTensorValue(GeneratorTensor_2<A0DataType>{-2, 2});
        b_k_n.GenerateTensorValue(GeneratorTensor_2<B0DataType>{-2, 2});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_3<A1DataType>{0, 1.0});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_3<B1DataType>{0, 1.0});
        break;
    case 2:
        a_m_k.GenerateTensorValue(GeneratorTensor_1<A0DataType>{});
        b_k_n.GenerateTensorValue(GeneratorTensor_1<B0DataType>{});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_1<A1DataType>{});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_1<B1DataType>{});
        break;
    case 3:
        a_m_k.GenerateTensorValue(GeneratorTensor_2<A0DataType>{-2, 2});
        b_k_n.GenerateTensorValue(GeneratorTensor_2<B0DataType>{-2, 2});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_1<A1DataType>{});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_1<B1DataType>{});
        break;
    case 4:
        a_m_k.GenerateTensorValue(GeneratorTensor_1<A0DataType>{});
        b_k_n.GenerateTensorValue(GeneratorTensor_1<B0DataType>{});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_3<A1DataType>{0, 1.0});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_3<B1DataType>{0, 1.0});
        break;
    case 5:
        a_m_k.GenerateTensorValue(GeneratorTensor_1<A0DataType>{});
        b_k_n.GenerateTensorValue(GeneratorTensor_1<B0DataType>{});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_1<A1DataType>{});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_3<B1DataType>{0, 1.0});
        break;
    case 6:
        a_m_k.GenerateTensorValue(GeneratorTensor_1<A0DataType>{});
        b_k_n.GenerateTensorValue(GeneratorTensor_1<B0DataType>{});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_3<A1DataType>{0, 1.0});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_1<B1DataType>{});
        break;
    default:
        a_m_k.GenerateTensorValue(GeneratorTensor_3<A0DataType>{-0.5, 0.5});
        b_k_n.GenerateTensorValue(GeneratorTensor_3<B0DataType>{-0.5, 0.5});
        a_m_k_scale.GenerateTensorValue(GeneratorTensor_3<A1DataType>{0, 1.0});
        b_k_n_scale.GenerateTensorValue(GeneratorTensor_3<B1DataType>{0, 1.0});
    }

    DeviceMem a_device_buf(sizeof(A0DataType) * a_m_k.mDesc.GetElementSpaceSize() / 2);
    DeviceMem a_scale_device_buf(sizeof(A1DataType) * a_m_k_scale.mDesc.GetElementSpaceSize());
    DeviceMem b_device_buf(sizeof(B0DataType) * b_k_n.mDesc.GetElementSpaceSize() / 2);
    DeviceMem b_scale_device_buf(sizeof(B1DataType) * b_k_n_scale.mDesc.GetElementSpaceSize());
    DeviceMem c_device_buf(sizeof(CDataType) * c_m_n_device_result.mDesc.GetElementSpaceSize());

    a_device_buf.ToDevice(a_m_k.mData.data());
    a_scale_device_buf.ToDevice(a_m_k_scale.mData.data());
    b_scale_device_buf.ToDevice(b_k_n_scale.mData.data());

    auto a_element_op   = AElementOp{};
    auto b_element_op   = BElementOp{};
    auto cde_element_op = CElementOp{};

    // do GEMM
    auto device_op = DeviceOpInstance{};
    int NPerXdl    = device_op.GetPreShuffleParameters();

    preShuffleBuffer(b_k_n.mData.data(), b_preshuffled.mData.data(), N, K, NPerXdl);
    b_device_buf.ToDevice(b_preshuffled.mData.data());

#ifdef FF_DBG
    ck::index_t blockSize = DeviceOpInstance::blockSize;
    ck::index_t grid_size_x, grid_size_y, grid_size_z;
    std::tie(grid_size_x, grid_size_y, grid_size_z) = DeviceOpInstance::GridwiseGemm::CalculateGridSize(M, N, 1);

    ck::index_t threadNumber = blockSize * grid_size_x * grid_size_y;
    Tensor<int>     dbg_i32(HostTensorDescriptor({threadNumber  * FF_DBG_CNT}));
    Tensor<float>   dbg_f32(HostTensorDescriptor({threadNumber  * FF_DBG_CNT}));
    Tensor<BF16>    dbg_f16(HostTensorDescriptor({threadNumber  * FF_DBG_CNT}));
    Tensor<F8>      dbg_f8(HostTensorDescriptor({threadNumber   * FF_DBG_CNT}));
    Tensor<F4PK>    dbg_f4pk(HostTensorDescriptor({threadNumber * FF_DBG_CNT}));

    DeviceMem dbg_i32_dev(sizeof(int)   * dbg_i32.mDesc.GetElementSpaceSize());
    DeviceMem dbg_f32_dev(sizeof(float) * dbg_f32.mDesc.GetElementSpaceSize());
    DeviceMem dbg_f16_dev(sizeof(BF16)  * dbg_f16.mDesc.GetElementSpaceSize());
    DeviceMem dbg_f8_dev(sizeof(F8)     * dbg_f8.mDesc.GetElementSpaceSize());
    DeviceMem dbg_f4pk_dev(sizeof(F4PK) * dbg_f4pk.mDesc.GetElementSpaceSize());

    dbg_i32_dev.ToDevice(dbg_i32.mData.data());
    dbg_f32_dev.ToDevice(dbg_f32.mData.data());
    dbg_f16_dev.ToDevice(dbg_f16.mData.data());
    dbg_f8_dev.ToDevice(dbg_f8.mData.data());
    dbg_f4pk_dev.ToDevice(dbg_f4pk.mData.data());
#endif    

    auto invoker = device_op.MakeInvoker();
    auto argument =
        device_op.MakeArgument(static_cast<A0DataType*>(a_device_buf.GetDeviceBuffer()),
                               static_cast<XDataType*>(a_scale_device_buf.GetDeviceBuffer()),
                               static_cast<B0DataType*>(b_device_buf.GetDeviceBuffer()),
                               static_cast<XDataType*>(b_scale_device_buf.GetDeviceBuffer()),
                               static_cast<CDataType*>(c_device_buf.GetDeviceBuffer()),
                               M,
                               N,
                               K,
                               StrideA,
                               Scale_Stride_AM,
                               StrideB,
                               Scale_Stride_BN,
                               StrideC,
                               1, // KBatch
                               a_element_op,
                               b_element_op,
                               cde_element_op,
                               // FF_DBG                            
                               static_cast<int*>(dbg_i32_dev.GetDeviceBuffer()),
                               static_cast<float*>(dbg_f32_dev.GetDeviceBuffer()),
                               static_cast<BF16*>(dbg_f16_dev.GetDeviceBuffer()),
                               static_cast<F8*>(dbg_f8_dev.GetDeviceBuffer()),
                               static_cast<F4PK*>(dbg_f4pk_dev.GetDeviceBuffer()));

    if(!device_op.IsSupportedArgument(argument))
    {
        throw std::runtime_error(
            "wrong! device_gemm with the specified compilation parameters does "
            "not support this GEMM problem");
    }

    std::size_t flop = std::size_t(2) * M * N * K + std::size_t(2) * M * N * K / ScaleBlockSize;
    std::size_t num_btype = sizeof(A0DataType) * M * K + sizeof(B0DataType) * K * N +
                            sizeof(CDataType) * M * N +
                            sizeof(XDataType) * (M * K + K * N) / ScaleBlockSize;

    float ave_time = .0;

    if(flush_cache)
    {
        int rotating_buf = (512 * 1024 * 1024 + num_btype - 1) / num_btype;

        ave_time = invoker.Run(argument,
                               StreamConfig{nullptr, time_kernel, 0, 50, 100, true, rotating_buf});
    }
    else
    {
        ave_time = invoker.Run(argument, StreamConfig{nullptr, time_kernel, 0, 50, 100});
    }

#if 0
    dbg_i32_dev.FromDevice(dbg_i32.mData.data());
    //dbg_f32_dev.FromDevice(dbg_f32.mData.data());
    //dbg_f8_dev.FromDevice(dbg_f8.mData.data());
    dbg_f4pk_dev.FromDevice(dbg_f4pk.mData.data());
    printf("[SHFF]   NPerXdl = %d\n", NPerXdl);
    printf("[FF]     M               = %4d, N               = %4d, K = %d\n", M, N, K);
    //printf("[FF]     scaleM          = %4d, scaleN          = %4d, scaleK = %d\n", scaleM, scaleN, scaleK);
    //printf("[FF]     shfflK          = %4d, shfflN          = %4d\n", shuffledK, shuffledN);
    printf("[ARG]    StrideA         = %4d, StrideB         = %4d\n", StrideA, StrideB);
    printf("[ARG]    Scale_Stride_AM = %4d, Scale_Stride_BN = %4d\n", Scale_Stride_AM, Scale_Stride_BN);
    printf("[ARG]    StrideC = %d\n", StrideC);
    //printf("[ARG]    KBatch = %d\n", KBatch);
    printf("[LAUNCH] blockSize = %d\n", blockSize);
    printf("[LAUNCH] grid_size_x = %d, grid_size_y = %d, grid_size_z = %d\n", grid_size_x, grid_size_y, grid_size_z);
    // a_pk_host
    {
        std::ofstream file("ff_a_pk_host.txt");
        int X = static_cast<int>(K);
        int Y = static_cast<int>(M);
        file << " [a_pk_host]: Row = " << Y << ", Col = " << X << std::endl;

        for(int y = 0; y < Y; y++)
        {
            file << "\n ========== row : [" << y << " / " << Y << "] ==========";
            for(int x = 0; x < X/2; x++)
            {
                int idx = X * y + x;

                if(idx % 32 == 0)
                {
                    file << "\n [" << x*2 << " : " << x*2 + 31 << " ]: ";
                }

                ck::f4x2_pk_t apk  = a_m_k.mData[idx];
                ck::f4_t a0 = apk.unpack(ck::Number<0>{});
                ck::f4_t a1 = apk.unpack(ck::Number<1>{});
                file << "[";
                file << ck::type_convert<float>(a0) << "; ";
                file << ck::type_convert<float>(a1) << "], ";
            }
        }

        file.close();
    }
    // dbg_i32
    {
        std::ofstream file("./ff_dbg_int.txt");
        file << " [dbg_i32]: Grid = [" << grid_size_x << ", " << grid_size_y << "], Block = " << blockSize << std::endl;

        for(int bidy = 0; bidy < grid_size_y; bidy++)
        {
            for(int bidx = 0; bidx < grid_size_x; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < blockSize; tid++)
                {
                    int gid = (blockSize * grid_size_x) * bidy + blockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < FF_DBG_CNT; i++)
                    {
                        file << dbg_i32.mData[gid * FF_DBG_CNT + i] << ", ";
                    }
                }
            }
        }

        file.close();
    }
    // dbg_f4pk
    {
        std::ofstream file("./ff_dbg_f4pk.txt");
        file << " [dbg_f4pk]: Grid = [" << grid_size_x << ", " << grid_size_y << "], Block = " << blockSize << std::endl;

        for(int bidy = 0; bidy < grid_size_y; bidy++)
        {
            for(int bidx = 0; bidx < grid_size_x; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < blockSize; tid++)
                {
                    int gid = (blockSize * grid_size_x) * bidy + blockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < FF_DBG_CNT; i++)
                    {
                        ck::f4x2_pk_t f4pk  = dbg_f4pk.mData[gid * FF_DBG_CNT + i];
                        ck::f4_t f40 = f4pk.unpack(ck::Number<0>{});
                        ck::f4_t f41 = f4pk.unpack(ck::Number<1>{});
                        file << "[";
                        file << ck::type_convert<float>(f40) << "; ";
                        file << ck::type_convert<float>(f41) << "], ";
                    }
                }
            }
        }

        file.close();
    }
#endif

    float tflops = static_cast<float>(flop) / 1.E9 / ave_time;

    float gb_per_sec = num_btype / 1.E6 / ave_time;

    std::cout << "Perf: " << ave_time << " ms, " << tflops << " TFlops, " << gb_per_sec << " GB/s, "
              << device_op.GetTypeString() << std::endl;

    if(do_verification)
    {
        using ReferenceGemmInstance = ck::tensor_operation::host::ReferenceMXGemm<A0DataType,
                                                                                  B0DataType,
                                                                                  CDataType,
                                                                                  AccDataType,
                                                                                  XDataType,
                                                                                  PassThrough,
                                                                                  PassThrough,
                                                                                  PassThrough,
                                                                                  float,
                                                                                  float>;
        auto ref_gemm               = ReferenceGemmInstance{};
        auto ref_invoker            = ref_gemm.MakeInvoker();

        auto ref_argument = ref_gemm.MakeArgument(a_m_k,
                                                  a_m_k_scale,
                                                  b_k_n,
                                                  b_k_n_scale,
                                                  c_m_n_host_result,
                                                  PassThrough{},
                                                  PassThrough{},
                                                  PassThrough{});

        ref_invoker.Run(ref_argument);

        c_device_buf.FromDevice(c_m_n_device_result.mData.data());

        bool pass =  ck::utils::check_err(
                   c_m_n_device_result, c_m_n_host_result, "Error: Incorrect results!", 5e-2, 5e-2)
                   ? 0
                   : 1;
        printf("\n%s\n", pass == 0 ? "PASS" : "FAILE");
    }

    return 0;
}
