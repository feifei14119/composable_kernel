// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2024, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include "ck_tile/core.hpp"
#include "ck_tile/ops/gemm/warp/warp_gemm.hpp"
#include "ck_tile/ops/flatmm/block/flatmm_uk_config.hpp"

namespace ck_tile {

struct Flatmm_ff_128x128x128_1x4x1_16x16x32_Base // for f8/bf8
{
    int M = 0;
    int N = 0;
    int K = 0;
    static constexpr index_t Block_M = 128;
    static constexpr index_t Block_N = 128;
    static constexpr index_t Block_K = 128;

    static constexpr index_t WavePerBlock_M = 1;
    static constexpr index_t WavePerBlock_N = 4;
    static constexpr index_t WavePerBlock_K = 1;

    static constexpr index_t NumWaves = 4;

    static constexpr index_t Wave_M = 16;
    static constexpr index_t Wave_N = 16;
    static constexpr index_t Wave_K = 32;

    static constexpr index_t BlockSize = 256;

    static constexpr index_t SubKPacks = 4; // this is used to gurantee every threads can do dwordx4

    // TODO: note Nr/Kr/W need consider SubKPacks
    static constexpr index_t Block_W  = Wave_N * Wave_K;  // 512 element
    static constexpr index_t Block_Mr = Block_M / Wave_M; // 2
    static constexpr index_t Block_Nr = Block_N / Wave_N; // 8
    static constexpr index_t Block_Kr = Block_K / Wave_K; // 8

    static constexpr index_t Repeat_M = Block_M / (Wave_M * WavePerBlock_M); // 2
    static constexpr index_t Repeat_N = Block_N / (Wave_N * WavePerBlock_N); // 2
    static constexpr index_t Repeat_K = Block_K / (Wave_K * WavePerBlock_K); // 8

    static constexpr index_t APerDword = 4;

    CK_TILE_DEVICE void Print()
    {
        if(threadIdx.x == 0 && blockIdx.x == 0 && threadIdx.y == 0 && blockIdx.y == 0)
        {
            printf("[UK] Block_M = %d, Block_N = %d, Block_K = %d\n",
                static_cast<int>(Block_M),
                static_cast<int>(Block_N),
                static_cast<int>(Block_K));
            printf("[UK] Wave_M = %d, Wave_N = %d, Wave_K = %d, BlockSize = %d\n",
                static_cast<int>(Wave_M),
                static_cast<int>(Wave_N),
                static_cast<int>(Wave_K),
                static_cast<int>(BlockSize));
            printf("[UK] WavePerBlock_M = %d, WavePerBlock_N = %d, WavePerBlock_K = %d, NumWaves = %d\n",
                static_cast<int>(WavePerBlock_M),
                static_cast<int>(WavePerBlock_N),
                static_cast<int>(WavePerBlock_K),
                static_cast<int>(NumWaves));
            printf("[UK] Repeat_M = %d, Repeat_N = %d, Repeat_K = %d\n",
                static_cast<int>(Repeat_M),
                static_cast<int>(Repeat_N),
                static_cast<int>(Repeat_K));
            printf("[UK] Block_Mr = %d, Block_Nr = %d, Block_Kr = %d, Block_W = %d\n",
                static_cast<int>(Block_Mr),
                static_cast<int>(Block_Nr),
                static_cast<int>(Block_Kr),
                static_cast<int>(Block_W));
            printf("[UK] K = %d, loop_cnt = %d\n", static_cast<int>(K), static_cast<int>(K / Block_K));
        }
    } 

    static CK_TILE_DEVICE constexpr auto MakeCBlockDist()
    {
        constexpr auto c_block_outer_dstr_encoding = tile_distribution_encoding<
            sequence<>,
            tuple<sequence<Repeat_M, WavePerBlock_M>, sequence<Repeat_N, WavePerBlock_N>>,
            tuple<sequence<1, 2>>,
            tuple<sequence<1, 1>>,
            sequence<2, 1>, // !! note here is different
            sequence<0, 0>>{};

        using WG = WarpGemmMfmaF16F16F32M16N16K32TransposedCDistribution;

        constexpr auto c_block_dstr_encode = detail::make_embed_tile_distribution_encoding(
            c_block_outer_dstr_encoding, typename WG::CWarpDstrEncoding{});
        constexpr auto c_block_dstr = make_static_tile_distribution(c_block_dstr_encode);
        return c_block_dstr;
    }

    static CK_TILE_DEVICE constexpr auto MakeCBlockTile()
    {
        using CDataType             = float;
        constexpr auto c_block_dstr = MakeCBlockDist();
        auto c_block_tensor         = make_static_distributed_tensor<CDataType>(c_block_dstr);
        return c_block_tensor;
    }

    CK_TILE_HOST_DEVICE static constexpr auto MakeLdsStoreDesc_A()
    {
        // A async->LDS
        // constexpr index_t Block_M = Problem::BlockShape::Block_M0;
        // constexpr index_t Block_K = Problem::BlockShape::Block_K0;
        // constexpr index_t BlockSize = Problem::BlockShape::BlockSize;
        constexpr index_t warpSize = ck_tile::get_warp_size(); // 64
        // constexpr index_t NumWaves = Problem::BlockShape::NumWaves;

        constexpr index_t KPack_  = 16;      // GetSmemKPack_A<Problem>(); // LDS
        constexpr index_t KVector = APerDword;      // GetAlignment_A<Problem>(); // async copy 1 dword // 4
        constexpr index_t KPad    = KPack_; // pad between warps

        static_assert(Block_K % KVector == 0); // 128 % 4
        constexpr index_t LanesPerK = Block_K / KVector; // how many thread loading K // 128 / 4 = 32

            static_assert(LanesPerK == 32);
            static_assert(warpSize == 64);
        if constexpr(LanesPerK >= warpSize)
        {
            // need multiple waves to load K
            //static_assert(LanesPerK % warpSize == 0);
            constexpr index_t wavesPerK = LanesPerK / warpSize;
            if constexpr(wavesPerK > NumWaves)
            {
                // TODO: need multiple issues along K to load all data
            }
            else
            {
                /*constexpr index_t wavesPerM     = NumWaves / wavesPerK;
                constexpr index_t NumIssues     = Block_M / wavesPerM;
                constexpr auto lds_block_desc_0 = make_naive_tensor_descriptor(
                    make_tuple(number<NumIssues>{},                             // m0
                               number<wavesPerM>{},                             // m1
                               number<wavesPerK>{},                             // k0
                               number<warpSize>{},                              // k1
                               number<KVector>{}),                              // k2
                    make_tuple(number<NumWaves*(warpSize * KVector + KPad)>{},  // m0
                               number<wavesPerK*(warpSize * KVector + KPad)>{}, // m1
                               number<warpSize * KVector + KPad>{},             // k0
                               number<KVector>{},                               // k1
                               number<1>{}),                                    // k2
                    number<KVector>{}, // lds store vector(actually no explicit store)
                    number<1>{});

                constexpr auto lds_block_desc_issues_warps_lanes = transform_tensor_descriptor(
                    lds_block_desc_0,
                    make_tuple(
                        make_pass_through_transform(number<NumIssues>{}),
                        make_merge_transform(make_tuple(number<wavesPerM>{}, number<wavesPerK>{})),
                        make_merge_transform(make_tuple(number<warpSize>{}, number<KVector>{}))),
                    make_tuple(sequence<0>{}, sequence<1, 2>{}, sequence<3, 4>{}),
                    make_tuple(sequence<0>{}, sequence<1>{}, sequence<2>{}));

                return lds_block_desc_issues_warps_lanes;*/
                return 0;
            }
        }
        else
        {
            // lanes within a wave load different M but same K
            static_assert(warpSize % LanesPerK == 0);
            constexpr index_t LaneGroups = warpSize / LanesPerK; // along m
            constexpr index_t NumIssues  = Block_M / (LaneGroups * NumWaves);

            constexpr auto lds_block_desc_0 = make_naive_tensor_descriptor(
                make_tuple(number<NumIssues>{},                            // m0
                           number<LaneGroups>{},                           // m1
                           number<NumWaves>{},                             // m2
                           number<LanesPerK>{},                            // k0
                           number<KVector>{}),                             // k1
                make_tuple(number<NumWaves*(warpSize * KVector + KPad)>{}, // m0
                           number<Block_K>{},                              // m1
                           number<warpSize * KVector + KPad>{},            // m2
                           number<KVector>{},                              // k0
                           number<1>{}),                                   // k1
                number<KVector>{}, // lds store vector(actually no explicit store)
                number<1>{});

            constexpr auto lds_block_desc_issues_warps_lanes = transform_tensor_descriptor(
                lds_block_desc_0,
                make_tuple(make_pass_through_transform(number<NumIssues>{}),
                           make_pass_through_transform(number<NumWaves>{}),
                           make_merge_transform(make_tuple(
                               number<LaneGroups>{}, number<LanesPerK>{}, number<KVector>{}))),
                make_tuple(sequence<0>{}, sequence<2>{}, sequence<1, 3, 4>{}),
                make_tuple(sequence<0>{}, sequence<1>{}, sequence<2>{}));

            return lds_block_desc_issues_warps_lanes;
        }
    }

    // template <typename Problem>
    CK_TILE_HOST_DEVICE static constexpr auto MakeLdsLoadDesc_A()
    {
        // load from LDS to register, every wave has same layout
        constexpr index_t KPack_ = 16;      // GetSmemKPack_A<Problem>(); // LDS
        constexpr index_t KPad   = KPack_; // pad between warps

        constexpr index_t kAMLane     = 16; // 16 thread in M(row) direction
        constexpr index_t kABKLane    = 4;
        constexpr index_t kABKPerLane = 4;
        constexpr index_t kKIter      = 4;
        static_assert(KPack_ == (kABKPerLane * kKIter));

        constexpr auto lds_block_desc_0 =
            make_naive_tensor_descriptor(make_tuple(number<Repeat_M>{}, // m0 y
                                                    number<kAMLane>{},  // m1 p
                                                    number<Repeat_K>{}, // k0 y
                                                    number<kABKLane>{}, // k1 p
                                                    number<KPack_>{}),  // k2 y-vector
                                         make_tuple(number<kAMLane*(Block_K + KPad)>{}, // m0
                                                    number<Block_K + KPad>{},           // m1
                                                    number<kABKLane * KPack_>{},        // k0
                                                    number<KPack_>{},                   // k1
                                                    number<1>{}),                       // k2
                                         number<KPack_>{}, // lds load vector
                                         number<1>{});

        constexpr auto lds_desc_m_k = transform_tensor_descriptor(
            lds_block_desc_0,
            make_tuple(make_merge_transform(make_tuple(number<Repeat_M>{}, number<kAMLane>{})),
                       make_merge_transform(
                           make_tuple(number<Repeat_K>{}, number<kABKLane>{}, number<KPack_>{}))),
            make_tuple(sequence<0, 1>{}, sequence<2, 3, 4>{}),
            make_tuple(sequence<0>{}, sequence<1>{}));

        return lds_desc_m_k;
    }

    static constexpr auto GetGemm_AWaveEnc()
    {
        constexpr index_t kAMLane     = 16; // 16 thread in M(row) direction
        constexpr index_t kABKLane    = 4;
        constexpr index_t kABKPerLane = 4;  // every thread read A count in K direction
        constexpr index_t kKIter      = 4;

        using enc_ = tile_distribution_encoding<
            sequence<>,
            tuple<sequence<kAMLane>, sequence<kABKLane, kABKPerLane * kKIter>>,
            tuple<sequence<2, 1>>,
            tuple<sequence<0, 0>>,
            sequence<2>,
            sequence<1>>;
        return enc_{};
    }

    CK_TILE_HOST_DEVICE static constexpr ck_tile::index_t GetSmemSize()
    {
        return 32 * (128 + 8) * sizeof(bf16_t);
    }
};

struct Flatmm_ff_128x128x128_1x4x1_16x16x32_FP8 : public Flatmm_ff_128x128x128_1x4x1_16x16x32_Base
{
    using ADataType = fp8_t;
    using BDataType = fp8_t;
    using ODataType = fp16_t;

    // TODO: need paired with tile_window_linear!
    // TODO: need call init_raw() before call this function!
    template <typename ARes, typename ACoords, typename BRes, typename BCoords>
    CK_TILE_DEVICE auto
    operator()(const ARes& res_a,
               const ACoords& cached_coords_a,
               const BRes& res_b,
               const BCoords& cached_coords_b,
               const void* a_scale_ptr,
               const void* b_scale_ptr,
               void * d_ptr,
               const int b_scale_idx0,
               const int b_scale_step,
               CK_TILE_LDS_ADDR void* smem,
               index_t k,
               index_t tile_offset_a, // for each tile, the offset to move for each unroll
               index_t tile_offset_b
#ifdef FEIFEI_DEBUG
               ,              
               int * dbg_int,
               char* dbg_fp8,
               void* dbg_bf16,
               float* dbg_fp32
#endif
               )
    {
#ifdef FEIFEI_DEBUG
        Print();

        uint32_t tidx = threadIdx.x;
        uint32_t tidy = threadIdx.y;
        uint32_t bidx = blockIdx.x;
        uint32_t bidy = blockIdx.y;
        uint32_t bdmx = blockDim.x;
        uint32_t bdmy = blockDim.y;
        uint32_t gdmx = gridDim.x;
        uint32_t gdmy = gridDim.y;
        //uint32_t gid  = ((bdmx * bdmy) * gdmx) * bidy + (bdmx * bdmy) * bidx + bdmx * tidy + tidx;
        uint32_t gid  = ((bdmx * bdmy) * gdmx) * bidy + (bdmx * bdmy) * bidx + tidx;

        half_t* dbg_f16 = reinterpret_cast<half_t*>(dbg_bf16);
        for(int i = 0; i < DEBUG_CNT; i++)
        {
            dbg_int[gid * DEBUG_CNT + i]  = 1;
            dbg_fp32[gid * DEBUG_CNT + i] = 1.0f;
            dbg_f16[gid * DEBUG_CNT + i]  = ck_tile::type_convert<ck_tile::half_t>(1.0f);
        }
#endif

        static_assert(ACoords::size() == Block_M * Block_K / BlockSize / APerDword);
        //static_assert(BCoords::size() == Repeat_N);

        auto a_sst = make_tile_window(
            make_tensor_view<address_space_enum::lds>(
                reinterpret_cast<CK_TILE_LDS_ADDR ADataType*>(smem), MakeLdsStoreDesc_A()),
            MakeLdsStoreDesc_A().get_lengths(),
            {0, 0, 0});

        auto a_sld = [&]() {
            constexpr auto a_warp_enc_      = GetGemm_AWaveEnc();
            constexpr auto a_outer_dstr_enc = tile_distribution_encoding<
                sequence<WavePerBlock_M>,
                tuple<sequence<Repeat_M, WavePerBlock_M>, sequence<Repeat_K>>,
                tuple<sequence<1, 0>>,
                tuple<sequence<1, 0>>,
                sequence<1, 2>,
                sequence<0, 0>>{};
            constexpr auto a_block_dstr_encode =
                detail::make_embed_tile_distribution_encoding(a_outer_dstr_enc, a_warp_enc_);
            return make_tile_window_linear(
                make_tensor_view<address_space_enum::lds>(
                    reinterpret_cast<CK_TILE_LDS_ADDR ADataType*>(smem), MakeLdsLoadDesc_A()),
                MakeLdsLoadDesc_A().get_lengths(),
                {0, 0},
                make_static_tile_distribution(a_block_dstr_encode));
        }();

        const index_t tile_offset_a_bytes = tile_offset_a * sizeof(ADataType);
        const index_t tile_offset_b_bytes = tile_offset_b * sizeof(BDataType);

        const auto [m0_init_value, size_per_issue] = get_async_store_smem_info(a_sst);
        constexpr auto smem_buf_size = MakeLdsStoreDesc_A().get_element_space_size() * sizeof(ADataType);
            
        /*constexpr auto sld_os = generate_tuple(
            [&](auto i_access) {
                return number<a_sld.get_bottom_linear_offset(i_access) * sizeof(ADataType)>{};
            },
            number<a_sld.get_num_of_access()>{});*/

        index_t loop_cnt = k / Block_K;
        index_t s_loop_cnt = __builtin_amdgcn_readfirstlane(loop_cnt); // current block base row idx

        int mfmaM = 16;
        int tileM = 128;
        int tileK = 128;
        int waveNum = 4;
        int waveSize = 64;
        int waveIdx = threadIdx.x / waveSize;
        int tidInWave = threadIdx.x % waveSize;
        int thdPerBlock = waveSize * waveNum; // 64 * 4 = 256
        int dbg_tid = threadIdx.x * 16;

        //index_t v_os_slda = (static_cast<index_t>(a_sld.cached_coords_[number<0>{}].get_offset() * sizeof(ADataType))) % ((256+16) * 16);
        int ldsRowIdx = threadIdx.x % 16;
        int ldsReadVec = 16; // ds_read_b128
        int ldsColIdx = tidInWave / 16 * ldsReadVec;
        int ldsWidth = 128; // tile_K
        index_t v_os_slda = static_cast<index_t>(ldsRowIdx * ldsWidth + ldsColIdx);

#define A_VM_LDS true
#if A_VM_LDS
        // buffer load dword to lds A address
        int ldsAPad = 4 * 8; // pad = 8 dword
        int ldsAPadRound = 4 * 4; // pad = 4 dword
        int elementPerVmInst = 4; // dword load 4 A
        int thdPerK = tileK / elementPerVmInst; // 128 / 4 = 32
        int thdPerM = thdPerBlock / thdPerK; // 256 / 32 = 8
        int repeatARound = tileM / thdPerM; // 128 / 8 = 16

        int vmARowCntPerBank = waveNum;             // 4
        int vmARowBankIdx = tidInWave / thdPerK;    // 0, 1
        int vmARowIdxInBank = waveIdx;              // 0, 1, 2, 3
        int vmARowIdxInRound = vmARowBankIdx * vmARowCntPerBank + vmARowIdxInBank;
        int vmARowCntPerRound = thdPerM;

        int vmABlkIdx = blockIdx.y;
        int vmABlkRowIdx = vmABlkIdx * Block_M;

        int vmARowIdx = vmARowIdxInRound + vmABlkRowIdx;
        int vmAColIdx = threadIdx.x % thdPerK * elementPerVmInst;
        // address count = repeatARound
        int AvmLdAddr0  = (vmARowIdx + vmARowCntPerRound * 0 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr1  = (vmARowIdx + vmARowCntPerRound * 1 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr2  = (vmARowIdx + vmARowCntPerRound * 2 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr3  = (vmARowIdx + vmARowCntPerRound * 3 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr4  = (vmARowIdx + vmARowCntPerRound * 4 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr5  = (vmARowIdx + vmARowCntPerRound * 5 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr6  = (vmARowIdx + vmARowCntPerRound * 6 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr7  = (vmARowIdx + vmARowCntPerRound * 7 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr8  = (vmARowIdx + vmARowCntPerRound * 8 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr9  = (vmARowIdx + vmARowCntPerRound * 9 ) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr10 = (vmARowIdx + vmARowCntPerRound * 10) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr11 = (vmARowIdx + vmARowCntPerRound * 11) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr12 = (vmARowIdx + vmARowCntPerRound * 12) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr13 = (vmARowIdx + vmARowCntPerRound * 13) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr14 = (vmARowIdx + vmARowCntPerRound * 14) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr15 = (vmARowIdx + vmARowCntPerRound * 15) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        
        int ldsAWaveWidth = waveSize * sizeof(ADataType) * elementPerVmInst + ldsAPad;
        int AvmLdsM0 = waveIdx * ldsAWaveWidth;
        int AvmLdsM0Offset = waveNum * ldsAWaveWidth - ldsAPad + ldsAPadRound;
        int AldsPingPangSize = AvmLdsM0Offset * repeatARound - ldsAPadRound;
        
        int sAvmLdsM0 = __builtin_amdgcn_readfirstlane(AvmLdsM0);
        int sAvmLdsM0Offset = __builtin_amdgcn_readfirstlane(AvmLdsM0Offset);
        int sAldsPingPangSize = __builtin_amdgcn_readfirstlane(AldsPingPangSize);

        // lds read 128b A address
        int elementPerLdsInst = 16; // ds read 128b = 16 A
        int ldsARdThdPerMPerBank = 8;
        int ldsARdThdPerMPerMfma = mfmaM; // 16
        int ldsARowBankIdx = (tidInWave % ldsARdThdPerMPerBank) / 4; // 0~3=0; 4~7=1; ...
        int ldsARowIdxInBank = tidInWave % 4; // 0~3
        int ldsAColBankIdx = (tidInWave % ldsARdThdPerMPerMfma) / ldsARdThdPerMPerBank; // 0~7=0; 8~15=1; ...
        int ldsAColIdxInBank = (tidInWave / ldsARdThdPerMPerMfma); // 0~15=0; 16~31=1; 32~47=2; 48~63=3;

        int ldsARowBankSize = AvmLdsM0Offset; 
        int ldsARowRowIdxInBankSize = ldsAWaveWidth; 
        int ldsAColBankSize = tileK;
        int ldsAColIdxInBankSize = elementPerLdsInst * sizeof(ADataType);
        int AldsRdAddr =
            ldsARowBankSize * ldsARowBankIdx + ldsARowRowIdxInBankSize * ldsARowIdxInBank +
            ldsAColBankSize * ldsAColBankIdx + ldsAColIdxInBankSize * ldsAColIdxInBank;

        int AldsRdOffsetInCol = 64 * sizeof(ADataType);
        int AldsRdOffsetInRow = ldsARowBankSize * 2;
#else
        // buffer load dwordx4 A address
        int tileK = 128;
        int elementPerInst = 16; // dwordx4 load 16 A
        int thdPerK = tileK / elementPerInst; // 128 / 16 = 8
        int waveNum = 4;
        int waveSize = 64;
        int thdPerBlock = waveSize * waveNum; // 64 * 4 = 256
        int thdPerM = thdPerBlock / thdPerK; // 256 / 8 = 32
        int tileM = 128;
        int repeatA = tileM / thdPerM; // 128 / 32 = 4
        int vmABlkIdx = blockIdx.y;
        int vmABlkRowIdx = vmABlkIdx * Block_M;
        int vmARowIdx = threadIdx.x / thdPerK + vmABlkRowIdx;
        int vmAColIdx = threadIdx.x % thdPerK * elementPerInst;
        // address count = repeatA
        int AvmLdAddr0 = (vmARowIdx + thdPerM * 0) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr1 = (vmARowIdx + thdPerM * 1) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr2 = (vmARowIdx + thdPerM * 2) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);
        int AvmLdAddr3 = (vmARowIdx + thdPerM * 3) * K * sizeof(ADataType) + vmAColIdx * sizeof(ADataType);

        // lds write 128b A address
        int ldsAPad = 4; // pad = 1 dword
        int ldsAWidth = tileK * sizeof(ADataType) + ldsAPad; // 128 + 4 = 132
        int AldsARowIdx = threadIdx.x / thdPerK;
        int AldsAColIdx = threadIdx.x % thdPerK * elementPerInst;
        int AldsWrAddr = AldsARowIdx * ldsAWidth + AldsAColIdx * sizeof(ADataType);
        int AldsWrOffset = thdPerM * ldsAWidth;
        int AldsWrAddr0 = AldsWrAddr + AldsWrOffset * 0;
        int AldsWrAddr1 = AldsWrAddr + AldsWrOffset * 1;
        int AldsWrAddr2 = AldsWrAddr + AldsWrOffset * 2;
        int AldsWrAddr3 = AldsWrAddr + AldsWrOffset * 3;
        int AldsPingPangSize = ldsAWidth * tileM - ldsAPad; // 132 * 128 = 16896 byte

        // lds read 128b A address
        int mfmaM = 16;
        int elementPerLdsInst = 16; // ds read 128b = 16 A
        int ldsARdThdPerM = mfmaM; // 16
        int thdInWave = threadIdx.x % waveSize; // 0~63
        int ldsARdRowIdx = thdInWave % ldsARdThdPerM; // 0~15
        int ldsAColBlkIdx = thdInWave / ldsARdThdPerM; // 0~3
        int ldsAColPerBlk = elementPerLdsInst; // 16
        int ldsARdBlkNum = waveSize / ldsARdThdPerM; // 64 / 16 = 4
        int ldsARdColPerRd = ldsAColPerBlk * ldsARdBlkNum; // 16 * 4 = 64
        int ldsARdColInRead0 = ldsAColPerBlk * ldsAColBlkIdx; // 0,16,32,48
        int ldsARdColInRead1 = ldsAColPerBlk * ldsAColBlkIdx + ldsARdColPerRd; // 64,80,96,112
        int AldsRdAddr = ldsARdRowIdx * ldsAWidth + ldsARdColInRead0 * sizeof(ADataType);
        int AldsRdOffsetInCol = ldsARdColPerRd * sizeof(ADataType);
        int AldsRdOffsetInRow = ldsARdThdPerM * ldsAWidth;
#endif

        // buffer load dowrdx4 B address
        int mfmaN = 16;
        int mfmaK = 16;
        int warpNum = 4;
        int flatBvmLdOffset = mfmaN * mfmaK * warpNum; // 1024

#if A_VM_LDS
        // buffer load dword SA address
        int saBlkColId = blockIdx.y * Block_M;
        int tidInMfmaM = tidInWave % 16;
        int swz1 = tidInMfmaM / 4 / 2;
        int swz2 = tidInMfmaM / 4 % 2;
        int swz3 = swz2 * 2 + swz1;
        int MIdxInMfmaM = swz3 * 4 +  tidInMfmaM % 4;
        int saColIdx = MIdxInMfmaM + saBlkColId;
        //int saColIdx = threadIdx.x % mfmaM + saBlkColId; // 0~15

        int saRowIdx = 0;
        int SAvmLdAddr = saRowIdx * M * sizeof(float) + saColIdx * sizeof(float);
        auto res_sa = make_wave_buffer_resource(reinterpret_cast<const float*>(a_scale_ptr), M * K/128 * sizeof(float));
        int SAvmLdOffset = mfmaM * sizeof(float);
        int tSAvmTileOffset = M * sizeof(float);
        int SAvmTileOffset = __builtin_amdgcn_readfirstlane(tSAvmTileOffset);
#else
        // buffer load dword SA address
        int saBlkColId = blockIdx.y * Block_M;
        int saColIdx = threadIdx.x % mfmaM + saBlkColId; // 0~15
        int saRowIdx = 0;
        int SAvmLdAddr = saRowIdx * M * sizeof(float) + saColIdx * sizeof(float);
        auto res_sa = make_wave_buffer_resource(reinterpret_cast<const float*>(a_scale_ptr), M * K/128 * sizeof(float));
        int SAvmLdOffset = mfmaM * sizeof(float);
        int tSAvmTileOffset = M * sizeof(float);
        int SAvmTileOffset = __builtin_amdgcn_readfirstlane(tSAvmTileOffset);
#endif        

        // s load dword SB address
        const float * sb_cfp32_ptr = reinterpret_cast<const float*>(b_scale_ptr);
        float * sb_fp32_ptr = const_cast<float*>(sb_cfp32_ptr);
        float * sb_blk_ptr = sb_fp32_ptr + blockIdx.x;
        auto res_sb = make_wave_buffer_resource(reinterpret_cast<const float*>(sb_blk_ptr), (N/128) * (K/128) * sizeof(float));
        int tSBvmTileOffset = N / 128 * sizeof(float);
        int SBvmTileOffset = __builtin_amdgcn_readfirstlane(tSBvmTileOffset);

#pragma region ASM
        register float acc_0x asm("v64");
        register float acc_0y asm("v65");
        register float acc_0z asm("v66");
        register float acc_0w asm("v67");
        register float acc_1x asm("v68");
        register float acc_1y asm("v69");
        register float acc_1z asm("v70");
        register float acc_1w asm("v71");
        register float acc_2x asm("v72");
        register float acc_2y asm("v73");
        register float acc_2z asm("v74");
        register float acc_2w asm("v75");
        register float acc_3x asm("v76");
        register float acc_3y asm("v77");
        register float acc_3z asm("v78");
        register float acc_3w asm("v79");
        register float acc_4x asm("v80");
        register float acc_4y asm("v81");
        register float acc_4z asm("v82");
        register float acc_4w asm("v83");
        register float acc_5x asm("v84");
        register float acc_5y asm("v85");
        register float acc_5z asm("v86");
        register float acc_5w asm("v87");
        register float acc_6x asm("v88");
        register float acc_6y asm("v89");
        register float acc_6z asm("v90");
        register float acc_6w asm("v91");
        register float acc_7x asm("v92");
        register float acc_7y asm("v93");
        register float acc_7z asm("v94");
        register float acc_7w asm("v95");
        register float acc_8x  asm("v96");
        register float acc_8y  asm("v97");
        register float acc_8z  asm("v98");
        register float acc_8w  asm("v99");
        register float acc_9x  asm("v100");
        register float acc_9y  asm("v101");
        register float acc_9z  asm("v102");
        register float acc_9w  asm("v103");
        register float acc_10x asm("v104");
        register float acc_10y asm("v105");
        register float acc_10z asm("v106");
        register float acc_10w asm("v107");
        register float acc_11x asm("v108");
        register float acc_11y asm("v109");
        register float acc_11z asm("v110");
        register float acc_11w asm("v111");
        register float acc_12x asm("v112");
        register float acc_12y asm("v113");
        register float acc_12z asm("v114");
        register float acc_12w asm("v115");
        register float acc_13x asm("v116");
        register float acc_13y asm("v117");
        register float acc_13z asm("v118");
        register float acc_13w asm("v119");
        register float acc_14x asm("v120");
        register float acc_14y asm("v121");
        register float acc_14z asm("v122");
        register float acc_14w asm("v123");
        register float acc_15x asm("v124");
        register float acc_15y asm("v125");
        register float acc_15z asm("v126");
        register float acc_15w asm("v127");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winline-asm"
        // clang-format off
        asm volatile(
#define _A_VM_LDS_ A_VM_LDS
#include "uk/flatmm_f8_uk_gfx9_128x128x128_1x4x1_16x16x32.inc"
    "  s_nop 2 \n"
#undef _A_VM_LDS_
            :   [s_loop_cnt]"+s"(s_loop_cnt),
                [v_c_0x]"+v"(acc_0x),
                [v_c_0y]"+v"(acc_0y),
                [v_c_0z]"+v"(acc_0z),
                [v_c_0w]"+v"(acc_0w),
                [v_c_1x]"+v"(acc_1x),
                [v_c_1y]"+v"(acc_1y),
                [v_c_1z]"+v"(acc_1z),
                [v_c_1w]"+v"(acc_1w),
                [v_c_2x]"+v"(acc_2x),
                [v_c_2y]"+v"(acc_2y),
                [v_c_2z]"+v"(acc_2z),
                [v_c_2w]"+v"(acc_2w),
                [v_c_3x]"+v"(acc_3x),
                [v_c_3y]"+v"(acc_3y),
                [v_c_3z]"+v"(acc_3z),
                [v_c_3w]"+v"(acc_3w),
                [v_c_4x]"+v"(acc_4x),
                [v_c_4y]"+v"(acc_4y),
                [v_c_4z]"+v"(acc_4z),
                [v_c_4w]"+v"(acc_4w),
                [v_c_5x]"+v"(acc_5x),
                [v_c_5y]"+v"(acc_5y),
                [v_c_5z]"+v"(acc_5z),
                [v_c_5w]"+v"(acc_5w),
                [v_c_6x]"+v"(acc_6x),
                [v_c_6y]"+v"(acc_6y),
                [v_c_6z]"+v"(acc_6z),
                [v_c_6w]"+v"(acc_6w),
                [v_c_7x]"+v"(acc_7x),
                [v_c_7y]"+v"(acc_7y),
                [v_c_7z]"+v"(acc_7z),
                [v_c_7w]"+v"(acc_7w),
                [v_c_8x]"+v"(acc_8x),
                [v_c_8y]"+v"(acc_8y),
                [v_c_8z]"+v"(acc_8z),
                [v_c_8w]"+v"(acc_8w),
                [v_c_9x]"+v"(acc_9x),
                [v_c_9y]"+v"(acc_9y),
                [v_c_9z]"+v"(acc_9z),
                [v_c_9w]"+v"(acc_9w),
                [v_c_10x]"+v"(acc_10x),
                [v_c_10y]"+v"(acc_10y),
                [v_c_10z]"+v"(acc_10z),
                [v_c_10w]"+v"(acc_10w),
                [v_c_11x]"+v"(acc_11x),
                [v_c_11y]"+v"(acc_11y),
                [v_c_11z]"+v"(acc_11z),
                [v_c_11w]"+v"(acc_11w),
                [v_c_12x]"+v"(acc_12x),
                [v_c_12y]"+v"(acc_12y),
                [v_c_12z]"+v"(acc_12z),
                [v_c_12w]"+v"(acc_12w),
                [v_c_13x]"+v"(acc_13x),
                [v_c_13y]"+v"(acc_13y),
                [v_c_13z]"+v"(acc_13z),
                [v_c_13w]"+v"(acc_13w),
                [v_c_14x]"+v"(acc_14x),
                [v_c_14y]"+v"(acc_14y),
                [v_c_14z]"+v"(acc_14z),
                [v_c_14w]"+v"(acc_14w),
                [v_c_15x]"+v"(acc_15x),
                [v_c_15y]"+v"(acc_15y),
                [v_c_15z]"+v"(acc_15z),
                [v_c_15w]"+v"(acc_15w),
                [s_mem_]"+r"(smem)
            :   [s_res_a0]"s"(res_a[0]),
                [s_res_a1]"s"(res_a[1]),
                [s_res_a2]"s"(res_a[2]),
                [s_res_a3]"s"(res_a[3]),
                [s_res_b0]"s"(res_b[0]),
                [s_res_b1]"s"(res_b[1]),
                [s_res_b2]"s"(res_b[2]),
                [s_res_b3]"s"(res_b[3]),
                [s_res_sa0]"s"(res_sa[0]),
                [s_res_sa1]"s"(res_sa[1]),
                [s_res_sa2]"s"(res_sa[2]),
                [s_res_sa3]"s"(res_sa[3]),
                [s_base_sb_l]"s"(res_sb[0]),
                [s_base_sb_h]"s"(res_sb[1]),
                [s_tile_os_a]"s"(tile_offset_a_bytes),
                [s_tile_os_b]"s"(tile_offset_b_bytes),
                [v_os_slda]"v"(v_os_slda),
                [s_m0_init]"s"(m0_init_value),
                [s_size_per_issue]"s"(size_per_issue),
                [smem_sz]"n"(smem_buf_size),
                [n_a_lds_rd_offset_0]"n"(AldsRdOffsetInCol),
                [n_a_lds_rd_offset_1]"n"(AldsRdOffsetInRow),
                [n_a_lds_pingpang_sz]"n"(AldsPingPangSize),
                [n_fb_vm_ld_offset]"n"(flatBvmLdOffset),
                [n_sa_vm_ld_offset]"n"(SAvmLdOffset),
                [s_tile_sa]"s"(SAvmTileOffset),
                [s_tile_sb]"s"(SBvmTileOffset),
                [v_a_vm_ld_addr0]"v"(static_cast<index_t>(AvmLdAddr0)),
                [v_a_vm_ld_addr1]"v"(static_cast<index_t>(AvmLdAddr1)),
                [v_a_vm_ld_addr2]"v"(static_cast<index_t>(AvmLdAddr2)),
                [v_a_vm_ld_addr3]"v"(static_cast<index_t>(AvmLdAddr3)),
                // vmem -> lds
                [v_a_vm_ld_addr4]"v"(static_cast<index_t>(AvmLdAddr4)),
                [v_a_vm_ld_addr5]"v"(static_cast<index_t>(AvmLdAddr5)),
                [v_a_vm_ld_addr6]"v"(static_cast<index_t>(AvmLdAddr6)),
                [v_a_vm_ld_addr7]"v"(static_cast<index_t>(AvmLdAddr7)),
                [v_a_vm_ld_addr8]"v"(static_cast<index_t>(AvmLdAddr8)),
                [v_a_vm_ld_addr9]"v"(static_cast<index_t>(AvmLdAddr9)),
                [v_a_vm_ld_addr10]"v"(static_cast<index_t>(AvmLdAddr10)),
                [v_a_vm_ld_addr11]"v"(static_cast<index_t>(AvmLdAddr11)),
                [v_a_vm_ld_addr12]"v"(static_cast<index_t>(AvmLdAddr12)),
                [v_a_vm_ld_addr13]"v"(static_cast<index_t>(AvmLdAddr13)),
                [v_a_vm_ld_addr14]"v"(static_cast<index_t>(AvmLdAddr14)),
                [v_a_vm_ld_addr15]"v"(static_cast<index_t>(AvmLdAddr15)),
                [s_a_vm_lds_m0]"s"(static_cast<index_t>(sAvmLdsM0)),
                [s_a_m0_offset]"s"(static_cast<index_t>(sAvmLdsM0Offset)),
                [s_a_lds_pingpang_sz]"s"(static_cast<index_t>(sAldsPingPangSize)),
                // vmem -> vgpr -> lds
                //[n_a_lds_wr_offset]"n"(AldsWrOffset),
                //[v_a_lds_wr_addr]"v"(static_cast<index_t>(AldsWrAddr)),
                // debug
                [v_dbg_tid]"v"(static_cast<index_t>(dbg_tid)),
                [v_a_lds_rd_addr]"v"(static_cast<index_t>(AldsRdAddr)),
                [v_sa_vm_ld_addr]"v"(static_cast<index_t>(SAvmLdAddr)),
                [v_os_b0]"v"(static_cast<index_t>(cached_coords_b[number<0>{}] * sizeof(BDataType))),
                [v_os_b1]"v"(static_cast<index_t>(cached_coords_b[number<1>{}] * sizeof(BDataType)))
            :   "memory", 
                "s16", "s17", "s18", "s19", // A  buffer desc
                "s20", "s21", "s22", "s23", // B  buffer desc
                "s24", "s25", "s26", "s27", // SA buffer desc
                "s28", "s29",               // SB base addr
                "s30", "s31",               // Scale B ping pang
                "s86",                      // temp
                "v64",  "v65",  "v66",  "v67",  "v68",  "v69",
                "v70",  "v71",  "v72",  "v73",  "v74",  "v75",  "v76",  "v77",  "v78",  "v79",
                "v80",  "v81",  "v82",  "v83",  "v84",  "v85",  "v86",  "v87",  "v88",  "v89",
                "v90",  "v91",  "v92",  "v93",  "v94",  "v95",  "v96",  "v97",  "v98",  "v99",
                "v100", "v101", "v102", "v103", "v104", "v105", "v106", "v107", "v108", "v109", 
                "v110", "v111", "v112", "v113", "v114", "v115", "v116", "v117", "v118", "v119", 
                "v120", "v121", "v122", "v123", "v124", "v125", "v126", "v127", "v128", "v129", 
                "v130", "v131", "v132", "v133", "v134", "v135", "v136", "v137", "v138", "v139", 
                "v140", "v141", "v142", "v143", "v144", "v145", "v146", "v147", "v148", "v149", 
                "v150", "v151", "v152", "v153", "v154", "v155", "v156", "v157", "v158", "v159", 
                "v160", "v161", "v162", "v163", "v164", "v165", "v166", "v167", "v168", "v169", 
                "v170", "v171", "v172", "v173", "v174", "v175", "v176", "v177", "v178", "v179", 
                "v180", "v181", "v182", "v183", "v184", "v185", "v186", "v187", "v188", "v189", 
                "v190", "v191", "v192", "v193", "v194", "v195", "v196", "v197", "v198", "v199", 
                "v200", "v201", "v202", "v203", "v204", "v205", "v206", "v207", "v208", "v209", 
                "v210", "v211", "v212", "v213", "v214", "v215", "v216", "v217", "v218", "v219", 
                "v220", "v221", "v222", "v223", "v224", "v225", "v226", "v227", "v228", "v229", 
                "v230", "v231", "v232", "v233", "v234", "v235", "v236", "v237", "v238", "v239", 
                "v240", "v241", "v242", "v243", "v244", "v245", "v246", "v247", "v248", "v249", 
                "v250", "v251", "v252", "v253", "v254", "v255",
                "a0",   "a1",   "a2",   "a3",   "a4",   "a5",   "a6",   "a7",   "a8",   "a9",
                "a10",  "a11",  "a12",  "a13",  "a14",  "a15",  "a16",  "a17",  "a18",  "a19",
                "a20",  "a21",  "a22",  "a23",  "a24",  "a25",  "a26",  "a27",  "a28",  "a29",
                "a30",  "a31",  "a32",  "a33",  "a34",  "a35",  "a36",  "a37",  "a38",  "a39",
                "a40",  "a41",  "a42",  "a43",  "a44",  "a45",  "a46",  "a47",  "a48",  "a49",
                "a50",  "a51",  "a52",  "a53",  "a54",  "a55",  "a56",  "a57",  "a58",  "a59",
                "a60",  "a61",  "a62",  "a63",  "a64",  "a65",  "a66",  "a67",  "a68",  "a69",
                "a70",  "a71",  "a72",  "a73",  "a74",  "a75",  "a76",  "a77",  "a78",  "a79",
                "a80",  "a81",  "a82",  "a83",  "a84",  "a85",  "a86",  "a87",  "a88",  "a89",
                "a90",  "a91",  "a92",  "a93",  "a94",  "a95",  "a96",  "a97",  "a98",  "a99",
                "a100", "a101", "a102", "a103", "a104", "a105", "a106", "a107", "a108", "a109", 
                "a110", "a111", "a112", "a113", "a114", "a115", "a116", "a117", "a118", "a119", 
                "a120", "a121", "a122", "a123", "a124", "a125", "a126", "a127", "a128", "a129", 
                "a130", "a131", "a132", "a133", "a134", "a135", "a136", "a137", "a138", "a139",
                "a140", "a141", "a142", "a143", "a144", "a145", "a146", "a147", "a148", "a149", 
                "a150", "a151", "a152", "a153", "a154", "a155", "a156", "a157", "a158", "a159", 
                "a160", "a161", "a162", "a163", "a164", "a165", "a166", "a167", "a168", "a169", 
                "a170", "a171", "a172", "a173", "a174", "a175", "a176", "a177", "a178", "a179",
                "a180", "a181", "a182", "a183", "a184", "a185", "a186", "a187", "a188", "a189", 
                "a190", "a191", "a192", "a193", "a194", "a195", "a196", "a197", "a198", "a199", 
                "a200", "a201", "a202", "a203", "a204", "a205", "a206", "a207", "a208", "a209", 
                "a210", "a211", "a212", "a213", "a214", "a215", "a216", "a217", "a218", "a219",
                "a220", "a221", "a222", "a223", "a224", "a225", "a226", "a227", "a228", "a229", 
                "a230", "a231", "a232", "a233", "a234", "a235", "a236", "a237", "a238", "a239", 
                "a240", "a241", "a242", "a243", "a244", "a245", "a246", "a247", "a248", "a249", 
                "a250", "a251", "a252", "a253", "a254", "a255"
        );
        // clang-format on
#pragma clang diagnostic pop
#pragma endregion

#ifdef FEIFEI_DEBUG
        // debug fp32 (4*8 = 32) -------------------------
        int dbg_idx = 0;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_0x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_0y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_0z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_0w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_1x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_1y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_1z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_1w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_2x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_2y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_2z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_2w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_3x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_3y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_3z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_3w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_4x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_4y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_4z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_4w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_5x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_5y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_5z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_5w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_6x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_6y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_6z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_6w;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_7x;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_7y;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_7z;
        dbg_fp32[gid * DEBUG_CNT + dbg_idx++] = acc_7w;

        // debug fp8 (4*4*4 = 64) ---------------------
        dbg_idx = 0;
        auto print_f8 = [&](float dbg_f32, int &idx) {
            fp8_t * pf8_tmp = reinterpret_cast<fp8_t*>(&dbg_f32);
            for(int i = 0; i < 4; i++)
            {
                fp8_t f8v = pf8_tmp[i];
                dbg_fp8[gid * DEBUG_CNT + idx++] = f8v;
            }
        };
        print_f8(acc_0x, dbg_idx);
        print_f8(acc_0y, dbg_idx);
        print_f8(acc_0z, dbg_idx);
        print_f8(acc_0w, dbg_idx);
        print_f8(acc_1x, dbg_idx);
        print_f8(acc_1y, dbg_idx);
        print_f8(acc_1z, dbg_idx);
        print_f8(acc_1w, dbg_idx);
        print_f8(acc_2x, dbg_idx);
        print_f8(acc_2y, dbg_idx);
        print_f8(acc_2z, dbg_idx);
        print_f8(acc_2w, dbg_idx);
        print_f8(acc_3x, dbg_idx);
        print_f8(acc_3y, dbg_idx);
        print_f8(acc_3z, dbg_idx);
        print_f8(acc_3w, dbg_idx);

        dbg_idx = 0;
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(tidInMfmaM);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(swz1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(swz2);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(swz3);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(MIdxInMfmaM);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(-1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 0 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 0 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 1 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 1 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 2 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 2 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 3 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 3 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 4 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 4 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 5 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 5 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 6 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 6 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 7 + AldsRdOffsetInCol * 0);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(AldsRdAddr + AldsRdOffsetInRow * 7 + AldsRdOffsetInCol * 1);
        dbg_int[gid * DEBUG_CNT + dbg_idx++] = static_cast<index_t>(-1);
#endif

        // c store to vmem
        ODataType * d_fp16_ptr = reinterpret_cast<ODataType*>(d_ptr);
        int vmAccMBlkIdx = blockIdx.y;
        int vmAccBlkRowIdx = vmAccMBlkIdx * Block_M;
#if A_VM_LDS
        int accVmStRowIdx = MIdxInMfmaM + vmAccBlkRowIdx;
#else
        int accVmStRowIdx = threadIdx.x % mfmaM + vmAccBlkRowIdx;
#endif        

        int vmAccNBlkIdx = blockIdx.x;
        int vmAccBlkColIdx = vmAccNBlkIdx * 128;
        int accVmStCol4Idx = threadIdx.x / mfmaM;

        int accVmStOffsetInCol = 64;
        int accVmStOffsetInRow = mfmaM;        

        int accVmStOffset = 0;
        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 0) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_0x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_0y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_0z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_0w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_1x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_1y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_1z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_1w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 1) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_2x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_2y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_2z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_2w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_3x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_3y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_3z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_3w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 2) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_4x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_4y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_4z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_4w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_5x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_5y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_5z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_5w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 3) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_6x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_6y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_6z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_6w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_7x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_7y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_7z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_7w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 4) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_8x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_8y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_8z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_8w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_9x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_9y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_9z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_9w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 5) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_10x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_10y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_10z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_10w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_11x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_11y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_11z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_11w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 6) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_12x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_12y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_12z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_12w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_13x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_13y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_13z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_13w);

        accVmStOffset = (accVmStRowIdx + accVmStOffsetInRow * 7) * N + accVmStCol4Idx * 4 + vmAccBlkColIdx;
        d_fp16_ptr[accVmStOffset+0] = type_convert<ODataType>(acc_14x);
        d_fp16_ptr[accVmStOffset+1] = type_convert<ODataType>(acc_14y);
        d_fp16_ptr[accVmStOffset+2] = type_convert<ODataType>(acc_14z);
        d_fp16_ptr[accVmStOffset+3] = type_convert<ODataType>(acc_14w);
        d_fp16_ptr[accVmStOffset+0 + accVmStOffsetInCol] = type_convert<ODataType>(acc_15x);
        d_fp16_ptr[accVmStOffset+1 + accVmStOffsetInCol] = type_convert<ODataType>(acc_15y);
        d_fp16_ptr[accVmStOffset+2 + accVmStOffsetInCol] = type_convert<ODataType>(acc_15z);
        d_fp16_ptr[accVmStOffset+3 + accVmStOffsetInCol] = type_convert<ODataType>(acc_15w);
        
        return nullptr;
    }
};

} // namespace ck_tile
