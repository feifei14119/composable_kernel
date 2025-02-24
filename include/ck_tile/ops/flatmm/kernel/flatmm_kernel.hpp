// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2025, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include <iostream>
#include <string>

#include "ck_tile/core.hpp"
#include "ck_tile/ops/common.hpp"
#include "ck_tile/ops/gemm/pipeline/gemm_pipeline_ag_bg_cr_scheduler.hpp"

namespace ck_tile {

struct FlatmmProblem
{
    CK_TILE_HOST FlatmmProblem() = default;
    CK_TILE_HOST FlatmmProblem(
        index_t M_, index_t N_, index_t K_, index_t stride_A_, index_t stride_B_, index_t stride_C_)
        : M(M_), N(N_), K(K_), stride_A(stride_A_), stride_B(stride_B_), stride_C(stride_C_)
    {
    }

    index_t M;
    index_t N;
    index_t K;
    index_t stride_A;
    index_t stride_B;
    index_t stride_C;
};

struct FlatmmHostArgs : public FlatmmProblem
{
    CK_TILE_HOST FlatmmHostArgs() = default;
    CK_TILE_HOST FlatmmHostArgs(const void* a_ptr_,
                                const void* b_shuffle_ptr_,
                                void* c_ptr_,
                                index_t k_batch_,
                                index_t M_,
                                index_t N_,
                                index_t K_,
                                index_t stride_A_,
                                index_t stride_B_,
                                index_t stride_C_)
        : FlatmmProblem(M_, N_, K_, stride_A_, stride_B_, stride_C_),
          a_ptr(a_ptr_),
          b_shuffle_ptr(b_shuffle_ptr_),
          c_ptr(c_ptr_),
          k_batch(k_batch_)
    {
    }

    const void* a_ptr;
    const void* b_shuffle_ptr;
    void* c_ptr;
    index_t k_batch;

#ifdef FEIFEI_DEBUG
    const void* b_ptr;

    void* dbg_int_ptr;
    void* dbg_fp32_ptr;
    void* dbg_f168_ptr;
#endif
};

template <typename TilePartitioner_, typename FlatmmPipeline_, typename EpiloguePipeline_>
struct FlatmmKernel
{
    using TilePartitioner                    = remove_cvref_t<TilePartitioner_>;
    using FlatmmPipeline                     = remove_cvref_t<FlatmmPipeline_>;
    using BlockGemmShape                     = remove_cvref_t<typename FlatmmPipeline::BlockGemmShape>; // TileFlatmmShape
    using EpiloguePipeline                   = remove_cvref_t<EpiloguePipeline_>;
    using ALayout                            = remove_cvref_t<typename FlatmmPipeline::ALayout>;
    using BLayout                            = remove_cvref_t<typename FlatmmPipeline::BLayout>;
    using CLayout                            = remove_cvref_t<typename FlatmmPipeline::CLayout>;
    static constexpr index_t KernelBlockSize = FlatmmPipeline::BlockSize;

    using ADataType = remove_cvref_t<typename FlatmmPipeline::ADataType>;
    using BDataType = remove_cvref_t<typename FlatmmPipeline::BDataType>;
    // Below type is actually accumulation data type - the output of block GEMM.
    using CDataType = remove_cvref_t<typename EpiloguePipeline::ODataType>;

    static constexpr auto I0 = number<0>();
    static constexpr auto I1 = number<1>();
    static constexpr auto I2 = number<2>();
    static constexpr auto idxM = I0;
    static constexpr auto idxN = I1;
    static constexpr auto idxK = I2;

    [[nodiscard]] CK_TILE_HOST static const std::string GetName()
    {
        // clang-format off
        return concat('_', "gemm", gemm_prec_str<ADataType, BDataType>, FlatmmPipeline::GetName());
        // clang-format on
    }

    CK_TILE_HOST static constexpr auto GridSize(index_t M, index_t N, index_t KBatch)
    {
        return dim3(TilePartitioner::GridSize(M, N), 1, KBatch);
    }

    CK_TILE_HOST static constexpr auto BlockSize() { return dim3(KernelBlockSize); }

    struct FlatmmKernelArgs
    {
        const void* a_ptr;
        const void* b_shuffle_ptr;
        void* c_ptr;
        index_t M;
        index_t N;
        index_t K;
        index_t stride_A;
        index_t stride_B;
        index_t stride_C;
        index_t k_batch;
#ifdef FEIFEI_DEBUG
        const void* b_ptr;

        void* dbg_int_ptr;
        void* dbg_fp32_ptr;
        void* dbg_f168_ptr;
#endif
    };

    CK_TILE_HOST static constexpr FlatmmKernelArgs MakeKernelArgs(const FlatmmHostArgs& hostArgs)
    {
        return FlatmmKernelArgs{hostArgs.a_ptr,
                                hostArgs.b_shuffle_ptr,
                                hostArgs.c_ptr,
                                hostArgs.M,
                                hostArgs.N,
                                hostArgs.K,
                                hostArgs.stride_A,
                                hostArgs.stride_B,
                                hostArgs.stride_C,
                                hostArgs.k_batch
#ifdef FEIFEI_DEBUG
                                ,
                                hostArgs.b_ptr,
                                hostArgs.dbg_int_ptr,
                                hostArgs.dbg_fp32_ptr,
                                hostArgs.dbg_f168_ptr
#endif
        };
    }

    CK_TILE_HOST_DEVICE static constexpr index_t GetSmemSize()
    {
        return max(FlatmmPipeline::GetSmemSize(), EpiloguePipeline::GetSmemSize());
    }

    struct SplitKBatchOffset
    {
        __device__ SplitKBatchOffset(const FlatmmKernelArgs& kargs,
                                     const std::size_t k_id = blockIdx.z)
        {
            constexpr auto K1   = TilePartitioner::BlockGemmShape::WarpTile::at(number<2>{});
            const index_t K_t   = kargs.k_batch * K1;
            const index_t KRead = (kargs.K + K_t - 1) / K_t * K1;

            if constexpr(std::is_same_v<tensor_layout::gemm::RowMajor, ALayout>)
            {
                a_k_split_offset = k_id * KRead;
            }
            else if constexpr(std::is_same_v<tensor_layout::gemm::ColumnMajor, ALayout>)
            {
                a_k_split_offset = k_id * KRead * kargs.stride_A;
            }

            if constexpr(std::is_same_v<tensor_layout::gemm::RowMajor, BLayout>)
            {
                b_k_split_offset = k_id * KRead * kargs.stride_B;
            }
            else if constexpr(std::is_same_v<tensor_layout::gemm::ColumnMajor, BLayout>)
            {
                b_k_split_offset = k_id * KRead;
            }

            if(k_id < static_cast<uint32_t>(kargs.k_batch - 1))
            {
                splitted_k = KRead;
            }
            else
            {
                splitted_k = kargs.K - KRead * (kargs.k_batch - 1);
            }
        }

        index_t a_k_split_offset;
        index_t b_k_split_offset;
        index_t splitted_k;
    };

    CK_TILE_HOST static bool IsSupportedArgument(const FlatmmKernelArgs& kargs)
    {
        if constexpr(EpiloguePipeline::template GetVectorSizeC<CDataType>() % 2 != 0 &&
                     is_any_of<CDataType, fp16_t, bf16_t>::value)
        {
            if(kargs.k_batch != 1)
            {
                std::cerr << "Conditions not met for Kbatch >1 !" << std::endl;
                return false;
            }
        }

        if constexpr(std::is_same_v<ALayout, tensor_layout::gemm::RowMajor>)
        {
            if(kargs.K % TilePartitioner::KPerBlock != 0 && FlatmmPipeline::kPadK == false)
            {
                std::cerr << "Can't support K that is not a multiple of KPerBlock"
                             " without padding!"
                          << std::endl;
                return false;
            }
            if(kargs.K % FlatmmPipeline::GetVectorSizeA() != 0)
            {
                std::cerr << "K is not a multiple of vector load size for A tensor!" << std::endl;
                return false;
            }
        }
        else
        {
            if(kargs.M % TilePartitioner::MPerBlock != 0 && FlatmmPipeline::kPadM == false)
            {
                std::cerr << "Can't support M that is not a multiple of MPerBlock"
                             " without padding!"
                          << std::endl;
                return false;
            }
            if(kargs.M % FlatmmPipeline::GetVectorSizeA() != 0)
            {
                std::cerr << "M is not a multiple of vector load size for A tensor!" << std::endl;
                return false;
            }
        }

        if constexpr(std::is_same_v<BLayout, tensor_layout::gemm::RowMajor>)
        {
            if(kargs.N % TilePartitioner::NPerBlock != 0 && FlatmmPipeline::kPadN == false)
            {
                std::cerr << "Can't support N that is not a multiple of NPerBlock"
                             " without padding!"
                          << std::endl;
                return false;
            }
            if(kargs.N % FlatmmPipeline::GetVectorSizeB() != 0)
            {
                std::cerr << "N is not a multiple of vector load size for B tensor!" << std::endl;
                return false;
            }
        }
        else
        {
            if(kargs.K % TilePartitioner::KPerBlock != 0 && FlatmmPipeline::kPadK == false)
            {
                std::cerr << "Can't support K that is not a multiple of KPerBlock"
                             " without padding!"
                          << std::endl;
                return false;
            }
            if(kargs.K % FlatmmPipeline::GetVectorSizeB() != 0)
            {
                std::cerr << "K is not a multiple of vector load size for B tensor!" << std::endl;
                return false;
            }
        }

        if constexpr(std::is_same_v<CLayout, tensor_layout::gemm::RowMajor>)
        {
            if(kargs.N % TilePartitioner::NPerBlock != 0 && FlatmmPipeline::kPadN == false)
            {
                std::cerr << "Can't support N that is not a multiple of NPerBlock"
                             " without padding!"
                          << std::endl;
                return false;
            }
            if(kargs.N % EpiloguePipeline::template GetVectorSizeC<CDataType>() != 0)
            {
                std::cerr << "N is not a multiple of vector load size for C tensor!" << std::endl;
                return false;
            }
        }
        else
        {
            if(kargs.M % TilePartitioner::MPerBlock != 0 && FlatmmPipeline::kPadM == false)
            {
                std::cerr << "Can't support M that is not a multiple of MPerBlock"
                             " without padding!"
                          << std::endl;
                return false;
            }
            if(kargs.M % EpiloguePipeline::template GetVectorSizeC<CDataType>() != 0)
            {
                std::cerr << "M is not a multiple of vector load size for C tensor!" << std::endl;
                return false;
            }
        }
        return true;
    }

    template <memory_operation_enum DstInMemOp = memory_operation_enum::set>
    CK_TILE_DEVICE static auto MakeGemmTensorViews(const ADataType* a_ptr,
                                                   const BDataType* b_ptr,
                                                   CDataType* c_ptr,
                                                   const FlatmmKernelArgs& kargs,
                                                   const SplitKBatchOffset& splitk_batch_offset)
    {
        const auto& a_tensor_view = [&]() {
            if constexpr(std::is_same_v<ALayout, tensor_layout::gemm::RowMajor>)
            {
                return make_naive_tensor_view<address_space_enum::global>(
                    a_ptr,
                    make_tuple(kargs.M, splitk_batch_offset.splitted_k),
                    make_tuple(kargs.stride_A, 1),
                    number<FlatmmPipeline::GetVectorSizeA()>{},
                    number<1>{});
            }
            else
            {
                return make_naive_tensor_view<address_space_enum::global>(
                    a_ptr,
                    make_tuple(splitk_batch_offset.splitted_k, kargs.M),
                    make_tuple(kargs.stride_A, 1),
                    number<FlatmmPipeline::GetVectorSizeA()>{},
                    number<1>{});
            }
        }();

        const auto& b_tensor_view = [&]() {
            if constexpr(std::is_same_v<BLayout, tensor_layout::gemm::RowMajor>)
            {
                return make_naive_tensor_view<address_space_enum::global>(
                    b_ptr,
                    make_tuple(splitk_batch_offset.splitted_k, kargs.N),
                    make_tuple(kargs.stride_B, 1),
                    number<FlatmmPipeline::GetVectorSizeB()>{},
                    number<1>{});
            }
            else
            {
                return make_naive_tensor_view<address_space_enum::global>(
                    b_ptr,
                    make_tuple(kargs.N, splitk_batch_offset.splitted_k),
                    make_tuple(kargs.stride_B, 1),
                    number<FlatmmPipeline::GetVectorSizeB()>{},
                    number<1>{});
            }
        }();

        // TODO: enable vector write for C in ColMajor
        const auto& c_tensor_view = [&]() {
            if constexpr(std::is_same_v<CLayout, tensor_layout::gemm::RowMajor>)
            {
                return make_naive_tensor_view<address_space_enum::global, DstInMemOp>(
                    c_ptr,
                    make_tuple(kargs.M, kargs.N),
                    make_tuple(kargs.stride_C, 1),
                    number<EpiloguePipeline::template GetVectorSizeC<CDataType>()>{},
                    number<1>{});
            }
            else
            {
                return make_naive_tensor_view<address_space_enum::global, DstInMemOp>(
                    c_ptr,
                    make_tuple(kargs.M, kargs.N),
                    make_tuple(1, kargs.stride_C),
                    number<1>{},
                    number<1>{});
            }
        }();

        return make_tuple(a_tensor_view, b_tensor_view, c_tensor_view);
    }

    template <typename TensorView>
    CK_TILE_DEVICE static auto MakeGemmPadViews(const TensorView& views)
    {
        const auto& a_pad_view = [&]() {
            const auto& a_tensor_view = views.at(I0);
            if constexpr(std::is_same_v<ALayout, tensor_layout::gemm::RowMajor>)
            {
                return pad_tensor_view(a_tensor_view,
                                       make_tuple(number<TilePartitioner::MPerBlock>{},
                                                  number<TilePartitioner::KPerBlock>{}),
                                       sequence<false, FlatmmPipeline::kPadK>{});
            }
            else
            {
                return pad_tensor_view(a_tensor_view,
                                       make_tuple(number<TilePartitioner::KPerBlock>{},
                                                  number<TilePartitioner::MPerBlock>{}),
                                       sequence<false, FlatmmPipeline::kPadM>{});
            }
        }();

        const auto& b_pad_view = [&]() {
            const auto& b_tensor_view = views.at(I1);
            if constexpr(std::is_same_v<BLayout, tensor_layout::gemm::ColumnMajor>)
            {
                return pad_tensor_view(b_tensor_view,
                                       make_tuple(number<TilePartitioner::NPerBlock>{},
                                                  number<TilePartitioner::KPerBlock>{}),
                                       sequence<false, FlatmmPipeline::kPadK>{});
            }
            else
            {
                return pad_tensor_view(b_tensor_view,
                                       make_tuple(number<TilePartitioner::KPerBlock>{},
                                                  number<TilePartitioner::NPerBlock>{}),
                                       sequence<false, FlatmmPipeline::kPadN>{});
            }
        }();

        // TODO vector write in for C in ColMajor
        const auto& c_pad_view = [&]() {
            const auto& c_tensor_view = views.at(I2);
            if constexpr(std::is_same_v<CLayout, tensor_layout::gemm::RowMajor>)
            {
                return pad_tensor_view(c_tensor_view,
                                       make_tuple(number<TilePartitioner::MPerBlock>{},
                                                  number<TilePartitioner::NPerBlock>{}),
                                       sequence<false, FlatmmPipeline::kPadN>{});
            }
            else
            {
                return pad_tensor_view(c_tensor_view,
                                       make_tuple(number<TilePartitioner::MPerBlock>{},
                                                  number<TilePartitioner::NPerBlock>{}),
                                       sequence<FlatmmPipeline::kPadM, false>{});
            }
        }();

        return make_tuple(a_pad_view, b_pad_view, c_pad_view);
    }

    template <typename PadView>
    CK_TILE_DEVICE static auto
    MakeGemmTileWindows(const PadView& views, const index_t i_m, const index_t i_n)
    {
        const auto& a_pad_view = views.at(I0);
        const auto& b_pad_view = views.at(I1);
        const auto& c_pad_view = views.at(I2);

        const auto& a_block_window = [&]() {
            if constexpr(std::is_same_v<ALayout, tensor_layout::gemm::RowMajor>)
            {
                return make_tile_window(a_pad_view,
                                        make_tuple(number<TilePartitioner::MPerBlock>{},
                                                   number<TilePartitioner::KPerBlock>{}),
                                        {i_m, 0});
            }
            else
            {
                return make_tile_window(a_pad_view,
                                        make_tuple(number<TilePartitioner::KPerBlock>{},
                                                   number<TilePartitioner::MPerBlock>{}),
                                        {0, i_m});
            }
        }();

        const auto& b_block_window = [&]() {
            if constexpr(std::is_same_v<BLayout, tensor_layout::gemm::ColumnMajor>)
            {
                return make_tile_window(b_pad_view,
                                        make_tuple(number<TilePartitioner::NPerBlock>{},
                                                   number<TilePartitioner::KPerBlock>{}),
                                        {i_n, 0});
            }
            else
            {
                return make_tile_window(b_pad_view,
                                        make_tuple(number<TilePartitioner::KPerBlock>{},
                                                   number<TilePartitioner::NPerBlock>{}),
                                        {0, i_n});
            }
        }();

        auto c_block_window = make_tile_window(
            c_pad_view,
            make_tuple(number<TilePartitioner::MPerBlock>{}, number<TilePartitioner::NPerBlock>{}),
            {i_m, i_n});

        return make_tuple(a_block_window, b_block_window, c_block_window);
    }

    template <memory_operation_enum DstInMemOp = memory_operation_enum::set>
    CK_TILE_DEVICE static void RunFlatmm(const ADataType* a_ptr,
                                         const BDataType* b_shuffle_ptr,
                                         CDataType* c_ptr,
                                         void* smem_ptr_0,
                                         const FlatmmKernelArgs& kargs,
                                         const SplitKBatchOffset& splitk_batch_offset,
                                         const index_t block_idx_m,
                                         const index_t block_idx_n
#ifdef FEIFEI_DEBUG
                                         ,
                                         const BDataType* b_ptr,
                                         int* dbg_int,
                                         float* dbg_fp32,
                                         void* dbg_f168
#endif
    )
    {
#ifdef FEIFEI_DEBUG
        uint32_t tidx = threadIdx.x;
        uint32_t tidy = threadIdx.y;
        uint32_t bidx = blockIdx.x;
        uint32_t bidy = blockIdx.y;
        uint32_t bdmx = blockDim.x;
        uint32_t bdmy = blockDim.y;
        uint32_t gdmx = gridDim.x;
        uint32_t gdmy = gridDim.y;
        uint32_t gid  = ((bdmx * bdmy) * gdmx) * bidy + (bdmx * bdmy) * bidx + bdmx * tidy + tidx;

        half_t* dbg_f16 = static_cast<half_t*>(kargs.dbg_f168_ptr);
#endif

        // Create Gemm tensor views, pad views and tile windows
        const auto& gemm_tensor_views_tuple =
            MakeGemmTensorViews<DstInMemOp>(a_ptr, b_ptr, c_ptr, kargs, splitk_batch_offset);
        const auto& gemm_pad_views = MakeGemmPadViews(gemm_tensor_views_tuple);
        auto gemm_tile_windows     = MakeGemmTileWindows(gemm_pad_views, block_idx_m, block_idx_n);

        const index_t num_loop = TilePartitioner::GetLoopNum(splitk_batch_offset.splitted_k);

        index_t kFlatK =
            FlatmmPipeline::flatKPerWarp *
            (splitk_batch_offset.splitted_k / BlockGemmShape::WarpTile::at(number<2>{}));
        index_t kFlatN = kargs.N * kargs.K / kFlatK;

#ifdef FEIFEI_DEBUG
        ////////////////////////////////////////////////////////
        const auto& a_gemm_tensor_views = gemm_tensor_views_tuple.at(I0); // tensor_view
        const auto& a_gemm_tensor_desc  = a_gemm_tensor_views.desc_;      // tensor_descriptor
        const auto& a_gemm_buff_views   = a_gemm_tensor_views.buf_;       // buffer_view
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[KERNEL] a_gemm_tensor_view: size = %ld, len = [%d, %d], top = [%d, %d], upper "
                   "= %d, lower = %d\n",
                   a_gemm_tensor_desc.get_element_space_size(),
                   a_gemm_tensor_desc.get_length(I0),
                   a_gemm_tensor_desc.get_length(I1),
                   a_gemm_tensor_desc.get_top_dimension_hidden_ids()[0],
                   a_gemm_tensor_desc.get_top_dimension_hidden_ids()[1],
                   a_gemm_tensor_desc.get_upper_dimension_hidden_idss()(I0)[0],
                   a_gemm_tensor_desc.get_lower_dimension_hidden_idss()(I0)[0]);
        }

        const auto& a_pad_tensor_views = gemm_pad_views.at(I0);    // tensor_view
        const auto& a_pad_tensor_desc  = a_pad_tensor_views.desc_; // tensor_descriptor
        const auto& a_pad_buff_views   = a_pad_tensor_views.buf_;  // buffer_view
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[KERNEL] a_pad_tensor_view:  size = %ld, len = [%d, %d], top = [%d, %d], upper "
                   "= %d, lower = %d\n",
                   a_pad_tensor_desc.get_element_space_size(),
                   a_pad_tensor_desc.get_length(I0),
                   a_pad_tensor_desc.get_length(I1),
                   a_pad_tensor_desc.get_top_dimension_hidden_ids()[0],
                   a_pad_tensor_desc.get_top_dimension_hidden_ids()[1],
                   a_pad_tensor_desc.get_upper_dimension_hidden_idss()(I0)[0],
                   a_pad_tensor_desc.get_lower_dimension_hidden_idss()(I0)[0]);
        }

        const auto& a_tile_win = gemm_tile_windows.at(I0); // tile_window_with_static_lengths
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[KERNEL] a_gemm_tile_window: dim_num = %d\n",
                   a_tile_win.get_num_of_dimension());
        }
        if(threadIdx.x == 0 && threadIdx.y == 0)
        {
            printf("[KERNEL] blockIdx.x = %d: block_idx_n = %d, block_idx_m = %d\n",
                   static_cast<int>(blockIdx.x), block_idx_n, block_idx_n);
        }
        ////////////////////////////////////////////////////////
#endif

        const auto& b_flat_tensor_view = [&]() {
            return make_naive_tensor_view<address_space_enum::global>(
                b_shuffle_ptr,
                make_tuple(kFlatN, kFlatK),
                make_tuple(kFlatK, 1),
                number<FlatmmPipeline::GetVectorSizeB()>{},
                number<1>{});
        }();
        /*const auto& b_flat_pad_view = [&]() {
            return pad_tensor_view(b_flat_tensor_view,
                                   make_tuple(number<TilePartitioner::NPerBlock>{},
                                              number<TilePartitioner::KPerBlock>{}),
                                   sequence<false, FlatmmPipeline::kPadK>{});
        }();
        const auto& b_flat_block_window = make_tile_window(
            b_flat_pad_view,
            make_tuple(number<TilePartitioner::NPerBlock>{}, number<TilePartitioner::KPerBlock>{}),
            {block_idx_n, 0});*/
        const auto& b_flat_block_window = make_tile_window(
            b_flat_tensor_view,
            make_tuple(number<FlatmmPipeline::flatNPerWarp>{}, number<FlatmmPipeline::flatKPerWarp>{}),
            {static_cast<int>(block_idx_n / BlockGemmShape::WarpTile::at(idxN)), 0});

        // Run GEMM cooperatively by whole workgroup.
        const auto& a_block_window = gemm_tile_windows.at(I0);
        const auto& b_block_window = gemm_tile_windows.at(I1);
        const auto& c_block_tile = FlatmmPipeline{}.template operator()(a_block_window,
                                                                        b_flat_block_window,
                                                                        num_loop,
                                                                        smem_ptr_0
#ifdef FEIFEI_DEBUG
                                                                        ,
                                                                        b_block_window,
                                                                        dbg_int,
                                                                        dbg_fp32,
                                                                        dbg_f168
#endif
        );

        // Run Epilogue Pipeline
        auto& c_block_window = gemm_tile_windows.at(I2);

        EpiloguePipeline{}
            .template operator()<decltype(c_block_window), decltype(c_block_tile), DstInMemOp>(
                c_block_window, c_block_tile, smem_ptr_0);
    }

    CK_TILE_DEVICE void operator()(FlatmmKernelArgs kargs) const
    {
#ifdef FEIFEI_DEBUG
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[KERNEL] ===== FlatmmKernel() =====\n");
            printf("[KERNEL] blockDim: [%d, %d], gridDim: [%d, %d]\n",
                   static_cast<int>(blockDim.x),
                   static_cast<int>(blockDim.y),
                   static_cast<int>(gridDim.x),
                   static_cast<int>(gridDim.y));
            printf("[KERNEL] lds = %.3f (KB)\n", GetSmemSize() / 1024.0f);
        }

        uint32_t tidx = threadIdx.x;
        uint32_t tidy = threadIdx.y;
        uint32_t bidx = blockIdx.x;
        uint32_t bidy = blockIdx.y;
        uint32_t bdmx = blockDim.x;
        uint32_t bdmy = blockDim.y;
        uint32_t gdmx = gridDim.x;
        uint32_t gdmy = gridDim.y;
        uint32_t gid  = ((bdmx * bdmy) * gdmx) * bidy + (bdmx * bdmy) * bidx + bdmx * tidy + tidx;

        const SplitKBatchOffset _splitk_batch_offset(kargs);
        const BDataType* b_ptr =
            static_cast<const BDataType*>(kargs.b_ptr) + _splitk_batch_offset.b_k_split_offset;

        int* dbg_int    = static_cast<int*>(kargs.dbg_int_ptr);
        float* dbg_fp32 = static_cast<float*>(kargs.dbg_fp32_ptr);
        half_t* dbg_f16 = static_cast<half_t*>(kargs.dbg_f168_ptr);

        for(int i = 0; i < DEBUG_CNT; i++)
        {
            dbg_int[gid * DEBUG_CNT + i]  = 0;
            dbg_fp32[gid * DEBUG_CNT + i] = .0f;
            dbg_f16[gid * DEBUG_CNT + i]  = ck_tile::type_convert<ck_tile::half_t>(0.f);
        }
#endif
        const auto [iM, iN] = TilePartitioner{kargs.M, kargs.N}.GetOutputTileIndex(blockIdx.x);
        const index_t i_m   = __builtin_amdgcn_readfirstlane(iM * TilePartitioner::MPerBlock);
        const index_t i_n   = __builtin_amdgcn_readfirstlane(iN * TilePartitioner::NPerBlock);

        const SplitKBatchOffset splitk_batch_offset(kargs);
        // options
        const ADataType* a_ptr =
            static_cast<const ADataType*>(kargs.a_ptr) + splitk_batch_offset.a_k_split_offset;
        const BDataType* b_shuffle_ptr = static_cast<const BDataType*>(kargs.b_shuffle_ptr) +
                                         splitk_batch_offset.b_k_split_offset;
        CDataType* c_ptr = static_cast<CDataType*>(kargs.c_ptr);

        // allocate LDS
        __shared__ char smem_ptr_0[GetSmemSize()];
        __shared__ char smem_ptr_1[GetSmemSize()];

        if(kargs.k_batch == 1)
        {
            RunFlatmm(a_ptr,
                      b_shuffle_ptr,
                      c_ptr,
                      smem_ptr_0,
                      kargs,
                      splitk_batch_offset,
                      i_m,
                      i_n
#ifdef FEIFEI_DEBUG
                      ,
                      b_ptr,
                      dbg_int,
                      dbg_fp32,
                      kargs.dbg_f168_ptr
#endif
            );
        }
        else
        {
            // Do not compile in case where we have unsupported
            // VectorSizeC & data type configuration.
            if constexpr(!(EpiloguePipeline::template GetVectorSizeC<CDataType>() % 2 != 0 &&
                           is_any_of<CDataType, fp16_t, bf16_t>::value))
            {
                RunFlatmm<memory_operation_enum::atomic_add>(a_ptr,
                                                             b_ptr,
                                                             c_ptr,
                                                             smem_ptr_0,
                                                             kargs,
                                                             splitk_batch_offset,
                                                             i_m,
                                                             i_n
#ifdef FEIFEI_DEBUG
                                                             ,
                                                             b_ptr,
                                                             dbg_int,
                                                             dbg_fp32,
                                                             kargs.dbg_f168_ptr
#endif
                );
            }
        }
    }
};

} // namespace ck_tile
