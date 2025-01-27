// SPDX-License-Identifier: MIT
// Copyright (c) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#pragma once
#include "ck_tile/host.hpp"
template <typename Layout>
static constexpr inline auto is_row_major(Layout layout_)
{
    return ck_tile::bool_constant<std::is_same_v<ck_tile::remove_cvref_t<decltype(layout_)>,
                                                 ck_tile::tensor_layout::gemm::RowMajor>>{};
}

// mfma_type, 0:32x32, 1:16x16
template <typename T>
auto shuffle_b(const ck_tile::HostTensor<T>& t, std::string mfma_dtype, int mfma_type = 0)
{
    assert(t.get_lengths().size() == 2);
    int n_ = t.get_lengths()[0];
    int k_ = t.get_lengths()[1];
    printf("[FF] shuffle_b: mfma_dtype = %s, mfma_type = %d, n_ = %d, k_ = %d\n", mfma_dtype.c_str(), mfma_type, n_, k_);
    if((mfma_dtype == "bf16" || mfma_dtype == "fp16") && mfma_type == 0)
    {
        ck_tile::HostTensor<T> t_view({n_ / 32, 32, k_ / 16, 2, 8});
        std::copy(t.begin(), t.end(), t_view.begin());
        return ck_tile::reference_permute(t_view, {0, 2, 3, 1, 4});
    }
    else if((mfma_dtype == "bf16" || mfma_dtype == "fp16") && mfma_type == 1)
    {
        ck_tile::HostTensor<T> t_view({n_ / 16, 16, k_ / 32, 4, 8});
        std::copy(t.begin(), t.end(), t_view.begin());
        return ck_tile::reference_permute(t_view, {0, 2, 3, 1, 4});
    }
    else if((mfma_dtype == "int8" || mfma_dtype == "fp8") && mfma_type == 0)
    {
        ck_tile::HostTensor<T> t_view({n_ / 32, 32, k_ / 32, 2, 16});
        std::copy(t.begin(), t.end(), t_view.begin());
        return ck_tile::reference_permute(t_view, {0, 2, 3, 1, 4});
    }
    else if((mfma_dtype == "int8" || mfma_dtype == "fp8") && mfma_type == 1)
    {
        ck_tile::HostTensor<T> t_view({n_ / 16, 16, k_ / 64, 4, 16});
        std::copy(t.begin(), t.end(), t_view.begin());
        return ck_tile::reference_permute(t_view, {0, 2, 3, 1, 4});
    }
    return t;
}

auto calculate_rtol_atol(const ck_tile::index_t K,
                         const ck_tile::index_t kbatch,
                         const float max_accumulated_value)
{
    using ComputeType =
        std::conditional_t<sizeof(ADataType) < sizeof(BDataType), ADataType, BDataType>;
    // Calculate thresholds
    const auto rtol = ck_tile::get_relative_threshold<ComputeType, CDataType, AccDataType>(
        ck_tile::integer_divide_ceil(K, kbatch));
    const auto atol = ck_tile::get_absolute_threshold<ComputeType, CDataType, AccDataType>(
        max_accumulated_value / kbatch, ck_tile::integer_divide_ceil(K, kbatch));
    // Calculate error due to split_k accumulation
    const auto rtol_split_k =
        ck_tile::get_relative_threshold<CDataType, CDataType, CDataType>(kbatch);
    const auto atol_split_k = ck_tile::get_absolute_threshold<CDataType, CDataType, CDataType>(
        max_accumulated_value, kbatch);
    // Use higher threshold
    return ck_tile::make_tuple(std::max(rtol, rtol_split_k), std::max(atol, atol_split_k));
}

template <typename ALayout, typename BLayout, typename CLayout>
float invoke_gemm(ck_tile::DeviceMem& a_m_k_dev_buf,
                  ck_tile::DeviceMem& b_k_n_dev_buf,
                  ck_tile::DeviceMem& b_shuffle_dev_buf,
                  ck_tile::DeviceMem& c_m_n_dev_buf,
                  ck_tile::index_t M,
                  ck_tile::index_t N,
                  ck_tile::index_t K,
                  ck_tile::index_t stride_A,
                  ck_tile::index_t stride_B,
                  ck_tile::index_t stride_C,
                  ck_tile::index_t kbatch,
                  int n_warmup,
                  int n_repeat,
                  ck_tile::DeviceMem& dbg_int_buf,
                  ck_tile::DeviceMem& dbg_fp32_buf,
                  ck_tile::DeviceMem& dbg_f168_buf)
{
    ck_tile::GemmHostArgs args;
    args.a_ptr    = a_m_k_dev_buf.GetDeviceBuffer();
    args.b_ptr    = b_k_n_dev_buf.GetDeviceBuffer();
    args.c_ptr    = c_m_n_dev_buf.GetDeviceBuffer();
    args.k_batch  = kbatch;
    args.M        = M;
    args.N        = N;
    args.K        = K;
    args.stride_A = stride_A;
    args.stride_B = stride_B;
    args.stride_C = stride_C;
    args.b_shuffle_ptr  = b_shuffle_dev_buf.GetDeviceBuffer();
    args.dbg_int_ptr    = dbg_int_buf.GetDeviceBuffer();
    args.dbg_fp32_ptr   = dbg_fp32_buf.GetDeviceBuffer();
    args.dbg_f168_ptr   = dbg_f168_buf.GetDeviceBuffer();

    printf("[FF] --- invoke_gemm: ---\n");
    printf("[FF] args.M = %d\n", static_cast<int>(args.M));
    printf("[FF] args.N = %d\n", static_cast<int>(args.N));
    printf("[FF] args.K = %d\n", static_cast<int>(args.K));
    printf("[FF] args.stride_A = %d\n", static_cast<int>(args.stride_A));
    printf("[FF] args.stride_B = %d\n", static_cast<int>(args.stride_B));
    printf("[FF] args.stride_C = %d\n", static_cast<int>(args.stride_C));
    printf("[FF] args.k_batch = %d\n", static_cast<int>(args.k_batch));

    float ave_time = gemm_calc<ALayout, BLayout, CLayout>(args, ck_tile::stream_config{nullptr, true, 1, n_warmup, n_repeat});

    std::size_t flop = std::size_t(2) * M * N * K;
    std::size_t num_byte =
        sizeof(ADataType) * M * K + sizeof(BDataType) * N * K + sizeof(CDataType) * M * N;
    float tflops     = static_cast<float>(flop) / 1.E9 / ave_time;
    float gb_per_sec = num_byte / 1.E6 / ave_time;

    std::cout << "Run Gemm kernel with M =" << M << " N =" << N << " K =" << K
              << " StrideA =" << stride_A << " StrideB =" << stride_B << " StrideC =" << stride_C
              << " : " << ave_time << " ms, " << tflops << " TFlops, " << gb_per_sec << " GB/s, "
              << std::endl;

    return ave_time;
}

template <typename ALayout, typename BLayout, typename CLayout>
int run_gemm_example_with_layouts(int argc,
                                  char* argv[],
                                  const ALayout a_layout                  = ALayout{},
                                  const BLayout b_layout                  = BLayout{},
                                  [[maybe_unused]] const CLayout c_layout = CLayout{})
{
    auto [result, arg_parser] = create_args(argc, argv);
    if(!result)
        return -1;

    ck_tile::index_t M = arg_parser.get_int("m");
    ck_tile::index_t N = arg_parser.get_int("n");
    ck_tile::index_t K = arg_parser.get_int("k");

    ck_tile::index_t stride_A = arg_parser.get_int("stride_a");
    ck_tile::index_t stride_B = arg_parser.get_int("stride_b");
    ck_tile::index_t stride_C = arg_parser.get_int("stride_c");

    ck_tile::index_t kbatch = arg_parser.get_int("split_k");
    int n_warmup            = arg_parser.get_int("warmup");
    int n_repeat            = arg_parser.get_int("repeat");

    stride_A = ck_tile::get_default_stride(M, K, stride_A, is_row_major(a_layout));
    stride_B = ck_tile::get_default_stride(K, N, stride_B, is_row_major(b_layout));
    stride_C = ck_tile::get_default_stride(M, N, stride_C, is_row_major(CLayout{}));

    ck_tile::HostTensor<ADataType> a_m_k(ck_tile::host_tensor_descriptor(M, K, stride_A, is_row_major(a_layout)));
    ck_tile::HostTensor<BDataType> b_k_n(ck_tile::host_tensor_descriptor(K, N, stride_B, is_row_major(b_layout)));
    ck_tile::HostTensor<CDataType> c_m_n_dev_result(ck_tile::host_tensor_descriptor(M, N, stride_C, is_row_major(CLayout{})));

    // debug
    ck_tile::HostTensor<int> dbg_int({M*N*64});
    ck_tile::HostTensor<float> dbg_fp32({M*N*64});
    ck_tile::HostTensor<ADataType> dbg_f168({M*N*64});

    // TODO: add different init types
    ck_tile::FillUniformDistribution<ADataType>{-5.f, 5.f}(a_m_k);
    ck_tile::FillUniformDistribution<BDataType>{-5.f, 5.f}(b_k_n);

    // do pre-shuffle
    std::string mfma  = arg_parser.get_str("prec");
    ck_tile::HostTensor<BDataType> b_shuffle_host = shuffle_b(b_k_n, mfma, 1);

    ck_tile::DeviceMem a_m_k_dev_buf(a_m_k.get_element_space_size_in_bytes());
    ck_tile::DeviceMem b_k_n_dev_buf(b_k_n.get_element_space_size_in_bytes());
    ck_tile::DeviceMem b_shuffle_dev_buf(b_shuffle_host.get_element_space_size_in_bytes());
    ck_tile::DeviceMem c_m_n_dev_buf(c_m_n_dev_result.get_element_space_size_in_bytes());

    ck_tile::DeviceMem dbg_int_buf(dbg_int.get_element_space_size_in_bytes());
    ck_tile::DeviceMem dbg_fp32_buf(dbg_fp32.get_element_space_size_in_bytes());
    ck_tile::DeviceMem dbg_f168_buf(dbg_f168.get_element_space_size_in_bytes());

    a_m_k_dev_buf.ToDevice(a_m_k.data());
    b_k_n_dev_buf.ToDevice(b_k_n.data());
    b_shuffle_dev_buf.ToDevice(b_shuffle_host.data());
    c_m_n_dev_buf.SetZero();
    c_m_n_dev_result.SetZero();

    invoke_gemm<ALayout, BLayout, CLayout>(a_m_k_dev_buf,
                                           b_k_n_dev_buf,
                                           b_shuffle_dev_buf,
                                           c_m_n_dev_buf,
                                           M,
                                           N,
                                           K,
                                           stride_A,
                                           stride_B,
                                           stride_C,
                                           kbatch,
                                           n_warmup,
                                           n_repeat,                                           
                                           dbg_int_buf,
                                           dbg_fp32_buf,
                                           dbg_f168_buf);

    c_m_n_dev_buf.FromDevice(c_m_n_dev_result.data());
    bool pass = true;

    if(arg_parser.get_int("v") == 1)
    {
        ck_tile::HostTensor<CDataType> c_m_n_host_ref(
            ck_tile::host_tensor_descriptor(M, N, stride_C, is_row_major(CLayout{})));
        c_m_n_host_ref.SetZero();

        ck_tile::reference_gemm<ADataType, BDataType, AccDataType, CDataType>(
            a_m_k, b_k_n, c_m_n_host_ref);
        const float max_accumulated_value =
            *std::max_element(c_m_n_host_ref.mData.begin(), c_m_n_host_ref.mData.end());
        const auto rtol_atol = calculate_rtol_atol(K, kbatch, max_accumulated_value);
        pass                 = ck_tile::check_err(c_m_n_dev_result,
                                  c_m_n_host_ref,
                                  "Error: Incorrect results!",
                                  rtol_atol.at(ck_tile::number<0>{}),
                                  rtol_atol.at(ck_tile::number<1>{}));

        std::cout << "Relative error threshold: " << rtol_atol.at(ck_tile::number<0>{})
                  << " Absolute error threshold: " << rtol_atol.at(ck_tile::number<1>{})
                  << std::endl;
        std::cout << "The CPU veification result is:" << (pass ? "correct" : "fail") << std::endl;
#if 1
        // c_ref
        {
            std::ofstream file("ff_c_cpu_ref.txt");
            int X = static_cast<int>(N);
            int Y = static_cast<int>(M);
            file << " [c_cpu_ref]: Row = " << Y << ", Col = " << X << std::endl;

            for(int y = 0; y < Y; y++)
            {
                file << "\n ========== row : [" << y << " / " << Y << "] ==========";
                for(int x = 0; x < X; x++)
                {
                    if(x % 64 == 0)
                    {
                        file << "\n [" << x << " : " << x + 63 << "]: ";
                    }
                    int idx = X * y + x;
                    file << ck_tile::type_convert<float>(c_m_n_host_ref.mData[idx]) << ", ";
                }
            }

            file.close();
        }
#endif        
    }
    else if(arg_parser.get_int("v") == 2)
    {
        ck_tile::HostTensor<CDataType> c_m_n_gpu_ref(
            ck_tile::host_tensor_descriptor(M, N, stride_C, is_row_major(CLayout{})));
        ck_tile::DeviceMem c_m_n_gpu_buf_ref(c_m_n_gpu_ref.get_element_space_size_in_bytes());
        c_m_n_gpu_ref.SetZero();
        c_m_n_gpu_buf_ref.SetZero();

        ADataType* d_A;
        BDataType* d_B;
        CDataType* d_C;

        ck_tile::hip_check_error(hipMalloc(&d_A, M * K * sizeof(ADataType)));
        ck_tile::hip_check_error(hipMalloc(&d_B, N * K * sizeof(BDataType)));
        ck_tile::hip_check_error(hipMalloc(&d_C, M * N * sizeof(CDataType)));

        ck_tile::hip_check_error(hipMemcpy(d_A,
                                           a_m_k_dev_buf.GetDeviceBuffer(),
                                           M * K * sizeof(ADataType),
                                           hipMemcpyHostToDevice));
        ck_tile::hip_check_error(hipMemcpy(d_B,
                                           b_k_n_dev_buf.GetDeviceBuffer(),
                                           N * K * sizeof(BDataType),
                                           hipMemcpyHostToDevice));

        ck_tile::reference_gemm_gpu<ADataType,
                                    BDataType,
                                    AccDataType,
                                    CDataType,
                                    ALayout,
                                    BLayout,
                                    CLayout>(d_A, d_B, d_C, M, N, K, stride_A, stride_B, stride_C);

        ck_tile::hip_check_error(hipMemcpy(c_m_n_gpu_buf_ref.GetDeviceBuffer(),
                                           d_C,
                                           M * N * sizeof(CDataType),
                                           hipMemcpyDeviceToHost));

        ck_tile::hip_check_error(hipFree(d_A));
        ck_tile::hip_check_error(hipFree(d_B));
        ck_tile::hip_check_error(hipFree(d_C));

        c_m_n_gpu_buf_ref.FromDevice(c_m_n_gpu_ref.data());
        const float max_accumulated_value =
            *std::max_element(c_m_n_gpu_ref.mData.begin(), c_m_n_gpu_ref.mData.end());
        const auto rtol_atol = calculate_rtol_atol(K, kbatch, max_accumulated_value);
        pass                 = ck_tile::check_err(c_m_n_dev_result,
                                  c_m_n_gpu_ref,
                                  "Error: Incorrect results!",
                                  rtol_atol.at(ck_tile::number<0>{}),
                                  rtol_atol.at(ck_tile::number<1>{}));

        std::cout << "Relative error threshold: " << rtol_atol.at(ck_tile::number<0>{})
                  << " Absolute error threshold: " << rtol_atol.at(ck_tile::number<1>{})
                  << std::endl;
        std::cout << "The GPU veification result is: " << (pass ? "correct" : "fail") << std::endl;
#if 1
        // c_ref
        {
            std::ofstream file("ff_c_gpu_ref.txt");
            int X = static_cast<int>(N);
            int Y = static_cast<int>(M);
            file << " [c_gpu_ref]: Row = " << Y << ", Col = " << X << std::endl;

            for(int y = 0; y < Y; y++)
            {
                file << "\n ========== row : [" << y << " / " << Y << "] ==========";
                for(int x = 0; x < X; x++)
                {
                    if(x % 64 == 0)
                    {
                        file << "\n [" << x << " : " << x + 63 << "]: ";
                    }
                    int idx = X * y + x;
                    file << ck_tile::type_convert<float>(c_m_n_gpu_ref.mData[idx]) << ", ";
                }
            }

            file.close();
        }
#endif 
    }

#if 1
    int GridDimX  = 1;
    int GridDimY  = 1;
    int BlockDimX = 64;
    int BlockDimY = 4;
    int DbgCnt = 64;
    int BlockSize = BlockDimX * BlockDimY;
    // a_host
    {
        std::ofstream file("ff_a_host.txt");
        int X = static_cast<int>(K);
        int Y = static_cast<int>(M);
        file << " [a_host]: Row = " << Y << ", Col = " << X << std::endl;

        for(int y = 0; y < Y; y++)
        {
            file << "\n ========== row : [" << y << " / " << Y << "] ==========";
            for(int x = 0; x < X; x++)
            {
                int idx = X * y + x;
                if(idx % 16 == 0)
                {
                    file << "\n [" << x << " : " << x + 15 << " ]: ";
                }

                file << ck_tile::type_convert<float>(a_m_k.mData[idx]) << ", ";
            }
        }

        file.close();
    }
    // b_host
    {
        std::ofstream file("ff_b_host.txt");
        int X = static_cast<int>(K);
        int Y = static_cast<int>(N);
        file << " [b_host]: Row = " << Y << ", Col = " << X << std::endl;

        for(int y = 0; y < Y; y++)
        {
            file << "\n ========== row : [" << y << " / " << Y << "] ==========";
            for(int x = 0; x < X; x++)
            {
                int idx = X * y + x;
                if(idx % 16 == 0)
                {
                    file << "\n [" << x << " : " << x + 15 << " ]: ";
                }

                file << ck_tile::type_convert<float>(b_k_n.mData[idx]) << ", ";
            }
        }

        file.close();
    }
    // b_shuffle
    {
        std::ofstream file("ff_b_shuffle_host.txt");
        int X = static_cast<int>(K);
        int Y = static_cast<int>(N);
        file << " [b_shuffle_host]: Row = " << Y << ", Col = " << X << std::endl;

        for(int y = 0; y < Y; y++)
        {
            file << "\n ========== row : [" << y << " / " << Y << "] ==========";
            for(int x = 0; x < X; x++)
            {
                int idx = X * y + x;
                if(idx % 16 == 0)
                {
                    file << "\n [" << x << " : " << x + 15 << " ]: ";
                }

                file << ck_tile::type_convert<float>(b_shuffle_host.mData[idx]) << ", ";
            }
        }

        file.close(); 
    }
    // c_dev ---> kernel
    {
        auto c_dev = c_m_n_dev_buf.ToHost<CDataType>();
        std::ofstream file("ff_c_dev_kernel.txt");
        file << " [c_dev]: Grid = [" << GridDimX << ", " << GridDimY << "], Block = " << BlockSize
             << std::endl;

        for(int bidy = 0; bidy < GridDimY; bidy++)
        {
            for(int bidx = 0; bidx < GridDimX; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < BlockSize; tid++)
                {
                    int gid = (BlockSize * GridDimX) * bidy + BlockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < DbgCnt; i++) // multi output per thread
                        file << ck_tile::type_convert<float>(c_dev.mData[gid * DbgCnt + i]) << ", ";
                }
            }
        }

        file.close();
    }
    // c_dev
    {
        //auto d_dev = d_buf.ToHost<float>();
        auto c_dev = c_m_n_dev_buf.ToHost<CDataType>();
        std::ofstream file("ff_c_dev.txt");
        int X = static_cast<int>(N);
        int Y = static_cast<int>(M);
        file << " [c_dev]: Row = " << Y << ", Col = " << X << std::endl;

        for(int y = 0; y < Y; y++)
        {
            file << "\n ========== row : [" << y << " / " << Y << "] ==========";
            for(int x = 0; x < X; x++)
            {
                if(x % 64 == 0)
                {
                    file << "\n [" << x << " : " << x + 63 << "]: ";
                }
                int idx = X * y + x;
                file << ck_tile::type_convert<float>(c_dev.mData[idx]) << ", ";
            }
        }

        file.close();
    }
    // dbg_int ---> kernel
    {
        auto dbg_int_dev = dbg_int_buf.ToHost<int>();
        std::ofstream file("ff_dbg_int_kernel.txt");
        file << " [dbg_int]: Grid = [" << GridDimX << ", " << GridDimY << "], Block = " << BlockSize
             << std::endl;

        for(int bidy = 0; bidy < GridDimY; bidy++)
        {
            for(int bidx = 0; bidx < GridDimX; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < BlockSize; tid++)
                {
                    int gid = (BlockSize * GridDimX) * bidy + BlockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < DbgCnt; i++)
                        file << ck_tile::type_convert<int>(dbg_int_dev.mData[gid * DbgCnt + i]) << ", ";
                }
            }
        }

        file.close();
    }
    // dbg_int
    {
        auto dbg_int_dev = dbg_int_buf.ToHost<int>();
        std::ofstream file("ff_dbg_int.txt");
        int X = static_cast<int>(N);
        int Y = static_cast<int>(M);
        file << " [dbg_int]: Row = " << Y << ", Col = " << X << std::endl;

        for(int m = 0; m < Y; m++)
        {
            file << "\n ========== row : [" << m << " / " << Y << "] ==========";
            for(int n = 0; n < X; n++)
            {
                if(n % 64 == 0)
                {
                    file << "\n [" << n << " : " << n + 63 << "]: ";
                }
                int idx = X * m + n;
                file << ck_tile::type_convert<int>(dbg_int_dev.mData[idx]) << ", ";
            }
        }

        file.close();
    }
    // dbg_fp32 ---> kernel
    {
        auto dbg_fp32_dev = dbg_fp32_buf.ToHost<float>();
        std::ofstream file("ff_dbg_fp32_kernel.txt");
        file << " [dbg_fp32]: Grid = [" << GridDimX << ", " << GridDimY
             << "], Block = " << BlockSize << std::endl;

        for(int bidy = 0; bidy < GridDimY; bidy++)
        {
            for(int bidx = 0; bidx < GridDimX; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < BlockSize; tid++)
                {
                    int gid = (BlockSize * GridDimX) * bidy + BlockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < DbgCnt; i++)
                        file << ck_tile::type_convert<float>(dbg_fp32_dev.mData[gid * DbgCnt + i]) << ", ";
                }
            }
        }

        file.close();
    }
    // dbg_fp32
    {
        auto dbg_fp32_dev = dbg_fp32_buf.ToHost<float>();
        std::ofstream file("ff_dbg_fp32.txt");
        int X = static_cast<int>(N);
        int Y = static_cast<int>(M);
        file << " [dbg_fp32]: Row = " << Y << ", Col = " << X << std::endl;

        for(int m = 0; m < Y; m++)
        {
            file << "\n ========== row : [" << m << " / " << Y << "] ==========";
            for(int n = 0; n < X; n++)
            {
                if(n % 64 == 0)
                {
                    file << "\n [" << n << " : " << n + 63 << "]: ";
                }
                int idx = X * m + n;
                file << ck_tile::type_convert<float>(dbg_fp32_dev.mData[idx]) << ", ";
            }
        }

        file.close();
    }
    // dbg_fp16 ---> kernel
    {
        auto dbg_fp16_dev = dbg_f168_buf.ToHost<ck_tile::half_t>();
        std::ofstream file("ff_dbg_fp16_kernel.txt");
        file << " [dbg_fp16]: Grid = [" << GridDimX << ", " << GridDimY
             << "], Block = " << BlockSize << std::endl;

        for(int bidy = 0; bidy < GridDimY; bidy++)
        {
            for(int bidx = 0; bidx < GridDimX; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < BlockSize; tid++)
                {
                    int gid = (BlockSize * GridDimX) * bidy + BlockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < DbgCnt; i++)
                        file << ck_tile::type_convert<float>(dbg_fp16_dev.mData[gid * DbgCnt + i]) << ", ";
                }
            }
        }

        file.close();
    }
    // dbg_fp16
    {
        auto dbg_fp16_dev = dbg_f168_buf.ToHost<ck_tile::half_t>();
        std::ofstream file("ff_dbg_fp16.txt");
        int X = static_cast<int>(N);
        int Y = static_cast<int>(M);
        file << " [dbg_fp16]: Row = " << Y << ", Col = " << X << std::endl;

        for(int m = 0; m < Y; m++)
        {
            file << "\n ========== row : [" << m << " / " << Y << "] ==========";
            for(int n = 0; n < X; n++)
            {
                if(n % 64 == 0)
                {
                    file << "\n [" << n << " : " << n + 63 << "]: ";
                }
                int idx = X * m + n;
                file << ck_tile::type_convert<float>(dbg_fp16_dev.mData[idx]) << ", ";
            }
        }

        file.close();
    }
    // dbg_fp8 ---> kernel
    {
        auto dbg_fp8_dev = dbg_f168_buf.ToHost<ck_tile::fp8_t>();
        std::ofstream file("ff_dbg_fp8_kernel.txt");
        file << " [dbg_fp8]: Grid = [" << GridDimX << ", " << GridDimY
             << "], Block = " << BlockSize << std::endl;

        for(int bidy = 0; bidy < GridDimY; bidy++)
        {
            for(int bidx = 0; bidx < GridDimX; bidx++)
            {
                file << "\n ========== block : [" << bidx << ", " << bidy << "] ==========";
                for(int tid = 0; tid < BlockSize; tid++)
                {
                    int gid = (BlockSize * GridDimX) * bidy + BlockSize * bidx + tid;

                    file << "\n [" << tid << "]: ";
                    for(int i = 0; i < DbgCnt; i++)
                        file << ck_tile::type_convert<float>(dbg_fp8_dev.mData[gid * DbgCnt + i]) << ", ";
                }
            }
        }

        file.close();
    }
    // dbg_fp8
    {
        auto dbg_fp8_dev = dbg_f168_buf.ToHost<ck_tile::fp8_t>();
        std::ofstream file("ff_dbg_fp8.txt");
        int X = static_cast<int>(N);
        int Y = static_cast<int>(M);
        file << " [dbg_fp8]: Row = " << Y << ", Col = " << X << std::endl;

        for(int m = 0; m < Y; m++)
        {
            file << "\n ========== row : [" << m << " / " << Y << "] ==========";
            for(int n = 0; n < X; n++)
            {
                if(n % 64 == 0)
                {
                    file << "\n [" << n << " : " << n + 63 << "]: ";
                }
                int idx = X * m + n;
                file << ck_tile::type_convert<float>(dbg_fp8_dev.mData[idx]) << ", ";
            }
        }

        file.close();
    }
#endif

    return pass;
}

int run_gemm_example(int argc, char* argv[])
{
    auto [result, arg_parser] = create_args(argc, argv);
    if(!result)
        return -1;

    using Row = ck_tile::tensor_layout::gemm::RowMajor;
    using Col = ck_tile::tensor_layout::gemm::ColumnMajor;

    std::string a_layout = arg_parser.get_str("a_layout");
    std::string b_layout = arg_parser.get_str("b_layout");

    if(a_layout == "R" && b_layout == "R")
    {
        return run_gemm_example_with_layouts(argc, argv, Row{}, Row{}, Row{});
    }
    else if(a_layout == "R" && b_layout == "C")
    {
        return run_gemm_example_with_layouts(argc, argv, Row{}, Col{}, Row{});
    }
    // TODO: Fixme: with latest changes to GemmPipelineAGmemBGmemCRegV1DefaultPolicy below do not
    // work.
    // else if(a_layout == "C" && b_layout == "C")
    // {
    //     return run_gemm_example_with_layouts(argc, argv, Col{}, Col{}, Row{});
    // }
    // else if(a_layout == "C" && b_layout == "R")
    // {
    //     return run_gemm_example_with_layouts(argc, argv, Col{}, Row{}, Row{});
    // }
    else
    {
        throw std::runtime_error("Unsupported data layout configuration for A,B and C tensors!");
    }
}
