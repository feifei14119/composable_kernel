// SPDX-License-Identifier: MIT
// Copyright (c) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include <hip/hip_runtime.h>

#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>

#include "ck_tile/host.hpp"
#include "gemm_basic.hpp"

template <typename ALayout, typename BLayout, typename CLayout>
float gemm_calc(const ck_tile::GemmHostArgs& args, const ck_tile::stream_config& s)
{
    // The kPadM, kPadN, kPadK & kBlockPerCu should also come from the Codegen part.
    constexpr bool kPadM = false;
    constexpr bool kPadN = false;
    constexpr bool kPadK = false;

    constexpr bool kTilePermute = false;
    // The rank and permutation will also be generate out by the CodeGen part.
    constexpr ck_tile::index_t kOutputRank = 2;

    constexpr int kBlockPerCu = 1;

    // This part comes from the Codegen
    constexpr ck_tile::index_t M_Tile = 128;
    constexpr ck_tile::index_t N_Tile = 128;
    constexpr ck_tile::index_t K_Tile = 32;

    constexpr ck_tile::index_t M_Warp = 2;
    constexpr ck_tile::index_t N_Warp = 2;
    constexpr ck_tile::index_t K_Warp = 1;

    constexpr ck_tile::index_t M_Warp_Tile = 32;
    constexpr ck_tile::index_t N_Warp_Tile = 32;
    constexpr ck_tile::index_t K_Warp_Tile = 8;

    // Whether doing the CShuffle (transpose before the global memory), depending on the output
    // layout.
    constexpr bool CShuffleEpilogue =
        std::is_same_v<CLayout, ck_tile::tensor_layout::gemm::ColumnMajor>;

    using CodegenGemmShape =
        ck_tile::TileGemmShape<ck_tile::sequence<M_Tile, N_Tile, K_Tile>,
                               ck_tile::sequence<M_Warp, N_Warp, K_Warp>,
                               ck_tile::sequence<M_Warp_Tile, N_Warp_Tile, K_Warp_Tile>>;
    auto _gemm_shape = CodegenGemmShape{};
    (void)_gemm_shape;

    // --------------------------------------------------------------------------------------------------------------
    // discribe c store
    using TilePartitioner = ck_tile::GemmTile2DPartitioner<CodegenGemmShape>;
    auto _tile_part = TilePartitioner{};
    (void)_tile_part;

    using CShuffleEpilogueProblem = ck_tile::CShuffleEpilogueProblem<AccDataType,
                                                                   CDataType,
                                                                   kPadM,
                                                                   kPadN,
                                                                   kTilePermute,
                                                                   kOutputRank,
                                                                   1,
                                                                   0,
                                                                   TilePartitioner::MPerBlock,
                                                                   TilePartitioner::NPerBlock>;
    auto c_shuffle_problem = CShuffleEpilogueProblem{};
    (void)c_shuffle_problem;

    using CShuffleEpilogueT = ck_tile::CShuffleEpilogue<CShuffleEpilogueProblem>;
    auto c_shuffle = CShuffleEpilogueT{};
    (void)c_shuffle;

    using Default2DEpilogueProblem = ck_tile::Default2DEpilogueProblem<AccDataType, CDataType, kPadM, kPadN>;
    auto default_2d_problem = Default2DEpilogueProblem{};
    (void)default_2d_problem;

    using Default2DEpilogue = ck_tile::Default2DEpilogue<Default2DEpilogueProblem>;
    auto default_2d = Default2DEpilogue{};
    (void)default_2d;

    using GemmEpilogue = std::conditional_t<CShuffleEpilogue, CShuffleEpilogueT, Default2DEpilogue>;
    auto _gemm = GemmEpilogue{};
    (void)_gemm;
    // --------------------------------------------------------------------------------------------------------------

    using CodegenGemmTraits = ck_tile::TileGemmTraits<kPadM, kPadN, kPadK, ALayout, BLayout, CLayout>;
    auto gemm_trait = CodegenGemmTraits{};
    (void)gemm_trait;
    
    using CodegenPipelineProblem = ck_tile:: GemmPipelineProblem<ADataType, BDataType, AccDataType, CodegenGemmShape, CodegenGemmTraits>;
    auto pipe_problem = CodegenPipelineProblem{};
    (void)pipe_problem;
    
    using CodegenGemmPolicy = ck_tile::UniversalGemmPipelineAgBgCrPolicy;
    auto gemm_policy = CodegenGemmPolicy{};
    (void)gemm_policy;
    
    using CodegenGemmPipeline = ck_tile::GemmPipelineAGmemBGmemCRegV1<CodegenPipelineProblem, CodegenGemmPolicy>; // here
    auto gemm_pipe = CodegenGemmPipeline{};
    (void)gemm_pipe;
    
    using Kernel = ck_tile::GemmKernel<TilePartitioner, CodegenGemmPipeline, GemmEpilogue>;
    auto _kernel_impl = Kernel{};
    (void)_kernel_impl;
    
    auto kargs = Kernel::MakeKernelArgs(args);

    const dim3 grids      = Kernel::GridSize(args.M, args.N, args.k_batch);
    constexpr dim3 blocks = Kernel::BlockSize();
    printf("[FF] --- gemm_calc() ---\n");
    printf("[FF] BlockPerCu = %d\n", static_cast<int>(kBlockPerCu));
    printf("[FF] BlockTile M = %d\n", static_cast<int>(M_Tile));
    printf("[FF] BlockTile N = %d\n", static_cast<int>(N_Tile));
    printf("[FF] BlockTile K = %d\n", static_cast<int>(K_Tile));
    printf("[FF] WavePerBlock M = %d\n", static_cast<int>(M_Warp));
    printf("[FF] WavePerBlock N = %d\n", static_cast<int>(N_Warp));
    printf("[FF] WavePerBlock K = %d\n", static_cast<int>(K_Warp));
    printf("[FF] WaveTile M = %d\n", static_cast<int>(M_Warp_Tile));
    printf("[FF] WaveTile N = %d\n", static_cast<int>(N_Warp_Tile));
    printf("[FF] WaveTile K = %d\n", static_cast<int>(K_Warp_Tile));
    printf("[FF] grids = [%d, %d, %d]\n", grids.x, grids.y, grids.z);
    printf("[FF] blocks = [%d, %d, %d]\n", blocks.x, blocks.y, blocks.z);

    if(!Kernel::IsSupportedArgument(kargs))
    {
        throw std::runtime_error("Wrong! Arguments not supported! Skipping gemm!\n");
    }

    if(s.log_level_ > 0)
    {
        std::cout << "Launching kernel with args:"
                  << " grid: {" << grids.x << ", " << grids.y << ", " << grids.z << "}"
                  << ", blocks: {" << blocks.x << ", " << blocks.y << ", " << blocks.z << "}"
                  << std::endl;
    }

    auto _kernel_launch =  ck_tile::make_kernel<blocks.x, kBlockPerCu>(_kernel_impl, grids, blocks, 0, kargs);

    float ave_time = ck_tile::launch_kernel(s, _kernel_launch);
    // float ave_time = 1.0f;

    return ave_time;
}

#include "run_gemm_example.inc"

int main(int argc, char* argv[]) { return !run_gemm_example(argc, argv); }
