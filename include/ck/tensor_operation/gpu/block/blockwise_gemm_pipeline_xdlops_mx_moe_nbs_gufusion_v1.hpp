// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2025, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include "ck/tensor_operation/gpu/block/blockwise_gemm_pipeline_xdlops_base.hpp"

namespace ck {

// Compute optimized pipeline
// GlobalPrefetchStages: 2
// LocalPreFillStages: 1
// LocalPreFetchStages: 1
// LocalSharedMemoryBuffer: 1

template <BlockGemmPipelineScheduler BlkGemmPipelineVer,
          index_t BlockSize,
          index_t ScaleBlockSize,
          typename ADataType,
          typename AScaleDataType,
          typename BDataType,
          typename BScaleDataType,
          typename ComputeDataType,
          typename AccDataType,
          typename ATileDesc,
          typename BTileDesc,
          typename AMmaTileDesc,
          typename BMmaTileDesc,
          index_t ABlockTransferSrcScalarPerVector,
          index_t BBlockTransferSrcScalarPerVector,
          index_t MPerBlock,
          index_t NPerBlock,
          index_t KPerBlock,
          index_t MPerXDL,
          index_t NPerXDL,
          index_t MRepeat,
          index_t NRepeat,
          index_t KPacks>
struct BlockwiseGemmXdlops_pipeline_bns_gufusion_v1
{
};

template <index_t BlockSize,
          index_t ScaleBlockSize,
          typename ADataType,
          typename AScaleDataType,
          typename BDataType,
          typename BScaleDataType,
          typename ComputeDataType,
          typename AccDataType,
          typename ATileDesc,
          typename BTileDesc,
          typename AMmaTileDesc,
          typename BMmaTileDesc,
          index_t ABlockTransferSrcScalarPerVector,
          index_t BBlockTransferSrcScalarPerVector,
          index_t MPerBlock,
          index_t NPerBlock,
          index_t KPerBlock,
          index_t MPerXDL,
          index_t NPerXDL,
          index_t MRepeat,
          index_t NRepeat,
          index_t KPack
          >
struct BlockwiseGemmXdlops_pipeline_bns_gufusion_v1<BlockGemmPipelineScheduler::Intrawave,
                                                            BlockSize,
                                                            ScaleBlockSize,
                                                            ADataType,
                                                            AScaleDataType,
                                                            BDataType,
                                                            BScaleDataType,
                                                            ComputeDataType,
                                                            AccDataType,
                                                            ATileDesc,
                                                            BTileDesc,
                                                            AMmaTileDesc,
                                                            BMmaTileDesc,
                                                            ABlockTransferSrcScalarPerVector,
                                                            BBlockTransferSrcScalarPerVector,
                                                            MPerBlock,
                                                            NPerBlock,
                                                            KPerBlock,
                                                            MPerXDL,
                                                            NPerXDL,
                                                            MRepeat,
                                                            NRepeat,
                                                            KPack>
        : BlockwiseGemmXdlops_mx_pipeline_base<BlockSize,
                                                ADataType,
                                                BDataType,
                                                ATileDesc,
                                                BTileDesc,
                                                AMmaTileDesc,
                                                BMmaTileDesc,
                                                ABlockTransferSrcScalarPerVector,
                                                BBlockTransferSrcScalarPerVector,
                                                MPerBlock,
                                                NPerBlock,
                                                KPerBlock,
                                                MPerXDL,
                                                NPerXDL,
                                                MRepeat,
                                                NRepeat,
                                                KPack>

{
    using Base = BlockwiseGemmXdlops_mx_pipeline_base<BlockSize,
                                                      ADataType,
                                                      BDataType,
                                                      ATileDesc,
                                                      BTileDesc,
                                                      AMmaTileDesc,
                                                      BMmaTileDesc,
                                                      ABlockTransferSrcScalarPerVector,
                                                      BBlockTransferSrcScalarPerVector,
                                                      MPerBlock,
                                                      NPerBlock,
                                                      KPerBlock,
                                                      MPerXDL,
                                                      NPerXDL,
                                                      MRepeat,
                                                      NRepeat,
                                                      KPack>;
    using Base::A_K1;
    using Base::B_K1;
    using Base::I0;
    using Base::I1;
    //using Base::KGroup;
    static constexpr index_t KGroup = 1;
    using Base::KRepeat;
    using Base::xdlops_gemm;
    using typename Base::HotLoopInstList;

    using Base::a_block_desc_m0_m1_m2_m3_k;
    using Base::b_block_desc_n0_n1_n2_n3_k;

    using Base::CalculateCThreadOriginDataIndex;
    //using Base::CalculateCThreadOriginDataIndex8D;
    using Base::GetCBlockDescriptor_G_M0_N0_M1_N1_M2_M3_M4_N2;
    using Base::GetCBlockDescriptor_M0_N0_M1_N1_M2_M3_M4_N2;
    using Base::GetCBlockDescriptor_M0_N0_M1_N1_M2_N2_N3_N4;
    using Base::GetCThreadBuffer;
    using Base::GetCThreadDescriptor_G_M0_N0_M1_N1_M2_M3_M4_N2;
    using Base::GetCThreadDescriptor_M0_N0_M1_N1_M2_M3_M4_N2;
    using Base::GetCThreadDescriptor_M0_N0_M1_N1_M2_N2_N3_N4;
    using Base::MakeCGridDescriptor_G_M0_N0_M1_N1_M2_M3_M4_N2;
    using Base::MakeCGridDescriptor_M0_N0_M1_N1_M2_M3_M4_N2;

    using Base::AMmaKStride;
    using Base::BMmaKStride;

    using Base::MWaves;
    using Base::NWaves;
    using Base::APackedSize;
    using Base::BPackedSize;
    using Base::KThreadChunk;
    static constexpr index_t MXdlPack = 2;
    static constexpr index_t NXdlPack = 2;
    static constexpr index_t KXdlPack = 2;

    static constexpr index_t PrefetchStages  = 2;
    static constexpr index_t PrefillStages   = 1;
    static constexpr index_t GlobalBufferNum = 2;

    using ComputeTypeA = typename Base::ComputeTypeA;
    using ComputeTypeB = typename Base::ComputeTypeB;

    template <typename TileDesc_M0_M1_M2_K>
    __host__ __device__ static constexpr auto MakeAGemmMmaTileDescriptor(const TileDesc_M0_M1_M2_K&)
    {
        constexpr index_t M0 = TileDesc_M0_M1_M2_K{}.GetLength(Number<0>{});
        constexpr index_t M1 = TileDesc_M0_M1_M2_K{}.GetLength(Number<1>{});
        constexpr index_t M2 = TileDesc_M0_M1_M2_K{}.GetLength(Number<2>{});
        constexpr index_t K2 = KPack / KGroup;
        constexpr index_t K1 = 64 / NPerXDL;
        constexpr index_t K0 = KRepeat * KGroup;

        return transform_tensor_descriptor(
            TileDesc_M0_M1_M2_K{},
            make_tuple(
                make_pass_through_transform(Number<M0>{}),
                make_pass_through_transform(Number<M1>{}),
                make_pass_through_transform(Number<M2>{}),
                make_unmerge_transform(make_tuple(Number<K0>{}, Number<K1>{}, Number<K2>{}))),
            make_tuple(Sequence<0>{}, Sequence<1>{}, Sequence<2>{}, Sequence<3>{}),
            make_tuple(Sequence<0>{}, Sequence<1>{}, Sequence<2>{}, Sequence<3, 4, 5>{}));
    }

    //static constexpr auto a_block_desc_m0_m1_m2_k0_k1_k2 = MakeAGemmMmaTileDescriptor(a_block_desc_m0_m1_m2_k);

    __host__ __device__ static constexpr bool BlockHasHotloop(index_t num_loop)
    {
        return num_loop > PrefetchStages;
    }

    __host__ __device__ static constexpr TailNumber BlockLoopTailNum(index_t num_loop)
    {
        return num_loop % 2 == 0 ? TailNumber::Even : TailNumber::Odd;
    }

    __device__ static constexpr auto HotLoopScheduler()
    {
        constexpr auto num_ds_read_inst_a     = HotLoopInstList::A_LDS_Read_Inst_Num;
        constexpr auto num_buffer_load_inst_a = HotLoopInstList::A_Buffer_Load_Inst_Num;
        constexpr auto num_buffer_load_inst_b =
            HotLoopInstList::B_Buffer_Load_Inst_Num * MWaves * 2;
        constexpr auto mfma_interleave = MPerXDL == 32 ? 1 : 2;
        // B global
        static_for<0, num_buffer_load_inst_b, 1>{}([&](auto i) {
            ignore = i;
            if constexpr(MPerBlock >= 128 && NPerBlock >= 64)
            {
                __builtin_amdgcn_sched_group_barrier(0x008, 2 * mfma_interleave, 0);
            }
            else
            {
                __builtin_amdgcn_sched_group_barrier(0x008, mfma_interleave, 0);
            }
            __builtin_amdgcn_sched_group_barrier(0x020, 1, 0); // VMEM read
            // if constexpr(i.value < num_buffer_load_inst_a) {
            //     __builtin_amdgcn_sched_group_barrier(0x008, 1, 0); // MFMA
            //     __builtin_amdgcn_sched_group_barrier(0x200, 1, 0); // DS write
            //     __builtin_amdgcn_sched_group_barrier(0x008, 1, 0); // MFMA
            //     __builtin_amdgcn_sched_group_barrier(0x020, 1, 0); // VMEM read
            // }
        });

        // A global
        static_for<0, num_buffer_load_inst_a, 1>{}([&](auto i) {
            ignore = i;
            __builtin_amdgcn_sched_group_barrier(0x008, 1, 0); // MFMA
            __builtin_amdgcn_sched_group_barrier(0x200, 1, 0); // DS write
            __builtin_amdgcn_sched_group_barrier(0x008, 1, 0); // MFMA
            __builtin_amdgcn_sched_group_barrier(0x020, 1, 0); // VMEM read
        });

        // A local
        static_for<0, MPerXDL == 32 ? num_ds_read_inst_a / 2 : num_ds_read_inst_a, 1>{}(
            [&](auto i) {
                ignore = i;
                __builtin_amdgcn_sched_group_barrier(0x008, 1, 0);                     // MFMA
                __builtin_amdgcn_sched_group_barrier(0x100, MPerXDL == 32 ? 2 : 1, 0); // DS read
            });
    }

    template <bool HasMainLoop,
              TailNumber TailNum,
              typename AGridDesc,
              typename ABlockDesc,
              typename ABlockTransfer,
              typename AGridBuffer,
              typename ABlockBuffer,
              typename ABlockTransferStep,
              typename BGridDesc,
              typename BBlockDesc,
              typename BBlockTransfer,
              typename BGridBuffer,
              typename BBlockBuffer,
              typename BBlockTransferStep,
              typename CThreadBuffer,
              typename AScaleGridBuffer,
              typename AScaleGridDesc,
              typename AScaleThreadTransfer,
              typename BScaleGridBuffer,
              typename BScaleGridDesc,
              typename BScaleThreadTransfer>
    __device__ void Run(
        // A
        const AGridDesc& a_grid_desc,
        const ABlockDesc& a_block_desc,
        ABlockTransfer& a_blockwise_copy,
        const AGridBuffer& a_grid_buf,
        ABlockBuffer& a_block_buf,
        const ABlockTransferStep& a_block_copy_step,
        // B0/B1
        const BGridDesc& b_grid_desc,
        const BBlockDesc& b_block_desc,
        BBlockTransfer& b_blockwise_copy,
        BBlockTransfer& b_blockwise_copy_up,
        const BGridBuffer& b_grid_buf,
        const BGridBuffer& b_grid_buf_up,
        BBlockBuffer& b_block_buf,
        const BBlockTransferStep& b_block_copy_step,
        // C
        CThreadBuffer& c_thread_buf,
        CThreadBuffer& c_thread_buf_up,
        // A scale
        const AScaleGridDesc& a_scale_grid_desc,
        AScaleThreadTransfer& a_scale_thread_copy,
        const AScaleGridBuffer& a_scale_grid_buf,
        // B0/B1 scale
        const BScaleGridDesc& b_scale_grid_desc,
        BScaleThreadTransfer& b_scale_thread_copy,
        BScaleThreadTransfer& b_scale_thread_copy_up,
        const BScaleGridBuffer& b_scale_grid_buf,
        const BScaleGridBuffer& b_scale_grid_buf_up,
        index_t num_loop) const
    {
        auto a_thread_buf           = make_static_buffer<AddressSpaceEnum::Vgpr, ComputeDataType>(a_thread_desc_.GetElementSpaceSize());
        auto b_thread_buf           = make_static_buffer<AddressSpaceEnum::Vgpr, ComputeDataType>(b_thread_desc_.GetElementSpaceSize());
        auto b_thread_buf_up        = make_static_buffer<AddressSpaceEnum::Vgpr, ComputeDataType>(b_thread_desc_.GetElementSpaceSize());
        auto a_scale_thread_buf     = make_static_buffer<AddressSpaceEnum::Vgpr, AScaleDataType>(a_scale_thread_desc.GetElementSpaceSize());
        auto b_scale_thread_buf     = make_static_buffer<AddressSpaceEnum::Vgpr, BScaleDataType>(b_scale_thread_desc.GetElementSpaceSize());
        auto b_scale_thread_buf_up  = make_static_buffer<AddressSpaceEnum::Vgpr, BScaleDataType>(b_scale_thread_desc.GetElementSpaceSize());

        constexpr auto b_block_origin_idx = make_tuple(I0, I0, I0, I0);

        // A/B0/B1: vmem->vgpr
        a_blockwise_copy.RunRead(a_grid_desc,    a_grid_buf,    I0);
        b_blockwise_copy.RunRead(b_grid_desc,    b_grid_buf,    I0);
        b_blockwise_copy_up.RunRead(b_grid_desc, b_grid_buf_up, I0);

        a_blockwise_copy.MoveSrcSliceWindow(a_grid_desc,    a_block_copy_step);
        b_blockwise_copy.MoveSrcSliceWindow(b_grid_desc,    b_block_copy_step);
        b_blockwise_copy_up.MoveSrcSliceWindow(b_grid_desc, b_block_copy_step);

        // A/B0/B1: vgpr->lds
        a_blockwise_copy.RunWrite(a_block_desc,     a_block_buf, I0);
        b_blockwise_copy.RunWrite(b_block_desc,     b_block_buf, I0);
        b_blockwise_copy_up.RunWrite(b_block_desc,  b_block_buf, I0);

        // A/B0/B1: lds->acc
        block_sync_lds();
        static_for<0, KRepeat, 1>{}([&](auto k) {
            constexpr auto k_step = k * xdlops_gemm.KPerXdlops / APackedSize * (APackedSize * KPack / xdlops_gemm.K1PerXdlops);
            static_for<0, MRepeat, 1>{}([&](auto m0) {
                static_for<0, xdlops_gemm.K1PerXdlops / (APackedSize * KThreadChunk), 1>{}([&](auto chunk) {
                    constexpr auto a_k_step_chunk = k_step + chunk * KThreadChunk * xdlops_gemm.mfma_instr.num_input_blks;
                    a_thread_copy_.Run(a_block_desc_m0_m1_m2_m3_k,
                                        make_tuple(Number<m0 / MXdlPack>{},
                                                    I0,
                                                    Number<m0 % MXdlPack>{},
                                                    I0,
                                                    Number<a_k_step_chunk>{}),
                                        a_block_buf,
                                        a_thread_desc_,
                                        make_tuple(Number<m0 / MXdlPack>{},
                                                    I0,
                                                    Number<m0 % MXdlPack>{},
                                                    k,
                                                    Number<chunk * KThreadChunk>{}),
                                        a_thread_buf);
                });
            });

            static_for<0, NRepeat, 1>{}([&](auto n0) {
                static_for<0, xdlops_gemm.K1PerXdlops / (BPackedSize * KThreadChunk), 1>{}([&](auto chunk) {
                    constexpr auto b_k_step_chunk = k_step + chunk * KThreadChunk * xdlops_gemm.mfma_instr.num_input_blks;
                    b_thread_copy_.Run(b_block_desc_n0_n1_n2_n3_k,
                                        make_tuple(Number<n0 / NXdlPack>{},
                                                    I0,
                                                    Number<n0 % NXdlPack>{},
                                                    I0,
                                                    Number<b_k_step_chunk>{}),
                                        b_block_buf,
                                        b_thread_desc_,
                                        make_tuple(Number<n0 / NXdlPack>{},
                                                    I0,
                                                    Number<n0 % NXdlPack>{},
                                                    k,
                                                    Number<chunk * KThreadChunk>{}),
                                        b_thread_buf);
                });
            });

            static_for<0, NRepeat, 1>{}([&](auto n0) {
                static_for<0, xdlops_gemm.K1PerXdlops / (BPackedSize * KThreadChunk), 1>{}([&](auto chunk) {
                    constexpr auto b_k_step_chunk = k_step + chunk * KThreadChunk * xdlops_gemm.mfma_instr.num_input_blks;
                    b_thread_copy_.Run(b_block_desc_n0_n1_n2_n3_k,
                                        make_tuple(Number<n0 / NXdlPack>{},
                                                    I0,
                                                    Number<n0 % NXdlPack>{},
                                                    I0,
                                                    Number<b_k_step_chunk>{}),
                                        b_block_buf,
                                        b_thread_desc_,
                                        make_tuple(Number<n0 / NXdlPack>{},
                                                    I0,
                                                    Number<n0 % NXdlPack>{},
                                                    k,
                                                    Number<chunk * KThreadChunk>{}),
                                        b_thread_buf);
                });
            });
        });
        /*if(threadIdx.x == 0)
        {
            printf("a_thread_buf = %d\n", a_thread_desc_.GetElementSpaceSize().value);
            printf("[00~07]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*0>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*0>{}].unpack(Number<1>{}))
            );
            printf("[08~15]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*1>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*1>{}].unpack(Number<1>{}))
            );
            printf("[16~23]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*2>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*2>{}].unpack(Number<1>{}))
            );
            printf("[24~31]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*3>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*3>{}].unpack(Number<1>{}))
            );
            printf("[32~39]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*4>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*4>{}].unpack(Number<1>{}))
            );
            printf("[40~47]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*5>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*5>{}].unpack(Number<1>{}))
            );
            printf("[48~55]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*6>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*6>{}].unpack(Number<1>{}))
            );
            printf("[56~63]: <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>, <%.1f,%.1f>\n", 
                type_convert<float>(a_thread_buf[Number<0+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<0+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<1+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<1+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<2+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<2+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<3+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<3+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<4+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<4+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<5+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<5+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<6+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<6+8*7>{}].unpack(Number<1>{})),
                type_convert<float>(a_thread_buf[Number<7+8*7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_buf[Number<7+8*7>{}].unpack(Number<1>{}))
            );
        }
        return;*/

        // a scale 
        static_for<0, MRepeat / MXdlPack, 1>{}([&](auto m0) {
            static_for<0, KRepeat / KXdlPack, 1>{}([&](auto k0) {
                a_scale_thread_copy.Run(a_scale_grid_desc,
                                        a_scale_grid_buf,
                                        a_scale_thread_desc,
                                        make_tuple(m0, k0, I0),
                                        a_scale_thread_buf);

                a_scale_thread_copy.MoveSrcSliceWindow(a_scale_grid_desc, make_multi_index(0, I1, 0));
            });
            a_scale_thread_copy.MoveSrcSliceWindow(a_scale_grid_desc, make_multi_index(MWaves, -KRepeat / KXdlPack, 0));
        });
        a_scale_thread_copy.MoveSrcSliceWindow(a_scale_grid_desc, make_multi_index(-MWaves * MRepeat / MXdlPack, KRepeat / KXdlPack, 0));

        // b0 scale 
        static_for<0, NRepeat / NXdlPack, 1>{}([&](auto n0) {
            static_for<0, KRepeat / KXdlPack, 1>{}([&](auto k0) {
                b_scale_thread_copy.Run(b_scale_grid_desc,
                                        b_scale_grid_buf,
                                        b_scale_thread_desc,
                                        make_tuple(n0, k0, I0),
                                        b_scale_thread_buf);

                b_scale_thread_copy.MoveSrcSliceWindow(b_scale_grid_desc, make_multi_index(0, I1, 0));
            });
            b_scale_thread_copy.MoveSrcSliceWindow(b_scale_grid_desc, make_multi_index(NWaves, -KRepeat / KXdlPack, 0));
        });
        b_scale_thread_copy.MoveSrcSliceWindow(b_scale_grid_desc, make_multi_index(-NWaves * NRepeat / NXdlPack, KRepeat / KXdlPack, 0));

        // b1 scale 
        static_for<0, NRepeat / NXdlPack, 1>{}([&](auto n0) {
            static_for<0, KRepeat / KXdlPack, 1>{}([&](auto k0) {
                b_scale_thread_copy_up.Run(b_scale_grid_desc,
                                           b_scale_grid_buf_up,
                                           b_scale_thread_desc,
                                           make_tuple(n0, k0, I0),
                                           b_scale_thread_buf_up);

                b_scale_thread_copy_up.MoveSrcSliceWindow(b_scale_grid_desc, make_multi_index(0, I1, 0));
            });
            b_scale_thread_copy_up.MoveSrcSliceWindow(b_scale_grid_desc, make_multi_index(NWaves, -KRepeat / KXdlPack, 0));
        });
        b_scale_thread_copy_up.MoveSrcSliceWindow(b_scale_grid_desc, make_multi_index(-NWaves * NRepeat / NXdlPack, KRepeat / KXdlPack, 0));

        // Initialize C
        c_thread_buf.Clear();
        c_thread_buf_up.Clear();

#if 0
        // main body
        if constexpr(HasMainLoop)
        {
            index_t i = 0;
            do
            {
                auto LoopFunc = [&](auto mfma_reg_buf, auto local_read_buf) {
                    b_blockwise_copy.Run(b_grid_desc,
                                         b_grid_buf,
                                         b_block_desc_n0_n1_k0_k1,
                                         b_block_origin_idx,
                                         b_thread_bufs(local_read_buf));
                    b_blockwise_copy.MoveSrcSliceWindow(b_grid_desc, b_block_copy_step);

                    b_blockwise_copy_up.Run(b_grid_desc,
                                            b_grid_buf_up,
                                            b_block_desc_n0_n1_k0_k1,
                                            b_block_origin_idx,
                                            b_thread_bufs_up(local_read_buf));
                    b_blockwise_copy_up.MoveSrcSliceWindow(b_grid_desc, b_block_copy_step);
                    block_sync_lds();
                    a_blockwise_copy.RunWrite(a_block_desc, a_block_buf, mfma_reg_buf);

                    a_blockwise_copy.RunRead(a_grid_desc, a_grid_buf, local_read_buf);
                    a_blockwise_copy.MoveSrcSliceWindow(a_grid_desc, a_block_copy_step);
                    static_for<0, MRepeat, 1>{}([&](auto m0) {
                        static_for<0, NRepeat, 1>{}([&](auto n0) {
                            static_for<0, KRepeat, 1>{}([&](auto k0) {
                                vector_type<ComputeDataType, KPack> a_thread_vec;
                                vector_type<ComputeDataType, KPack> b_thread_vec;
                                vector_type<ComputeDataType, KPack> b_thread_vec_up;

                                static_for<0, KPack, 1>{}([&](auto ik) {
                                    a_thread_vec.template AsType<ComputeDataType>()(ik) =
                                        a_thread_buf[Number<a_thread_desc_.CalculateOffset(
                                            make_tuple(m0, I0, I0, k0, I0, ik))>{}];
                                    b_thread_vec.template AsType<ComputeDataType>()(ik) =
                                        b_thread_bufs[mfma_reg_buf]
                                                     [Number<b_thread_desc_.CalculateOffset(
                                                         make_tuple(n0, I0, k0, ik))>{}];
                                    b_thread_vec_up.template AsType<ComputeDataType>()(ik) =
                                        b_thread_bufs_up[mfma_reg_buf]
                                                        [Number<b_thread_desc_.CalculateOffset(
                                                            make_tuple(n0, I0, k0, ik))>{}];
                                });
                                using mfma_input_type =
                                    typename vector_type<ComputeDataType,
                                                         xdlops_gemm.K1PerXdlops>::type;

                                constexpr index_t c_offset =
                                    c_thread_desc_.CalculateOffset(make_tuple(m0, n0, 0));

                                xdlops_gemm.Run(
                                    a_thread_vec.template AsType<mfma_input_type>(),
                                    b_thread_vec.template AsType<mfma_input_type>(),
                                    c_thread_buf.GetVectorTypeReference(Number<c_offset>{}));

                                xdlops_gemm.Run(
                                    a_thread_vec.template AsType<mfma_input_type>(),
                                    b_thread_vec_up.template AsType<mfma_input_type>(),
                                    c_thread_buf_up.GetVectorTypeReference(Number<c_offset>{}));
                            });
                        });
                    });

                    block_sync_lds();

                    static_for<0, MRepeat, 1>{}([&](auto m0) {
                        static_for<0, KRepeat, 1>{}([&](auto k0) {
                            static_for<0, KGroup, 1>{}([&](auto kg0) {
                                a_thread_copy_.Run(
                                    a_block_desc_m0_m1_m2_k0_k1_k2,
                                    make_tuple(m0, I0, I0, Number<k0 * KGroup + kg0>{}, I0, I0),
                                    a_block_buf,
                                    a_thread_desc_,
                                    make_tuple(m0, I0, I0, k0, I0, Number<kg0 * A_K1>{}),
                                    a_thread_buf);
                            });
                        });
                    });

                    HotLoopScheduler();
                    __builtin_amdgcn_sched_barrier(0);
                };

                LoopFunc(I0, I1);
                LoopFunc(I1, I0);

                i += 2;
            } while(i < (num_loop - 2));
        }
        // tail
        if constexpr(TailNum == TailNumber::Even)
        {
            b_blockwise_copy.Run(b_grid_desc,
                                 b_grid_buf,
                                 b_block_desc_n0_n1_k0_k1,
                                 b_block_origin_idx,
                                 b_thread_bufs(I1));

            b_blockwise_copy_up.Run(b_grid_desc,
                                    b_grid_buf_up,
                                    b_block_desc_n0_n1_k0_k1,
                                    b_block_origin_idx,
                                    b_thread_bufs_up(I1));
            block_sync_lds();
            a_blockwise_copy.RunWrite(a_block_desc, a_block_buf);

            static_for<0, MRepeat, 1>{}([&](auto m0) {
                static_for<0, NRepeat, 1>{}([&](auto n0) {
                    static_for<0, KRepeat, 1>{}([&](auto k0) {
                        vector_type<ComputeDataType, KPack> a_thread_vec;
                        vector_type<ComputeDataType, KPack> b_thread_vec;
                        vector_type<ComputeDataType, KPack> b_thread_vec_up;

                        static_for<0, KPack, 1>{}([&](auto ik) {
                            a_thread_vec.template AsType<ComputeDataType>()(ik) =
                                a_thread_buf[Number<a_thread_desc_.CalculateOffset(
                                    make_tuple(m0, I0, I0, k0, I0, ik))>{}];
                            b_thread_vec.template AsType<ComputeDataType>()(ik) =
                                b_thread_bufs[I0][Number<b_thread_desc_.CalculateOffset(
                                    make_tuple(n0, I0, k0, ik))>{}];
                            b_thread_vec_up.template AsType<ComputeDataType>()(ik) =
                                b_thread_bufs_up[I0][Number<b_thread_desc_.CalculateOffset(
                                    make_tuple(n0, I0, k0, ik))>{}];
                        });

                        using mfma_input_type =
                            typename vector_type<ComputeDataType, xdlops_gemm.K1PerXdlops>::type;

                        constexpr index_t c_offset =
                            c_thread_desc_.CalculateOffset(make_tuple(m0, n0, 0));

                        xdlops_gemm.Run(a_thread_vec.template AsType<mfma_input_type>(),
                                        b_thread_vec.template AsType<mfma_input_type>(),
                                        c_thread_buf.GetVectorTypeReference(Number<c_offset>{}));

                        xdlops_gemm.Run(a_thread_vec.template AsType<mfma_input_type>(),
                                        b_thread_vec_up.template AsType<mfma_input_type>(),
                                        c_thread_buf_up.GetVectorTypeReference(Number<c_offset>{}));
                    });
                });
            });

            block_sync_lds();

            static_for<0, MRepeat, 1>{}([&](auto m0) {
                static_for<0, KRepeat, 1>{}([&](auto k0) {
                    static_for<0, KGroup, 1>{}([&](auto kg0) {
                        a_thread_copy_.Run(a_block_desc_m0_m1_m2_k0_k1_k2,
                                           make_tuple(m0, I0, I0, Number<k0 * KGroup + kg0>{}, I0, I0),
                                           a_block_buf,
                                           a_thread_desc_,
                                           make_tuple(m0, I0, I0, k0, I0, Number<kg0 * A_K1>{}),
                                           a_thread_buf);
                    });
                });
            });

            __builtin_amdgcn_sched_barrier(0);

            static_for<0, MRepeat, 1>{}([&](auto m0) {
                static_for<0, NRepeat, 1>{}([&](auto n0) {
                    static_for<0, KRepeat, 1>{}([&](auto k0) {
                        vector_type<ComputeDataType, KPack> a_thread_vec;
                        vector_type<ComputeDataType, KPack> b_thread_vec;
                        vector_type<ComputeDataType, KPack> b_thread_vec_up;

                        static_for<0, KPack, 1>{}([&](auto ik) {
                            a_thread_vec.template AsType<ComputeDataType>()(ik) =
                                a_thread_buf[Number<a_thread_desc_.CalculateOffset(
                                    make_tuple(m0, I0, I0, k0, I0, ik))>{}];
                            b_thread_vec.template AsType<ComputeDataType>()(ik) =
                                b_thread_bufs[I1][Number<b_thread_desc_.CalculateOffset(
                                    make_tuple(n0, I0, k0, ik))>{}];
                            b_thread_vec_up.template AsType<ComputeDataType>()(ik) =
                                b_thread_bufs_up[I1][Number<b_thread_desc_.CalculateOffset(
                                    make_tuple(n0, I0, k0, ik))>{}];
                        });

                        using mfma_input_type =
                            typename vector_type<ComputeDataType, xdlops_gemm.K1PerXdlops>::type;

                        constexpr index_t c_offset =
                            c_thread_desc_.CalculateOffset(make_tuple(m0, n0, 0));

                        xdlops_gemm.Run(a_thread_vec.template AsType<mfma_input_type>(),
                                        b_thread_vec.template AsType<mfma_input_type>(),
                                        c_thread_buf.GetVectorTypeReference(Number<c_offset>{}));
                        xdlops_gemm.Run(a_thread_vec.template AsType<mfma_input_type>(),
                                        b_thread_vec_up.template AsType<mfma_input_type>(),
                                        c_thread_buf_up.GetVectorTypeReference(Number<c_offset>{}));
                    });
                });
            });
            // Let's leak last MFMA block to epilogue region, cover the potential lds-shuffle
            // latency
            // __builtin_amdgcn_sched_barrier(0);
        }
        else 
#endif
        if constexpr(TailNum == TailNumber::Odd)
        {
            static_for<0, MRepeat / MXdlPack, 1>{}([&](auto m0) {
                static_for<0, NRepeat / NXdlPack, 1>{}([&](auto n0) {
                    static_for<0, KRepeat / KXdlPack, 1>{}([&](auto k0) {
                        constexpr index_t a_scale_offset = a_scale_thread_desc.CalculateOffset(make_tuple(m0, k0, I0));
                        constexpr index_t b_scale_offset = b_scale_thread_desc.CalculateOffset(make_tuple(n0, k0, I0));

                        vector_type<AScaleDataType, a_scale_thread_vec_size> a_scale_thread_vec;
                        vector_type<BScaleDataType, b_scale_thread_vec_size> b_scale_thread_vec;
                        vector_type<BScaleDataType, b_scale_thread_vec_size> b_scale_thread_vec_up;

                        // Pack scale_thread_buf into scale_thread_vec
                        static_for<0, a_scale_thread_vec_size, 1>{}([&](auto s) {
                            a_scale_thread_vec.template AsType<AScaleDataType>()(s) = a_scale_thread_buf[Number<a_scale_offset + s>{}];
                        });
                        static_for<0, b_scale_thread_vec_size, 1>{}([&](auto s) {
                            b_scale_thread_vec.template AsType<BScaleDataType>()(s) = b_scale_thread_buf[Number<b_scale_offset + s>{}];
                        });
                        static_for<0, b_scale_thread_vec_size, 1>{}([&](auto s) {
                            b_scale_thread_vec_up.template AsType<BScaleDataType>()(s) = b_scale_thread_buf_up[Number<b_scale_offset + s>{}];
                        });

                        static_for<0, KXdlPack, 1>{}([&](auto ikxdl) {
                            static_for<0, MXdlPack, 1>{}([&](auto imxdl) {
                                static_for<0, NXdlPack, 1>{}([&](auto inxdl) {
                                    constexpr auto kxdl = ikxdl + k0 * KXdlPack;

                                    vector_type<ComputeTypeA, KPack> a_thread_vec;
                                    vector_type<ComputeTypeB, KPack> b_thread_vec;
                                    vector_type<ComputeTypeB, KPack> b_thread_vec_up;

                                    static_for<0, KPack, 1>{}([&](auto ik) {
                                        a_thread_vec.template AsType<ComputeTypeA>()(ik) = a_thread_buf[Number<a_thread_desc_.CalculateOffset(make_tuple(m0, I0, imxdl, kxdl, ik))>{}];
                                        b_thread_vec.template AsType<ComputeTypeB>()(ik) = b_thread_buf[Number<b_thread_desc_.CalculateOffset(make_tuple(n0, I0, inxdl, kxdl, ik))>{}];
                                        b_thread_vec_up.template AsType<ComputeTypeB>()(ik) = b_thread_buf[Number<b_thread_desc_.CalculateOffset(make_tuple(n0, I0, inxdl, kxdl, ik))>{}];
                                    });

                                    using mfma_input_type_a = typename vector_type<ComputeTypeA, xdlops_gemm.K1PerXdlops / APackedSize>::type;
                                    using mfma_input_type_b = typename vector_type<ComputeTypeB, xdlops_gemm.K1PerXdlops / BPackedSize>::type;

                                    using mfma_scale_input_type_a = typename vector_type<AScaleDataType, a_scale_thread_vec_size>::type;
                                    using mfma_scale_input_type_b = typename vector_type<BScaleDataType, b_scale_thread_vec_size>::type;

                                    constexpr index_t c_offset = c_thread_desc_.CalculateOffset(make_tuple(m0, n0, imxdl, inxdl, 0));

                                    // MFMA accumulation
                                    xdlops_gemm.template Run<ikxdl * MXdlPack + imxdl,
                                                             ikxdl * NXdlPack + inxdl>(
                                        a_thread_vec.template           AsType<mfma_input_type_a>(),
                                        a_scale_thread_vec.template     AsType<mfma_scale_input_type_a>(),
                                        b_thread_vec.template           AsType<mfma_input_type_b>(),
                                        b_scale_thread_vec.template     AsType<mfma_scale_input_type_b>(),
                                        c_thread_buf.GetVectorTypeReference(Number<c_offset>{}));
                                    xdlops_gemm.template Run<ikxdl * MXdlPack + imxdl,
                                                             ikxdl * NXdlPack + inxdl>(
                                        a_thread_vec.template           AsType<mfma_input_type_a>(),
                                        a_scale_thread_vec.template     AsType<AScaleDataType>(),
                                        b_thread_vec_up.template        AsType<mfma_input_type_b>(),
                                        b_scale_thread_vec_up.template  AsType<BScaleDataType>(),
                                        c_thread_buf_up.GetVectorTypeReference(Number<c_offset>{}));

                                    if(blockIdx.x == 0 && blockIdx.y == 0 && ikxdl.value == 1)
                                    {
                                        // a
                                        /*printf("[%d-%d-%d][0~7] f4x2: <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>\n",
                                            imxdl.value, inxdl.value, ikxdl.value,
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<0>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<0>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<1>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<1>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<2>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<2>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<3>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<3>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<4>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<4>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<5>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<5>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<6>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<6>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<7>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<7>{}].unpack(Number<1>{}))
                                        );
                                        printf("[%d-%d-%d][8~16]f4x2: <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>\n",
                                            imxdl.value, inxdl.value, ikxdl.value,
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<0+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<0+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<1+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<1+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<2+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<2+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<3+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<3+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<4+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<4+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<5+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<5+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<6+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<6+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<7+8>{}].unpack(Number<0>{})), type_convert<float>(a_thread_vec.template AsType<ComputeTypeA>()[Number<7+8>{}].unpack(Number<1>{}))
                                        );*/
                                        // b0
                                        /*printf("[%d-%d-%d][0~7] f4x2: <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>\n",
                                            imxdl.value, inxdl.value, ikxdl.value, 
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<0>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<0>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<1>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<1>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<2>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<2>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<3>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<3>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<4>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<4>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<5>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<5>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<6>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<6>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<7>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<7>{}].unpack(Number<1>{}))
                                        );
                                        printf("[%d-%d-%d][8~16]f4x2: <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>\n",
                                            imxdl.value, inxdl.value, ikxdl.value,
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<0+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<0+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<1+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<1+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<2+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<2+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<3+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<3+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<4+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<4+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<5+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<5+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<6+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<6+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<7+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec.template AsType<ComputeTypeA>()[Number<7+8>{}].unpack(Number<1>{}))
                                        );*/
                                        // b1
                                        /*printf("[%d-%d-%d][0~7] f4x2: <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>\n",
                                            imxdl.value, inxdl.value, ikxdl.value, 
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<0>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<0>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<1>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<1>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<2>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<2>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<3>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<3>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<4>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<4>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<5>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<5>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<6>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<6>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<7>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<7>{}].unpack(Number<1>{}))
                                        );
                                        printf("[%d-%d-%d][8~16]f4x2: <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>, <%.1f, %.1f>\n",
                                            imxdl.value, inxdl.value, ikxdl.value,
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<0+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<0+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<1+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<1+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<2+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<2+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<3+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<3+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<4+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<4+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<5+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<5+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<6+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<6+8>{}].unpack(Number<1>{})),
                                            type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<7+8>{}].unpack(Number<0>{})), type_convert<float>(b_thread_vec_up.template AsType<ComputeTypeA>()[Number<7+8>{}].unpack(Number<1>{}))
                                        );*/
                                        // scale a/b0/b1
                                        /*printf("[%d-%d-%d]: 0x%08X, 0x%08X, 0x%08X\n",
                                            ikxdl.value, imxdl.value, inxdl.value, 
                                            a_scale_thread_vec.template AsType<BScaleDataType>()[Number<0>{}],
                                            b_scale_thread_vec.template AsType<BScaleDataType>()[Number<0>{}],
                                            b_scale_thread_vec_up.template AsType<BScaleDataType>()[Number<0>{}] // error
                                        );*/
                                        // c
                                        printf("[%d-%d-%d]: %.1f, %.1f, %.1f, %.1f\n",
                                            ikxdl.value, imxdl.value, inxdl.value, 
                                            type_convert<float>(c_thread_buf.GetVectorTypeReference(Number<c_offset>{}).template AsType<float>()[Number<0>{}]),
                                            type_convert<float>(c_thread_buf.GetVectorTypeReference(Number<c_offset>{}).template AsType<float>()[Number<1>{}]),
                                            type_convert<float>(c_thread_buf.GetVectorTypeReference(Number<c_offset>{}).template AsType<float>()[Number<2>{}]),
                                            type_convert<float>(c_thread_buf.GetVectorTypeReference(Number<c_offset>{}).template AsType<float>()[Number<3>{}])
                                        );
                                    }
                                }); // NXdlPack
                            });     // MXdlPack
                        });         // KXdlPack
                    }); // KRepeat / KXdlPack
                });     // NRepeat / NXdlPack
            });         // MRepeat / MXdlPack
        }
    }
    
    // TODO: make this field protected when a_scale_thread_copy_ is moved
    // here

    
    using mx_scale_t                        = e8m0_bexp_t;
    static constexpr auto scale_pack_size_a = sizeof(AScaleDataType) / sizeof(mx_scale_t);
    static constexpr auto scale_pack_size_b = sizeof(BScaleDataType) / sizeof(mx_scale_t);
    
    static constexpr auto a_scale_thread_vec_size = KXdlPack * MXdlPack / scale_pack_size_a;
    static constexpr auto b_scale_thread_vec_size = KXdlPack * NXdlPack / scale_pack_size_b;

    static constexpr auto ScalesPerXdlopsRun          = (APackedSize * KPack * xdlops_gemm.K0PerXdlops) / ScaleBlockSize;
    static constexpr auto ScalesPerXdlopsRunPerThread = ScalesPerXdlopsRun / xdlops_gemm.mfma_instr.num_input_blks;

    static constexpr auto a_scale_thread_desc = make_naive_tensor_descriptor_packed(
        make_tuple(Number<MRepeat / MXdlPack>{},
                   Number<KRepeat / KXdlPack>{},
                   Number<ScalesPerXdlopsRunPerThread * a_scale_thread_vec_size>{}));

    // TODO: make this field protected when b_scale_thread_copy_ is moved
    // here
    static constexpr auto b_scale_thread_desc = make_naive_tensor_descriptor_packed(
        make_tuple(Number<NRepeat / NXdlPack>{},
                   Number<KRepeat / KXdlPack>{},
                   Number<ScalesPerXdlopsRunPerThread * b_scale_thread_vec_size>{}));

    protected:
    using Base::a_thread_copy_;
    using Base::a_thread_desc_;
    using Base::b_thread_copy_;
    using Base::b_thread_desc_;
    using Base::c_thread_desc_;

    protected:
    // MRepeat MWave MLane KRepeat KLane KPack
    // KRepeat -> MRepeat-> Mwave->KLane->MLane->KPack
    /*static constexpr auto a_thread_desc_ = make_naive_tensor_descriptor_packed(
        make_tuple(Number<MRepeat>{}, I1, I1, Number<KRepeat>{}, I1, Number<KPack>{}));

    using AThreadCopy = ThreadwiseTensorSliceTransfer_v4<ADataType,
                                                         ComputeDataType,
                                                         decltype(a_block_desc_m0_m1_m2_k0_k1_k2),
                                                         decltype(a_thread_desc_),
                                                         Sequence<1, 1, 1, 1, 1, KPack / KGroup>,
                                                         Sequence<0, 1, 2, 3, 4, 5>,
                                                         5,
                                                         A_K1,
                                                         A_K1>;

    AThreadCopy a_thread_copy_{Base::CalculateAThreadOriginDataIndex6D()};

    static constexpr auto b_thread_desc_ = make_naive_tensor_descriptor_packed(
        make_tuple(Number<NRepeat>{}, I1, Number<KRepeat>{}, Number<KPack>{}));

    static constexpr BTileDesc b_block_desc_n0_n1_k0_k1;*/
};

} // namespace ck
