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
    static constexpr index_t flatKPerBlock = BlockGemmShape::flatKPerBlock;

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

    // ================================================================================================
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
        static_assert(std::is_same_v<ADataType, remove_cvref_t<typename ADramBlockWindowTmp::DataType>>, "wrong!");
        static_assert(kMPerBlock == ADramBlockWindowTmp{}.get_window_lengths()[number<0>{}], "wrong!");
        // static_assert(kNPerBlock == BDramBlockWindowTmp{}.get_window_lengths()[number<0>{}], "wrong!");
        static_assert(kKPerBlock == ADramBlockWindowTmp{}.get_window_lengths()[number<1>{}], "wrong!");

        // A tile in LDS
        ADataType* p_a_lds0 = static_cast<ADataType*>(p_smem);
        ADataType* p_a_lds1 = p_a_lds0 + PipelinePolicy::template GetSmemSizeA<Problem>() / sizeof(ADataType);
        constexpr auto a_lds_block_desc = PipelinePolicy::template MakeALdsBlockDescriptor<Problem>();
        auto a_lds_block0 = make_tensor_view<address_space_enum::lds>(p_a_lds0, a_lds_block_desc);
        auto a_lds_block1 = make_tensor_view<address_space_enum::lds>(p_a_lds1, a_lds_block_desc);

        // A DRAM tile window for load
        auto a_copy_dram_window =
            make_tile_window(a_dram_block_window_tmp.get_bottom_tensor_view(),
                             make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}),
                             a_dram_block_window_tmp.get_window_origin(),
                             PipelinePolicy::template MakeADramTileDistribution<Problem>());

        // A LDS tile window for store
        auto a_copy_lds_window0 = make_tile_window(a_lds_block0, make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}), {0, 0});
        auto a_copy_lds_window1 = make_tile_window(a_lds_block1, make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}), {0, 0});

        // A LDS tile for block GEMM
        auto a_lds_gemm_window0 = make_tile_window(a_lds_block0, make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}), {0, 0});
        auto a_lds_gemm_window1 = make_tile_window(a_lds_block1, make_tuple(number<kMPerBlock>{}, number<kKPerBlock>{}), {0, 0});

#ifdef FEIFEI_DEBUG
        constexpr index_t a_lds_block_space_size_aligned =
            integer_divide_ceil(sizeof(ADataType) * a_lds_block_desc.get_element_space_size(),
                                kLdsAlignmentInBytes) *
            kLdsAlignmentInBytes;

        // B tile in LDS
        BDataType* p_b_lds = static_cast<BDataType*>(static_cast<void*>(static_cast<char*>(p_smem) + a_lds_block_space_size_aligned));
        constexpr auto b_lds_block_desc = PipelinePolicy::template MakeBLdsBlockDescriptor<Problem>();
        auto b_lds_block = make_tensor_view<address_space_enum::lds>(p_b_lds, b_lds_block_desc);

        // B DRAM tile window for load
        auto b_copy_dram_window =
            make_tile_window(b_dram_block_window_tmp.get_bottom_tensor_view(),
                             make_tuple(number<kNPerBlock>{}, number<kKPerBlock>{}),
                             b_dram_block_window_tmp.get_window_origin(),
                             PipelinePolicy::template MakeBDramTileDistribution<Problem>());

        // B LDS tile window for store
        auto b_copy_lds_window = make_tile_window(b_lds_block, make_tuple(number<kNPerBlock>{}, number<kKPerBlock>{}), {0, 0});

        // B LDS tile for block GEMM
        auto b_lds_gemm_window = make_tile_window(b_lds_block, make_tuple(number<kNPerBlock>{}, number<kKPerBlock>{}), {0, 0});
#endif

        // Block GEMM
        auto block_flatmm = BlockFlatmm();

        // B flat DRAM window for load
        auto b_flat_distribution = PipelinePolicy::template MakeBFlatDramTileDistribution<Problem>();
        auto b_flat_dram_window =
            make_tile_window(
                b_flat_dram_block_window_tmp.get_bottom_tensor_view(), // from kernel gemm_pad_views
                make_tuple(number<flatNPerWarp>{}, number<flatKPerWarp>{}),
                b_flat_dram_block_window_tmp.get_window_origin(),
                b_flat_distribution);
        auto b_flat_dram_window_next = b_flat_dram_window;

        constexpr index_t MPerBlock = a_lds_gemm_window0.get_window_lengths()[number<0>{}];
        constexpr index_t KPerBlock = a_lds_gemm_window0.get_window_lengths()[number<1>{}];

        constexpr auto config = BlockFlatmm::BlockPolicy::template GetWarpGemmMWarpNWarp<Problem>();
        using WG              = remove_cvref_t<decltype(config.template at<0>())>;

        constexpr index_t MWarp = config.template at<1>();
        constexpr index_t NWarp = config.template at<2>();

        constexpr index_t MIterPerWarp = MPerBlock / (MWarp * WG::kM);
        constexpr index_t NIterPerWarp = BlockTile::at(idxN) / (WarpTile::at(idxN) * BlockWarps::at(idxN));
        constexpr index_t KIterPerWarp = KPerBlock / WG::kK;

        constexpr index_t MPerBlockPerIter = MPerBlock / MIterPerWarp;
        constexpr index_t KPerBlockPerIter = KPerBlock / KIterPerWarp;

        constexpr index_t NFlatPerBlockPerIter = BlockGemmShape::flatNPerWarp;
        constexpr index_t KFlatPerBlockPerIter = BlockGemmShape::flatKPerWarp;

        const index_t iMWarp = get_warp_id() / NWarp;

        // construct A-warp-window
        auto a_lds_warp_window0 = make_tile_window(
            a_lds_gemm_window0.get_bottom_tensor_view(),
            make_tuple(number<WG::kM>{}, number<WG::kK>{}),
            a_lds_gemm_window0.get_window_origin() + multi_index<2>{iMWarp * WG::kM, 0},
            make_static_tile_distribution(typename WG::AWarpDstrEncoding{}));
        auto a_lds_warp_window1 = make_tile_window(
            a_lds_gemm_window1.get_bottom_tensor_view(),
            make_tuple(number<WG::kM>{}, number<WG::kK>{}),
            a_lds_gemm_window1.get_window_origin() + multi_index<2>{iMWarp * WG::kM, 0},
            make_static_tile_distribution(typename WG::AWarpDstrEncoding{}));

        auto a_gemm_warp_window = a_lds_warp_window0;
        auto b_gemm_warp_window = b_flat_dram_window;

        using a_warp_tensors_type = decltype(load_tile(a_lds_warp_window0));
        using b_warp_tensors_type = decltype(load_tile(b_flat_dram_window));

        auto a_block_tile0 = decltype(load_tile(a_copy_dram_window)){};
        auto a_block_tile1 = decltype(load_tile(a_copy_dram_window)){};
        auto c_block_tile = block_flatmm.MakeCBlockTile();
        statically_indexed_array<statically_indexed_array<a_warp_tensors_type, KIterPerWarp>, MIterPerWarp> a_warp_tensors0;
        statically_indexed_array<statically_indexed_array<a_warp_tensors_type, KIterPerWarp>, MIterPerWarp> a_warp_tensors1;
        statically_indexed_array<statically_indexed_array<b_warp_tensors_type, KIterPerWarp>, NIterPerWarp> b_warp_tensors0;
        statically_indexed_array<statically_indexed_array<b_warp_tensors_type, KIterPerWarp>, NIterPerWarp> b_warp_tensors1;

        auto store_a_to_lds = [&](auto &a_block_tile, auto& a_copy_lds_window){
            if constexpr(std::is_same_v<ALayout, tensor_layout::gemm::ColumnMajor>)
            {
                auto a_shuffle_tmp = make_static_distributed_tensor<ADataType>(PipelinePolicy::template MakeShuffledARegBlockDistribution<Problem>());
                shuffle_tile(a_shuffle_tmp, a_block_tile);
                const auto a_block_tile_tmp = tile_elementwise_in(a_element_func, a_shuffle_tmp);
                store_tile(a_copy_lds_window, a_block_tile_tmp);
            }
            else
            {
                store_tile(a_copy_lds_window, tile_elementwise_in(a_element_func, a_block_tile));
            }
        };
        auto load_a_to_vgpr = [&](auto &warp_tensors, auto& lds_warp_window){
            static_for<0, MIterPerWarp, 1>{}([&](auto mIter) {
                static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                    a_gemm_warp_window = lds_warp_window;
                    move_tile_window(a_gemm_warp_window, {mIter * MPerBlockPerIter, kIter * KPerBlockPerIter});
                    warp_tensors(mIter)(kIter) = load_tile(a_gemm_warp_window);
                });
            });
        };
        auto load_b_to_vgpr = [&](auto &b_warp_tensors){
            static_for<0, NIterPerWarp, 1>{}([&](auto nIter) {
                static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                    b_gemm_warp_window = b_flat_dram_window;
                    move_tile_window(b_gemm_warp_window, {nIter * NFlatPerBlockPerIter, kIter * KFlatPerBlockPerIter});
                    b_warp_tensors(nIter)(kIter) = load_tile(b_gemm_warp_window);
                });
            });
        };

        using CWarpDstr   = typename WG::CWarpDstr;
        using CWarpTensor = typename WG::CWarpTensor;

        constexpr auto c_warp_y_lengths = to_sequence(CWarpDstr{}.get_ys_to_d_descriptor().get_lengths());
        constexpr auto c_warp_y_index_zeros = uniform_sequence_gen_t<CWarpDstr::NDimY, 0>{};

#ifdef FEIFEI_DEBUG
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[BLOCK ] num_loop = %d\n", num_loop);
            printf("[BLOCK ] MIterPerWarp = %d, NIterPerWarp = %d, KIterPerWarp = %d\n", MIterPerWarp, NIterPerWarp, KIterPerWarp);
        }
#endif

        // prefetch
        {
            // A0:vmem->vgpr
            a_block_tile0 = load_tile(a_copy_dram_window);
            // A0:vgpr->lds
            store_a_to_lds(a_block_tile0, a_copy_lds_window0);
            // A0:lds->vgpr
            block_sync_lds();
            load_a_to_vgpr(a_warp_tensors0, a_lds_warp_window0);

            if (num_loop >= 2)
            {
                // move to A1
                move_tile_window(a_copy_dram_window, {0, kKPerBlock});
                // A1:vmem->vgpr
                a_block_tile1 = load_tile(a_copy_dram_window);
                // A1:vgpr->lds
                store_a_to_lds(a_block_tile1, a_copy_lds_window1);
            }

            if (num_loop >= 3)
            {
                // move to A0+
                move_tile_window(a_copy_dram_window, {0, kKPerBlock});
                // A0+:vmem->vgpr
                a_block_tile0 = load_tile(a_copy_dram_window);
            }

            // B0:vmem->vgpr
            load_b_to_vgpr(b_warp_tensors0);

            if (num_loop >= 2)
                move_tile_window(b_flat_dram_window, {0, flatKPerBlock}); // move to B1
        }

        while(num_loop > 0)
        {
            if (num_loop >= 4)
                move_tile_window(a_copy_dram_window, {0, kKPerBlock}); // move to A1+

            // A1+:vmem->vgpr
            a_block_tile1 = load_tile(a_copy_dram_window);
            
            // A0+:vgpr->lds
            store_a_to_lds(a_block_tile0, a_copy_lds_window0);

            // A1:lds->vgpr
            block_sync_lds();
            load_a_to_vgpr(a_warp_tensors1, a_lds_warp_window1);

            // B1:vmem->vgpr
            load_b_to_vgpr(b_warp_tensors1);

            // C0 += A0 * B0
            static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                static_for<0, NIterPerWarp, 1>{}([&](auto nIter) {
                    auto b_gemm = b_warp_tensors0(nIter)(kIter);
    
                    static_for<0, MIterPerWarp, 1>{}([&](auto mIter) {
                        auto a_gemm = a_warp_tensors0(mIter)(kIter);
    
                        // read C warp tensor from C block tensor
                        CWarpTensor c_warp_tensor;
    
                        c_warp_tensor.get_thread_buffer() = c_block_tile.get_y_sliced_thread_data(
                            merge_sequences(sequence<mIter, nIter>{}, c_warp_y_index_zeros),
                            merge_sequences(sequence<1, 1>{}, c_warp_y_lengths));
    
                        // warp GEMM
                        WG{}(c_warp_tensor, a_gemm, b_gemm);
    
                        // write C warp tensor into C block tensor
                        c_block_tile.set_y_sliced_thread_data(
                            merge_sequences(sequence<mIter, nIter>{}, c_warp_y_index_zeros),
                            merge_sequences(sequence<1, 1>{}, c_warp_y_lengths),
                            c_warp_tensor.get_thread_buffer());
    
                    });
                });
            });

            // --------------------------------------------------------------------------------------
            if(num_loop == 1)
                return c_block_tile;
            num_loop--;
            move_tile_window(b_flat_dram_window, {0, flatKPerBlock}); // move to B0+
            // --------------------------------------------------------------------------------------

            if (num_loop >= 4)
                move_tile_window(a_copy_dram_window, {0, kKPerBlock}); // move to A0++

            // A0++:vmem->vgpr
            a_block_tile0 = load_tile(a_copy_dram_window);

            // A1+:vgpr->lds
            store_a_to_lds(a_block_tile1, a_copy_lds_window1);

            // A0+:lds->vgpr
            block_sync_lds();
            load_a_to_vgpr(a_warp_tensors0, a_lds_warp_window0);

            // B0+:vmem->vgpr
            load_b_to_vgpr(b_warp_tensors0);

            // C1 += A1 * B1
            static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                static_for<0, NIterPerWarp, 1>{}([&](auto nIter) {
                    auto b_gemm = b_warp_tensors1(nIter)(kIter);
    
                    static_for<0, MIterPerWarp, 1>{}([&](auto mIter) {
                        auto a_gemm = a_warp_tensors1(mIter)(kIter);
    
                        // read C warp tensor from C block tensor
                        CWarpTensor c_warp_tensor;
    
                        c_warp_tensor.get_thread_buffer() = c_block_tile.get_y_sliced_thread_data(
                            merge_sequences(sequence<mIter, nIter>{}, c_warp_y_index_zeros),
                            merge_sequences(sequence<1, 1>{}, c_warp_y_lengths));
    
                        // warp GEMM
                        WG{}(c_warp_tensor, a_gemm, b_gemm);
    
                        // write C warp tensor into C block tensor
                        c_block_tile.set_y_sliced_thread_data(
                            merge_sequences(sequence<mIter, nIter>{}, c_warp_y_index_zeros),
                            merge_sequences(sequence<1, 1>{}, c_warp_y_lengths),
                            c_warp_tensor.get_thread_buffer());
    
                    });
                });
            });

            num_loop--;
            move_tile_window(b_flat_dram_window, {0, flatKPerBlock}); // move to B1
        }
        
        return c_block_tile;
    }

    // ================================================================================================
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
