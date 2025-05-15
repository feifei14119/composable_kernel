	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx950"
	.amdhsa_code_object_version 6
	.section	.text._ZN2ckL12flush_icacheEv,"axG",@progbits,_ZN2ckL12flush_icacheEv,comdat
	.globl	_ZN2ckL12flush_icacheEv         ; -- Begin function _ZN2ckL12flush_icacheEv
	.p2align	8
	.type	_ZN2ckL12flush_icacheEv,@function
_ZN2ckL12flush_icacheEv:                ; @_ZN2ckL12flush_icacheEv
; %bb.0:                                ; %entry
	;;#ASMSTART
	s_icache_inv 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	s_nop 0 
	
	;;#ASMEND
	s_endpgm
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
	.amdhsa_kernel _ZN2ckL12flush_icacheEv
		.amdhsa_group_segment_fixed_size 0
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_kernarg_size 0
		.amdhsa_user_sgpr_count 0
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 0
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_kernarg_preload_length 0
		.amdhsa_user_sgpr_kernarg_preload_offset 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_uses_dynamic_stack 0
		.amdhsa_enable_private_segment 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 0
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 0
		.amdhsa_next_free_vgpr 1
		.amdhsa_next_free_sgpr 0
		.amdhsa_accum_offset 4
		.amdhsa_reserve_vcc 0
		.amdhsa_float_round_mode_32 0
		.amdhsa_float_round_mode_16_64 0
		.amdhsa_float_denorm_mode_32 3
		.amdhsa_float_denorm_mode_16_64 3
		.amdhsa_dx10_clamp 1
		.amdhsa_ieee_mode 1
		.amdhsa_fp16_overflow 0
		.amdhsa_tg_split 0
		.amdhsa_exception_fp_ieee_invalid_op 0
		.amdhsa_exception_fp_denorm_src 0
		.amdhsa_exception_fp_ieee_div_zero 0
		.amdhsa_exception_fp_ieee_overflow 0
		.amdhsa_exception_fp_ieee_underflow 0
		.amdhsa_exception_fp_ieee_inexact 0
		.amdhsa_exception_int_div_zero 0
	.end_amdhsa_kernel
	.section	.text._ZN2ckL12flush_icacheEv,"axG",@progbits,_ZN2ckL12flush_icacheEv,comdat
.Lfunc_end0:
	.size	_ZN2ckL12flush_icacheEv, .Lfunc_end0-_ZN2ckL12flush_icacheEv
                                        ; -- End function
	.section	.AMDGPU.csdata,"",@progbits
; Kernel info:
; codeLenInByte = 276
; NumSgprs: 6
; NumVgprs: 0
; NumAgprs: 0
; TotalNumVgprs: 0
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 0 bytes/workgroup (compile time only)
; SGPRBlocks: 0
; VGPRBlocks: 0
; NumSGPRsForWavesPerEU: 6
; NumVGPRsForWavesPerEU: 1
; AccumOffset: 4
; Occupancy: 8
; WaveLimiterHint : 0
; COMPUTE_PGM_RSRC2:SCRATCH_EN: 0
; COMPUTE_PGM_RSRC2:USER_SGPR: 0
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0
; COMPUTE_PGM_RSRC3_GFX90A:ACCUM_OFFSET: 0
; COMPUTE_PGM_RSRC3_GFX90A:TG_SPLIT: 0
	.section	.text._ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE,"axG",@progbits,_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE,comdat
	.protected	_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE ; -- Begin function _ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE
	.globl	_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE
	.p2align	8
	.type	_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE,@function
_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE: ; @_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE
; %bb.0:                                ; %entry
	s_load_dwordx4 s[16:19], s[0:1], 0x28
	s_load_dwordx2 s[24:25], s[0:1], 0x80
	s_waitcnt lgkmcnt(0)
	s_load_dword s19, s[0:1], 0x88
	s_load_dwordx4 s[12:15], s[0:1], 0x10
	s_load_dword s36, s[0:1], 0x20
	s_load_dwordx2 s[20:21], s[0:1], 0x54
	s_load_dwordx8 s[4:11], s[0:1], 0x60
	s_waitcnt lgkmcnt(0)
	s_lshr_b32 s19, s19, 24
	s_cmp_gt_i32 s18, 1
	s_cselect_b64 s[22:23], -1, 0
	s_bitcmp1_b32 s19, 0
	s_cselect_b64 s[26:27], -1, 0
	s_and_b64 s[22:23], s[22:23], s[26:27]
	s_andn2_b64 vcc, exec, s[22:23]
	s_mov_b64 s[26:27], 0
	s_cbranch_vccnz .LBB1_2
; %bb.1:                                ; %if.then30.i
	s_mul_i32 s19, s12, s3
	s_mul_i32 s26, s19, s13
	s_ashr_i32 s27, s26, 31
.LBB1_2:                                ; %_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE17SplitKBatchOffsetC2ERNSI_8ArgumentEi.exit
	s_load_dword s29, s[0:1], 0x3c
	s_load_dword s22, s[0:1], 0x44
	s_getpc_b64 s[0:1]
	s_add_u32 s0, s0, _ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE@rel32@lo+4
	s_addc_u32 s1, s1, _ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE@rel32@hi+12
	s_load_dword s33, s[0:1], 0x0
	s_add_i32 s31, s12, -1
	s_add_i32 s23, s13, -1
	s_cmpk_lt_u32 s31, 0x80
	s_mov_b32 s28, 0
	s_cbranch_scc1 .LBB1_6
; %bb.3:                                ; %if.else.i.i.i
	s_cmpk_lt_u32 s23, 0x80
	s_mov_b32 s0, 0
	s_cbranch_scc1 .LBB1_10
; %bb.4:                                ; %if.else10.i.i.i
	s_add_i32 s0, s12, 0x7f
	s_ashr_i32 s1, s0, 31
	s_lshr_b32 s1, s1, 25
	s_add_i32 s0, s0, s1
	s_ashr_i32 s30, s0, 7
	s_add_i32 s0, s13, 0x7f
	s_ashr_i32 s1, s0, 31
	s_lshr_b32 s1, s1, 25
	s_add_i32 s0, s0, s1
	s_ashr_i32 s19, s0, 7
	s_mul_i32 s0, s19, s30
	s_add_i32 s1, s0, 7
	s_ashr_i32 s34, s1, 31
	s_lshr_b32 s34, s34, 29
	s_add_i32 s1, s1, s34
	s_ashr_i32 s34, s1, 3
	s_and_b32 s1, s1, -8
	s_sub_i32 s35, s0, s1
	s_add_i32 s35, s35, 8
	s_ashr_i32 s0, s2, 31
	s_lshr_b32 s0, s0, 29
	s_add_i32 s38, s2, s0
	s_and_b32 s0, s38, -8
	s_sub_i32 s37, s2, s0
	s_cmp_gt_i32 s37, s35
	s_cbranch_scc1 .LBB1_7
; %bb.5:                                ; %cond.true.i.i.i
	s_mul_i32 s2, s34, s37
	s_mov_b64 vcc, exec
	s_ashr_i32 s0, s38, 3
	s_cbranch_execz .LBB1_8
	s_branch .LBB1_9
.LBB1_6:
	s_mov_b32 s19, 0
	s_branch .LBB1_11
.LBB1_7:
                                        ; implicit-def: $sgpr2
	s_mov_b64 vcc, 0
	s_ashr_i32 s0, s38, 3
.LBB1_8:                                ; %cond.false.i.i.i
	s_add_i32 s1, s34, -1
	s_mul_i32 s1, s1, s37
	s_add_i32 s2, s35, s1
.LBB1_9:                                ; %cond.end.i.i.i
	s_abs_i32 s38, s19
	v_cvt_f32_u32_e32 v1, s38
	v_rcp_iflag_f32_e32 v1, v1
	s_add_i32 s2, s2, s0
	s_ashr_i32 s1, s19, 31
	s_ashr_i32 s0, s2, 31
	v_mul_f32_e32 v1, 0x4f7ffffe, v1
	v_cvt_u32_f32_e32 v1, v1
	s_abs_i32 s34, s2
	s_sub_i32 s35, 0, s38
	s_xor_b32 s1, s0, s1
	v_readfirstlane_b32 s2, v1
	s_mul_i32 s35, s35, s2
	s_mul_hi_u32 s35, s2, s35
	s_add_i32 s2, s2, s35
	s_mul_hi_u32 s2, s34, s2
	s_mul_i32 s35, s2, s38
	s_add_i32 s37, s2, 1
	s_sub_i32 s34, s34, s35
	s_sub_i32 s35, s34, s38
	s_cmp_ge_u32 s34, s38
	s_cselect_b32 s2, s37, s2
	s_cselect_b32 s34, s35, s34
	s_add_i32 s37, s2, 1
	s_sub_i32 s35, s34, s38
	s_cmp_ge_u32 s34, s38
	s_cselect_b32 s2, s37, s2
	s_cselect_b32 s34, s35, s34
	s_xor_b32 s2, s2, s1
	s_sub_i32 s2, s2, s1
	s_lshr_b32 s1, s30, 30
	s_add_i32 s1, s30, s1
	s_and_b32 s35, s1, -4
	s_sub_i32 s1, s30, s35
	s_cmp_ge_i32 s2, s35
	s_cselect_b32 s30, s1, 4
	s_ashr_i32 s1, s2, 31
	s_abs_i32 s35, s2
	s_mul_hi_u32 s37, s35, 0x3fffffff
	s_lshl_b32 s37, s37, 2
	s_sub_i32 s35, s35, s37
	s_add_i32 s37, s35, -4
	s_cmp_gt_u32 s35, 3
	s_cselect_b32 s35, s37, s35
	s_add_i32 s37, s35, -4
	s_cmp_gt_u32 s35, 3
	s_cselect_b32 s35, s37, s35
	s_xor_b32 s35, s35, s1
	s_abs_i32 s37, s30
	v_cvt_f32_u32_e32 v1, s37
	s_sub_i32 s1, s35, s1
	s_xor_b32 s34, s34, s0
	s_sub_i32 s0, s34, s0
	v_rcp_iflag_f32_e32 v1, v1
	s_mul_i32 s19, s1, s19
	s_add_i32 s19, s19, s0
	s_xor_b32 s0, s19, s30
	v_mul_f32_e32 v1, 0x4f7ffffe, v1
	v_cvt_u32_f32_e32 v1, v1
	s_ashr_i32 s0, s0, 31
	s_abs_i32 s35, s19
	s_sub_i32 s38, 0, s37
	v_readfirstlane_b32 s34, v1
	s_mul_i32 s38, s38, s34
	s_mul_hi_u32 s38, s34, s38
	s_add_i32 s34, s34, s38
	s_mul_hi_u32 s34, s35, s34
	s_mul_i32 s38, s34, s37
	s_sub_i32 s35, s35, s38
	s_add_i32 s39, s34, 1
	s_sub_i32 s38, s35, s37
	s_cmp_ge_u32 s35, s37
	s_cselect_b32 s34, s39, s34
	s_cselect_b32 s38, s38, s35
	s_add_i32 s35, s34, 1
	s_cmp_ge_u32 s38, s37
	s_cselect_b32 s34, s35, s34
	s_xor_b32 s34, s34, s0
	s_sub_i32 s0, s34, s0
	s_mul_i32 s30, s0, s30
	s_sub_i32 s19, s19, s30
	s_add_i32 s2, s19, s2
	s_sub_i32 s2, s2, s1
.LBB1_10:                               ; %Flow771
	s_mov_b32 s19, s2
	s_mov_b32 s2, s0
.LBB1_11:                               ; %_ZNK2ck39BlockToCTileMap_Grouped_M00_N0_M01AdaptILi8ELi128ELi128EE20CalculateBottomIndexINS_5TupleIJiEEEEEDaRKT_.exit.i.i
	s_waitcnt lgkmcnt(0)
	s_mul_i32 s0, s29, s3
	s_lshr_b32 s1, s0, 31
	s_add_i32 s1, s0, s1
	s_ashr_i32 s1, s1, 1
	s_lshl_b32 s34, s0, 3
	s_ashr_i32 s30, s0, 31
	s_lshr_b32 s30, s30, 27
	s_add_i32 s0, s0, s30
	s_ashr_i32 s30, s0, 5
	s_add_i32 s18, s18, -1
	s_mul_i32 s0, s18, s29
	s_sub_i32 s0, s14, s0
	s_cmp_gt_i32 s18, s3
	s_cselect_b32 s37, s29, s0
	s_ashr_i32 s0, s1, 31
	s_add_u32 s1, s4, s1
	s_addc_u32 s0, s5, s0
	s_ashr_i32 s3, s30, 31
	s_add_u32 s4, s6, s30
	s_addc_u32 s5, s7, s3
	s_ashr_i32 s6, s34, 31
	s_add_u32 s35, s8, s34
	s_addc_u32 s34, s9, s6
	s_add_u32 s8, s10, s30
	s_addc_u32 s9, s11, s3
	s_add_u32 s29, 0, 0
	s_addc_u32 s30, s12, -1
	s_mul_i32 s3, s30, s15
	s_ashr_i32 s6, s3, 31
	s_ashr_i32 s7, s37, 31
	s_add_u32 s3, s3, s37
	s_addc_u32 s11, s6, s7
	s_ashr_i32 s6, s20, 31
	s_lshr_b32 s6, s6, 30
	s_add_i32 s6, s20, s6
	s_ashr_i32 s6, s6, 2
	s_lshl_b32 s18, s21, 13
	s_lshl_b32 s14, s21, 11
	s_add_i32 s6, s6, -1
	s_mul_i32 s10, s18, s6
	s_add_i32 s7, s33, -1
	s_mul_i32 s7, s7, s14
	s_ashr_i32 s20, s7, 31
	s_add_i32 s21, s14, 0xfffff800
	s_ashr_i32 s6, s21, 31
	s_movk_i32 s38, 0x800
	s_or_b32 s33, s10, 0x800
	s_ashr_i32 s10, s33, 31
	s_add_u32 s33, s33, s21
	s_addc_u32 s21, s10, s6
	s_add_i32 s6, s37, 31
	s_ashr_i32 s10, s6, 31
	s_lshr_b32 s10, s10, 27
	s_add_i32 s6, s6, s10
	s_ashr_i32 s10, s6, 5
	s_mul_i32 s6, s36, s31
	s_add_i32 s6, s10, s6
	s_mul_i32 s23, s16, s23
	s_add_i32 s10, s10, s23
	s_lshl_b32 s40, s2, 1
	v_lshrrev_b32_e32 v42, 1, v0
	v_and_b32_e32 v41, 0x7e, v42
	v_lshrrev_b32_e32 v1, 6, v0
	v_lshlrev_b32_e32 v6, 5, v0
	s_mul_i32 s40, s40, s18
	v_mul_lo_u32 v7, s14, v1
	v_and_b32_e32 v2, 48, v0
	v_and_b32_e32 v3, 63, v0
	v_sub_u32_e32 v22, v3, v2
	s_ashr_i32 s14, s22, 31
	s_lshr_b32 s14, s14, 30
	s_add_i32 s22, s22, s14
	s_ashr_i32 s37, s22, 2
	v_and_b32_e32 v144, 15, v0
	s_lshl_b32 s39, s19, 7
	v_and_b32_e32 v8, 3, v0
	v_add_u32_e32 v2, s39, v41
	v_mul_lo_u32 v40, v2, s15
	v_lshlrev_b32_e32 v38, 5, v8
	v_add_u32_e32 v39, v40, v38
	s_lshl_b32 s14, s2, 7
	s_add_u32 s7, s33, s7
	s_addc_u32 s2, s21, s20
	v_mov_b32_e32 v2, s7
	v_alignbit_b32 v4, s2, v2, 1
	s_mov_b32 s7, 0x20000
	v_mov_b32_e32 v2, s35
	v_mov_b32_e32 v3, s34
	v_mov_b32_e32 v5, 0x20000
	v_lshrrev_b32_e32 v9, 31, v39
	v_add_u32_e32 v9, v39, v9
	v_ashrrev_i32_e32 v9, 1, v9
	s_lshr_b32 s2, s11, 31
	s_add_u32 s3, s3, s2
	s_addc_u32 s2, s11, 0
	v_mov_b32_e32 v10, s3
	v_alignbit_b32 v134, s2, v10, 1
	v_mov_b32_e32 v136, s1
	v_mov_b32_e32 v137, s0
	v_mov_b32_e32 v135, v5
	s_mov_b64 s[2:3], exec
.LBB1_12:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[14:17], v9, s[20:23], 0 offen
                                        ; implicit-def: $vgpr9
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_12
; %bb.13:
	s_mov_b64 exec, s[2:3]
	v_add_u32_e32 v9, s15, v39
	v_lshrrev_b32_e32 v10, 31, v9
	v_add_u32_e32 v9, v9, v10
	v_ashrrev_i32_e32 v9, 1, v9
	s_mov_b64 s[2:3], exec
.LBB1_14:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[18:21], v9, s[20:23], 0 offen
                                        ; implicit-def: $vgpr9
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_14
; %bb.15:
	s_mov_b64 exec, s[2:3]
	s_movk_i32 s2, 0x80
	s_getpc_b64 s[0:1]
	s_add_u32 s0, s0, _ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE@rel32@lo+4
	s_addc_u32 s1, s1, _ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE@rel32@hi+12
	s_load_dword s34, s[0:1], 0x0
	v_add_u32_e32 v23, 0x80, v39
	v_lshlrev_b32_e32 v27, 4, v1
	s_lshl_b32 s31, s36, 4
	s_mul_i32 s0, s36, 0xffffff90
	s_waitcnt lgkmcnt(0)
	s_add_i32 s35, s34, s0
	s_mov_b32 s11, s7
	s_lshl_b32 s33, s16, 6
	v_and_b32_e32 v1, 2, v42
	v_bitop3_b32 v1, v1, v8, 1 bitop3:0x36
	v_bitop3_b32 v43, v42, v8, 2 bitop3:0x6c
	v_lshlrev_b32_e32 v8, 7, v41
	v_lshl_or_b32 v25, v43, 5, v8
	v_sub_u32_e32 v8, v1, v43
	v_lshlrev_b32_e32 v8, 5, v8
	s_movk_i32 s0, 0x7e0
	v_and_or_b32 v6, v6, s0, v7
	v_add3_u32 v24, v8, v25, s2
	v_add_u32_e32 v26, s40, v6
	v_ashrrev_i32_e32 v10, 1, v26
	s_mov_b64 s[2:3], exec
.LBB1_16:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s20, v2
	v_readfirstlane_b32 s21, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[2:3]
	v_readfirstlane_b32 s22, v4
	v_readfirstlane_b32 s23, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[6:9], v10, s[20:23], 0 offen
                                        ; implicit-def: $vgpr10
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_16
; %bb.17:
	s_mov_b64 exec, s[2:3]
	v_add_u32_e32 v10, s18, v26
	v_ashrrev_i32_e32 v28, 1, v10
	s_mov_b64 s[2:3], exec
.LBB1_18:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s20, v2
	v_readfirstlane_b32 s21, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[2:3]
	v_readfirstlane_b32 s22, v4
	v_readfirstlane_b32 s23, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[10:13], v28, s[20:23], 0 offen
                                        ; implicit-def: $vgpr28
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_18
; %bb.19:
	s_mov_b64 exec, s[2:3]
	v_bfe_u32 v28, v0, 4, 2
	v_or_b32_e32 v29, s39, v144
	v_mad_u64_u32 v[30:31], s[0:1], v29, s36, v[28:29]
	v_add_u32_e32 v29, s31, v30
	buffer_load_ubyte v36, v30, s[4:7], 0 offen
	buffer_load_ubyte v37, v29, s[4:7], 0 offen
	v_add_u32_e32 v30, s31, v29
	v_add_u32_e32 v44, s31, v30
	v_add_u32_e32 v46, s31, v44
	v_or3_b32 v27, v27, v144, s14
	v_add_u32_e32 v45, s31, v46
	v_mad_u64_u32 v[28:29], s[0:1], v27, s16, v[28:29]
	v_add_u32_e32 v27, s33, v28
	buffer_load_ubyte v33, v28, s[8:11], 0 offen
	buffer_load_ubyte v35, v27, s[8:11], 0 offen
	buffer_load_ubyte v34, v30, s[4:7], 0 offen
	v_add_u32_e32 v27, s31, v45
	buffer_load_ubyte v32, v44, s[4:7], 0 offen
	buffer_load_ubyte v29, v46, s[4:7], 0 offen
	buffer_load_ubyte v31, v45, s[4:7], 0 offen
	buffer_load_ubyte v30, v27, s[4:7], 0 offen
	v_add_u32_e32 v44, s31, v27
	buffer_load_ubyte v27, v44, s[4:7], 0 offen
	v_lshrrev_b32_e32 v47, 1, v25
	v_ashrrev_i32_e32 v46, 1, v24
	v_sub_u32_e32 v49, v43, v1
	s_movk_i32 s0, 0xff80
	v_add_u32_e32 v1, 0x800, v26
	v_lshrrev_b32_e32 v48, 31, v23
	v_add_u32_e32 v45, s15, v23
	v_lshrrev_b32_e32 v26, 4, v0
	v_lshlrev_b32_e32 v25, 7, v22
	v_or_b32_e32 v47, 0x14000, v47
	v_add_u32_e32 v46, 0x14000, v46
	v_lshlrev_b32_e32 v50, 5, v49
	v_add_u32_e32 v49, v23, v48
	v_lshrrev_b32_e32 v48, 31, v45
	v_xor_b32_e32 v23, v26, v0
	v_add3_u32 v130, v50, v24, s0
	v_ashrrev_i32_e32 v24, 1, v49
	v_add_u32_e32 v45, v45, v48
	v_lshlrev_b32_e32 v23, 5, v23
	v_add_u32_e32 v141, s34, v28
	s_waitcnt vmcnt(13)
	ds_write_b128 v47, v[14:17]
	s_waitcnt vmcnt(12)
	ds_write_b128 v46, v[18:21]
	v_ashrrev_i32_e32 v14, 1, v45
	v_add_u32_e32 v142, s35, v44
	s_waitcnt vmcnt(8)
	v_lshl_or_b32 v15, v37, 8, v36
	s_waitcnt vmcnt(6)
	v_lshlrev_b16_e32 v16, 8, v35
	s_waitcnt vmcnt(5)
	v_lshl_or_b32 v15, v34, 16, v15
	v_or_b32_e32 v155, v16, v33
	s_waitcnt vmcnt(4)
	v_lshl_or_b32 v15, v32, 24, v15
	v_or3_b32 v140, 0, v15, 0
	s_waitcnt vmcnt(2)
	v_lshlrev_b32_e32 v16, 8, v31
	s_waitcnt vmcnt(1)
	v_lshlrev_b32_e32 v15, 16, v30
	v_or_b32_e32 v16, v16, v29
	s_waitcnt vmcnt(0)
	v_lshlrev_b32_e32 v17, 24, v27
	v_or3_b32 v139, v15, v16, v17
	s_mov_b64 s[2:3], exec
.LBB1_20:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[50:53], v24, s[20:23], 0 offen
                                        ; implicit-def: $vgpr24
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_20
; %bb.21:
	s_mov_b64 exec, s[2:3]
	s_mov_b64 s[2:3], exec
.LBB1_22:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[94:97], v14, s[20:23], 0 offen
                                        ; implicit-def: $vgpr14
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_22
; %bb.23:
	s_mov_b64 exec, s[2:3]
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_add_u16_e32 v14, 16, v22
	v_mov_b32_e32 v15, 13
	v_lshrrev_b16_sdwa v15, v15, sext(v14) dst_sel:DWORD dst_unused:UNUSED_PAD src0_sel:DWORD src1_sel:BYTE_0
	v_and_b32_e32 v15, 3, v15
	v_add_u16_e32 v15, v14, v15
	v_and_b32_e32 v15, 0xfc, v15
	v_sub_u16_e32 v14, v14, v15
	v_lshrrev_b16_e32 v15, 13, v22
	v_and_b32_e32 v15, 3, v15
	v_add_u16_e32 v15, v22, v15
	v_and_b32_e32 v15, 0xfc, v15
	v_sub_u16_e32 v15, v22, v15
	v_bfe_i32 v14, v14, 0, 8
	v_bfe_i32 v15, v15, 0, 8
	v_bitop3_b32 v15, v26, v15, 3 bitop3:0x6c
	v_bitop3_b32 v14, v26, v14, 3 bitop3:0x6c
	v_sub_u32_e32 v14, v14, v15
	v_lshlrev_b32_e32 v16, 5, v14
	v_add_u16_e32 v14, 32, v22
	v_lshrrev_b16_e32 v17, 14, v14
	v_add_u16_e32 v17, v14, v17
	v_and_b32_e32 v17, -4, v17
	v_sub_u16_e32 v14, v14, v17
	v_bfe_i32 v14, v14, 0, 16
	v_bitop3_b32 v17, v26, v14, 3 bitop3:0x6c
	v_lshl_add_u32 v14, v15, 5, v25
	v_sub_u32_e32 v15, v17, v15
	v_add3_u32 v16, v14, v16, s38
	v_ashrrev_i32_e32 v145, 1, v16
	v_ashrrev_i32_e32 v146, 1, v14
	v_add_u32_e32 v17, 0x14000, v146
	v_add_u32_e32 v16, 0x14000, v145
	ds_read_b128 v[74:77], v17
	ds_read_b128 v[58:61], v16
	v_lshlrev_b32_e32 v16, 5, v15
	s_movk_i32 s0, 0x60
	v_and_or_b32 v15, v23, s0, v25
	s_movk_i32 s0, 0x1000
	v_add3_u32 v16, v14, v16, s0
	v_add_u32_e32 v14, 0x1800, v15
	v_ashrrev_i32_e32 v147, 1, v16
	v_ashrrev_i32_e32 v148, 1, v14
	v_add_u32_e32 v16, 0x14000, v147
	v_add_u32_e32 v14, 0x14000, v148
	ds_read_b128 v[34:37], v16
	ds_read_b128 v[30:33], v14
	v_add_u32_e32 v14, 0x2000, v15
	v_add_u32_e32 v16, 0x2800, v15
	v_ashrrev_i32_e32 v149, 1, v16
	v_ashrrev_i32_e32 v150, 1, v14
	v_add_u32_e32 v16, 0x14000, v150
	v_add_u32_e32 v14, 0x14000, v149
	ds_read_b128 v[26:29], v16
	ds_read_b128 v[22:25], v14
	v_add_u32_e32 v14, 0x3000, v15
	v_add_u32_e32 v15, 0x3800, v15
	v_ashrrev_i32_e32 v151, 1, v15
	v_ashrrev_i32_e32 v152, 1, v14
	v_add_u32_e32 v15, 0x14000, v152
	v_add_u32_e32 v14, 0x14000, v151
	ds_read_b128 v[18:21], v15
	ds_read_b128 v[14:17], v14
	s_add_i32 s16, s37, -2
	v_or_b32_e32 v44, 1, v42
	v_bitop3_b32 v42, v44, 3, v0 bitop3:0x48
	v_sub_u32_e32 v41, v44, v41
	v_sub_u32_e32 v44, v43, v42
	v_mov_b32_e32 v45, 0xffffff80
	v_lshl_add_u32 v138, v44, 5, v45
	v_sub_u32_e32 v42, v42, v43
	v_lshlrev_b32_e32 v42, 5, v42
	v_add_u32_e32 v153, 0x80, v42
	v_lshl_add_u32 v154, v41, 7, v42
	v_mov_b32_e32 v143, 0x1000
	v_add_u32_e32 v40, s15, v40
	s_movk_i32 s0, 0x180
	v_add3_u32 v156, v40, v38, s0
	v_add_u32_e32 v157, 0x180, v39
	v_mov_b32_e32 v98, 0
	s_mov_b32 s15, 0xffff
	v_mov_b32_e32 v99, v98
	v_mov_b32_e32 v100, v98
	v_mov_b32_e32 v101, v98
	v_mov_b32_e32 v114, v98
	v_mov_b32_e32 v115, v98
	v_mov_b32_e32 v116, v98
	v_mov_b32_e32 v117, v98
	v_mov_b32_e32 v110, v98
	v_mov_b32_e32 v111, v98
	v_mov_b32_e32 v112, v98
	v_mov_b32_e32 v113, v98
	v_mov_b32_e32 v106, v98
	v_mov_b32_e32 v107, v98
	v_mov_b32_e32 v108, v98
	v_mov_b32_e32 v109, v98
	v_mov_b32_e32 v102, v98
	v_mov_b32_e32 v103, v98
	v_mov_b32_e32 v104, v98
	v_mov_b32_e32 v105, v98
	v_mov_b32_e32 v82, v98
	v_mov_b32_e32 v83, v98
	v_mov_b32_e32 v84, v98
	v_mov_b32_e32 v85, v98
	v_mov_b32_e32 v90, v98
	v_mov_b32_e32 v91, v98
	v_mov_b32_e32 v92, v98
	v_mov_b32_e32 v93, v98
	v_mov_b32_e32 v86, v98
	v_mov_b32_e32 v87, v98
	v_mov_b32_e32 v88, v98
	v_mov_b32_e32 v89, v98
	v_mov_b32_e32 v38, v98
	v_mov_b32_e32 v39, v98
	v_mov_b32_e32 v40, v98
	v_mov_b32_e32 v41, v98
	v_mov_b32_e32 v42, v98
	v_mov_b32_e32 v43, v98
	v_mov_b32_e32 v44, v98
	v_mov_b32_e32 v45, v98
	v_mov_b32_e32 v46, v98
	v_mov_b32_e32 v47, v98
	v_mov_b32_e32 v48, v98
	v_mov_b32_e32 v49, v98
	v_mov_b32_e32 v66, v98
	v_mov_b32_e32 v67, v98
	v_mov_b32_e32 v68, v98
	v_mov_b32_e32 v69, v98
	v_mov_b32_e32 v54, v98
	v_mov_b32_e32 v55, v98
	v_mov_b32_e32 v56, v98
	v_mov_b32_e32 v57, v98
	v_mov_b32_e32 v62, v98
	v_mov_b32_e32 v63, v98
	v_mov_b32_e32 v64, v98
	v_mov_b32_e32 v65, v98
	v_mov_b32_e32 v70, v98
	v_mov_b32_e32 v71, v98
	v_mov_b32_e32 v72, v98
	v_mov_b32_e32 v73, v98
	v_mov_b32_e32 v78, v98
	v_mov_b32_e32 v79, v98
	v_mov_b32_e32 v80, v98
	v_mov_b32_e32 v81, v98
.LBB1_24:                               ; %do.body.i.i.i
                                        ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB1_25 Depth 2
                                        ;     Child Loop BB1_27 Depth 2
                                        ;     Child Loop BB1_29 Depth 2
                                        ;     Child Loop BB1_31 Depth 2
                                        ;     Child Loop BB1_33 Depth 2
                                        ;     Child Loop BB1_35 Depth 2
                                        ;     Child Loop BB1_37 Depth 2
                                        ;     Child Loop BB1_39 Depth 2
	v_add_u32_e32 v123, s31, v142
	v_add_u32_e32 v122, s31, v123
	v_add_u32_e32 v121, s31, v122
	v_add_u32_e32 v120, s31, v121
	v_add_u32_e32 v119, s31, v120
	v_add_u32_e32 v118, s31, v119
	buffer_load_ubyte v173, v123, s[4:7], 0 offen
	buffer_load_ubyte v172, v122, s[4:7], 0 offen
	buffer_load_ubyte v171, v121, s[4:7], 0 offen
	buffer_load_ubyte v170, v120, s[4:7], 0 offen
	buffer_load_ubyte v167, v119, s[4:7], 0 offen
	buffer_load_ubyte v164, v118, s[4:7], 0 offen
	v_add_u32_e32 v119, s31, v118
	buffer_load_ubyte v174, v142, s[4:7], 0 offen
	buffer_load_ubyte v158, v119, s[4:7], 0 offen
	v_add_u32_e32 v118, s33, v141
	buffer_load_ubyte v163, v141, s[8:11], 0 offen
	buffer_load_ubyte v159, v118, s[8:11], 0 offen
	v_add_u32_e32 v118, 0xffffff80, v157
	v_add_u32_e32 v160, s35, v119
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_add_u32_e32 v142, v154, v130
	v_lshrrev_b32_e32 v119, 31, v130
	v_add_u32_e32 v119, v130, v119
	v_ashrrev_i32_e32 v120, 1, v119
	v_lshrrev_b32_e32 v119, 31, v142
	v_add_u32_e32 v119, v142, v119
	v_ashrrev_i32_e32 v119, 1, v119
	s_waitcnt vmcnt(11)
	ds_write_b128 v120, v[50:53]
	s_waitcnt vmcnt(10)
	ds_write_b128 v119, v[94:97]
	v_lshrrev_b32_e32 v50, 31, v118
	v_add_u32_e32 v50, v118, v50
	v_ashrrev_i32_e32 v94, 1, v50
	s_mov_b64 s[2:3], exec
.LBB1_25:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[50:53], v94, s[20:23], 0 offen
                                        ; implicit-def: $vgpr94
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_25
; %bb.26:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_add_u32_e32 v94, 0xffffff80, v156
	v_lshrrev_b32_e32 v95, 31, v94
	v_add_u32_e32 v94, v94, v95
	v_ashrrev_i32_e32 v118, 1, v94
	s_mov_b64 s[2:3], exec
.LBB1_27:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[94:97], v118, s[20:23], 0 offen
                                        ; implicit-def: $vgpr118
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_27
; %bb.28:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_ashrrev_i32_e32 v118, 1, v1
	s_mov_b64 s[2:3], exec
.LBB1_29:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v2
	v_readfirstlane_b32 s21, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[2:3]
	v_readfirstlane_b32 s22, v4
	v_readfirstlane_b32 s23, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[122:125], v118, s[20:23], 0 offen
                                        ; implicit-def: $vgpr118
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_29
; %bb.30:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_add_u32_e32 v118, s18, v1
	v_lshrrev_b32_e32 v119, 31, v118
	v_add_u32_e32 v118, v118, v119
	v_ashrrev_i32_e32 v126, 1, v118
	s_mov_b64 s[2:3], exec
.LBB1_31:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v2
	v_readfirstlane_b32 s21, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[2:3]
	v_readfirstlane_b32 s22, v4
	v_readfirstlane_b32 s23, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[118:121], v126, s[20:23], 0 offen
                                        ; implicit-def: $vgpr126
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_31
; %bb.32:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_and_b32_e32 v127, 0xff, v140
	v_and_b32_sdwa v126, s15, v155 dst_sel:DWORD dst_unused:UNUSED_PAD src0_sel:DWORD src1_sel:BYTE_0
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[98:101], v[74:77], v[6:9], v[98:101], v127, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_lshrrev_b16_e32 v155, 8, v155
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[114:117], v[74:77], v[10:13], v[114:117], v127, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v74, v140, 8, 8
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[110:113], v[58:61], v[6:9], v[110:113], v74, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[106:109], v[58:61], v[10:13], v[106:109], v74, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v58, v140, 16, 8
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[102:105], v[34:37], v[6:9], v[102:105], v58, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[82:85], v[34:37], v[10:13], v[82:85], v58, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_lshrrev_b32_e32 v34, 24, v140
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[58:61], v[30:33], v[6:9], v[90:93], v34, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[74:77], v[30:33], v[10:13], v[86:89], v34, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_and_b32_e32 v30, 0xff, v139
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[38:41], v[26:29], v[6:9], v[38:41], v30, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[42:45], v[26:29], v[10:13], v[42:45], v30, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v26, v139, 8, 8
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[34:37], v[22:25], v[6:9], v[46:49], v26, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[26:29], v[22:25], v[10:13], v[66:69], v26, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v22, v139, 16, 8
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[130:133], v146
	ds_read_b128 v[90:93], v145
	v_mfma_scale_f32_16x16x128_f8f6f4 v[30:33], v[18:21], v[6:9], v[54:57], v22, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[86:89], v147
	ds_read_b128 v[66:69], v148
	v_mfma_scale_f32_16x16x128_f8f6f4 v[22:25], v[18:21], v[10:13], v[62:65], v22, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_add_u32_e32 v175, 0x800, v1
	v_lshrrev_b32_e32 v139, 24, v139
	ds_read_b128 v[46:49], v150
	s_nop 0
	v_mfma_scale_f32_16x16x128_f8f6f4 v[18:21], v[14:17], v[6:9], v[70:73], v139, v126 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[54:57], v149
	ds_read_b128 v[62:65], v152
	ds_read_b128 v[126:129], v151
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000100) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000100) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000100) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	v_mfma_scale_f32_16x16x128_f8f6f4 v[78:81], v[14:17], v[10:13], v[78:81], v139, v155 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	; sched_barrier mask(0x00000000)
	v_add_u32_e32 v7, s31, v160
	v_add_u32_e32 v6, s31, v7
	buffer_load_ubyte v165, v160, s[4:7], 0 offen
	buffer_load_ubyte v161, v7, s[4:7], 0 offen
	s_nop 0
	buffer_load_ubyte v160, v6, s[4:7], 0 offen
	v_add_u32_e32 v6, s31, v6
	v_add_u32_e32 v12, s34, v141
	v_add_u32_e32 v7, s33, v12
	buffer_load_ubyte v168, v12, s[8:11], 0 offen
	buffer_load_ubyte v166, v7, s[8:11], 0 offen
	buffer_load_ubyte v162, v6, s[4:7], 0 offen
	v_add_u32_e32 v8, s31, v6
	v_add_u32_e32 v6, s31, v8
	buffer_load_ubyte v11, v6, s[4:7], 0 offen
	v_add_u32_e32 v6, s31, v6
	buffer_load_ubyte v7, v6, s[4:7], 0 offen
	buffer_load_ubyte v10, v8, s[4:7], 0 offen
	v_add_u32_e32 v13, s31, v6
	buffer_load_ubyte v8, v13, s[4:7], 0 offen
	v_add_u32_e32 v9, v142, v138
	v_lshrrev_b32_e32 v6, 31, v157
	v_add_u32_e32 v141, s34, v12
	v_add_u32_e32 v14, v9, v153
	v_lshrrev_b32_e32 v12, 31, v9
	v_add_u32_e32 v6, v157, v6
	v_add_u32_e32 v12, v9, v12
	v_lshrrev_b32_e32 v9, 31, v14
	v_ashrrev_i32_e32 v6, 1, v6
	v_ashrrev_i32_e32 v12, 1, v12
	v_add_u32_e32 v9, v14, v9
	v_ashrrev_i32_e32 v9, 1, v9
	v_add_u32_e32 v12, 0x14000, v12
	v_add_u32_e32 v9, 0x14000, v9
	v_add_u32_e32 v142, s35, v13
	s_waitcnt lgkmcnt(0)
	s_barrier
	s_waitcnt vmcnt(13)
	ds_write_b128 v12, v[50:53]
	s_waitcnt vmcnt(12)
	ds_write_b128 v9, v[94:97]
	s_waitcnt vmcnt(8)
	v_lshlrev_b32_e32 v13, 8, v161
	s_waitcnt vmcnt(7)
	v_lshlrev_b32_e32 v9, 16, v160
	s_waitcnt vmcnt(5)
	v_lshlrev_b16_e32 v15, 8, v166
	s_waitcnt vmcnt(4)
	v_lshlrev_b32_e32 v12, 24, v162
	v_or_b32_e32 v155, v15, v168
	v_or_b32_e32 v12, v13, v12
	s_waitcnt vmcnt(3)
	v_lshlrev_b32_e32 v11, 8, v11
	v_or3_b32 v9, v9, v165, v12
	s_waitcnt vmcnt(2)
	v_lshlrev_b32_e32 v7, 16, v7
	s_waitcnt vmcnt(1)
	v_or_b32_e32 v169, v11, v10
	v_or3_b32 v140, 0, 0, v9
	s_waitcnt vmcnt(0)
	v_lshlrev_b32_e32 v8, 24, v8
	v_or3_b32 v139, v8, v7, v169
	s_mov_b64 s[2:3], exec
.LBB1_33:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[50:53], v6, s[20:23], 0 offen
                                        ; implicit-def: $vgpr6
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_33
; %bb.34:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_lshrrev_b32_e32 v6, 31, v156
	v_add_u32_e32 v6, v156, v6
	v_ashrrev_i32_e32 v6, 1, v6
	s_mov_b64 s[2:3], exec
.LBB1_35:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v136
	v_readfirstlane_b32 s21, v137
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[136:137]
	v_readfirstlane_b32 s22, v134
	v_readfirstlane_b32 s23, v135
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[134:135]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[94:97], v6, s[20:23], 0 offen
                                        ; implicit-def: $vgpr6
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_35
; %bb.36:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_lshrrev_b32_e32 v6, 31, v175
	v_add_u32_e32 v6, v175, v6
	v_ashrrev_i32_e32 v10, 1, v6
	s_mov_b64 s[2:3], exec
.LBB1_37:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v2
	v_readfirstlane_b32 s21, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[2:3]
	v_readfirstlane_b32 s22, v4
	v_readfirstlane_b32 s23, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[6:9], v10, s[20:23], 0 offen
                                        ; implicit-def: $vgpr10
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_37
; %bb.38:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_add_u32_e32 v10, s18, v175
	v_lshrrev_b32_e32 v11, 31, v10
	v_add_u32_e32 v10, v10, v11
	v_ashrrev_i32_e32 v15, 1, v10
	s_mov_b64 s[2:3], exec
.LBB1_39:                               ;   Parent Loop BB1_24 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_readfirstlane_b32 s20, v2
	v_readfirstlane_b32 s21, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[20:21], v[2:3]
	v_readfirstlane_b32 s22, v4
	v_readfirstlane_b32 s23, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[22:23], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[10:13], v15, s[20:23], 0 offen
                                        ; implicit-def: $vgpr15
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_39
; %bb.40:                               ;   in Loop: Header=BB1_24 Depth=1
	s_mov_b64 exec, s[2:3]
	v_mfma_scale_f32_16x16x128_f8f6f4 v[98:101], v[130:133], v[122:125], v[98:101], v174, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[114:117], v[130:133], v[118:121], v[114:117], v174, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[110:113], v[90:93], v[122:125], v[110:113], v173, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[106:109], v[90:93], v[118:121], v[106:109], v173, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[102:105], v[86:89], v[122:125], v[102:105], v172, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[82:85], v[86:89], v[118:121], v[82:85], v172, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[90:93], v[66:69], v[122:125], v[58:61], v171, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[86:89], v[66:69], v[118:121], v[74:77], v171, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[38:41], v[46:49], v[122:125], v[38:41], v170, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[42:45], v[46:49], v[118:121], v[42:45], v170, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_add_u32_e32 v1, v1, v143
	v_add_u32_e32 v130, v14, v138
	v_mfma_scale_f32_16x16x128_f8f6f4 v[46:49], v[54:57], v[122:125], v[34:37], v167, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_add_u32_e32 v14, 0x14000, v146
	v_mfma_scale_f32_16x16x128_f8f6f4 v[66:69], v[54:57], v[118:121], v[26:29], v167, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[74:77], v14
	v_add_u32_e32 v14, 0x14000, v145
	ds_read_b128 v[58:61], v14
	v_add_u32_e32 v14, 0x14000, v147
	v_mfma_scale_f32_16x16x128_f8f6f4 v[54:57], v[62:65], v[122:125], v[30:33], v164, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[34:37], v14
	v_add_u32_e32 v14, 0x14000, v148
	s_nop 0
	ds_read_b128 v[30:33], v14
	v_add_u32_e32 v14, 0x14000, v150
	v_mfma_scale_f32_16x16x128_f8f6f4 v[62:65], v[62:65], v[118:121], v[22:25], v164, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[26:29], v14
	v_add_u32_e32 v14, 0x14000, v149
	s_nop 0
	ds_read_b128 v[22:25], v14
	v_mfma_scale_f32_16x16x128_f8f6f4 v[70:73], v[126:129], v[122:125], v[18:21], v158, v163 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_add_u32_e32 v14, 0x14000, v152
	s_nop 1
	ds_read_b128 v[18:21], v14
	v_add_u32_e32 v14, 0x14000, v151
	ds_read_b128 v[14:17], v14
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000200) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000020) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000100) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000100) size(2) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000100) size(1) SyncID(0)
	; sched_group_barrier mask(0x00000008) size(1) SyncID(0)
	v_mfma_scale_f32_16x16x128_f8f6f4 v[78:81], v[126:129], v[118:121], v[78:81], v158, v159 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	; sched_barrier mask(0x00000000)
	s_add_i32 s28, s28, 2
	v_add_u32_e32 v156, 0x100, v156
	s_cmp_lt_i32 s28, s16
	v_add_u32_e32 v157, 0x100, v157
	s_cbranch_scc1 .LBB1_24
; %bb.41:                               ; %_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE8Run_2LdsILb1ELNS_25InMemoryDataOperationEnumE0ELNS_10TailNumberE1EEEvPKS5_PKS6_SN_SP_PtPvSR_RKNSI_7ProblemE.exit
	v_add_u32_e32 v123, s31, v142
	v_add_u32_e32 v122, s31, v123
	v_add_u32_e32 v121, s31, v122
	v_add_u32_e32 v120, s31, v121
	v_add_u32_e32 v119, s31, v120
	v_add_u32_e32 v118, s31, v119
	buffer_load_ubyte v126, v123, s[4:7], 0 offen
	buffer_load_ubyte v125, v122, s[4:7], 0 offen
	buffer_load_ubyte v124, v121, s[4:7], 0 offen
	s_nop 0
	buffer_load_ubyte v123, v120, s[4:7], 0 offen
	buffer_load_ubyte v122, v119, s[4:7], 0 offen
	s_nop 0
	buffer_load_ubyte v120, v118, s[4:7], 0 offen
	v_add_u32_e32 v118, s31, v118
	buffer_load_ubyte v127, v142, s[4:7], 0 offen
	s_nop 0
	buffer_load_ubyte v118, v118, s[4:7], 0 offen
	v_add_u32_e32 v119, s33, v141
	buffer_load_ubyte v121, v141, s[8:11], 0 offen
	s_nop 0
	buffer_load_ubyte v119, v119, s[8:11], 0 offen
	s_lshl_b64 s[0:1], s[26:27], 1
	s_add_u32 s8, s24, s0
	s_addc_u32 s9, s25, s1
	s_mul_i32 s30, s30, s17
	s_mul_hi_u32 s10, s29, s17
	s_add_i32 s10, s10, s30
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_add_u32_e32 v128, v130, v153
	v_lshrrev_b32_e32 v129, 31, v130
	v_add_u32_e32 v129, v130, v129
	v_ashrrev_i32_e32 v129, 1, v129
	v_lshrrev_b32_e32 v130, 31, v128
	v_add_u32_e32 v128, v128, v130
	v_ashrrev_i32_e32 v128, 1, v128
	s_waitcnt vmcnt(13)
	ds_write_b128 v129, v[50:53]
	s_waitcnt vmcnt(12)
	ds_write_b128 v128, v[94:97]
	v_lshrrev_b32_e32 v50, 31, v1
	v_add_u32_e32 v50, v1, v50
	v_ashrrev_i32_e32 v94, 1, v50
	s_mov_b64 s[2:3], exec
.LBB1_42:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s4, v2
	v_readfirstlane_b32 s5, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[4:5], v[2:3]
	v_readfirstlane_b32 s6, v4
	v_readfirstlane_b32 s7, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[6:7], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[50:53], v94, s[4:7], 0 offen
                                        ; implicit-def: $vgpr94
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_42
; %bb.43:
	s_mov_b64 exec, s[2:3]
	v_add_u32_e32 v1, s18, v1
	v_lshrrev_b32_e32 v94, 31, v1
	v_add_u32_e32 v1, v1, v94
	v_ashrrev_i32_e32 v1, 1, v1
	s_mov_b64 s[2:3], exec
.LBB1_44:                               ; =>This Inner Loop Header: Depth=1
	v_readfirstlane_b32 s4, v2
	v_readfirstlane_b32 s5, v3
	s_nop 1
	v_cmp_eq_u64_e32 vcc, s[4:5], v[2:3]
	v_readfirstlane_b32 s6, v4
	v_readfirstlane_b32 s7, v5
	s_nop 1
	v_cmp_eq_u64_e64 s[0:1], s[6:7], v[4:5]
	s_and_b64 s[0:1], vcc, s[0:1]
	s_and_saveexec_b64 s[0:1], s[0:1]
	buffer_load_dwordx4 v[94:97], v1, s[4:7], 0 offen
                                        ; implicit-def: $vgpr2_vgpr3_vgpr4_vgpr5
                                        ; implicit-def: $vgpr1
	s_xor_b64 exec, exec, s[0:1]
	s_cbranch_execnz .LBB1_44
; %bb.45:
	s_mov_b64 exec, s[2:3]
	v_and_b32_e32 v128, 0xffff, v165
	v_and_b32_e32 v1, 0xffff, v168
	s_waitcnt vmcnt(13)
	s_nop 0
	v_mfma_scale_f32_16x16x128_f8f6f4 v[2:5], v[74:77], v[6:9], v[98:101], v128, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_and_b32_e32 v153, 0xffff, v166
	s_waitcnt vmcnt(12)
	s_nop 0
	v_mfma_scale_f32_16x16x128_f8f6f4 v[174:177], v[74:77], v[10:13], v[114:117], v128, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_and_b32_e32 v74, 0xffff, v161
	s_nop 1
	v_mfma_scale_f32_16x16x128_f8f6f4 v[170:173], v[58:61], v[6:9], v[110:113], v74, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_and_b32_e32 v75, 0xffff, v160
	v_mfma_scale_f32_16x16x128_f8f6f4 v[164:167], v[58:61], v[10:13], v[106:109], v74, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_and_b32_e32 v74, 0xffff, v162
	s_nop 1
	v_and_b32_e32 v106, 0xff, v169
	v_lshrrev_b32_e32 v168, 8, v169
	v_mfma_scale_f32_16x16x128_f8f6f4 v[158:161], v[34:37], v[6:9], v[102:105], v75, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v163, v139, 16, 8
	v_lshrrev_b32_e32 v162, 24, v139
	s_waitcnt lgkmcnt(0)
	v_mfma_scale_f32_16x16x128_f8f6f4 v[140:143], v[34:37], v[10:13], v[82:85], v75, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	s_barrier
	ds_read_b128 v[58:61], v146
	ds_read_b128 v[34:37], v145
	v_mfma_scale_f32_16x16x128_f8f6f4 v[136:139], v[30:33], v[6:9], v[90:93], v74, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[154:157], v147
	ds_read_b128 v[132:135], v148
	ds_read_b128 v[110:113], v150
	ds_read_b128 v[98:101], v149
	v_mfma_scale_f32_16x16x128_f8f6f4 v[128:131], v[30:33], v[10:13], v[86:89], v74, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_read_b128 v[82:85], v152
	ds_read_b128 v[74:77], v151
	v_lshrrev_b32_e32 v30, 2, v0
	v_mfma_scale_f32_16x16x128_f8f6f4 v[114:117], v[26:29], v[6:9], v[38:41], v106, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_and_b32_e32 v30, 48, v30
	v_lshlrev_b32_e32 v31, 4, v0
	v_and_b32_e32 v31, 0x300, v31
	v_mfma_scale_f32_16x16x128_f8f6f4 v[106:109], v[26:29], v[10:13], v[42:45], v106, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_or3_b32 v26, v30, v31, v144
	v_lshrrev_b32_e32 v148, 3, v0
	v_lshlrev_b32_e32 v0, 3, v0
	v_mfma_scale_f32_16x16x128_f8f6f4 v[102:105], v[22:25], v[6:9], v[46:49], v168, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	s_nop 2
	v_and_b32_e32 v49, 56, v0
	v_lshl_or_b32 v147, s19, 7, v148
	v_or_b32_e32 v146, s14, v49
	v_mfma_scale_f32_16x16x128_f8f6f4 v[90:93], v[22:25], v[10:13], v[66:69], v168, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_mad_u64_u32 v[144:145], s[0:1], v147, s17, v[146:147]
	s_waitcnt vmcnt(1) lgkmcnt(0)
	v_mfma_scale_f32_16x16x128_f8f6f4 v[22:25], v[58:61], v[50:53], v[2:5], v127, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	s_barrier
	s_nop 5
	v_bfe_u32 v0, v22, 16, 1
	s_movk_i32 s7, 0x7fff
	v_mfma_scale_f32_16x16x128_f8f6f4 v[86:89], v[18:21], v[6:9], v[54:57], v163, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_add3_u32 v0, v22, v0, s7
	v_lshrrev_b32_e32 v0, 16, v0
	v_mov_b32_e32 v145, 0x7fc0
	v_mfma_scale_f32_16x16x128_f8f6f4 v[66:69], v[18:21], v[10:13], v[62:65], v163, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v22, v22
	s_nop 1
	v_cndmask_b32_e32 v4, v145, v0, vcc
	v_lshlrev_b32_e32 v149, 1, v26
	v_mfma_scale_f32_16x16x128_f8f6f4 v[0:3], v[14:17], v[6:9], v[70:73], v162, v1 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	s_mov_b32 s0, 0x14000
	s_nop 1
	v_or_b32_e32 v73, 0x14000, v149
	ds_write_b16 v73, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[8:11], v[14:17], v[10:13], v[78:81], v162, v153 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v4, v23, 16, 1
	v_add3_u32 v4, v23, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	s_waitcnt vmcnt(0)
	v_mfma_scale_f32_16x16x128_f8f6f4 v[58:61], v[58:61], v[94:97], v[174:177], v127, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v23, v23
	s_nop 1
	v_cndmask_b32_e32 v4, v145, v4, vcc
	v_or_b32_e32 v72, 0x14080, v149
	ds_write_b16 v72, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[62:65], v[34:37], v[50:53], v[170:173], v126, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v4, v24, 16, 1
	v_add3_u32 v4, v24, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[54:57], v[34:37], v[94:97], v[164:167], v126, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v24, v24
	s_nop 1
	v_cndmask_b32_e32 v4, v145, v4, vcc
	v_or_b32_e32 v71, 0x14100, v149
	ds_write_b16 v71, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[36:39], v[154:157], v[50:53], v[158:161], v125, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v4, v25, 16, 1
	v_add3_u32 v4, v25, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[44:47], v[154:157], v[94:97], v[140:143], v125, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v25, v25
	s_nop 1
	v_cndmask_b32_e32 v4, v145, v4, vcc
	v_or_b32_e32 v70, 0x14180, v149
	ds_write_b16 v70, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[32:35], v[132:135], v[50:53], v[136:139], v124, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v4, v65, 16, 1
	v_add3_u32 v4, v65, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[40:43], v[132:135], v[94:97], v[128:131], v124, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v65, v65
	s_nop 1
	v_cndmask_b32_e32 v4, v145, v4, vcc
	v_or_b32_e32 v65, 0x14980, v149
	ds_write_b16 v65, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[28:31], v[110:113], v[50:53], v[114:117], v123, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v4, v64, 16, 1
	v_add3_u32 v4, v64, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[20:23], v[110:113], v[94:97], v[106:109], v123, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v64, v64
	s_nop 1
	v_cndmask_b32_e32 v4, v145, v4, vcc
	v_or_b32_e32 v64, 0x14900, v149
	ds_write_b16 v64, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[24:27], v[98:101], v[50:53], v[102:105], v122, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_bfe_u32 v4, v63, 16, 1
	v_add3_u32 v4, v63, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_mfma_scale_f32_16x16x128_f8f6f4 v[16:19], v[98:101], v[94:97], v[90:93], v122, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_cmp_o_f32_e32 vcc, v63, v63
	s_nop 1
	v_cndmask_b32_e32 v12, v145, v4, vcc
	v_or_b32_e32 v63, 0x14880, v149
	v_mfma_scale_f32_16x16x128_f8f6f4 v[4:7], v[82:85], v[50:53], v[86:89], v120, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	ds_write_b16 v63, v12
	v_bfe_u32 v12, v62, 16, 1
	v_add3_u32 v48, v62, v12, s7
	v_mfma_scale_f32_16x16x128_f8f6f4 v[12:15], v[82:85], v[94:97], v[66:69], v120, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e32 vcc, v62, v62
	s_nop 1
	v_cndmask_b32_e32 v48, v145, v48, vcc
	v_mfma_scale_f32_16x16x128_f8f6f4 v[0:3], v[74:77], v[50:53], v[0:3], v118, v121 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	v_or_b32_e32 v62, 0x14800, v149
	ds_write_b16 v62, v48
	s_waitcnt lgkmcnt(0)
	v_mfma_scale_f32_16x16x128_f8f6f4 v[8:11], v[74:77], v[94:97], v[8:11], v118, v119 op_sel_hi:[0,0,0] cbsz:4 blgp:4
	s_barrier
	v_lshlrev_b32_e32 v48, 7, v148
	v_lshlrev_b32_e32 v49, 1, v49
	v_or3_b32 v53, v48, v49, s0
	ds_read_b128 v[48:51], v53
	v_cmp_gt_i32_e32 vcc, s13, v146
	v_cmp_gt_i32_e64 s[0:1], s12, v147
	s_add_i32 s2, s13, s10
	s_lshl_b32 s10, s2, 1
	s_mov_b32 s11, 0x20000
	v_bfrev_b32_e32 v52, 1
	s_and_b64 s[2:3], s[0:1], vcc
	v_cndmask_b32_e64 v66, v52, 0, s[2:3]
	v_lshl_add_u32 v66, v144, 1, v66
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[48:51], v66, s[8:11], 0 offen
	v_or_b32_e32 v66, 64, v146
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v48, v58, 16, 1
	v_add3_u32 v48, v58, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v58, v58
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v73, v48
	v_bfe_u32 v48, v59, 16, 1
	v_add3_u32 v48, v59, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v59, v59
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v72, v48
	v_bfe_u32 v48, v60, 16, 1
	v_add3_u32 v48, v60, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v60, v60
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v71, v48
	v_bfe_u32 v48, v61, 16, 1
	v_add3_u32 v48, v61, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v61, v61
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v70, v48
	v_bfe_u32 v48, v57, 16, 1
	v_add3_u32 v48, v57, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v57, v57
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v65, v48
	v_bfe_u32 v48, v56, 16, 1
	v_add3_u32 v48, v56, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v56, v56
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v64, v48
	v_bfe_u32 v48, v55, 16, 1
	v_add3_u32 v48, v55, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v55, v55
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v63, v48
	v_bfe_u32 v48, v54, 16, 1
	v_add3_u32 v48, v54, v48, s7
	v_lshrrev_b32_e32 v48, 16, v48
	v_cmp_o_f32_e64 s[2:3], v54, v54
	s_nop 1
	v_cndmask_b32_e64 v48, v145, v48, s[2:3]
	ds_write_b16 v62, v48
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[48:51], v53
	v_cmp_gt_i32_e64 s[2:3], s13, v66
	s_and_b64 s[0:1], s[0:1], s[2:3]
	v_cndmask_b32_e64 v56, v52, 0, s[0:1]
	v_or_b32_e32 v54, 32, v147
	v_add_u32_e32 v55, 64, v144
	v_lshl_add_u32 v56, v55, 1, v56
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[48:51], v56, s[8:11], 0 offen
	s_lshl_b32 s6, s17, 5
	s_nop 0
	v_add_u32_e32 v48, s6, v55
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v49, v44, 16, 1
	v_add3_u32 v49, v44, v49, s7
	v_lshrrev_b32_e32 v49, 16, v49
	v_cmp_o_f32_e64 s[0:1], v44, v44
	s_nop 1
	v_cndmask_b32_e64 v44, v145, v49, s[0:1]
	ds_write_b16 v73, v44
	v_bfe_u32 v44, v45, 16, 1
	v_add3_u32 v44, v45, v44, s7
	v_lshrrev_b32_e32 v44, 16, v44
	v_cmp_o_f32_e64 s[0:1], v45, v45
	s_nop 1
	v_cndmask_b32_e64 v44, v145, v44, s[0:1]
	ds_write_b16 v72, v44
	v_bfe_u32 v44, v46, 16, 1
	v_add3_u32 v44, v46, v44, s7
	v_lshrrev_b32_e32 v44, 16, v44
	v_cmp_o_f32_e64 s[0:1], v46, v46
	s_nop 1
	v_cndmask_b32_e64 v44, v145, v44, s[0:1]
	ds_write_b16 v71, v44
	v_bfe_u32 v44, v47, 16, 1
	v_add3_u32 v44, v47, v44, s7
	v_lshrrev_b32_e32 v44, 16, v44
	v_cmp_o_f32_e64 s[0:1], v47, v47
	s_nop 1
	v_cndmask_b32_e64 v44, v145, v44, s[0:1]
	ds_write_b16 v70, v44
	v_bfe_u32 v44, v43, 16, 1
	v_add3_u32 v44, v43, v44, s7
	v_lshrrev_b32_e32 v44, 16, v44
	v_cmp_o_f32_e64 s[0:1], v43, v43
	s_nop 1
	v_cndmask_b32_e64 v43, v145, v44, s[0:1]
	ds_write_b16 v65, v43
	v_bfe_u32 v43, v42, 16, 1
	v_add3_u32 v43, v42, v43, s7
	v_lshrrev_b32_e32 v43, 16, v43
	v_cmp_o_f32_e64 s[0:1], v42, v42
	s_nop 1
	v_cndmask_b32_e64 v42, v145, v43, s[0:1]
	ds_write_b16 v64, v42
	v_bfe_u32 v42, v41, 16, 1
	v_add3_u32 v42, v41, v42, s7
	v_lshrrev_b32_e32 v42, 16, v42
	v_cmp_o_f32_e64 s[0:1], v41, v41
	s_nop 1
	v_cndmask_b32_e64 v41, v145, v42, s[0:1]
	ds_write_b16 v63, v41
	v_bfe_u32 v41, v40, 16, 1
	v_add3_u32 v41, v40, v41, s7
	v_lshrrev_b32_e32 v41, 16, v41
	v_cmp_o_f32_e64 s[0:1], v40, v40
	s_nop 1
	v_cndmask_b32_e64 v40, v145, v41, s[0:1]
	ds_write_b16 v62, v40
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[40:43], v53
	v_cmp_gt_i32_e64 s[0:1], s12, v54
	s_and_b64 s[4:5], s[0:1], s[2:3]
	v_cndmask_b32_e64 v44, v52, 0, s[4:5]
	v_lshl_add_u32 v44, v48, 1, v44
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[40:43], v44, s[8:11], 0 offen
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v40, v36, 16, 1
	v_add3_u32 v40, v36, v40, s7
	v_lshrrev_b32_e32 v40, 16, v40
	v_cmp_o_f32_e64 s[4:5], v36, v36
	s_nop 1
	v_cndmask_b32_e64 v36, v145, v40, s[4:5]
	ds_write_b16 v73, v36
	v_bfe_u32 v36, v37, 16, 1
	v_add3_u32 v36, v37, v36, s7
	v_lshrrev_b32_e32 v36, 16, v36
	v_cmp_o_f32_e64 s[4:5], v37, v37
	s_nop 1
	v_cndmask_b32_e64 v36, v145, v36, s[4:5]
	ds_write_b16 v72, v36
	v_bfe_u32 v36, v38, 16, 1
	v_add3_u32 v36, v38, v36, s7
	v_lshrrev_b32_e32 v36, 16, v36
	v_cmp_o_f32_e64 s[4:5], v38, v38
	s_nop 1
	v_cndmask_b32_e64 v36, v145, v36, s[4:5]
	ds_write_b16 v71, v36
	v_bfe_u32 v36, v39, 16, 1
	v_add3_u32 v36, v39, v36, s7
	v_lshrrev_b32_e32 v36, 16, v36
	v_cmp_o_f32_e64 s[4:5], v39, v39
	s_nop 1
	v_cndmask_b32_e64 v36, v145, v36, s[4:5]
	ds_write_b16 v70, v36
	v_bfe_u32 v36, v35, 16, 1
	v_add3_u32 v36, v35, v36, s7
	v_lshrrev_b32_e32 v36, 16, v36
	v_cmp_o_f32_e64 s[4:5], v35, v35
	s_nop 1
	v_cndmask_b32_e64 v35, v145, v36, s[4:5]
	ds_write_b16 v65, v35
	v_bfe_u32 v35, v34, 16, 1
	v_add3_u32 v35, v34, v35, s7
	v_lshrrev_b32_e32 v35, 16, v35
	v_cmp_o_f32_e64 s[4:5], v34, v34
	s_nop 1
	v_cndmask_b32_e64 v34, v145, v35, s[4:5]
	ds_write_b16 v64, v34
	v_bfe_u32 v34, v33, 16, 1
	v_add3_u32 v34, v33, v34, s7
	v_lshrrev_b32_e32 v34, 16, v34
	v_cmp_o_f32_e64 s[4:5], v33, v33
	s_nop 1
	v_cndmask_b32_e64 v33, v145, v34, s[4:5]
	ds_write_b16 v63, v33
	v_bfe_u32 v33, v32, 16, 1
	v_add3_u32 v33, v32, v33, s7
	v_lshrrev_b32_e32 v33, 16, v33
	v_cmp_o_f32_e64 s[4:5], v32, v32
	s_nop 1
	v_cndmask_b32_e64 v32, v145, v33, s[4:5]
	ds_write_b16 v62, v32
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[32:35], v53
	v_add_u32_e32 v36, s6, v144
	s_and_b64 s[0:1], vcc, s[0:1]
	v_cndmask_b32_e64 v37, v52, 0, s[0:1]
	v_lshl_add_u32 v37, v36, 1, v37
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[32:35], v37, s[8:11], 0 offen
	s_nop 1
	v_or_b32_e32 v33, 64, v147
	v_add_u32_e32 v32, s6, v36
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v34, v28, 16, 1
	v_add3_u32 v34, v28, v34, s7
	v_lshrrev_b32_e32 v34, 16, v34
	v_cmp_o_f32_e64 s[0:1], v28, v28
	s_nop 1
	v_cndmask_b32_e64 v28, v145, v34, s[0:1]
	ds_write_b16 v73, v28
	v_bfe_u32 v28, v29, 16, 1
	v_add3_u32 v28, v29, v28, s7
	v_lshrrev_b32_e32 v28, 16, v28
	v_cmp_o_f32_e64 s[0:1], v29, v29
	s_nop 1
	v_cndmask_b32_e64 v28, v145, v28, s[0:1]
	ds_write_b16 v72, v28
	v_bfe_u32 v28, v30, 16, 1
	v_add3_u32 v28, v30, v28, s7
	v_lshrrev_b32_e32 v28, 16, v28
	v_cmp_o_f32_e64 s[0:1], v30, v30
	s_nop 1
	v_cndmask_b32_e64 v28, v145, v28, s[0:1]
	ds_write_b16 v71, v28
	v_bfe_u32 v28, v31, 16, 1
	v_add3_u32 v28, v31, v28, s7
	v_lshrrev_b32_e32 v28, 16, v28
	v_cmp_o_f32_e64 s[0:1], v31, v31
	s_nop 1
	v_cndmask_b32_e64 v28, v145, v28, s[0:1]
	ds_write_b16 v70, v28
	v_bfe_u32 v28, v27, 16, 1
	v_add3_u32 v28, v27, v28, s7
	v_lshrrev_b32_e32 v28, 16, v28
	v_cmp_o_f32_e64 s[0:1], v27, v27
	s_nop 1
	v_cndmask_b32_e64 v27, v145, v28, s[0:1]
	ds_write_b16 v65, v27
	v_bfe_u32 v27, v26, 16, 1
	v_add3_u32 v27, v26, v27, s7
	v_lshrrev_b32_e32 v27, 16, v27
	v_cmp_o_f32_e64 s[0:1], v26, v26
	s_nop 1
	v_cndmask_b32_e64 v26, v145, v27, s[0:1]
	ds_write_b16 v64, v26
	v_bfe_u32 v26, v25, 16, 1
	v_add3_u32 v26, v25, v26, s7
	v_lshrrev_b32_e32 v26, 16, v26
	v_cmp_o_f32_e64 s[0:1], v25, v25
	s_nop 1
	v_cndmask_b32_e64 v25, v145, v26, s[0:1]
	ds_write_b16 v63, v25
	v_bfe_u32 v25, v24, 16, 1
	v_add3_u32 v25, v24, v25, s7
	v_lshrrev_b32_e32 v25, 16, v25
	v_cmp_o_f32_e64 s[0:1], v24, v24
	s_nop 1
	v_cndmask_b32_e64 v24, v145, v25, s[0:1]
	ds_write_b16 v62, v24
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[24:27], v53
	v_cmp_gt_i32_e64 s[0:1], s12, v33
	s_and_b64 s[4:5], vcc, s[0:1]
	v_cndmask_b32_e64 v28, v52, 0, s[4:5]
	v_lshl_add_u32 v28, v32, 1, v28
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[24:27], v28, s[8:11], 0 offen
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v24, v20, 16, 1
	v_add3_u32 v24, v20, v24, s7
	v_lshrrev_b32_e32 v24, 16, v24
	v_cmp_o_f32_e64 s[4:5], v20, v20
	s_nop 1
	v_cndmask_b32_e64 v20, v145, v24, s[4:5]
	ds_write_b16 v73, v20
	v_bfe_u32 v20, v21, 16, 1
	v_add3_u32 v20, v21, v20, s7
	v_lshrrev_b32_e32 v20, 16, v20
	v_cmp_o_f32_e64 s[4:5], v21, v21
	s_nop 1
	v_cndmask_b32_e64 v20, v145, v20, s[4:5]
	ds_write_b16 v72, v20
	v_bfe_u32 v20, v22, 16, 1
	v_add3_u32 v20, v22, v20, s7
	v_lshrrev_b32_e32 v20, 16, v20
	v_cmp_o_f32_e64 s[4:5], v22, v22
	s_nop 1
	v_cndmask_b32_e64 v20, v145, v20, s[4:5]
	ds_write_b16 v71, v20
	v_bfe_u32 v20, v23, 16, 1
	v_add3_u32 v20, v23, v20, s7
	v_lshrrev_b32_e32 v20, 16, v20
	v_cmp_o_f32_e64 s[4:5], v23, v23
	s_nop 1
	v_cndmask_b32_e64 v20, v145, v20, s[4:5]
	ds_write_b16 v70, v20
	v_bfe_u32 v20, v19, 16, 1
	v_add3_u32 v20, v19, v20, s7
	v_lshrrev_b32_e32 v20, 16, v20
	v_cmp_o_f32_e64 s[4:5], v19, v19
	s_nop 1
	v_cndmask_b32_e64 v19, v145, v20, s[4:5]
	ds_write_b16 v65, v19
	v_bfe_u32 v19, v18, 16, 1
	v_add3_u32 v19, v18, v19, s7
	v_lshrrev_b32_e32 v19, 16, v19
	v_cmp_o_f32_e64 s[4:5], v18, v18
	s_nop 1
	v_cndmask_b32_e64 v18, v145, v19, s[4:5]
	ds_write_b16 v64, v18
	v_bfe_u32 v18, v17, 16, 1
	v_add3_u32 v18, v17, v18, s7
	v_lshrrev_b32_e32 v18, 16, v18
	v_cmp_o_f32_e64 s[4:5], v17, v17
	s_nop 1
	v_cndmask_b32_e64 v17, v145, v18, s[4:5]
	ds_write_b16 v63, v17
	v_bfe_u32 v17, v16, 16, 1
	v_add3_u32 v17, v16, v17, s7
	v_lshrrev_b32_e32 v17, 16, v17
	v_cmp_o_f32_e64 s[4:5], v16, v16
	s_nop 1
	v_cndmask_b32_e64 v16, v145, v17, s[4:5]
	ds_write_b16 v62, v16
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[16:19], v53
	v_add_u32_e32 v20, 64, v32
	s_and_b64 s[0:1], s[0:1], s[2:3]
	v_cndmask_b32_e64 v21, v52, 0, s[0:1]
	v_lshl_add_u32 v21, v20, 1, v21
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[16:19], v21, s[8:11], 0 offen
	s_nop 1
	v_or_b32_e32 v17, 0x60, v147
	v_add_u32_e32 v16, s6, v20
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v18, v12, 16, 1
	v_add3_u32 v18, v12, v18, s7
	v_lshrrev_b32_e32 v18, 16, v18
	v_cmp_o_f32_e64 s[0:1], v12, v12
	s_nop 1
	v_cndmask_b32_e64 v12, v145, v18, s[0:1]
	ds_write_b16 v73, v12
	v_bfe_u32 v12, v13, 16, 1
	v_add3_u32 v12, v13, v12, s7
	v_lshrrev_b32_e32 v12, 16, v12
	v_cmp_o_f32_e64 s[0:1], v13, v13
	s_nop 1
	v_cndmask_b32_e64 v12, v145, v12, s[0:1]
	ds_write_b16 v72, v12
	v_bfe_u32 v12, v14, 16, 1
	v_add3_u32 v12, v14, v12, s7
	v_lshrrev_b32_e32 v12, 16, v12
	v_cmp_o_f32_e64 s[0:1], v14, v14
	s_nop 1
	v_cndmask_b32_e64 v12, v145, v12, s[0:1]
	ds_write_b16 v71, v12
	v_bfe_u32 v12, v15, 16, 1
	v_add3_u32 v12, v15, v12, s7
	v_lshrrev_b32_e32 v12, 16, v12
	v_cmp_o_f32_e64 s[0:1], v15, v15
	s_nop 1
	v_cndmask_b32_e64 v12, v145, v12, s[0:1]
	ds_write_b16 v70, v12
	v_bfe_u32 v12, v11, 16, 1
	v_add3_u32 v12, v11, v12, s7
	v_lshrrev_b32_e32 v12, 16, v12
	v_cmp_o_f32_e64 s[0:1], v11, v11
	s_nop 1
	v_cndmask_b32_e64 v11, v145, v12, s[0:1]
	ds_write_b16 v65, v11
	v_bfe_u32 v11, v10, 16, 1
	v_add3_u32 v11, v10, v11, s7
	v_lshrrev_b32_e32 v11, 16, v11
	v_cmp_o_f32_e64 s[0:1], v10, v10
	s_nop 1
	v_cndmask_b32_e64 v10, v145, v11, s[0:1]
	ds_write_b16 v64, v10
	v_bfe_u32 v10, v9, 16, 1
	v_add3_u32 v10, v9, v10, s7
	v_lshrrev_b32_e32 v10, 16, v10
	v_cmp_o_f32_e64 s[0:1], v9, v9
	s_nop 1
	v_cndmask_b32_e64 v9, v145, v10, s[0:1]
	ds_write_b16 v63, v9
	v_bfe_u32 v9, v8, 16, 1
	v_add3_u32 v9, v8, v9, s7
	v_lshrrev_b32_e32 v9, 16, v9
	v_cmp_o_f32_e64 s[0:1], v8, v8
	s_nop 1
	v_cndmask_b32_e64 v8, v145, v9, s[0:1]
	ds_write_b16 v62, v8
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[8:11], v53
	v_cmp_gt_i32_e64 s[0:1], s12, v17
	s_and_b64 s[2:3], s[0:1], s[2:3]
	v_cndmask_b32_e64 v12, v52, 0, s[2:3]
	v_lshl_add_u32 v12, v16, 1, v12
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[8:11], v12, s[8:11], 0 offen
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_bfe_u32 v8, v4, 16, 1
	v_add3_u32 v8, v4, v8, s7
	v_lshrrev_b32_e32 v8, 16, v8
	v_cmp_o_f32_e64 s[2:3], v4, v4
	s_nop 1
	v_cndmask_b32_e64 v4, v145, v8, s[2:3]
	ds_write_b16 v73, v4
	v_bfe_u32 v4, v5, 16, 1
	v_add3_u32 v4, v5, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_cmp_o_f32_e64 s[2:3], v5, v5
	s_nop 1
	v_cndmask_b32_e64 v4, v145, v4, s[2:3]
	ds_write_b16 v72, v4
	v_bfe_u32 v4, v6, 16, 1
	v_add3_u32 v4, v6, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_cmp_o_f32_e64 s[2:3], v6, v6
	s_nop 1
	v_cndmask_b32_e64 v4, v145, v4, s[2:3]
	ds_write_b16 v71, v4
	v_bfe_u32 v4, v7, 16, 1
	v_add3_u32 v4, v7, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_cmp_o_f32_e64 s[2:3], v7, v7
	s_nop 1
	v_cndmask_b32_e64 v4, v145, v4, s[2:3]
	ds_write_b16 v70, v4
	v_bfe_u32 v4, v3, 16, 1
	v_add3_u32 v4, v3, v4, s7
	v_lshrrev_b32_e32 v4, 16, v4
	v_cmp_o_f32_e64 s[2:3], v3, v3
	s_nop 1
	v_cndmask_b32_e64 v3, v145, v4, s[2:3]
	ds_write_b16 v65, v3
	v_bfe_u32 v3, v2, 16, 1
	v_add3_u32 v3, v2, v3, s7
	v_lshrrev_b32_e32 v3, 16, v3
	v_cmp_o_f32_e64 s[2:3], v2, v2
	s_nop 1
	v_cndmask_b32_e64 v2, v145, v3, s[2:3]
	ds_write_b16 v64, v2
	v_bfe_u32 v2, v1, 16, 1
	v_add3_u32 v2, v1, v2, s7
	v_lshrrev_b32_e32 v2, 16, v2
	v_cmp_o_f32_e64 s[2:3], v1, v1
	s_nop 1
	v_cndmask_b32_e64 v1, v145, v2, s[2:3]
	ds_write_b16 v63, v1
	v_bfe_u32 v1, v0, 16, 1
	v_add3_u32 v1, v0, v1, s7
	v_lshrrev_b32_e32 v1, 16, v1
	v_cmp_o_f32_e64 s[2:3], v0, v0
	s_nop 1
	v_cndmask_b32_e64 v0, v145, v1, s[2:3]
	ds_write_b16 v62, v0
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read_b128 v[0:3], v53
	v_add_u32_e32 v4, s6, v32
	s_and_b64 s[0:1], vcc, s[0:1]
	v_cndmask_b32_e64 v5, v52, 0, s[0:1]
	v_lshl_add_u32 v4, v4, 1, v5
	s_waitcnt lgkmcnt(0)
	buffer_store_dwordx4 v[0:3], v4, s[8:11], 0 offen
	s_endpgm
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
	.amdhsa_kernel _ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE
		.amdhsa_group_segment_fixed_size 90112
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_kernarg_size 144
		.amdhsa_user_sgpr_count 2
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_kernarg_preload_length 0
		.amdhsa_user_sgpr_kernarg_preload_offset 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_uses_dynamic_stack 0
		.amdhsa_enable_private_segment 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 0
		.amdhsa_system_sgpr_workgroup_id_z 1
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 0
		.amdhsa_next_free_vgpr 178
		.amdhsa_next_free_sgpr 41
		.amdhsa_accum_offset 180
		.amdhsa_reserve_vcc 1
		.amdhsa_float_round_mode_32 0
		.amdhsa_float_round_mode_16_64 0
		.amdhsa_float_denorm_mode_32 3
		.amdhsa_float_denorm_mode_16_64 3
		.amdhsa_dx10_clamp 1
		.amdhsa_ieee_mode 1
		.amdhsa_fp16_overflow 0
		.amdhsa_tg_split 0
		.amdhsa_exception_fp_ieee_invalid_op 0
		.amdhsa_exception_fp_denorm_src 0
		.amdhsa_exception_fp_ieee_div_zero 0
		.amdhsa_exception_fp_ieee_overflow 0
		.amdhsa_exception_fp_ieee_underflow 0
		.amdhsa_exception_fp_ieee_inexact 0
		.amdhsa_exception_int_div_zero 0
	.end_amdhsa_kernel
	.section	.text._ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE,"axG",@progbits,_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE,comdat
.Lfunc_end1:
	.size	_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE, .Lfunc_end1-_ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE
                                        ; -- End function
	.section	.AMDGPU.csdata,"",@progbits
; Kernel info:
; codeLenInByte = 9668
; NumSgprs: 47
; NumVgprs: 178
; NumAgprs: 0
; TotalNumVgprs: 178
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 90112 bytes/workgroup (compile time only)
; SGPRBlocks: 5
; VGPRBlocks: 22
; NumSGPRsForWavesPerEU: 47
; NumVGPRsForWavesPerEU: 178
; AccumOffset: 180
; Occupancy: 1
; WaveLimiterHint : 0
; COMPUTE_PGM_RSRC2:SCRATCH_EN: 0
; COMPUTE_PGM_RSRC2:USER_SGPR: 2
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 1
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0
; COMPUTE_PGM_RSRC3_GFX90A:ACCUM_OFFSET: 44
; COMPUTE_PGM_RSRC3_GFX90A:TG_SPLIT: 0
	.text
	.p2alignl 6, 3212836864
	.fill 256, 4, 3212836864
	.protected	_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE ; @_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE
	.type	_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE,@object
	.section	.rodata._ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE,"aG",@progbits,_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE,comdat
	.weak	_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE
	.p2align	2, 0x0
_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE:
	.long	4                               ; 0x4
	.size	_ZN2ck43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS2_11ColumnMajorES3_NS_9f4x2_pk_tENS_11e8m0_bexp_tES5_S6_fttNS_16tensor_operation12element_wise11PassThroughES9_S9_LNS7_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSC_IJLi1ELi0ELi2EEEESE_Li2ELi32ELi32ELb0ELi0ESD_SE_SE_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSC_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES5_S5_Lb0ELb0EE5NWaveE, 4

	.protected	_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE ; @_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE
	.type	_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE,@object
	.section	.rodata._ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE,"aG",@progbits,_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE,comdat
	.weak	_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE
	.p2align	2, 0x0
_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE:
	.long	4                               ; 0x4
	.size	_ZN2ck46BlockwiseGemmXdlops_pipeline_bpreshuffle_v3_mxILNS_26BlockGemmPipelineSchedulerE0ELi256ELi32ENS_9f4x2_pk_tENS_11e8m0_bexp_tES2_S3_NS_16TensorDescriptorINS_5TupleIJNS_5EmbedINS5_IJNS_17integral_constantIiLi4EEENS7_IiLi128EEENS7_IiLi32EEEEEENS5_IJSA_S9_NS7_IiLi1EEEEEELb0EEENS_3XorINS5_IJS9_S8_EEELb1EEENS_11PassThroughISA_EEEEENS5_IJNS_8SequenceIJLi0EEEENSL_IJLi2ELi1EEEENSL_IJLi3EEEEEEENS5_IJNSL_IJLi1ELi2ELi3EEEENSL_IJLi4ELi5EEEENSL_IJLi6EEEEEEENSL_IJLi5ELi4ELi6EEEENS7_IlLl16384EEEEENS4_INS5_IJNS_7UnMergeINS5_IJNS7_IiLi2EEESC_SC_SA_EEELb0EEEEEENS5_IJSM_EEENS5_IJNSL_IJLi1ELi2ELi3ELi4EEEEEEES13_NS7_IlLl64EEEEENS4_INS5_IJSE_SH_SJ_NS_21Merge_v3_division_modINS5_IJS8_SA_EEEEENSX_INS5_IJNS7_IiLi8EEESC_NS7_IiLi16EEEEEELb0EEEEEENS5_IJSM_SN_SO_NSL_IJLi5ELi6EEEENSL_IJLi4EEEEEEENS5_IJSQ_SR_SS_NSL_IJLi7EEEENSL_IJLi8ELi9ELi10EEEEEEENSL_IJLi8ELi9ELi10ELi7EEEESV_EENS4_INS5_IJS10_NS17_INS5_IJSY_SC_EEEEENSX_INS5_IJSY_S8_S1B_EEELb0EEEEEENS5_IJSM_NSL_IJLi1ELi3EEEENSL_IJLi2EEEEEEENS5_IJS13_NSL_IJLi5EEEENSL_IJLi6ELi7ELi8EEEEEEENSL_IJLi6ELi7ELi8ELi5EEEES15_EELi32ELi32ELi128ELi128ELi128ELi16ELi16ELi8ELi2ELi32EE19ScalesPerKBlockSizeE, 4

	.type	__hip_cuid_2e1668655e2f5252,@object ; @__hip_cuid_2e1668655e2f5252
	.section	.bss,"aw",@nobits
	.globl	__hip_cuid_2e1668655e2f5252
__hip_cuid_2e1668655e2f5252:
	.byte	0                               ; 0x0
	.size	__hip_cuid_2e1668655e2f5252, 1

	.ident	"AMD clang version 19.0.0git (ssh://github-emu/AMD-Lightning-Internal/llvm-project  25102 f26e3467aefc6ac1a50fa9c45406f9636d5aad86)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __hip_cuid_2e1668655e2f5252
	.amdgpu_metadata
---
amdhsa.kernels:
  - .agpr_count:     0
    .args:           []
    .group_segment_fixed_size: 0
    .kernarg_segment_align: 4
    .kernarg_segment_size: 0
    .language:       OpenCL C
    .language_version:
      - 2
      - 0
    .max_flat_workgroup_size: 1024
    .name:           _ZN2ckL12flush_icacheEv
    .private_segment_fixed_size: 0
    .sgpr_count:     6
    .sgpr_spill_count: 0
    .symbol:         _ZN2ckL12flush_icacheEv.kd
    .uses_dynamic_stack: false
    .vgpr_count:     0
    .vgpr_spill_count: 0
    .wavefront_size: 64
  - .agpr_count:     0
    .args:
      - .offset:         0
        .size:           144
        .value_kind:     by_value
    .group_segment_fixed_size: 90112
    .kernarg_segment_align: 8
    .kernarg_segment_size: 144
    .language:       OpenCL C
    .language_version:
      - 2
      - 0
    .max_flat_workgroup_size: 256
    .name:           _ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE
    .private_segment_fixed_size: 0
    .sgpr_count:     47
    .sgpr_spill_count: 0
    .symbol:         _ZN2ck45kernel_gemm_xdl_cshuffle_v3_b_preshuffle_2ldsINS_43GridwiseGemmMX_xdl_cshuffle_v3_b_preshuffleINS_13tensor_layout4gemm8RowMajorENS3_11ColumnMajorES4_NS_9f4x2_pk_tENS_11e8m0_bexp_tES6_S7_fttNS_16tensor_operation12element_wise11PassThroughESA_SA_LNS8_6device18GemmSpecializationE0ELi32ELi256ELi128ELi128ELi128ELi32ELi32ELi16ELi16ELi8ELi2ENS_8SequenceIJLi4ELi64ELi1EEEENSD_IJLi1ELi0ELi2EEEESF_Li2ELi32ELi32ELb0ELi0ESE_SF_SF_Li2ELi32ELi32ELb0ELi0ELi2ELi1ENSD_IJLi1ELi32ELi1ELi8EEEELi8ELNS_26BlockGemmPipelineSchedulerE0ELNS_24BlockGemmPipelineVersionE2ES6_S6_Lb0ELb0EEELb1ELNS_25InMemoryDataOperationEnumE0ELi2ELNS_10TailNumberE1EEEvNT_8ArgumentE.kd
    .uses_dynamic_stack: false
    .vgpr_count:     178
    .vgpr_spill_count: 0
    .wavefront_size: 64
amdhsa.target:   amdgcn-amd-amdhsa--gfx950
amdhsa.version:
  - 1
  - 2
...

	.end_amdgpu_metadata
