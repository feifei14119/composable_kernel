// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2024, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include "ck_tile/core.hpp"
#include "ck_tile/ops/flatmm/block/block_flatmm_asmem_bsmem_creg_v1_default_policy.hpp"

namespace ck_tile {

// A is block window on shared memory
// B is block window on shared memory
// C is block distributed tensor
template <typename Problem_, typename BlockPolicy_ = BlockFlatmmASmemBSmemCRegV1DefaultPolicy>
struct BlockFlatmmASmemBSmemCRegV1
{
    using Problem        = remove_cvref_t<Problem_>;
    using BlockPolicy    = remove_cvref_t<BlockPolicy_>;
    using ADataType      = remove_cvref_t<typename Problem::ADataType>;
    using BDataType      = remove_cvref_t<typename Problem::BDataType>;
    using CDataType      = remove_cvref_t<typename Problem::CDataType>;
    using BlockGemmShape = remove_cvref_t<typename Problem::BlockGemmShape>; // TileFlatmmShape

    static constexpr index_t kBlockSize = Problem::kBlockSize;

    CK_TILE_DEVICE static constexpr auto MakeCBlockTile()
    {
        constexpr index_t MPerBlock = BlockGemmShape::kM;
        constexpr index_t NPerBlock = BlockGemmShape::kN;

        constexpr auto config = BlockPolicy::template GetWarpGemmMWarpNWarp<Problem>();

        using WG = remove_cvref_t<decltype(config.template at<0>())>;

        constexpr index_t MWarp = config.template at<1>();
        constexpr index_t NWarp = config.template at<2>();

        constexpr index_t MIterPerWarp = MPerBlock / (MWarp * WG::kM);
        constexpr index_t NIterPerWarp = NPerBlock / (NWarp * WG::kN);

        constexpr auto c_block_outer_dstr_encoding = tile_distribution_encoding<
            sequence<>,
            tuple<sequence<MIterPerWarp, MWarp>, sequence<NIterPerWarp, NWarp>>,
            tuple<sequence<1, 2>>,
            tuple<sequence<1, 1>>,
            sequence<1, 2>,
            sequence<0, 0>>{};

        constexpr auto c_block_dstr_encode = detail::make_embed_tile_distribution_encoding(
            c_block_outer_dstr_encoding, typename WG::CWarpDstrEncoding{});

        constexpr auto c_block_dstr = make_static_tile_distribution(c_block_dstr_encode);

        auto c_block_tensor = make_static_distributed_tensor<CDataType>(c_block_dstr);
        return c_block_tensor;
    }

    // C += A * B
    template <typename CBlockTensor,
              typename ABlockWindow,
              typename BFlatBlockWindow
#ifdef FEIFEI_DEBUG
              ,
              typename BOriginBlockWindow
#endif
              >
    CK_TILE_DEVICE void operator()(CBlockTensor& c_block_tensor,
                                   const ABlockWindow& a_block_window,
                                   const BFlatBlockWindow& b_flat_block_window
#ifdef FEIFEI_DEBUG
                                   ,
                                   const BOriginBlockWindow& b_origin_block_window,
                                   int* dbg_int,
                                   float* dbg_fp32,
                                   void* dbg_f168
#endif
    ) const
    {
#ifdef FEIFEI_DEBUG
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[BLOCK ] BlockFlatmmASmemBSmemCRegV1():\n");
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
            dbg_int[gid * DEBUG_CNT + i]  = -1;
            dbg_fp32[gid * DEBUG_CNT + i] = -1.0f;
            dbg_f16[gid * DEBUG_CNT + i] = ck_tile::type_convert<ck_tile::half_t>(-1.0f);
        }
#endif
        static_assert(std::is_same_v<ADataType, typename ABlockWindow::DataType> &&
                          std::is_same_v<BDataType, typename BFlatBlockWindow::DataType> &&
                          std::is_same_v<CDataType, typename CBlockTensor::DataType>,
                      "wrong!");
        constexpr index_t MPerBlock = ABlockWindow{}.get_window_lengths()[number<0>{}];
        //constexpr index_t NPerBlock = BFlatBlockWindow{}.get_window_lengths()[number<0>{}];
        constexpr index_t NPerBlock = BOriginBlockWindow{}.get_window_lengths()[number<0>{}]; // feifei TODO: get from flat window
        constexpr index_t KPerBlock = ABlockWindow{}.get_window_lengths()[number<1>{}];

        static_assert(MPerBlock == BlockGemmShape::kM && NPerBlock == BlockGemmShape::kN &&
                          KPerBlock == BlockGemmShape::kK,
                      "wrong!");

        constexpr auto config = BlockPolicy::template GetWarpGemmMWarpNWarp<Problem>();
        using WG              = remove_cvref_t<decltype(config.template at<0>())>;

        constexpr index_t MWarp = config.template at<1>();
        constexpr index_t NWarp = config.template at<2>();

        constexpr index_t MIterPerWarp = MPerBlock / (MWarp * WG::kM);
        constexpr index_t NIterPerWarp = NPerBlock / (NWarp * WG::kN);
        constexpr index_t KIterPerWarp = KPerBlock / WG::kK;

        constexpr index_t MPerBlockPerIter = MPerBlock / MIterPerWarp;
        constexpr index_t NPerBlockPerIter = NPerBlock / NIterPerWarp;
        constexpr index_t KPerBlockPerIter = KPerBlock / KIterPerWarp;

        constexpr index_t NFlatPerBlockPerIter = BlockGemmShape::kFlatNPerBlock;
        constexpr index_t KFlatPerBlockPerIter = BlockGemmShape::kFlatKPerBlock;

        const index_t iMWarp = get_warp_id() / NWarp;
        const index_t iNWarp = get_warp_id() % NWarp;

        // construct A-warp-window
        auto a_warp_window_tmp = make_tile_window(
            a_block_window.get_bottom_tensor_view(),
            make_tuple(number<WG::kM>{}, number<WG::kK>{}),
            a_block_window.get_window_origin() + multi_index<2>{iMWarp * WG::kM, 0},
            make_static_tile_distribution(typename WG::AWarpDstrEncoding{}));
        statically_indexed_array<
            statically_indexed_array<decltype(a_warp_window_tmp), KIterPerWarp>,
            MIterPerWarp>
            a_warp_windows;
        static_for<0, MIterPerWarp, 1>{}([&](auto mIter) {
            static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                a_warp_windows(mIter)(kIter) = a_warp_window_tmp;

                move_tile_window(a_warp_windows(mIter)(kIter),
                                 {mIter * MPerBlockPerIter, kIter * KPerBlockPerIter});
            });
        });

        // construct Bflat-warp-window
        auto b_flat_warp_windows_tmp = b_flat_block_window;
        statically_indexed_array<
            statically_indexed_array<decltype(b_flat_warp_windows_tmp), KIterPerWarp>,
            NIterPerWarp>
            b_flat_warp_windows;
        static_for<0, NIterPerWarp, 1>{}([&](auto nIter) {
            static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                b_flat_warp_windows(nIter)(kIter) = b_flat_warp_windows_tmp;

                move_tile_window(b_flat_warp_windows(nIter)(kIter),
                                 {nIter * NFlatPerBlockPerIter, kIter * KFlatPerBlockPerIter});
            });
        });

#ifdef FEIFEI_DEBUG
        // construct B-warp-window
        auto b_origin_warp_window_tmp = make_tile_window(
            b_origin_block_window.get_bottom_tensor_view(),
            make_tuple(number<WG::kN>{}, number<WG::kK>{}),
            b_origin_block_window.get_window_origin() + multi_index<2>{iNWarp * WG::kN, 0},
            make_static_tile_distribution(typename WG::BWarpDstrEncoding{}));
        statically_indexed_array<
            statically_indexed_array<decltype(b_origin_warp_window_tmp), KIterPerWarp>,
            NIterPerWarp>
            b_origin_warp_windows;
        static_for<0, NIterPerWarp, 1>{}([&](auto nIter) {
            static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
                b_origin_warp_windows(nIter)(kIter) = b_origin_warp_window_tmp;

                move_tile_window(b_origin_warp_windows(nIter)(kIter),
                                 {nIter * NPerBlockPerIter, kIter * KPerBlockPerIter});
            });
        });

        // Warp loop in block:
        constexpr auto mIterDbg  = number<0>{};
        constexpr auto nIterDbg  = number<0>{};
        constexpr auto kIterDbg  = number<0>{};
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[BLOCK ] WG::kM = %d, WG::kM = %d, WG::kK = %d, WG::kKPerThread = %d\n", WG::kM, WG::kN, WG::kK, WG::kKPerThread);
            printf("[BLOCK ] MIterPerWarp = %d, NIterPerWarp = %d, KIterPerWarp = %d\n", MIterPerWarp, NIterPerWarp, KIterPerWarp);
        }

        // debug A lds read
        const auto a_warp_tensor_dbg = load_tile(a_warp_windows(mIterDbg)(kIterDbg));
        int a_warp_tile_size_per_thread = a_warp_tensor_dbg.get_thread_buffer_size();
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[BLOCK ] a_warp_tile_size_per_thread = %d\n", a_warp_tile_size_per_thread);
        }
        for(auto i = 0; i < a_warp_tile_size_per_thread; i++)
        {
            //dbg_f16[gid * DEBUG_CNT + i] = a_warp_tensor_dbg.get_thread_buffer()[i];
        }

        // debug B lds read
        const auto b_origin_warp_tensor_dbg = load_tile(b_origin_warp_windows(nIterDbg)(kIterDbg));
        int b_origin_warp_tile_size_per_thread = b_origin_warp_tensor_dbg.get_thread_buffer_size();
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[BLOCK ] b_origin_warp_tile_size_per_thread = %d\n", b_origin_warp_tile_size_per_thread);
        }
        for(auto i = 0; i < b_origin_warp_tile_size_per_thread; i++)
        {
            dbg_f16[gid * DEBUG_CNT + i + 0] = b_origin_warp_tensor_dbg.get_thread_buffer()[i];
        }
        
        // debug B flat read
        auto b_flat_warp_tensor_dbg  = load_tile(b_flat_warp_windows(nIterDbg)(kIterDbg));
        int b_flat_warp_size_per_thread = b_flat_warp_tensor_dbg.get_thread_buffer_size();
        if(threadIdx.x == 64 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[BLOCK ] b_flat_warp_size_per_thread = %d\n", b_flat_warp_size_per_thread);
        }
        for(auto i = 0; i < b_flat_warp_size_per_thread; i++)
        {
            dbg_f16[gid * DEBUG_CNT + i + b_origin_warp_tile_size_per_thread + 4] = b_flat_warp_tensor_dbg.get_thread_buffer()[i];
        }
#endif
        //auto b_warp_windows = b_origin_warp_windows;
        auto b_warp_windows = b_flat_warp_windows;

        using CWarpDstr   = typename WG::CWarpDstr;
        using CWarpTensor = typename WG::CWarpTensor;

        constexpr auto c_warp_y_lengths     = to_sequence(CWarpDstr{}.get_ys_to_d_descriptor().get_lengths());
        constexpr auto c_warp_y_index_zeros = uniform_sequence_gen_t<CWarpDstr::NDimY, 0>{};

        // hot loop:
        static_for<0, KIterPerWarp, 1>{}([&](auto kIter) {
            static_for<0, MIterPerWarp, 1>{}([&](auto mIter) {
                // read A warp tensor from A block window
                const auto a_warp_tensor = load_tile(a_warp_windows(mIter)(kIter));

                static_for<0, NIterPerWarp, 1>{}([&](auto nIter) {
                    // read B warp tensor from B Block window
                    const auto b_warp_tensor = load_tile(b_warp_windows(nIter)(kIter));

                    // read C warp tensor from C block tensor
                    CWarpTensor c_warp_tensor;

                    c_warp_tensor.get_thread_buffer() = c_block_tensor.get_y_sliced_thread_data(
                        merge_sequences(sequence<mIter, nIter>{}, c_warp_y_index_zeros),
                        merge_sequences(sequence<1, 1>{}, c_warp_y_lengths));

                    // warp GEMM
                    WG{}(c_warp_tensor, a_warp_tensor, b_warp_tensor);

                    // write C warp tensor into C block tensor
                    c_block_tensor.set_y_sliced_thread_data(
                        merge_sequences(sequence<mIter, nIter>{}, c_warp_y_index_zeros),
                        merge_sequences(sequence<1, 1>{}, c_warp_y_lengths),
                        c_warp_tensor.get_thread_buffer());
                });
            });
        });
    }

    // C = A * B
    template <typename ABlockTensorTmp,
              typename BFlatBlockWindow
#ifdef FEIFEI_DEBUG
              ,
              typename BOriginBlockWindow
#endif
              >
    CK_TILE_DEVICE auto operator()(const ABlockTensorTmp& a_block_tensor_tmp,
                                   const BFlatBlockWindow& b_flat_block_window
#ifdef FEIFEI_DEBUG
                                   ,
                                   const BOriginBlockWindow& b_origin_block_window,
                                   int* dbg_int,
                                   float* dbg_fp32,
                                   void* dbg_f168
#endif
    ) const
    {
        auto c_block_tensor = MakeCBlockTile();
        operator()(c_block_tensor,
                   a_block_tensor_tmp,
                   b_flat_block_window
#ifdef FEIFEI_DEBUG
                   ,
                   b_origin_block_window,
                   dbg_int,
                   dbg_fp32,
                   dbg_f168
#endif
        );
        return c_block_tensor;
    }

};

} // namespace ck_tile
