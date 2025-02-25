// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2024, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include "ck_tile/core.hpp"
#include "ck_tile/ops/flatmm/pipeline/flatmm_pipeline_agmem_bgmem_creg_v1_policy.hpp"

namespace ck_tile {

template <typename Problem, typename PipelinePolicy = UniversalFlatmmPipelineAgBgCrPolicy>
struct FlatmmPipelineAGmemBGmemCRegV1
{
    using ADataType      = remove_cvref_t<typename Problem::ADataType>;
    using BDataType      = remove_cvref_t<typename Problem::BDataType>;
    using CDataType      = remove_cvref_t<typename Problem::CDataType>;
    using BlockGemmShape = remove_cvref_t<typename Problem::BlockGemmShape>; // TileFlatmmShape

    using ALayout = remove_cvref_t<typename Problem::ALayout>;
    using BLayout = remove_cvref_t<typename Problem::BLayout>;
    using CLayout = remove_cvref_t<typename Problem::CLayout>;

    using BlockFlatmm =
        remove_cvref_t<decltype(PipelinePolicy::template GetBlockFlatmm<Problem>())>;

    static constexpr index_t BlockSize = Problem::kBlockSize;

    static constexpr index_t kMPerBlock = BlockGemmShape::kM;
    static constexpr index_t kNPerBlock = BlockGemmShape::kN;
    static constexpr index_t kKPerBlock = BlockGemmShape::kK;

    static constexpr index_t flatKPerWarp = BlockGemmShape::flatKPerWarp;
    static constexpr index_t flatNPerWarp = BlockGemmShape::flatNPerWarp;

    static constexpr index_t GetVectorSizeA() { return Problem::VectorSizeA; }
    static constexpr index_t GetVectorSizeB() { return Problem::VectorSizeB; }
    static constexpr index_t GetVectorSizeC() { return Problem::VectorSizeC; }

    static constexpr bool kPadM = Problem::kPadM;
    static constexpr bool kPadN = Problem::kPadN;
    static constexpr bool kPadK = Problem::kPadK;

    static constexpr index_t kLdsAlignmentInBytes = 16;

    static constexpr auto I0   = number<0>();
    static constexpr auto I1   = number<1>();
    static constexpr auto I2   = number<2>();
    static constexpr auto idxM = I0;
    static constexpr auto idxN = I1;
    static constexpr auto idxK = I2;
    using BlockTile            = remove_cvref_t<typename BlockGemmShape::BlockTile>;
    using BlockWarps           = remove_cvref_t<typename BlockGemmShape::BlockWarps>;
    using WarpTile             = remove_cvref_t<typename BlockGemmShape::WarpTile>;

    [[nodiscard]] CK_TILE_HOST static const std::string GetName()
    {
        // clang-format off
        return concat('_', "pipeline_AGmemBGmemCRegV1", 
                      concat('x', kMPerBlock, kNPerBlock, kKPerBlock,  BlockSize),
                      concat('x', GetVectorSizeA(), GetVectorSizeB(), GetVectorSizeC()),
                      concat('x', kPadM, kPadN, kPadK));
        // clang-format on
    }

    // For the basic gemm pipelien DoubleSmemBuffer set to be false naturally.
    static constexpr bool DoubleSmemBuffer = false;

    CK_TILE_HOST_DEVICE static constexpr auto TransposeC() { return Problem::TransposeC; }

    CK_TILE_HOST_DEVICE static constexpr index_t GetSmemSize()
    {
        return PipelinePolicy::template GetSmemSize<Problem>();
    }

    template <typename ADramBlockWindowTmp,
              typename BFlatBlockWindowTmp,
              typename AElementFunction
#ifdef FEIFEI_DEBUG
              ,
              typename BDramBlockWindowTmp,
              typename BElementFunction
#endif
              >
    CK_TILE_HOST_DEVICE auto operator()(const ADramBlockWindowTmp& a_dram_block_window_tmp,
                                        const AElementFunction& a_element_func,
                                        const BFlatBlockWindowTmp& b_flat_dram_block_window_tmp,
                                        index_t num_loop,
                                        void* p_smem
#ifdef FEIFEI_DEBUG
                                        ,
                                        const BDramBlockWindowTmp& b_dram_block_window_tmp,
                                        const BElementFunction& b_element_func,
                                        int* dbg_int,
                                        float* dbg_fp32,
                                        void* dbg_f168
#endif
    ) const
    {
#ifdef FEIFEI_DEBUG
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[PIPELN] FlatmmPipelinen():\n");
            printf("[PIPELN] num_loop = %d\n", num_loop);
            printf("[PIPELN] flatKPerWarp = %d, flatNPerWarp = %d\n", flatKPerWarp, flatNPerWarp);

            printf("[debug ] BlockTile  = [%d, %d, %d]\n",
                   static_cast<int>(BlockTile::at(idxM)),
                   static_cast<int>(BlockTile::at(idxN)),
                   static_cast<int>(BlockTile::at(idxK)));
            printf("[debug ] BlockWarps = [%d, %d, %d]\n",
                   static_cast<int>(BlockWarps::at(idxM)),
                   static_cast<int>(BlockWarps::at(idxN)),
                   static_cast<int>(BlockWarps::at(idxK)));
            printf("[debug ] WarpTile   = [%d, %d, %d]\n",
                   static_cast<int>(WarpTile::at(idxM)),
                   static_cast<int>(WarpTile::at(idxN)),
                   static_cast<int>(WarpTile::at(idxK)));
            printf("[debug ] flatWarp   = [%d, %d]\n",
                   static_cast<int>(BlockGemmShape::flatNPerWarp),
                   static_cast<int>(BlockGemmShape::flatKPerWarp));
            printf("[debug ] flatBlock  = [%d]\n", static_cast<int>(BlockGemmShape::flatKPerBlock));
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

        half_t* dbg_f16 = static_cast<half_t*>(dbg_f168);
        for(int i = 0; i < DEBUG_CNT; i++)
        {
            dbg_int[gid * DEBUG_CNT + i]  = 1;
            dbg_fp32[gid * DEBUG_CNT + i] = 1.0f;
            dbg_f16[gid * DEBUG_CNT + i]  = ck_tile::type_convert<ck_tile::half_t>(1.0f);
        }
#endif
        static_assert(
            std::is_same_v<ADataType, remove_cvref_t<typename ADramBlockWindowTmp::DataType>>,
            "wrong!");

        static_assert(kMPerBlock == ADramBlockWindowTmp{}.get_window_lengths()[number<0>{}],
                      "wrong!");
        // static_assert(kNPerBlock == BDramBlockWindowTmp{}.get_window_lengths()[number<0>{}],
        //               "wrong!");
        static_assert(kKPerBlock == ADramBlockWindowTmp{}.get_window_lengths()[number<1>{}],
                      "wrong!");

        // A tile in LDS
        ADataType* p_a_lds = static_cast<ADataType*>(p_smem);

        constexpr auto a_lds_block_desc =
            PipelinePolicy::template MakeALdsBlockDescriptor<Problem>();

        auto a_lds_block = make_tensor_view<address_space_enum::lds>(p_a_lds, a_lds_block_desc);

        // A DRAM tile window for load
        auto a_copy_dram_window =
            make_tile_window(a_dram_block_window_tmp.get_bottom_tensor_view(),
                             make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}),
                             a_dram_block_window_tmp.get_window_origin(),
                             PipelinePolicy::template MakeADramTileDistribution<Problem>());

        // A LDS tile window for store
        auto a_copy_lds_window = make_tile_window(
            a_lds_block, make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}), {0, 0});

        // A LDS tile for block GEMM
        auto a_lds_gemm_window = make_tile_window(
            a_lds_block, make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}), {0, 0});

#ifdef FEIFEI_DEBUG
        constexpr index_t a_lds_block_space_size_aligned =
            integer_divide_ceil(sizeof(ADataType) * a_lds_block_desc.get_element_space_size(),
                                kLdsAlignmentInBytes) *
            kLdsAlignmentInBytes;
        // B tile in LDS
        BDataType* p_b_lds = static_cast<BDataType*>(
            static_cast<void*>(static_cast<char*>(p_smem) + a_lds_block_space_size_aligned));

        constexpr auto b_lds_block_desc =
            PipelinePolicy::template MakeBLdsBlockDescriptor<Problem>();

        auto b_lds_block = make_tensor_view<address_space_enum::lds>(p_b_lds, b_lds_block_desc);

        // B DRAM tile window for load
        auto b_copy_dram_window =
            make_tile_window(b_dram_block_window_tmp.get_bottom_tensor_view(),
                             make_tuple(number<kNPerBlock>{}, number<kKPerBlock>{}),
                             b_dram_block_window_tmp.get_window_origin(),
                             PipelinePolicy::template MakeBDramTileDistribution<Problem>());

        // B LDS tile window for store
        auto b_copy_lds_window = make_tile_window(
            b_lds_block, make_tuple(number<kNPerBlock>{}, number<kKPerBlock>{}), {0, 0});

        // B LDS tile for block GEMM
        auto b_lds_gemm_window = make_tile_window(
            b_lds_block, make_tuple(number<kNPerBlock>{}, number<kKPerBlock>{}), {0, 0});
#endif

        // Block GEMM
        auto block_flatmm = BlockFlatmm();

        // B flat DRAM window for load
        auto b_flat_distribution =
            PipelinePolicy::template MakeBFlatDramTileDistribution<Problem>();
        auto b_flat_dram_window = // tile_window_with_static_distribution
            make_tile_window(
                b_flat_dram_block_window_tmp.get_bottom_tensor_view(), // from kernel gemm_pad_views
                make_tuple(number<flatNPerWarp>{}, number<flatKPerWarp>{}),
                b_flat_dram_block_window_tmp.get_window_origin(),
                b_flat_distribution);

        // Acc register tile
        auto c_block_tile = decltype(block_flatmm(a_lds_gemm_window,
                                                  b_flat_dram_window
#ifdef FEIFEI_DEBUG
                                                  ,
                                                  b_lds_gemm_window,
                                                  dbg_int,
                                                  dbg_fp32,
                                                  dbg_f168
#endif
                                                  )){};

        // prefetch
        // global read 0
        auto a_block_tile = load_tile(a_copy_dram_window);

#ifdef FEIFEI_DEBUG
        auto b_block_tile = load_tile(b_copy_dram_window);

        // debug A global load
        int a_block_tile_size_per_thread = a_block_tile.get_thread_buffer_size();
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[PIPELN] a_block_tile_size_per_thread = %d\n", a_block_tile_size_per_thread);
        }
        for(auto i = 0; i < a_block_tile_size_per_thread; i++)
        {
            // dbg_f16[gid * DEBUG_CNT + i] = a_block_tile.get_thread_buffer()[i];
        }

        // debug B global load
        int b_block_tile_size_per_thread = b_block_tile.get_thread_buffer_size();
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[PIPELN] b_block_tile_size_per_thread = %d\n", b_block_tile_size_per_thread);
        }
        for(auto i = 0; i < b_block_tile_size_per_thread; i++)
        {
            // dbg_f16[gid * DEBUG_CNT + i] = b_block_tile.get_thread_buffer()[i];
        }

        // debug flat B global load
        auto b_flat_tile                = load_tile(b_flat_dram_window);
        int b_flat_tile_size_per_thread = b_flat_tile.get_thread_buffer_size();
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[PIPELN] b_flat_tile_size_per_thread = %d\n", b_flat_tile_size_per_thread);
        }
        for(auto i = 0; i < b_flat_tile_size_per_thread; i++)
        {
            // dbg_f16[gid * DEBUG_CNT + i] = b_flat_tile.get_thread_buffer()[i];
        }
        // return c_block_tile;
#endif

        {
            // move to 1
            move_tile_window(a_copy_dram_window, {0, kKPerBlock});

            // initialize C
            tile_elementwise_inout([](auto& c) { c = 0; }, c_block_tile);

            // LDS write 0
            if constexpr(std::is_same_v<ALayout, tensor_layout::gemm::ColumnMajor>)
            {
                auto a_shuffle_tmp = make_static_distributed_tensor<ADataType>(
                    PipelinePolicy::template MakeShuffledARegBlockDistribution<Problem>());
                shuffle_tile(a_shuffle_tmp, a_block_tile);
                const auto a_block_tile_tmp = tile_elementwise_in(a_element_func, a_shuffle_tmp);
                store_tile(a_copy_lds_window, a_block_tile_tmp);
            }
            else
            {
                store_tile(a_copy_lds_window, tile_elementwise_in(a_element_func, a_block_tile));
            }

#ifdef FEIFEI_DEBUG
            move_tile_window(b_copy_dram_window, {0, kKPerBlock});
            // LDS write 0
            if constexpr(std::is_same_v<BLayout, tensor_layout::gemm::RowMajor>)
            {
                auto b_shuffle_tmp = make_static_distributed_tensor<BDataType>(
                    PipelinePolicy::template MakeShuffledBRegBlockDistribution<Problem>());
                shuffle_tile(b_shuffle_tmp, b_block_tile);
                const auto b_block_tile_tmp = tile_elementwise_in(b_element_func, b_shuffle_tmp);
                store_tile(b_copy_lds_window, b_block_tile_tmp);
            }
            else
            {
                store_tile(b_copy_lds_window, tile_elementwise_in(b_element_func, b_block_tile));
            }
#endif
        }

        index_t iCounter = num_loop - 1;
        while(iCounter > 0)
        {
            // global read i + 1
            a_block_tile = load_tile(a_copy_dram_window);
#ifdef FEIFEI_DEBUG
            b_block_tile = load_tile(b_copy_dram_window);
#endif

            block_sync_lds();

            // GEMM i
            block_flatmm(c_block_tile,
                         a_lds_gemm_window,
                         b_flat_dram_window
#ifdef FEIFEI_DEBUG
                         ,
                         b_lds_gemm_window,
                         dbg_int,
                         dbg_fp32,
                         dbg_f168
#endif
            );

            block_sync_lds();

            // move to i + 2
            move_tile_window(a_copy_dram_window, {0, kKPerBlock});

            // LDS write i + 1
            const auto a_block_tile_tmp = tile_elementwise_in(a_element_func, a_block_tile);
            store_tile(a_copy_lds_window, a_block_tile_tmp);

#ifdef FEIFEI_DEBUG
            move_tile_window(b_copy_dram_window, {0, kKPerBlock});

            // LDS write i + 1
            if constexpr(std::is_same_v<BLayout, tensor_layout::gemm::RowMajor>)
            {
                auto b_shuffle_tmp_loop = make_static_distributed_tensor<BDataType>(
                    PipelinePolicy::template MakeShuffledBRegBlockDistribution<Problem>());
                shuffle_tile(b_shuffle_tmp_loop, b_block_tile);
                store_tile(b_copy_lds_window,
                           tile_elementwise_in(b_element_func, b_shuffle_tmp_loop));
            }
            else
            {
                const auto b_block_tile_tmp = tile_elementwise_in(b_element_func, b_block_tile);
                store_tile(b_copy_lds_window, b_block_tile_tmp);
            }
#endif

            // move to next flat K
            move_tile_window(b_flat_dram_window, {0, BlockGemmShape::flatKPerBlock});

            iCounter--;
        }

        // tail
        {
            block_sync_lds();

            // GEMM num_loop - 1
            block_flatmm(c_block_tile,
                         a_lds_gemm_window,
                         b_flat_dram_window
#ifdef FEIFEI_DEBUG
                         ,
                         b_lds_gemm_window,
                         dbg_int,
                         dbg_fp32,
                         dbg_f168
#endif
            );
        }

        return c_block_tile;
    }

    template <typename ADramBlockWindowTmp,
              typename BFlatBlockWindowTmp
#ifdef FEIFEI_DEBUG
              ,
              typename BDramBlockWindowTmp
#endif
              >
    CK_TILE_DEVICE auto operator()(const ADramBlockWindowTmp& a_dram_block_window_tmp,
                                   const BFlatBlockWindowTmp& b_flat_dram_block_window_tmp,
                                   index_t num_loop,
                                   void* p_smem
#ifdef FEIFEI_DEBUG
                                   ,
                                   const BDramBlockWindowTmp& b_dram_block_window_tmp,
                                   int* dbg_int,
                                   float* dbg_fp32,
                                   void* dbg_f168
#endif
    ) const
    {
        return operator()(
            a_dram_block_window_tmp,
            [](const ADataType & a) { return a; },
            b_flat_dram_block_window_tmp,
            num_loop,
            p_smem
#ifdef FEIFEI_DEBUG
            ,
            b_dram_block_window_tmp,
            [](const BDataType & b) { return b; },
            dbg_int,
            dbg_fp32,
            dbg_f168
#endif
        );
    }
};

} // namespace ck_tile
