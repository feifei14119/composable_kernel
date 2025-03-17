// acc: use 160, free 96 ------------------
// acc[0..15]    (16) : B_ping, K(0~127)
// acc[16..31]   (16) : B_pang, K(0~127)
// acc[32..47]   (16) : B_ping, K(128~256)
// acc[48..63]   (16) : B_pang, K(128~256)
// acc[64..127]  (64) : A_ping
// acc[128..191] (64) : A_pang
// acc[192..255] (64) : free
// ----------------------------------------
// vgpr: use 64~195, free 0 ---------------
// v[50]         (1)  : scale B temp
// v[56:59]           : acc_in_tmp1
// v[60:63]           : acc_in_tmp2
// v[64..127]    (64) : Acc result, float4 c[0..16]
// v[128..191]   (64) : Acc result, float4 c[16..31]
// v[192..207]   (16) : A_ping only for vmem->vgpr->lds
// v[208..223]   (16) : A_pang only for vmem->vgpr->lds
// v[224..231]   (8)  : scale A ping1
// v[232..239]   (8)  : scale A pang1
// v[240..247]   (8)  : scale A ping2
// v[248..255]   (8)  : scale A pang2
// ----------------------------------------
// s[30..31]     (2)  : scale B ping
// s[84..85]     (2)  : scale B pang

#pragma region FEIFEI_MACRO
#ifndef _A_VM_LDS_
#define _A_VM_LDS_ true
#endif
#ifndef _SCALE_A_LDS_
#define _SCALE_A_LDS_ true
#endif
#define _UK_MFMA_ "v_mfma_f32_16x16x32_fp8_fp8"
#define _FF_SYNC_ " s_waitcnt vmcnt(0)      \n" \
                  " s_waitcnt lgkmcnt(0)    \n" \
                  " s_barrier               \n" \
                  " s_nop 16                \n"
#define _FF_END_  " s_branch L_end%=        \n"
#define _INIT_ACC_ \
    " v_mov_b32 %[v_c_0x],  0 \n" \
    " v_mov_b32 %[v_c_0y],  0 \n" \
    " v_mov_b32 %[v_c_0z],  0 \n" \
    " v_mov_b32 %[v_c_0w],  0 \n" \
    " v_mov_b32 %[v_c_1x],  0 \n" \
    " v_mov_b32 %[v_c_1y],  0 \n" \
    " v_mov_b32 %[v_c_1z],  0 \n" \
    " v_mov_b32 %[v_c_1w],  0 \n" \
    " v_mov_b32 %[v_c_2x],  0 \n" \
    " v_mov_b32 %[v_c_2y],  0 \n" \
    " v_mov_b32 %[v_c_2z],  0 \n" \
    " v_mov_b32 %[v_c_2w],  0 \n" \
    " v_mov_b32 %[v_c_3x],  0 \n" \
    " v_mov_b32 %[v_c_3y],  0 \n" \
    " v_mov_b32 %[v_c_3z],  0 \n" \
    " v_mov_b32 %[v_c_3w],  0 \n" \
    " v_mov_b32 %[v_c_4x],  0 \n" \
    " v_mov_b32 %[v_c_4y],  0 \n" \
    " v_mov_b32 %[v_c_4z],  0 \n" \
    " v_mov_b32 %[v_c_4w],  0 \n" \
    " v_mov_b32 %[v_c_5x],  0 \n" \
    " v_mov_b32 %[v_c_5y],  0 \n" \
    " v_mov_b32 %[v_c_5z],  0 \n" \
    " v_mov_b32 %[v_c_5w],  0 \n" \
    " v_mov_b32 %[v_c_6x],  0 \n" \
    " v_mov_b32 %[v_c_6y],  0 \n" \
    " v_mov_b32 %[v_c_6z],  0 \n" \
    " v_mov_b32 %[v_c_6w],  0 \n" \
    " v_mov_b32 %[v_c_7x],  0 \n" \
    " v_mov_b32 %[v_c_7y],  0 \n" \
    " v_mov_b32 %[v_c_7z],  0 \n" \
    " v_mov_b32 %[v_c_7w],  0 \n" \
    " v_mov_b32 %[v_c_8x],  0 \n" \
    " v_mov_b32 %[v_c_8y],  0 \n" \
    " v_mov_b32 %[v_c_8z],  0 \n" \
    " v_mov_b32 %[v_c_8w],  0 \n" \
    " v_mov_b32 %[v_c_9x],  0 \n" \
    " v_mov_b32 %[v_c_9y],  0 \n" \
    " v_mov_b32 %[v_c_9z],  0 \n" \
    " v_mov_b32 %[v_c_9w],  0 \n" \
    " v_mov_b32 %[v_c_10x], 0 \n" \
    " v_mov_b32 %[v_c_10y], 0 \n" \
    " v_mov_b32 %[v_c_10z], 0 \n" \
    " v_mov_b32 %[v_c_10w], 0 \n" \
    " v_mov_b32 %[v_c_11x], 0 \n" \
    " v_mov_b32 %[v_c_11y], 0 \n" \
    " v_mov_b32 %[v_c_11z], 0 \n" \
    " v_mov_b32 %[v_c_11w], 0 \n" \
    " v_mov_b32 %[v_c_12x], 0 \n" \
    " v_mov_b32 %[v_c_12y], 0 \n" \
    " v_mov_b32 %[v_c_12z], 0 \n" \
    " v_mov_b32 %[v_c_12w], 0 \n" \
    " v_mov_b32 %[v_c_13x], 0 \n" \
    " v_mov_b32 %[v_c_13y], 0 \n" \
    " v_mov_b32 %[v_c_13z], 0 \n" \
    " v_mov_b32 %[v_c_13w], 0 \n" \
    " v_mov_b32 %[v_c_14x], 0 \n" \
    " v_mov_b32 %[v_c_14y], 0 \n" \
    " v_mov_b32 %[v_c_14z], 0 \n" \
    " v_mov_b32 %[v_c_14w], 0 \n" \
    " v_mov_b32 %[v_c_15x], 0 \n" \
    " v_mov_b32 %[v_c_15y], 0 \n" \
    " v_mov_b32 %[v_c_15z], 0 \n" \
    " v_mov_b32 %[v_c_15w], 0 \n" \
    " v_mov_b32 %[v_c_16x], 0 \n" \
    " v_mov_b32 %[v_c_16y], 0 \n" \
    " v_mov_b32 %[v_c_16z], 0 \n" \
    " v_mov_b32 %[v_c_16w], 0 \n" \
    " v_mov_b32 %[v_c_17x], 0 \n" \
    " v_mov_b32 %[v_c_17y], 0 \n" \
    " v_mov_b32 %[v_c_17z], 0 \n" \
    " v_mov_b32 %[v_c_17w], 0 \n" \
    " v_mov_b32 %[v_c_18x], 0 \n" \
    " v_mov_b32 %[v_c_18y], 0 \n" \
    " v_mov_b32 %[v_c_18z], 0 \n" \
    " v_mov_b32 %[v_c_18w], 0 \n" \
    " v_mov_b32 %[v_c_19x], 0 \n" \
    " v_mov_b32 %[v_c_19y], 0 \n" \
    " v_mov_b32 %[v_c_19z], 0 \n" \
    " v_mov_b32 %[v_c_19w], 0 \n" \
    " v_mov_b32 %[v_c_20x], 0 \n" \
    " v_mov_b32 %[v_c_20y], 0 \n" \
    " v_mov_b32 %[v_c_20z], 0 \n" \
    " v_mov_b32 %[v_c_20w], 0 \n" \
    " v_mov_b32 %[v_c_21x], 0 \n" \
    " v_mov_b32 %[v_c_21y], 0 \n" \
    " v_mov_b32 %[v_c_21z], 0 \n" \
    " v_mov_b32 %[v_c_21w], 0 \n" \
    " v_mov_b32 %[v_c_22x], 0 \n" \
    " v_mov_b32 %[v_c_22y], 0 \n" \
    " v_mov_b32 %[v_c_22z], 0 \n" \
    " v_mov_b32 %[v_c_22w], 0 \n" \
    " v_mov_b32 %[v_c_23x], 0 \n" \
    " v_mov_b32 %[v_c_23y], 0 \n" \
    " v_mov_b32 %[v_c_23z], 0 \n" \
    " v_mov_b32 %[v_c_23w], 0 \n" \
    " v_mov_b32 %[v_c_24x], 0 \n" \
    " v_mov_b32 %[v_c_24y], 0 \n" \
    " v_mov_b32 %[v_c_24z], 0 \n" \
    " v_mov_b32 %[v_c_24w], 0 \n" \
    " v_mov_b32 %[v_c_25x], 0 \n" \
    " v_mov_b32 %[v_c_25y], 0 \n" \
    " v_mov_b32 %[v_c_25z], 0 \n" \
    " v_mov_b32 %[v_c_25w], 0 \n" \
    " v_mov_b32 %[v_c_26x], 0 \n" \
    " v_mov_b32 %[v_c_26y], 0 \n" \
    " v_mov_b32 %[v_c_26z], 0 \n" \
    " v_mov_b32 %[v_c_26w], 0 \n" \
    " v_mov_b32 %[v_c_27x], 0 \n" \
    " v_mov_b32 %[v_c_27y], 0 \n" \
    " v_mov_b32 %[v_c_27z], 0 \n" \
    " v_mov_b32 %[v_c_27w], 0 \n" \
    " v_mov_b32 %[v_c_28x], 0 \n" \
    " v_mov_b32 %[v_c_28y], 0 \n" \
    " v_mov_b32 %[v_c_28z], 0 \n" \
    " v_mov_b32 %[v_c_28w], 0 \n" \
    " v_mov_b32 %[v_c_29x], 0 \n" \
    " v_mov_b32 %[v_c_29y], 0 \n" \
    " v_mov_b32 %[v_c_29z], 0 \n" \
    " v_mov_b32 %[v_c_29w], 0 \n" \
    " v_mov_b32 %[v_c_30x], 0 \n" \
    " v_mov_b32 %[v_c_30y], 0 \n" \
    " v_mov_b32 %[v_c_30z], 0 \n" \
    " v_mov_b32 %[v_c_30w], 0 \n" \
    " v_mov_b32 %[v_c_31x], 0 \n" \
    " v_mov_b32 %[v_c_31y], 0 \n" \
    " v_mov_b32 %[v_c_31z], 0 \n" \
    " v_mov_b32 %[v_c_31w], 0 \n"
#pragma endregion

    "s_mov_b32 s16, %[s_res_a0] \n"
    "s_mov_b32 s17, %[s_res_a1] \n"
    "s_mov_b32 s18, %[s_res_a2] \n"
    "s_mov_b32 s19, %[s_res_a3] \n"
    "s_mov_b32 s20, %[s_res_b0] \n"
    "s_mov_b32 s21, %[s_res_b1] \n"
    "s_mov_b32 s22, %[s_res_b2] \n"
    "s_mov_b32 s23, %[s_res_b3] \n"

    "s_mov_b32 s24, %[s_res_sa0]   \n"
    "s_mov_b32 s25, %[s_res_sa1]   \n"
    "s_mov_b32 s26, %[s_res_sa2]   \n"
    "s_mov_b32 s27, %[s_res_sa3]   \n"
    "s_mov_b32 s28, %[s_base_sb_l] \n"
    "s_mov_b32 s29, %[s_base_sb_h] \n"

    _INIT_ACC_

// address and mfma debug
#if 0
#pragma region DEBUG_MOVE_TILE
// use tileM * tileN * 2tileK gemm
#if 1
    // move to next A
    "s_add_u32     s16, %[s_tile_os_a], s16    \n"
    "s_addc_u32    s17, 0, s17                 \n"
    // move to next B
    "s_add_u32     s20, %[s_tile_os_b], s20    \n"
    "s_addc_u32    s21, 0, s21                 \n"
    // move to next SA
    "s_add_u32     s24, %[s_tile_sa], s24      \n"
    "s_addc_u32    s25, 0, s25                 \n"
    // move to next SB
    "s_add_u32     s28, %[s_tile_sb], s28      \n"
    "s_addc_u32    s29, 0, s29                 \n"
#endif
#pragma endregion

#pragma region DEBUG_ADDRESS
#if _A_VM_LDS_
    // A: vmem -> lds -> accvgpr
    _FF_SYNC_
    // debug A vmem
    _INIT_ACC_
    " buffer_load_dword  %[v_c_0x], %[v_a_vm_ld_addr0],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_0y], %[v_a_vm_ld_addr1],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_0z], %[v_a_vm_ld_addr2],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_0w], %[v_a_vm_ld_addr3],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_1x], %[v_a_vm_ld_addr4],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_1y], %[v_a_vm_ld_addr5],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_1z], %[v_a_vm_ld_addr6],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_1w], %[v_a_vm_ld_addr7],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_2x], %[v_a_vm_ld_addr8],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_2y], %[v_a_vm_ld_addr9],  s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_2z], %[v_a_vm_ld_addr10], s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_2w], %[v_a_vm_ld_addr11], s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_3x], %[v_a_vm_ld_addr12], s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_3y], %[v_a_vm_ld_addr13], s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_3z], %[v_a_vm_ld_addr14], s[16:19], 0 offen \n"
    " buffer_load_dword  %[v_c_3w], %[v_a_vm_ld_addr15], s[16:19], 0 offen \n"
    _FF_SYNC_
    // debug A vmem -> lds
    _INIT_ACC_
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 0   \n" // to lds ping
    //" s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 1   \n" // to lds pang
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds     \n"
    _FF_SYNC_
    // debug lds
    _INIT_ACC_
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_dbg_tid] offset: %[n_a_lds_pingpang_sz] * 0 \n" // debug ping
    //" ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_dbg_tid] offset: %[n_a_lds_pingpang_sz] * 1 \n" // debug pang
    _FF_SYNC_
#else
    // A: vmem -> vgpr -> lds -> accvgpr
    _FF_SYNC_
    // debug A vmem ------------------------------------------------------
    " buffer_load_dwordx4  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_vm_ld_addr0],  s[16:19], 0 offen \n"
    " buffer_load_dwordx4  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_vm_ld_addr1],  s[16:19], 0 offen \n"
    " buffer_load_dwordx4  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_vm_ld_addr2],  s[16:19], 0 offen \n"
    " buffer_load_dwordx4  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_vm_ld_addr3],  s[16:19], 0 offen \n"
    _FF_SYNC_
    // debug A lds write ------------------------------------------------
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]]  offset: %[n_a_lds_wr_offset] * 0 + %[n_a_lds_pingpang_sz]  \n"
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]]  offset: %[n_a_lds_wr_offset] * 1 + %[n_a_lds_pingpang_sz]  \n"
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]]  offset: %[n_a_lds_wr_offset] * 2 + %[n_a_lds_pingpang_sz]  \n"
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]]  offset: %[n_a_lds_wr_offset] * 3 + %[n_a_lds_pingpang_sz]  \n"
    _FF_SYNC_
#endif
    // debug A lds ping read 0/2/4/6/8/10/12/14/16 ---------------------------
    _INIT_ACC_
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    " ds_read_b128  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz] * 0  \n"
    _FF_SYNC_
    // debug A lds pang read 1/3/5/7/9/11/13/15/17 ---------------------------
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    " ds_read_b128  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz] * 1  \n"
    _FF_SYNC_
    // debug B vmem -----------------------------------------------------
    " buffer_load_dwordx4  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n"
    " buffer_load_dwordx4  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n"
    " buffer_load_dwordx4  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n"
    " buffer_load_dwordx4  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n"

    " buffer_load_dwordx4  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K
    _FF_SYNC_
    // debug scale A vmem -----------------------------------------------
    _INIT_ACC_
#if _SCALE_A_LDS_
    " buffer_load_dword    %[v_c_0x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen \n"
    _FF_SYNC_
    " s_add_u32     m0,    %[s_sa_vm_lds_m0], %[n_a_lds_pingpang_sz] * 2    \n" // to lds sa
    " buffer_load_dword    %[v_sa_vm_ld_addr],  s[24:27], 0 offen lds       \n"
    _FF_SYNC_
    // debug lds
    _INIT_ACC_
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_dbg_tid] offset: %[n_a_lds_pingpang_sz] * 2 \n"
    _FF_SYNC_
    _INIT_ACC_
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_sa_lds_rd_addr] offset: %[n_a_lds_pingpang_sz] * 2 + %[n_sa_lds_rd_offset] * 0 \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_sa_lds_rd_addr] offset: %[n_a_lds_pingpang_sz] * 2 + %[n_sa_lds_rd_offset] * 1 \n"
    _FF_SYNC_
#else
    " buffer_load_dword    %[v_c_0x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0   \n"
    " buffer_load_dword    %[v_c_0y], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1   \n"
    " buffer_load_dword    %[v_c_0z], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2   \n"
    " buffer_load_dword    %[v_c_0w], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3   \n"
    " buffer_load_dword    %[v_c_1x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4   \n"
    " buffer_load_dword    %[v_c_1y], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5   \n"
    " buffer_load_dword    %[v_c_1z], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6   \n"
    " buffer_load_dword    %[v_c_1w], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7   \n"
#endif    
    _FF_SYNC_
    // debug scale B vmem ---------------------------------------------
    " s_load_dwordx2  s[30:31], s[28:29],  0    \n"
    " s_waitcnt lgkmcnt(0)                      \n"
    " v_mov_b32            %[v_c_2x], 1.0       \n"
    " v_mov_b32            %[v_c_2y], s30       \n"
    " v_mov_b32            %[v_c_2z], 1.0       \n"
    " v_mov_b32            %[v_c_2w], s31       \n"
    _FF_SYNC_
#pragma endregion
    _FF_END_

#pragma region DEBUG_MFMA
#if _A_VM_LDS_
    // debug A vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 0   \n" // to lds ping
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds     \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0                           \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds     \n"
    " s_waitcnt vmcnt(0)                                                \n"
    " s_barrier                                                         \n"
#else
    _FF_SYNC_
    // A: vmem->vgpr
    " buffer_load_dwordx4  v[192:195], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[196:199], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[200:203], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[204:207], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[192:195]  offset: %[n_a_lds_wr_offset] * 0    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[196:199]  offset: %[n_a_lds_wr_offset] * 1    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[200:203]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[204:207]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " s_waitcnt lgkmcnt(0)                                                              \n"
    " s_barrier                                                                         \n"
#endif
    // A: lds->accvgpr
    " ds_read_b128  acc[64:67],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0   \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[68:71],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1   \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[72:75],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0   \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[76:79],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1   \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[80:83],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0   \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[84:87],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1   \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[88:91],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0   \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[92:95],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1   \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[96:99],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0   \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[100:103], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1   \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[104:107], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0   \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[108:111], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1   \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[112:115], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0   \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[116:119], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1   \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[120:123], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0   \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[124:127], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1   \n" // [112~127]M * [64~127]K
    " s_waitcnt lgkmcnt(0)                                                                                              \n"
    // B: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[32:35], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[36:39], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[40:43], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[44:47], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [192~255]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale A: vmem->vgpr 
    " buffer_load_dword    v224, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v225, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v226, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v227, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v228, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v229, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v230, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v231, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale B: vmem->sgpr->vgpr 
    " s_load_dwordx2       s[30:31], s[28:29],  0                       \n"
    " s_waitcnt lgkmcnt(0)                                              \n"
    // scale c = scale a * scale b
    " v_mov_b32  v50, s31                                               \n"
    " v_mul_f32  v240, v224, v50                                        \n"
    " v_mul_f32  v241, v225, v50                                        \n"
    " v_mul_f32  v242, v226, v50                                        \n"
    " v_mul_f32  v243, v227, v50                                        \n"
    " v_mul_f32  v244, v228, v50                                        \n"
    " v_mul_f32  v245, v229, v50                                        \n"
    " v_mul_f32  v246, v230, v50                                        \n"
    " v_mul_f32  v247, v231, v50                                        \n"
    " v_mov_b32  v50, s30                                               \n"
    " v_mul_f32  v224, v224, v50                                        \n"
    " v_mul_f32  v225, v225, v50                                        \n"
    " v_mul_f32  v226, v226, v50                                        \n"
    " v_mul_f32  v227, v227, v50                                        \n"
    " v_mul_f32  v228, v228, v50                                        \n"
    " v_mul_f32  v229, v229, v50                                        \n"
    " v_mul_f32  v230, v230, v50                                        \n"
    " v_mul_f32  v231, v231, v50                                        \n"
    _INIT_ACC_
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    // B( [0~63]N * [0~127]K ) * A( [0~15]M * [0~127]K )
    // Wave0, Warp0, thread0:  C[0]M  * [0~3]N
    // Wave0, Warp0, thread1:  C[1]M  * [0~3]N
    // Wave0, Warp0, ... ...
    // Wave0, Warp0, thread15: C[15]M * [0~3]N
    // Wave0, Warp1, thread0:  C[0]M  * [4~7]N
    // Wave0, ... ...
    // Wave0, Warp3, thread0:  C[0]M  * [12~15]N
    // Wave1, Warp0, thread0:  C[0]M  * [16~19]N
    // ... ...
    // Wave3, Warp0, thread0:  C[0]M  * [60~63]N
    // Wave3, Warp3, thread15: C[15]M * [60~63]N
    _UK_MFMA_   " v[56:59], acc[0:1],   acc[64:65], 0                   \n"
    _UK_MFMA_   " v[56:59], acc[2:3],   acc[66:67], v[56:59]            \n"
    _UK_MFMA_   " v[56:59], acc[4:5],   acc[68:69], v[56:59]            \n"
    _UK_MFMA_   " v[56:59], acc[6:7],   acc[70:71], v[56:59]            \n"
    " s_nop 16                                                          \n"
    // " v_mov_b32 %[v_c_0x], v56                                          \n"
    // " v_mov_b32 %[v_c_0y], v57                                          \n"
    // " v_mov_b32 %[v_c_0z], v58                                          \n"
    // " v_mov_b32 %[v_c_0w], v59                                          \n"
    // C = C * scale c
    " v_mul_f32 %[v_c_0x], v56, v224                                    \n"
    " v_mul_f32 %[v_c_0y], v57, v224                                    \n"
    " v_mul_f32 %[v_c_0z], v58, v224                                    \n"
    " v_mul_f32 %[v_c_0w], v59, v224                                    \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    // B( [64~127]N * [0~127]K ) * A( [0~15]M * [0~127]K )
    // Wave0, Warp0, thread0:  C[0]M  * [64~67]N
    // Wave0, Warp0, thread1:  C[1]M  * [64~67]N
    // Wave0, Warp0, ... ...
    // Wave0, Warp0, thread15: C[15]M * [64~67]N
    // Wave0, Warp1, thread0:  C[0]M  * [68~71]N
    // Wave0, ... ...
    // Wave0, Warp3, thread0:  C[0]M  * [76~79]N
    // Wave1, Warp0, thread0:  C[0]M  * [80~83]N
    // ... ...
    // Wave3, Warp0, thread0:  C[0]M  * [124~127]N
    // Wave3, Warp3, thread15: C[15]M * [124~127]N
    _UK_MFMA_   " v[56:59], acc[8:9],   acc[64:65], 0                   \n"
    _UK_MFMA_   " v[56:59], acc[10:11], acc[66:67], v[56:59]            \n"
    _UK_MFMA_   " v[56:59], acc[12:13], acc[68:69], v[56:59]            \n"
    _UK_MFMA_   " v[56:59], acc[14:15], acc[70:71], v[56:59]            \n"
    " s_nop 16                                                          \n"
    // " v_mov_b32 %[v_c_1x], v56                                          \n"
    // " v_mov_b32 %[v_c_1y], v57                                          \n"
    // " v_mov_b32 %[v_c_1z], v58                                          \n"
    // " v_mov_b32 %[v_c_1w], v59                                          \n"
    // C = C * scale c
    " v_mul_f32 %[v_c_1x], v56, v224                                    \n"
    " v_mul_f32 %[v_c_1y], v57, v224                                    \n"
    " v_mul_f32 %[v_c_1z], v58, v224                                    \n"
    " v_mul_f32 %[v_c_1w], v59, v224                                    \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    // B( [0~63]N * [0~127]K ) * A( [16~31]M * [0~127]K )
    // Wave0, Warp0, thread0:  C[16]M * [0~3]N
    // Wave0, Warp0, thread1:  C[17]M * [0~3]N
    // Wave0, Warp0, ... ...
    // Wave0, Warp0, thread15: C[31]M * [0~3]N
    // Wave0, Warp1, thread0:  C[16]M * [4~7]N
    // Wave0, ... ...
    // Wave0, Warp3, thread0:  C[16]M * [12~15]N
    // Wave1, Warp0, thread0:  C[16]M * [16~19]N
    // ... ...
    // Wave3, Warp0, thread0:  C[16]M * [60~63]N
    // Wave3, Warp3, thread15: C[31]M * [60~63]N
    _UK_MFMA_   " v[56:59], acc[0:1],   acc[72:73], 0                   \n"
    _UK_MFMA_   " v[56:59], acc[2:3],   acc[74:75], v[56:59]            \n"
    _UK_MFMA_   " v[56:59], acc[4:5],   acc[76:77], v[56:59]            \n"
    _UK_MFMA_   " v[56:59], acc[6:7],   acc[78:79], v[56:59]            \n"
    " s_nop 16                                                          \n"
    // " v_mov_b32 %[v_c_2x], v56                                          \n"
    // " v_mov_b32 %[v_c_2y], v57                                          \n"
    // " v_mov_b32 %[v_c_2z], v58                                          \n"
    // " v_mov_b32 %[v_c_2w], v59                                          \n"
    // C = C * scale c
    " v_mul_f32 %[v_c_2x], v56, v225                                    \n"
    " v_mul_f32 %[v_c_2y], v57, v225                                    \n"
    " v_mul_f32 %[v_c_2z], v58, v225                                    \n"
    " v_mul_f32 %[v_c_2w], v59, v225                                    \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[56:59], acc[8:9],   acc[120:121], 0                 \n"
    _UK_MFMA_   " v[56:59], acc[10:11], acc[122:123], v[56:59]          \n"
    _UK_MFMA_   " v[56:59], acc[12:13], acc[124:125], v[56:59]          \n"
    _UK_MFMA_   " v[56:59], acc[14:15], acc[126:127], v[56:59]          \n"
    " s_nop 16                                                          \n"
    // " v_mov_b32 %[v_c_15x], v56                                         \n"
    // " v_mov_b32 %[v_c_15y], v57                                         \n"
    // " v_mov_b32 %[v_c_15z], v58                                         \n"
    // " v_mov_b32 %[v_c_15w], v59                                         \n"
    // C = C * scale c
    " v_mul_f32 %[v_c_15x], v56, v231                                   \n"
    " v_mul_f32 %[v_c_15y], v57, v231                                   \n"
    " v_mul_f32 %[v_c_15z], v58, v231                                   \n"
    " v_mul_f32 %[v_c_15w], v59, v231                                   \n"
    // ===================================================================================
    // C: [0~15]M * [128~191]N -----------------------------------------------------------
    // B( [128~191]N * [0~127]K ) * A( [0~15]M * [0~127]K )
    // Wave0, Warp0, thread0:  C[0]M  * [128~131]N
    // Wave0, Warp0, thread1:  C[1]M  * [128~131]N
    // Wave0, Warp0, ... ...
    // Wave0, Warp0, thread15: C[15]M * [128~131]N
    // Wave0, Warp1, thread0:  C[0]M  * [132~135]N
    // Wave0, ... ...
    // Wave0, Warp3, thread0:  C[0]M  * [12~15]N
    // Wave1, Warp0, thread0:  C[0]M  * [16~19]N
    // ... ...
    // Wave3, Warp0, thread0:  C[0]M  * [188~191]N
    // Wave3, Warp3, thread15: C[15]M * [188~191]N
    _UK_MFMA_   " v[56:59], acc[32:33],   acc[64:65], 0                 \n"
    _UK_MFMA_   " v[56:59], acc[34:35],   acc[66:67], v[56:59]          \n"
    _UK_MFMA_   " v[56:59], acc[36:37],   acc[68:69], v[56:59]          \n"
    _UK_MFMA_   " v[56:59], acc[38:39],   acc[70:71], v[56:59]          \n"
    " s_nop 16                                                          \n"
    // " v_mov_b32 %[v_c_0x], v56                                          \n"
    // " v_mov_b32 %[v_c_0y], v57                                          \n"
    // " v_mov_b32 %[v_c_0z], v58                                          \n"
    // " v_mov_b32 %[v_c_0w], v59                                          \n"
    // C = C * scale c
    " v_mul_f32 %[v_c_16x], v56, v240                                    \n"
    " v_mul_f32 %[v_c_16y], v57, v240                                    \n"
    " v_mul_f32 %[v_c_16z], v58, v240                                    \n"
    " v_mul_f32 %[v_c_16w], v59, v240                                    \n"
    _FF_SYNC_
#pragma endregion
    _FF_END_
#endif

// pipeline w/o instr opt debug
#if 0
#pragma region PREFETCH
    // PREFETCH PING =====================================================================
    // prefetch A (ping) -----------------------------------------------------------------
    // A0: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 0   \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    " s_waitcnt vmcnt(0) \n"
    " s_barrier          \n"
    
    // A0: lds->accvgpr
    " ds_read_b128  acc[64:67],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[68:71],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[72:75],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[76:79],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[80:83],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[84:87],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[88:91],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[92:95],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[96:99],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[100:103], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[104:107], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[108:111], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[112:115], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[116:119], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[120:123], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[124:127], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 \n" // [112~127]M * [64~127]K
    " s_waitcnt lgkmcnt(0) \n"

    // prefetch B (ping) -----------------------------------------------------------------
    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[32:35], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[36:39], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[40:43], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[44:47], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K
    " s_waitcnt vmcnt(0) \n"

    // prefetch scale (ping) -------------------------------------------------------------
    // scale a0
    " buffer_load_dword  v224, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0 \n" // [0~15]M
    " buffer_load_dword  v225, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1 \n" // [16~31]M
    " buffer_load_dword  v226, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2 \n" // [32~47]M
    " buffer_load_dword  v227, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3 \n" // [48~63]M
    " buffer_load_dword  v228, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4 \n" // [64~79]M
    " buffer_load_dword  v229, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5 \n" // [80~95]M
    " buffer_load_dword  v230, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6 \n" // [96~111]M
    " buffer_load_dword  v231, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7 \n" // [112~127]M
    " s_waitcnt vmcnt(0) \n"
    // scale b0 (ping)
    " s_load_dwordx2 s[30:31], s[28:29],  0 \n"
    " s_waitcnt lgkmcnt(0) \n"

    // move to pang ----------------------------------------------------------------------
    // move to next A1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    "s_add_u32     s16, s86, s16          \n"
    "s_addc_u32    s17, 0, s17            \n"
    // move to next B1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0 \n"
    "s_add_u32     s20, s86, s20          \n"
    "s_addc_u32    s21, 0, s21            \n"
    // move to next SA1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0   \n"
    "s_add_u32     s24, s86, s24          \n"
    "s_addc_u32    s25, 0, s25            \n"
    // move to next SB1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0   \n"
    "s_add_u32     s28, s86, s28          \n"
    "s_addc_u32    s29, 0, s29            \n"

    // prefetch A (pang) -----------------------------------------------------------------
    // A1: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 1   \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    " s_waitcnt vmcnt(0) \n"
    " s_barrier          \n"

    // move to ping ----------------------------------------------------------------------
    // move to next A0
    "s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    "s_add_u32     s16, s86, s16          \n"
    "s_addc_u32    s17, 0, s17            \n"
    
#pragma endregion

"L_start%=: \n"

#pragma region LOOP_1
    // PREFETCH PANG =====================================================================
    // A0: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    " s_waitcnt vmcnt(0) \n"
    " s_barrier          \n"
    
    // A1: lds->accvgpr
    " ds_read_b128  acc[128:131], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[132:135], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[136:139], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[140:143], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[144:147], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[148:151], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[152:155], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[156:159], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[160:163], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[164:167], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[168:171], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[172:175], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[176:179], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[180:183], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[184:187], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[188:191], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [112~127]M * [64~127]K
    " s_waitcnt lgkmcnt(0)                                                                                                                      \n"

    // B1: vmem->acc
    " buffer_load_dwordx4  acc[16:19], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[20:23], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[24:27], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[28:31], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[48:51], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[52:55], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[56:59], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[60:63], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K
    " s_waitcnt vmcnt(0) \n"

    // scale a1
    " buffer_load_dword    v232, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0 \n" // [0~15]M
    " buffer_load_dword    v233, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1 \n" // [16~31]M
    " buffer_load_dword    v234, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2 \n" // [32~47]M
    " buffer_load_dword    v235, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3 \n" // [48~63]M
    " buffer_load_dword    v236, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4 \n" // [64~79]M
    " buffer_load_dword    v237, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5 \n" // [80~95]M
    " buffer_load_dword    v238, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6 \n" // [96~111]M
    " buffer_load_dword    v239, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7 \n" // [112~127]M
    " s_waitcnt vmcnt(0) \n"
    // scale b1
    " s_load_dwordx2 s[84:85], s[28:29], 0 \n"
    " s_waitcnt lgkmcnt(0)                 \n"
    
    // move to next A1
    " s_cmp_gt_i32  %[s_loop_cnt] 3        \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    " s_add_u32     s16, s86, s16          \n"
    " s_addc_u32    s17, 0, s17            \n"
    // move to next B0
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0 \n"
    " s_add_u32     s20, s86, s20          \n"
    " s_addc_u32    s21, 0, s21            \n"
    // move to next SA0
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0   \n"
    " s_add_u32     s24, s86, s24          \n"
    " s_addc_u32    s25, 0, s25            \n"
    // move to next SB0
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0   \n"
    " s_add_u32     s28, s86, s28          \n"
    " s_addc_u32    s29, 0, s29            \n"

    // MFMA PING =========================================================================
    // scale c0 = scale a0 * scale b0 (ping)
    " v_mov_b32  v50, s31        \n"
    " v_mul_f32  v240, v224, v50 \n"
    " v_mul_f32  v241, v225, v50 \n"
    " v_mul_f32  v242, v226, v50 \n"
    " v_mul_f32  v243, v227, v50 \n"
    " v_mul_f32  v244, v228, v50 \n"
    " v_mul_f32  v245, v229, v50 \n"
    " v_mul_f32  v246, v230, v50 \n"
    " v_mul_f32  v247, v231, v50 \n"
    " v_mov_b32  v50, s30        \n"
    " v_mul_f32  v224, v224, v50 \n"
    " v_mul_f32  v225, v225, v50 \n"
    " v_mul_f32  v226, v226, v50 \n"
    " v_mul_f32  v227, v227, v50 \n"
    " v_mul_f32  v228, v228, v50 \n"
    " v_mul_f32  v229, v229, v50 \n"
    " v_mul_f32  v230, v230, v50 \n"
    " v_mul_f32  v231, v231, v50 \n"
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[64:65], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[66:67], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[68:69], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[70:71], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_0x],  v56, v224 \n"
    " v_fmac_f32 %[v_c_0y],  v57, v224 \n"
    " v_fmac_f32 %[v_c_0z],  v58, v224 \n"
    " v_fmac_f32 %[v_c_0w],  v59, v224 \n"
    // C: [0~15]M  * [64~127]N  ----------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[64:65], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[66:67], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[68:69], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[70:71], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_1x],  v60, v224 \n"
    " v_fmac_f32 %[v_c_1y],  v61, v224 \n"
    " v_fmac_f32 %[v_c_1z],  v62, v224 \n"
    " v_fmac_f32 %[v_c_1w],  v63, v224 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[72:73], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[74:75], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[76:77], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[78:79], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_2x],  v56, v225 \n"
    " v_fmac_f32 %[v_c_2y],  v57, v225 \n"
    " v_fmac_f32 %[v_c_2z],  v58, v225 \n"
    " v_fmac_f32 %[v_c_2w],  v59, v225 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[72:73], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[74:75], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[76:77], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[78:79], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_3x],  v60, v225 \n"
    " v_fmac_f32 %[v_c_3y],  v61, v225 \n"
    " v_fmac_f32 %[v_c_3z],  v62, v225 \n"
    " v_fmac_f32 %[v_c_3w],  v63, v225 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[80:81], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[82:83], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[84:85], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[86:87], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_4x],  v56,v226 \n"
    " v_fmac_f32 %[v_c_4y],  v57,v226 \n"
    " v_fmac_f32 %[v_c_4z],  v58,v226 \n"
    " v_fmac_f32 %[v_c_4w],  v59,v226 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[80:81], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[82:83], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[84:85], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[86:87], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_5x],  v60, v226 \n"
    " v_fmac_f32 %[v_c_5y],  v61, v226 \n"
    " v_fmac_f32 %[v_c_5z],  v62, v226 \n"
    " v_fmac_f32 %[v_c_5w],  v63, v226 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[88:89], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[90:91], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[92:93], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[94:95], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_6x],  v56, v227 \n"
    " v_fmac_f32 %[v_c_6y],  v57, v227 \n"
    " v_fmac_f32 %[v_c_6z],  v58, v227 \n"
    " v_fmac_f32 %[v_c_6w],  v59, v227 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[88:89], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[90:91], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[92:93], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[94:95], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_7x],  v60, v227 \n"
    " v_fmac_f32 %[v_c_7y],  v61, v227 \n"
    " v_fmac_f32 %[v_c_7z],  v62, v227 \n"
    " v_fmac_f32 %[v_c_7w],  v63, v227 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[96:97],   0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[98:99],   v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[100:101], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[102:103], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_8x],  v56, v228 \n"
    " v_fmac_f32 %[v_c_8y],  v57, v228 \n"
    " v_fmac_f32 %[v_c_8z],  v58, v228 \n"
    " v_fmac_f32 %[v_c_8w],  v59, v228 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[96:97],   0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[98:99],   v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[100:101], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[102:103], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_9x],  v60, v228 \n"
    " v_fmac_f32 %[v_c_9y],  v61, v228 \n"
    " v_fmac_f32 %[v_c_9z],  v62, v228 \n"
    " v_fmac_f32 %[v_c_9w],  v63, v228 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[104:105], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[106:107], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[108:109], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[110:111], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_10x], v56, v229 \n"
    " v_fmac_f32 %[v_c_10y], v57, v229 \n"
    " v_fmac_f32 %[v_c_10z], v58, v229 \n"
    " v_fmac_f32 %[v_c_10w], v59, v229 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[104:105], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[106:107], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[108:109], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[110:111], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_11x], v60, v229 \n"
    " v_fmac_f32 %[v_c_11y], v61, v229 \n"
    " v_fmac_f32 %[v_c_11z], v62, v229 \n"
    " v_fmac_f32 %[v_c_11w], v63, v229 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[112:113], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[114:115], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[116:117], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[118:119], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_12x], v56, v230 \n"
    " v_fmac_f32 %[v_c_12y], v57, v230 \n"
    " v_fmac_f32 %[v_c_12z], v58, v230 \n"
    " v_fmac_f32 %[v_c_12w], v59, v230 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[112:113], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[114:115], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[116:117], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[118:119], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_13x], v60, v230 \n"
    " v_fmac_f32 %[v_c_13y], v61, v230 \n"
    " v_fmac_f32 %[v_c_13z], v62, v230 \n"
    " v_fmac_f32 %[v_c_13w], v63, v230 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[120:121], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[122:123], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[124:125], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[126:127], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_14x], v56, v231 \n"
    " v_fmac_f32 %[v_c_14y], v57, v231 \n"
    " v_fmac_f32 %[v_c_14z], v58, v231 \n"
    " v_fmac_f32 %[v_c_14w], v59, v231 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[120:121], 0        \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[122:123], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[124:125], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[126:127], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_15x], v60, v231 \n"
    " v_fmac_f32 %[v_c_15y], v61, v231 \n"
    " v_fmac_f32 %[v_c_15z], v62, v231 \n"
    " v_fmac_f32 %[v_c_15w], v63, v231 \n"
    // ===================================================================================
    // C: [0~15]M * [128~191]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[64:65], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[66:67], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[68:69], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[70:71], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_16x], v56, v240 \n"
    " v_fmac_f32 %[v_c_16y], v57, v240 \n"
    " v_fmac_f32 %[v_c_16z], v58, v240 \n"
    " v_fmac_f32 %[v_c_16w], v59, v240 \n"
    // C: [0~15]M  * [192~255]N  ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[64:65], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[66:67], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[68:69], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[70:71], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_17x], v60, v240 \n"
    " v_fmac_f32 %[v_c_17y], v61, v240 \n"
    " v_fmac_f32 %[v_c_17z], v62, v240 \n"
    " v_fmac_f32 %[v_c_17w], v63, v240 \n"
    // C: [16~31]M * [128~191]N ----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[72:73], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[74:75], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[76:77], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[78:79], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_18x], v56, v241 \n"
    " v_fmac_f32 %[v_c_18y], v57, v241 \n"
    " v_fmac_f32 %[v_c_18z], v58, v241 \n"
    " v_fmac_f32 %[v_c_18w], v59, v241 \n"
    // C: [16~31]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[72:73], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[74:75], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[76:77], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[78:79], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_19x], v60, v241 \n"
    " v_fmac_f32 %[v_c_19y], v61, v241 \n"
    " v_fmac_f32 %[v_c_19z], v62, v241 \n"
    " v_fmac_f32 %[v_c_19w], v63, v241 \n"
    // C: [32~47]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[80:81], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[82:83], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[84:85], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[86:87], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_20x], v56, v242 \n"
    " v_fmac_f32 %[v_c_20y], v57, v242 \n"
    " v_fmac_f32 %[v_c_20z], v58, v242 \n"
    " v_fmac_f32 %[v_c_20w], v59, v242 \n"
    // C: [32~47]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[80:81], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[82:83], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[84:85], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[86:87], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_21x], v60, v242 \n"
    " v_fmac_f32 %[v_c_21y], v61, v242 \n"
    " v_fmac_f32 %[v_c_21z], v62, v242 \n"
    " v_fmac_f32 %[v_c_21w], v63, v242 \n"
    // C: [48~63]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[88:89], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[90:91], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[92:93], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[94:95], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_22x], v56, v243 \n"
    " v_fmac_f32 %[v_c_22y], v57, v243 \n"
    " v_fmac_f32 %[v_c_22z], v58, v243 \n"
    " v_fmac_f32 %[v_c_22w], v59, v243 \n"
    // C: [48~63]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[88:89], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[90:91], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[92:93], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[94:95], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_23x], v60, v243 \n"
    " v_fmac_f32 %[v_c_23y], v61, v243 \n"
    " v_fmac_f32 %[v_c_23z], v62, v243 \n"
    " v_fmac_f32 %[v_c_23w], v63, v243 \n"
    // C: [64~79]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[96:97],   0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[98:99],   v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[100:101], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[102:103], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_24x], v56, v244 \n"
    " v_fmac_f32 %[v_c_24y], v57, v244 \n"
    " v_fmac_f32 %[v_c_24z], v58, v244 \n"
    " v_fmac_f32 %[v_c_24w], v59, v244 \n"
    // C: [64~79]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[96:97],   0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[98:99],   v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[100:101], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[102:103], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_25x], v60, v244 \n"
    " v_fmac_f32 %[v_c_25y], v61, v244 \n"
    " v_fmac_f32 %[v_c_25z], v62, v244 \n"
    " v_fmac_f32 %[v_c_25w], v63, v244 \n"
    // C: [80~95]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[104:105], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[106:107], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[108:109], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[110:111], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_26x], v56, v245 \n"
    " v_fmac_f32 %[v_c_26y], v57, v245 \n"
    " v_fmac_f32 %[v_c_26z], v58, v245 \n"
    " v_fmac_f32 %[v_c_26w], v59, v245 \n"
    // C: [80~95]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[104:105], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[106:107], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[108:109], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[110:111], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_27x], v60, v245 \n"
    " v_fmac_f32 %[v_c_27y], v61, v245 \n"
    " v_fmac_f32 %[v_c_27z], v62, v245 \n"
    " v_fmac_f32 %[v_c_27w], v63, v245 \n"
    // C: [96~111]M  * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[112:113], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[114:115], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[116:117], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[118:119], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_28x], v56, v246 \n"
    " v_fmac_f32 %[v_c_28y], v57, v246 \n"
    " v_fmac_f32 %[v_c_28z], v58, v246 \n"
    " v_fmac_f32 %[v_c_28w], v59, v246 \n"
    // C: [96~111]M  * [192~255]N --------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[112:113], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[114:115], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[116:117], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[118:119], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_29x], v60, v246 \n"
    " v_fmac_f32 %[v_c_29y], v61, v246 \n"
    " v_fmac_f32 %[v_c_29z], v62, v246 \n"
    " v_fmac_f32 %[v_c_29w], v63, v246 \n"
    // C: [112~127]M * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[120:121], 0        \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[122:123], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[124:125], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[126:127], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_30x], v56, v247 \n"
    " v_fmac_f32 %[v_c_30y], v57, v247 \n"
    " v_fmac_f32 %[v_c_30z], v58, v247 \n"
    " v_fmac_f32 %[v_c_30w], v59, v247 \n"
    // C: [112~127]M * [192~255]N --------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[120:121], 0        \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[122:123], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[124:125], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[126:127], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_31x], v60, v247 \n"
    " v_fmac_f32 %[v_c_31y], v61, v247 \n"
    " v_fmac_f32 %[v_c_31z], v62, v247 \n"
    " v_fmac_f32 %[v_c_31w], v63, v247 \n"
    // -----------------------------------------------------------------------------------
    // forloop
    " s_sub_i32     %[s_loop_cnt], %[s_loop_cnt], 1 \n"
    " s_cmp_gt_i32  %[s_loop_cnt] 0                 \n"
    " s_cbranch_scc0 L_end%=                        \n"
#pragma endregion

#pragma region LOOP_2
    // PREFETCH PING =====================================================================
    // A1: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 1 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    " s_waitcnt vmcnt(0) \n"
    " s_barrier          \n"
    
    // A0: lds->accvgpr
    " ds_read_b128  acc[64:67],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[68:71],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[72:75],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[76:79],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[80:83],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[84:87],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[88:91],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[92:95],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[96:99],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[100:103], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[104:107], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[108:111], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[112:115], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[116:119], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[120:123], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[124:127], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 \n" // [112~127]M * [64~127]K
    " s_waitcnt lgkmcnt(0) \n"

    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[32:35], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[36:39], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[40:43], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[44:47], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K
    " s_waitcnt vmcnt(0) \n"

    // scale a0
    " buffer_load_dword v224, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0 \n" // [0~15]M
    " buffer_load_dword v225, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1 \n" // [16~31]M
    " buffer_load_dword v226, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2 \n" // [32~47]M
    " buffer_load_dword v227, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3 \n" // [48~63]M
    " buffer_load_dword v228, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4 \n" // [64~79]M
    " buffer_load_dword v229, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5 \n" // [80~95]M
    " buffer_load_dword v230, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6 \n" // [96~111]M
    " buffer_load_dword v231, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7 \n" // [112~127]M
    " s_waitcnt vmcnt(0) \n"
    // scale b0
    " s_load_dwordx2 s[30:31], s[28:29],  0 \n"
    " s_waitcnt lgkmcnt(0)                  \n"

    // move to next A0
    " s_cmp_gt_i32  %[s_loop_cnt] 3        \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    " s_add_u32     s16, s86, s16          \n"
    " s_addc_u32    s17, 0, s17            \n"
    // move to next B1
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0 \n"
    " s_add_u32     s20, s86, s20          \n"
    " s_addc_u32    s21, 0, s21            \n"
    // move to next SA1
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0   \n"
    " s_add_u32     s24, s86, s24          \n"
    " s_addc_u32    s25, 0, s25            \n"
    // move to next SB1
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0   \n"
    " s_add_u32     s28, s86, s28          \n"
    " s_addc_u32    s29, 0, s29            \n"    

    // MFMA PANG =========================================================================
    // scale c1 = scale a1 * scale b1
    " v_mov_b32  v50, s85        \n"
    " v_mul_f32  v248, v232, v50 \n"
    " v_mul_f32  v249, v233, v50 \n"
    " v_mul_f32  v250, v234, v50 \n"
    " v_mul_f32  v251, v235, v50 \n"
    " v_mul_f32  v252, v236, v50 \n"
    " v_mul_f32  v253, v237, v50 \n"
    " v_mul_f32  v254, v238, v50 \n"
    " v_mul_f32  v255, v239, v50 \n"
    " v_mov_b32  v50, s84        \n"
    " v_mul_f32  v232, v232, v50 \n"
    " v_mul_f32  v233, v233, v50 \n"
    " v_mul_f32  v234, v234, v50 \n"
    " v_mul_f32  v235, v235, v50 \n"
    " v_mul_f32  v236, v236, v50 \n"
    " v_mul_f32  v237, v237, v50 \n"
    " v_mul_f32  v238, v238, v50 \n"
    " v_mul_f32  v239, v239, v50 \n"
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[128:129], 0        \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[130:131], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[132:133], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[134:135], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_0x],  v56, v232 \n"
    " v_fmac_f32 %[v_c_0y],  v57, v232 \n"
    " v_fmac_f32 %[v_c_0z],  v58, v232 \n"
    " v_fmac_f32 %[v_c_0w],  v59, v232 \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[128:129], 0        \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[130:131], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[132:133], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[134:135], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_1x],  v60, v232 \n"
    " v_fmac_f32 %[v_c_1y],  v61, v232 \n"
    " v_fmac_f32 %[v_c_1z],  v62, v232 \n"
    " v_fmac_f32 %[v_c_1w],  v63, v232 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[136:137], 0        \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[138:139], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[140:141], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[142:143], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_2x],  v56, v233 \n"
    " v_fmac_f32 %[v_c_2y],  v57, v233 \n"
    " v_fmac_f32 %[v_c_2z],  v58, v233 \n"
    " v_fmac_f32 %[v_c_2w],  v59, v233 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[136:137], 0        \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[138:139], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[140:141], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[142:143], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_3x],  v60, v233 \n"
    " v_fmac_f32 %[v_c_3y],  v61, v233 \n"
    " v_fmac_f32 %[v_c_3z],  v62, v233 \n"
    " v_fmac_f32 %[v_c_3w],  v63, v233 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[144:145], 0        \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[146:147], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[148:149], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[150:151], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_4x],  v56, v234 \n"
    " v_fmac_f32 %[v_c_4y],  v57, v234 \n"
    " v_fmac_f32 %[v_c_4z],  v58, v234 \n"
    " v_fmac_f32 %[v_c_4w],  v59, v234 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[144:145], 0        \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[146:147], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[148:149], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[150:151], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_5x],  v60, v234 \n"
    " v_fmac_f32 %[v_c_5y],  v61, v234 \n"
    " v_fmac_f32 %[v_c_5z],  v62, v234 \n"
    " v_fmac_f32 %[v_c_5w],  v63, v234 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[152:153], 0 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[154:155], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[156:157], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[158:159], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_6x],  v56, v235 \n"
    " v_fmac_f32 %[v_c_6y],  v57, v235 \n"
    " v_fmac_f32 %[v_c_6z],  v58, v235 \n"
    " v_fmac_f32 %[v_c_6w],  v59, v235 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[152:153], 0 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[154:155], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[156:157], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[158:159], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_7x],  v60, v235 \n"
    " v_fmac_f32 %[v_c_7y],  v61, v235 \n"
    " v_fmac_f32 %[v_c_7z],  v62, v235 \n"
    " v_fmac_f32 %[v_c_7w],  v63, v235 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[160:161], 0 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[162:163], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[164:165], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[166:167], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_8x],  v56, v236 \n"
    " v_fmac_f32 %[v_c_8y],  v57, v236 \n"
    " v_fmac_f32 %[v_c_8z],  v58, v236 \n"
    " v_fmac_f32 %[v_c_8w],  v59, v236 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[160:161], 0 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[162:163], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[164:165], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[166:167], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_9x],  v60, v236 \n"
    " v_fmac_f32 %[v_c_9y],  v61, v236 \n"
    " v_fmac_f32 %[v_c_9z],  v62, v236 \n"
    " v_fmac_f32 %[v_c_9w],  v63, v236 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[168:169], 0 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[170:171], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[172:173], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[174:175], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_10x], v56, v237 \n"
    " v_fmac_f32 %[v_c_10y], v57, v237 \n"
    " v_fmac_f32 %[v_c_10z], v58, v237 \n"
    " v_fmac_f32 %[v_c_10w], v59, v237 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[168:169], 0 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[170:171], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[172:173], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[174:175], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_11x], v60, v237 \n"
    " v_fmac_f32 %[v_c_11y], v61, v237 \n"
    " v_fmac_f32 %[v_c_11z], v62, v237 \n"
    " v_fmac_f32 %[v_c_11w], v63, v237 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[176:177], 0 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[178:179], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[180:181], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[182:183], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_12x], v56, v238 \n"
    " v_fmac_f32 %[v_c_12y], v57, v238 \n"
    " v_fmac_f32 %[v_c_12z], v58, v238 \n"
    " v_fmac_f32 %[v_c_12w], v59, v238 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[176:177], 0 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[178:179], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[180:181], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[182:183], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_13x], v60, v238 \n"
    " v_fmac_f32 %[v_c_13y], v61, v238 \n"
    " v_fmac_f32 %[v_c_13z], v62, v238 \n"
    " v_fmac_f32 %[v_c_13w], v63, v238 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[184:185], 0 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[186:187], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[188:189], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[190:191], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_14x], v56, v239 \n"
    " v_fmac_f32 %[v_c_14y], v57, v239 \n"
    " v_fmac_f32 %[v_c_14z], v58, v239 \n"
    " v_fmac_f32 %[v_c_14w], v59, v239 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[184:185], 0 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[186:187], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[188:189], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[190:191], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_15x], v60, v239 \n"
    " v_fmac_f32 %[v_c_15y], v61, v239 \n"
    " v_fmac_f32 %[v_c_15z], v62, v239 \n"
    " v_fmac_f32 %[v_c_15w], v63, v239 \n"
    // ===================================================================================
    // C: [0~15]M * [128~191]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[128:129], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[130:131], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[132:133], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[134:135], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_16x], v56, v248 \n"
    " v_fmac_f32 %[v_c_16y], v57, v248 \n"
    " v_fmac_f32 %[v_c_16z], v58, v248 \n"
    " v_fmac_f32 %[v_c_16w], v59, v248 \n"
    // C: [0~15]M  * [192~255]N  ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[128:129], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[130:131], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[132:133], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[134:135], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_17x], v60, v248 \n"
    " v_fmac_f32 %[v_c_17y], v61, v248 \n"
    " v_fmac_f32 %[v_c_17z], v62, v248 \n"
    " v_fmac_f32 %[v_c_17w], v63, v248 \n"
    // C: [16~31]M * [128~191]N ----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[136:137], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[138:139], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[140:141], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[142:143], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_18x], v56, v249 \n"
    " v_fmac_f32 %[v_c_18y], v57, v249 \n"
    " v_fmac_f32 %[v_c_18z], v58, v249 \n"
    " v_fmac_f32 %[v_c_18w], v59, v249 \n"
    // C: [16~31]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[136:137], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[138:139], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[140:141], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[142:143], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_19x], v60, v249 \n"
    " v_fmac_f32 %[v_c_19y], v61, v249 \n"
    " v_fmac_f32 %[v_c_19z], v62, v249 \n"
    " v_fmac_f32 %[v_c_19w], v63, v249 \n"
    // C: [32~47]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[144:145], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[146:147], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[148:149], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[150:151], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_20x], v56, v250 \n"
    " v_fmac_f32 %[v_c_20y], v57, v250 \n"
    " v_fmac_f32 %[v_c_20z], v58, v250 \n"
    " v_fmac_f32 %[v_c_20w], v59, v250 \n"
    // C: [32~47]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[144:145], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[146:147], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[148:149], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[150:151], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_21x], v60, v250 \n"
    " v_fmac_f32 %[v_c_21y], v61, v250 \n"
    " v_fmac_f32 %[v_c_21z], v62, v250 \n"
    " v_fmac_f32 %[v_c_21w], v63, v250 \n"
    // C: [48~63]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[152:153], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[154:155], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[156:157], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[158:159], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_22x], v56, v251 \n"
    " v_fmac_f32 %[v_c_22y], v57, v251 \n"
    " v_fmac_f32 %[v_c_22z], v58, v251 \n"
    " v_fmac_f32 %[v_c_22w], v59, v251 \n"
    // C: [48~63]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[152:153], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[154:155], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[156:157], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[158:159], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_23x], v60, v251 \n"
    " v_fmac_f32 %[v_c_23y], v61, v251 \n"
    " v_fmac_f32 %[v_c_23z], v62, v251 \n"
    " v_fmac_f32 %[v_c_23w], v63, v251 \n"
    // C: [64~79]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[160:161], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[162:163], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[164:165], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[166:167], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_24x], v56, v252 \n"
    " v_fmac_f32 %[v_c_24y], v57, v252 \n"
    " v_fmac_f32 %[v_c_24z], v58, v252 \n"
    " v_fmac_f32 %[v_c_24w], v59, v252 \n"
    // C: [64~79]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[160:161], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[162:163], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[164:165], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[166:167], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_25x], v60, v252 \n"
    " v_fmac_f32 %[v_c_25y], v61, v252 \n"
    " v_fmac_f32 %[v_c_25z], v62, v252 \n"
    " v_fmac_f32 %[v_c_25w], v63, v252 \n"
    // C: [80~95]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[168:169], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[170:171], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[172:173], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[174:175], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_26x], v56, v253 \n"
    " v_fmac_f32 %[v_c_26y], v57, v253 \n"
    " v_fmac_f32 %[v_c_26z], v58, v253 \n"
    " v_fmac_f32 %[v_c_26w], v59, v253 \n"
    // C: [80~95]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[168:169], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[170:171], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[172:173], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[174:175], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_27x], v60, v253 \n"
    " v_fmac_f32 %[v_c_27y], v61, v253 \n"
    " v_fmac_f32 %[v_c_27z], v62, v253 \n"
    " v_fmac_f32 %[v_c_27w], v63, v253 \n"
    // C: [96~111]M  * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[176:177], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[178:179], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[180:181], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[182:183], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_28x], v56, v254 \n"
    " v_fmac_f32 %[v_c_28y], v57, v254 \n"
    " v_fmac_f32 %[v_c_28z], v58, v254 \n"
    " v_fmac_f32 %[v_c_28w], v59, v254 \n"
    // C: [96~111]M * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[176:177], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[178:179], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[180:181], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[182:183], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_29x], v60, v254 \n"
    " v_fmac_f32 %[v_c_29y], v61, v254 \n"
    " v_fmac_f32 %[v_c_29z], v62, v254 \n"
    " v_fmac_f32 %[v_c_29w], v63, v254 \n"
    // C: [112~127]M * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[184:185], 0 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[186:187], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[188:189], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[190:191], v[56:59] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_30x], v56, v255 \n"
    " v_fmac_f32 %[v_c_30y], v57, v255 \n"
    " v_fmac_f32 %[v_c_30z], v58, v255 \n"
    " v_fmac_f32 %[v_c_30w], v59, v255 \n"
    // C: [112~127]M * [192~255]N --------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[184:185], 0 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[186:187], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[188:189], v[60:63] \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[190:191], v[60:63] \n"
    " s_nop 16 \n"
    " v_fmac_f32 %[v_c_31x], v60, v255 \n"
    " v_fmac_f32 %[v_c_31y], v61, v255 \n"
    " v_fmac_f32 %[v_c_31z], v62, v255 \n"
    " v_fmac_f32 %[v_c_31w], v63, v255 \n"
    // -----------------------------------------------------------------------------------
    // forloop
    " s_sub_i32    %[s_loop_cnt], %[s_loop_cnt], 1 \n"
    " s_cmp_gt_i32 %[s_loop_cnt] 0                 \n"
    " s_cbranch_scc0 L_end%=   \n"
    " s_branch       L_start%= \n"
#pragma endregion
#endif

#if 1
#pragma region PREFETCH
    // PREFETCH PING =====================================================================
    // prefetch A (ping) -----------------------------------------------------------------
    // A0: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 0   \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    " s_waitcnt vmcnt(0) \n"
    " s_barrier          \n"

    // A0: lds->accvgpr
    " ds_read_b128  acc[64:67],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[68:71],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[72:75],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[76:79],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[80:83],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[84:87],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[88:91],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[92:95],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[96:99],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[100:103], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[104:107], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[108:111], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[112:115], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[116:119], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[120:123], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[124:127], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 \n" // [112~127]M * [64~127]K
    " s_waitcnt lgkmcnt(0) \n"

    // prefetch B (ping) -----------------------------------------------------------------
    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[32:35], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[36:39], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[40:43], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[44:47], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K
    " s_waitcnt vmcnt(0) \n"

    // prefetch scale (ping) -------------------------------------------------------------
    // scale a0
    " buffer_load_dword  v224, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0 \n" // [0~15]M
    " buffer_load_dword  v225, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1 \n" // [16~31]M
    " buffer_load_dword  v226, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2 \n" // [32~47]M
    " buffer_load_dword  v227, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3 \n" // [48~63]M
    " buffer_load_dword  v228, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4 \n" // [64~79]M
    " buffer_load_dword  v229, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5 \n" // [80~95]M
    " buffer_load_dword  v230, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6 \n" // [96~111]M
    " buffer_load_dword  v231, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7 \n" // [112~127]M
    " s_waitcnt vmcnt(0) \n"
    // scale b0 (ping)
    " s_load_dwordx2 s[30:31], s[28:29],  0 \n"
    " s_waitcnt lgkmcnt(0) \n"

    // move to pang ----------------------------------------------------------------------
    // move to next A1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    "s_add_u32     s16, s86, s16          \n"
    "s_addc_u32    s17, 0, s17            \n"
    // move to next B1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0 \n"
    "s_add_u32     s20, s86, s20          \n"
    "s_addc_u32    s21, 0, s21            \n"
    // move to next SA1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0   \n"
    "s_add_u32     s24, s86, s24          \n"
    "s_addc_u32    s25, 0, s25            \n"
    // move to next SB1
    "s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0   \n"
    "s_add_u32     s28, s86, s28          \n"
    "s_addc_u32    s29, 0, s29            \n"

    // prefetch A (pang) -----------------------------------------------------------------
    // A1: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 1   \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    " s_waitcnt vmcnt(0) \n"
    " s_barrier          \n"

    // move to ping ----------------------------------------------------------------------
    // move to next A0
    "s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    "s_add_u32     s16, s86, s16          \n"
    "s_addc_u32    s17, 0, s17            \n"
#pragma endregion

"L_start%=: \n"

#pragma region LOOP_1
    
    " s_waitcnt lgkmcnt(0) \n"
    " s_waitcnt vmcnt(0)   \n"
    " s_barrier            \n"

    // PREFETCH PANG =====================================================================
    // A0: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    
    // A1: lds->accvgpr
    " ds_read_b128  acc[128:131], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[132:135], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[136:139], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[140:143], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[144:147], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[148:151], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[152:155], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[156:159], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[160:163], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[164:167], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[168:171], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[172:175], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[176:179], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[180:183], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[184:187], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[188:191], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [112~127]M * [64~127]K
    
    // B1: vmem->acc
    " buffer_load_dwordx4  acc[16:19], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[20:23], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[24:27], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[28:31], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[48:51], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[52:55], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[56:59], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[60:63], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K

    // scale a1, b1
    " buffer_load_dword    v232, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0 \n" // [0~15]M
    " buffer_load_dword    v233, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1 \n" // [16~31]M
    " buffer_load_dword    v234, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2 \n" // [32~47]M
    " buffer_load_dword    v235, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3 \n" // [48~63]M
    " buffer_load_dword    v236, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4 \n" // [64~79]M
    " buffer_load_dword    v237, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5 \n" // [80~95]M
    " buffer_load_dword    v238, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6 \n" // [96~111]M
    " buffer_load_dword    v239, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7 \n" // [112~127]M
    " s_load_dwordx2 s[84:85], s[28:29], 0 \n"
    
    // MFMA PING =========================================================================
    // scale c0 = scale a0 * scale b0 (ping)
    " v_mov_b32  v50, s31        \n"
    " v_mul_f32  v240, v224, v50 \n"
    " v_mul_f32  v241, v225, v50 \n"
    " v_mul_f32  v242, v226, v50 \n"
    " v_mul_f32  v243, v227, v50 \n"
    " v_mul_f32  v244, v228, v50 \n"
    " v_mul_f32  v245, v229, v50 \n"
    " v_mul_f32  v246, v230, v50 \n"
    " v_mul_f32  v247, v231, v50 \n"
    " v_mov_b32  v50, s30        \n"
    " v_mul_f32  v224, v224, v50 \n"
    " v_mul_f32  v225, v225, v50 \n"
    " v_mul_f32  v226, v226, v50 \n"
    " v_mul_f32  v227, v227, v50 \n"
    " v_mul_f32  v228, v228, v50 \n"
    " v_mul_f32  v229, v229, v50 \n"
    " v_mul_f32  v230, v230, v50 \n"
    " v_mul_f32  v231, v231, v50 \n"
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[64:65], 0        \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[66:67], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[68:69], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[70:71], v[56:59] \n"
    // C: [0~15]M  * [64~127]N  ----------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[64:65], 0        \n"
    " v_fmac_f32 %[v_c_0x],  v56, v224 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[66:67], v[60:63] \n"
    " v_fmac_f32 %[v_c_0y],  v57, v224 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[68:69], v[60:63] \n"
    " v_fmac_f32 %[v_c_0z],  v58, v224 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[70:71], v[60:63] \n"
    " v_fmac_f32 %[v_c_0w],  v59, v224 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[72:73], 0        \n"
    " v_fmac_f32 %[v_c_1x],  v60, v224 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[74:75], v[56:59] \n"
    " v_fmac_f32 %[v_c_1y],  v61, v224 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[76:77], v[56:59] \n"
    " v_fmac_f32 %[v_c_1z],  v62, v224 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[78:79], v[56:59] \n"
    " v_fmac_f32 %[v_c_1w],  v63, v224 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[72:73], 0        \n"
    " v_fmac_f32 %[v_c_2x],  v56, v225 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[74:75], v[60:63] \n"
    " v_fmac_f32 %[v_c_2y],  v57, v225 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[76:77], v[60:63] \n"
    " v_fmac_f32 %[v_c_2z],  v58, v225 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[78:79], v[60:63] \n"
    " v_fmac_f32 %[v_c_2w],  v59, v225 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[80:81], 0        \n"
    " v_fmac_f32 %[v_c_3x],  v60, v225 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[82:83], v[56:59] \n"
    " v_fmac_f32 %[v_c_3y],  v61, v225 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[84:85], v[56:59] \n"
    " v_fmac_f32 %[v_c_3z],  v62, v225 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[86:87], v[56:59] \n"
    " v_fmac_f32 %[v_c_3w],  v63, v225 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[80:81], 0        \n"
    " v_fmac_f32 %[v_c_4x],  v56,v226 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[82:83], v[60:63] \n"
    " v_fmac_f32 %[v_c_4y],  v57,v226 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[84:85], v[60:63] \n"
    " v_fmac_f32 %[v_c_4z],  v58,v226 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[86:87], v[60:63] \n"
    " v_fmac_f32 %[v_c_4w],  v59,v226 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[88:89], 0        \n"
    " v_fmac_f32 %[v_c_5x],  v60, v226 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[90:91], v[56:59] \n"
    " v_fmac_f32 %[v_c_5y],  v61, v226 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[92:93], v[56:59] \n"
    " v_fmac_f32 %[v_c_5z],  v62, v226 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[94:95], v[56:59] \n"
    " v_fmac_f32 %[v_c_5w],  v63, v226 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[88:89], 0        \n"
    " v_fmac_f32 %[v_c_6x],  v56, v227 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[90:91], v[60:63] \n"
    " v_fmac_f32 %[v_c_6y],  v57, v227 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[92:93], v[60:63] \n"
    " v_fmac_f32 %[v_c_6z],  v58, v227 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[94:95], v[60:63] \n"
    " v_fmac_f32 %[v_c_6w],  v59, v227 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[96:97],   0        \n"
    " v_fmac_f32 %[v_c_7x],  v60, v227 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[98:99],   v[56:59] \n"
    " v_fmac_f32 %[v_c_7y],  v61, v227 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[100:101], v[56:59] \n"
    " v_fmac_f32 %[v_c_7z],  v62, v227 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[102:103], v[56:59] \n"
    " v_fmac_f32 %[v_c_7w],  v63, v227 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[96:97],   0        \n"
    " v_fmac_f32 %[v_c_8x],  v56, v228 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[98:99],   v[60:63] \n"
    " v_fmac_f32 %[v_c_8y],  v57, v228 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[100:101], v[60:63] \n"
    " v_fmac_f32 %[v_c_8z],  v58, v228 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[102:103], v[60:63] \n"
    " v_fmac_f32 %[v_c_8w],  v59, v228 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[104:105], 0        \n"
    " v_fmac_f32 %[v_c_9x],  v60, v228 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[106:107], v[56:59] \n"
    " v_fmac_f32 %[v_c_9y],  v61, v228 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[108:109], v[56:59] \n"
    " v_fmac_f32 %[v_c_9z],  v62, v228 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[110:111], v[56:59] \n"
    " v_fmac_f32 %[v_c_9w],  v63, v228 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[104:105], 0        \n"
    " v_fmac_f32 %[v_c_10x], v56, v229 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[106:107], v[60:63] \n"
    " v_fmac_f32 %[v_c_10y], v57, v229 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[108:109], v[60:63] \n"
    " v_fmac_f32 %[v_c_10z], v58, v229 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[110:111], v[60:63] \n"
    " v_fmac_f32 %[v_c_10w], v59, v229 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[112:113], 0        \n"
    " v_fmac_f32 %[v_c_11x], v60, v229 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[114:115], v[56:59] \n"
    " v_fmac_f32 %[v_c_11y], v61, v229 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[116:117], v[56:59] \n"
    " v_fmac_f32 %[v_c_11z], v62, v229 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[118:119], v[56:59] \n"
    " v_fmac_f32 %[v_c_11w], v63, v229 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[112:113], 0        \n"
    " v_fmac_f32 %[v_c_12x], v56, v230 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[114:115], v[60:63] \n"
    " v_fmac_f32 %[v_c_12y], v57, v230 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[116:117], v[60:63] \n"
    " v_fmac_f32 %[v_c_12z], v58, v230 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[118:119], v[60:63] \n"
    " v_fmac_f32 %[v_c_12w], v59, v230 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[0:1],   acc[120:121], 0        \n"
    " v_fmac_f32 %[v_c_13x], v60, v230 \n"
    _UK_MFMA_ " v[56:59], acc[2:3],   acc[122:123], v[56:59] \n"
    " v_fmac_f32 %[v_c_13y], v61, v230 \n"
    _UK_MFMA_ " v[56:59], acc[4:5],   acc[124:125], v[56:59] \n"
    " v_fmac_f32 %[v_c_13z], v62, v230 \n"
    _UK_MFMA_ " v[56:59], acc[6:7],   acc[126:127], v[56:59] \n"
    " v_fmac_f32 %[v_c_13w], v63, v230 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[8:9],   acc[120:121], 0        \n"
    " v_fmac_f32 %[v_c_14x], v56, v231 \n"
    _UK_MFMA_ " v[60:63], acc[10:11], acc[122:123], v[60:63] \n"
    " v_fmac_f32 %[v_c_14y], v57, v231 \n"
    _UK_MFMA_ " v[60:63], acc[12:13], acc[124:125], v[60:63] \n"
    " v_fmac_f32 %[v_c_14z], v58, v231 \n"
    _UK_MFMA_ " v[60:63], acc[14:15], acc[126:127], v[60:63] \n"
    " v_fmac_f32 %[v_c_14w], v59, v231 \n"
    // ===================================================================================
    // C: [0~15]M * [128~191]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[64:65], 0        \n"
    " v_fmac_f32 %[v_c_15x], v60, v231 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[66:67], v[56:59] \n"
    " v_fmac_f32 %[v_c_15y], v61, v231 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[68:69], v[56:59] \n"
    " v_fmac_f32 %[v_c_15z], v62, v231 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[70:71], v[56:59] \n"
    " v_fmac_f32 %[v_c_15w], v63, v231 \n"
    // C: [0~15]M  * [192~255]N  ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[64:65], 0        \n"
    " v_fmac_f32 %[v_c_16x], v56, v240 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[66:67], v[60:63] \n"
    " v_fmac_f32 %[v_c_16y], v57, v240 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[68:69], v[60:63] \n"
    " v_fmac_f32 %[v_c_16z], v58, v240 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[70:71], v[60:63] \n"
    " v_fmac_f32 %[v_c_16w], v59, v240 \n"
    // C: [16~31]M * [128~191]N ----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[72:73], 0        \n"
    " v_fmac_f32 %[v_c_17x], v60, v240 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[74:75], v[56:59] \n"
    " v_fmac_f32 %[v_c_17y], v61, v240 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[76:77], v[56:59] \n"
    " v_fmac_f32 %[v_c_17z], v62, v240 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[78:79], v[56:59] \n"
    " v_fmac_f32 %[v_c_17w], v63, v240 \n"
    // C: [16~31]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[72:73], 0        \n"
    " v_fmac_f32 %[v_c_18x], v56, v241 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[74:75], v[60:63] \n"
    " v_fmac_f32 %[v_c_18y], v57, v241 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[76:77], v[60:63] \n"
    " v_fmac_f32 %[v_c_18z], v58, v241 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[78:79], v[60:63] \n"
    " v_fmac_f32 %[v_c_18w], v59, v241 \n"
    // C: [32~47]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[80:81], 0        \n"
    " v_fmac_f32 %[v_c_19x], v60, v241 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[82:83], v[56:59] \n"
    " v_fmac_f32 %[v_c_19y], v61, v241 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[84:85], v[56:59] \n"
    " v_fmac_f32 %[v_c_19z], v62, v241 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[86:87], v[56:59] \n"
    " v_fmac_f32 %[v_c_19w], v63, v241 \n"
    // C: [32~47]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[80:81], 0        \n"
    " v_fmac_f32 %[v_c_20x], v56, v242 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[82:83], v[60:63] \n"
    " v_fmac_f32 %[v_c_20y], v57, v242 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[84:85], v[60:63] \n"
    " v_fmac_f32 %[v_c_20z], v58, v242 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[86:87], v[60:63] \n"
    " v_fmac_f32 %[v_c_20w], v59, v242 \n"
    // C: [48~63]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[88:89], 0        \n"
    " v_fmac_f32 %[v_c_21x], v60, v242 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[90:91], v[56:59] \n"
    " v_fmac_f32 %[v_c_21y], v61, v242 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[92:93], v[56:59] \n"
    " v_fmac_f32 %[v_c_21z], v62, v242 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[94:95], v[56:59] \n"
    " v_fmac_f32 %[v_c_21w], v63, v242 \n"
    // C: [48~63]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[88:89], 0        \n"
    " v_fmac_f32 %[v_c_22x], v56, v243 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[90:91], v[60:63] \n"
    " v_fmac_f32 %[v_c_22y], v57, v243 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[92:93], v[60:63] \n"
    " v_fmac_f32 %[v_c_22z], v58, v243 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[94:95], v[60:63] \n"
    " v_fmac_f32 %[v_c_22w], v59, v243 \n"
    // C: [64~79]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[96:97],   0        \n"
    " v_fmac_f32 %[v_c_23x], v60, v243 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[98:99],   v[56:59] \n"
    " v_fmac_f32 %[v_c_23y], v61, v243 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[100:101], v[56:59] \n"
    " v_fmac_f32 %[v_c_23z], v62, v243 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[102:103], v[56:59] \n"
    " v_fmac_f32 %[v_c_23w], v63, v243 \n"
    // C: [64~79]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[96:97],   0        \n"
    " v_fmac_f32 %[v_c_24x], v56, v244 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[98:99],   v[60:63] \n"
    " v_fmac_f32 %[v_c_24y], v57, v244 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[100:101], v[60:63] \n"
    " v_fmac_f32 %[v_c_24z], v58, v244 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[102:103], v[60:63] \n"
    " v_fmac_f32 %[v_c_24w], v59, v244 \n"
    // C: [80~95]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[104:105], 0        \n"
    " v_fmac_f32 %[v_c_25x], v60, v244 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[106:107], v[56:59] \n"
    " v_fmac_f32 %[v_c_25y], v61, v244 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[108:109], v[56:59] \n"
    " v_fmac_f32 %[v_c_25z], v62, v244 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[110:111], v[56:59] \n"
    " v_fmac_f32 %[v_c_25w], v63, v244 \n"
    // C: [80~95]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[104:105], 0        \n"
    " v_fmac_f32 %[v_c_26x], v56, v245 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[106:107], v[60:63] \n"
    " v_fmac_f32 %[v_c_26y], v57, v245 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[108:109], v[60:63] \n"
    " v_fmac_f32 %[v_c_26z], v58, v245 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[110:111], v[60:63] \n"
    " v_fmac_f32 %[v_c_26w], v59, v245 \n"
    // C: [96~111]M  * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[112:113], 0        \n"
    " v_fmac_f32 %[v_c_27x], v60, v245 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[114:115], v[56:59] \n"
    " v_fmac_f32 %[v_c_27y], v61, v245 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[116:117], v[56:59] \n"
    " v_fmac_f32 %[v_c_27z], v62, v245 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[118:119], v[56:59] \n"
    " v_fmac_f32 %[v_c_27w], v63, v245 \n"
    // C: [96~111]M  * [192~255]N --------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[112:113], 0        \n"
    " v_fmac_f32 %[v_c_28x], v56, v246 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[114:115], v[60:63] \n"
    " v_fmac_f32 %[v_c_28y], v57, v246 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[116:117], v[60:63] \n"
    " v_fmac_f32 %[v_c_28z], v58, v246 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[118:119], v[60:63] \n"
    " v_fmac_f32 %[v_c_28w], v59, v246 \n"
    // C: [112~127]M * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[32:33], acc[120:121], 0        \n"
    " v_fmac_f32 %[v_c_29x], v60, v246 \n"
    _UK_MFMA_ " v[56:59], acc[34:35], acc[122:123], v[56:59] \n"
    " v_fmac_f32 %[v_c_29y], v61, v246 \n"
    _UK_MFMA_ " v[56:59], acc[36:37], acc[124:125], v[56:59] \n"
    " v_fmac_f32 %[v_c_29z], v62, v246 \n"
    _UK_MFMA_ " v[56:59], acc[38:39], acc[126:127], v[56:59] \n"
    " v_fmac_f32 %[v_c_29w], v63, v246 \n"
    // C: [112~127]M * [192~255]N --------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[40:41], acc[120:121], 0        \n"
    " v_fmac_f32 %[v_c_30x], v56, v247 \n"
    _UK_MFMA_ " v[60:63], acc[42:43], acc[122:123], v[60:63] \n"
    " v_fmac_f32 %[v_c_30y], v57, v247 \n"
    _UK_MFMA_ " v[60:63], acc[44:45], acc[124:125], v[60:63] \n"
    " v_fmac_f32 %[v_c_30z], v58, v247 \n"
    _UK_MFMA_ " v[60:63], acc[46:47], acc[126:127], v[60:63] \n"
    " v_fmac_f32 %[v_c_30w], v59, v247 \n"
    " s_nop 4 \n"
    " v_fmac_f32 %[v_c_31x], v60, v247 \n"
    " v_fmac_f32 %[v_c_31y], v61, v247 \n"
    " v_fmac_f32 %[v_c_31z], v62, v247 \n"
    " v_fmac_f32 %[v_c_31w], v63, v247 \n"
    // -----------------------------------------------------------------------------------
    // forloop
    " s_sub_i32     %[s_loop_cnt], %[s_loop_cnt], 1 \n"
    " s_cmp_gt_i32  %[s_loop_cnt] 0                 \n"
    " s_cbranch_scc0 L_end%=                        \n"
    // move to next A
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    " s_add_u32     s16, s86, s16          \n"
    " s_addc_u32    s17, 0, s17            \n"
    // move to next B
    " s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0 \n"
    " s_add_u32     s20, s86, s20          \n"
    " s_addc_u32    s21, 0, s21            \n"
    // move to next SA
    " s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0   \n"
    " s_add_u32     s24, s86, s24          \n"
    " s_addc_u32    s25, 0, s25            \n"
    // move to next SB
    " s_cmp_gt_i32  %[s_loop_cnt] 1        \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0   \n"
    " s_add_u32     s28, s86, s28          \n"
    " s_addc_u32    s29, 0, s29            \n"
#pragma endregion

#pragma region LOOP_2
    
    " s_waitcnt lgkmcnt(0) \n"
    " s_waitcnt vmcnt(0)   \n"
    " s_barrier            \n"

    // PREFETCH PING =====================================================================
    // A1: vmem -> lds
    " s_add_u32     m0,  %[s_a_vm_lds_m0], %[n_a_lds_pingpang_sz] * 1 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr0],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr1],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr2],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr3],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr4],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr5],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr6],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr7],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr8],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr9],  s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr10], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr11], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr12], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr13], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr14], s[16:19], 0 offen lds \n"
    " s_add_u32     m0,  %[s_a_m0_offset], m0 \n"
    " buffer_load_dword  %[v_a_vm_ld_addr15], s[16:19], 0 offen lds \n"
    
    // A0: lds->accvgpr
    " ds_read_b128  acc[64:67],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 \n" // [0~15]M    * [0~63]K
    " ds_read_b128  acc[68:71],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 \n" // [0~15]M    * [64~127]K
    " ds_read_b128  acc[72:75],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 \n" // [16~31]M   * [0~63]K
    " ds_read_b128  acc[76:79],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 \n" // [16~31]M   * [64~127]K
    " ds_read_b128  acc[80:83],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 \n" // [32~47]M   * [0~63]K
    " ds_read_b128  acc[84:87],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 \n" // [32~47]M   * [64~127]K
    " ds_read_b128  acc[88:91],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 \n" // [48~63]M   * [0~63]K
    " ds_read_b128  acc[92:95],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 \n" // [48~63]M   * [64~127]K
    " ds_read_b128  acc[96:99],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 \n" // [64~79]M   * [0~63]K
    " ds_read_b128  acc[100:103], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 \n" // [64~79]M   * [64~127]K
    " ds_read_b128  acc[104:107], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 \n" // [80~95]M   * [0~63]K
    " ds_read_b128  acc[108:111], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 \n" // [80~95]M   * [64~127]K
    " ds_read_b128  acc[112:115], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 \n" // [96~111]M  * [0~63]K
    " ds_read_b128  acc[116:119], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 \n" // [96~111]M  * [64~128]K
    " ds_read_b128  acc[120:123], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 \n" // [112~127]M * [0~63]K
    " ds_read_b128  acc[124:127], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 \n" // [112~127]M * [64~127]K

    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [64~127]N * [64~127]K

    " buffer_load_dwordx4  acc[32:35], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [128~191]N * [0~63]K
    " buffer_load_dwordx4  acc[36:39], %[v_os_b2], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [128~191]N * [64~127]K
    " buffer_load_dwordx4  acc[40:43], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n" // [192~255]N * [0~63]K
    " buffer_load_dwordx4  acc[44:47], %[v_os_b3], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n" // [192~255]N * [64~127]K

    // scale a0, b0
    " buffer_load_dword v224, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0 \n" // [0~15]M
    " buffer_load_dword v225, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1 \n" // [16~31]M
    " buffer_load_dword v226, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2 \n" // [32~47]M
    " buffer_load_dword v227, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3 \n" // [48~63]M
    " buffer_load_dword v228, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4 \n" // [64~79]M
    " buffer_load_dword v229, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5 \n" // [80~95]M
    " buffer_load_dword v230, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6 \n" // [96~111]M
    " buffer_load_dword v231, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7 \n" // [112~127]M
    " s_load_dwordx2 s[30:31], s[28:29],  0 \n"
    
    // MFMA PANG =========================================================================
    // scale c1 = scale a1 * scale b1
    " v_mov_b32  v50, s85        \n"
    " v_mul_f32  v248, v232, v50 \n"
    " v_mul_f32  v249, v233, v50 \n"
    " v_mul_f32  v250, v234, v50 \n"
    " v_mul_f32  v251, v235, v50 \n"
    " v_mul_f32  v252, v236, v50 \n"
    " v_mul_f32  v253, v237, v50 \n"
    " v_mul_f32  v254, v238, v50 \n"
    " v_mul_f32  v255, v239, v50 \n"
    " v_mov_b32  v50, s84        \n"
    " v_mul_f32  v232, v232, v50 \n"
    " v_mul_f32  v233, v233, v50 \n"
    " v_mul_f32  v234, v234, v50 \n"
    " v_mul_f32  v235, v235, v50 \n"
    " v_mul_f32  v236, v236, v50 \n"
    " v_mul_f32  v237, v237, v50 \n"
    " v_mul_f32  v238, v238, v50 \n"
    " v_mul_f32  v239, v239, v50 \n"
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[128:129], 0        \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[130:131], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[132:133], v[56:59] \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[134:135], v[56:59] \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[128:129], 0        \n"
    " v_fmac_f32 %[v_c_0x],  v56, v232 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[130:131], v[60:63] \n"
    " v_fmac_f32 %[v_c_0y],  v57, v232 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[132:133], v[60:63] \n"
    " v_fmac_f32 %[v_c_0z],  v58, v232 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[134:135], v[60:63] \n"
    " v_fmac_f32 %[v_c_0w],  v59, v232 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[136:137], 0        \n"
    " v_fmac_f32 %[v_c_1x],  v60, v232 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[138:139], v[56:59] \n"
    " v_fmac_f32 %[v_c_1y],  v61, v232 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[140:141], v[56:59] \n"
    " v_fmac_f32 %[v_c_1z],  v62, v232 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[142:143], v[56:59] \n"
    " v_fmac_f32 %[v_c_1w],  v63, v232 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[136:137], 0        \n"
    " v_fmac_f32 %[v_c_2x],  v56, v233 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[138:139], v[60:63] \n"
    " v_fmac_f32 %[v_c_2y],  v57, v233 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[140:141], v[60:63] \n"
    " v_fmac_f32 %[v_c_2z],  v58, v233 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[142:143], v[60:63] \n"
    " v_fmac_f32 %[v_c_2w],  v59, v233 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[144:145], 0        \n"
    " v_fmac_f32 %[v_c_3x],  v60, v233 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[146:147], v[56:59] \n"
    " v_fmac_f32 %[v_c_3y],  v61, v233 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[148:149], v[56:59] \n"
    " v_fmac_f32 %[v_c_3z],  v62, v233 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[150:151], v[56:59] \n"
    " v_fmac_f32 %[v_c_3w],  v63, v233 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[144:145], 0        \n"
    " v_fmac_f32 %[v_c_4x],  v56, v234 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[146:147], v[60:63] \n"
    " v_fmac_f32 %[v_c_4y],  v57, v234 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[148:149], v[60:63] \n"
    " v_fmac_f32 %[v_c_4z],  v58, v234 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[150:151], v[60:63] \n"
    " v_fmac_f32 %[v_c_4w],  v59, v234 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[152:153], 0 \n"
    " v_fmac_f32 %[v_c_5x],  v60, v234 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[154:155], v[56:59] \n"
    " v_fmac_f32 %[v_c_5y],  v61, v234 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[156:157], v[56:59] \n"
    " v_fmac_f32 %[v_c_5z],  v62, v234 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[158:159], v[56:59] \n"
    " v_fmac_f32 %[v_c_5w],  v63, v234 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[152:153], 0 \n"
    " v_fmac_f32 %[v_c_6x],  v56, v235 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[154:155], v[60:63] \n"
    " v_fmac_f32 %[v_c_6y],  v57, v235 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[156:157], v[60:63] \n"
    " v_fmac_f32 %[v_c_6z],  v58, v235 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[158:159], v[60:63] \n"
    " v_fmac_f32 %[v_c_6w],  v59, v235 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[160:161], 0 \n"
    " v_fmac_f32 %[v_c_7x],  v60, v235 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[162:163], v[56:59] \n"
    " v_fmac_f32 %[v_c_7y],  v61, v235 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[164:165], v[56:59] \n"
    " v_fmac_f32 %[v_c_7z],  v62, v235 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[166:167], v[56:59] \n"
    " v_fmac_f32 %[v_c_7w],  v63, v235 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[160:161], 0 \n"
    " v_fmac_f32 %[v_c_8x],  v56, v236 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[162:163], v[60:63] \n"
    " v_fmac_f32 %[v_c_8y],  v57, v236 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[164:165], v[60:63] \n"
    " v_fmac_f32 %[v_c_8z],  v58, v236 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[166:167], v[60:63] \n"
    " v_fmac_f32 %[v_c_8w],  v59, v236 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[168:169], 0 \n"
    " v_fmac_f32 %[v_c_9x],  v60, v236 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[170:171], v[56:59] \n"
    " v_fmac_f32 %[v_c_9y],  v61, v236 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[172:173], v[56:59] \n"
    " v_fmac_f32 %[v_c_9z],  v62, v236 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[174:175], v[56:59] \n"
    " v_fmac_f32 %[v_c_9w],  v63, v236 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[168:169], 0 \n"
    " v_fmac_f32 %[v_c_10x], v56, v237 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[170:171], v[60:63] \n"
    " v_fmac_f32 %[v_c_10y], v57, v237 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[172:173], v[60:63] \n"
    " v_fmac_f32 %[v_c_10z], v58, v237 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[174:175], v[60:63] \n"
    " v_fmac_f32 %[v_c_10w], v59, v237 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[176:177], 0 \n"
    " v_fmac_f32 %[v_c_11x], v60, v237 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[178:179], v[56:59] \n"
    " v_fmac_f32 %[v_c_11y], v61, v237 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[180:181], v[56:59] \n"
    " v_fmac_f32 %[v_c_11z], v62, v237 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[182:183], v[56:59] \n"
    " v_fmac_f32 %[v_c_11w], v63, v237 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[176:177], 0 \n"
    " v_fmac_f32 %[v_c_12x], v56, v238 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[178:179], v[60:63] \n"
    " v_fmac_f32 %[v_c_12y], v57, v238 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[180:181], v[60:63] \n"
    " v_fmac_f32 %[v_c_12z], v58, v238 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[182:183], v[60:63] \n"
    " v_fmac_f32 %[v_c_12w], v59, v238 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[16:17], acc[184:185], 0 \n"
    " v_fmac_f32 %[v_c_13x], v60, v238 \n"
    _UK_MFMA_ " v[56:59], acc[18:19], acc[186:187], v[56:59] \n"
    " v_fmac_f32 %[v_c_13y], v61, v238 \n"
    _UK_MFMA_ " v[56:59], acc[20:21], acc[188:189], v[56:59] \n"
    " v_fmac_f32 %[v_c_13z], v62, v238 \n"
    _UK_MFMA_ " v[56:59], acc[22:23], acc[190:191], v[56:59] \n"
    " v_fmac_f32 %[v_c_13w], v63, v238 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[24:25], acc[184:185], 0 \n"
    " v_fmac_f32 %[v_c_14x], v56, v239 \n"
    _UK_MFMA_ " v[60:63], acc[26:27], acc[186:187], v[60:63] \n"
    " v_fmac_f32 %[v_c_14y], v57, v239 \n"
    _UK_MFMA_ " v[60:63], acc[28:29], acc[188:189], v[60:63] \n"
    " v_fmac_f32 %[v_c_14z], v58, v239 \n"
    _UK_MFMA_ " v[60:63], acc[30:31], acc[190:191], v[60:63] \n"
    " v_fmac_f32 %[v_c_14w], v59, v239 \n"
    // ===================================================================================
    // C: [0~15]M * [128~191]N -----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[128:129], 0 \n"
    " v_fmac_f32 %[v_c_15x], v60, v239 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[130:131], v[56:59] \n"
    " v_fmac_f32 %[v_c_15y], v61, v239 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[132:133], v[56:59] \n"
    " v_fmac_f32 %[v_c_15z], v62, v239 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[134:135], v[56:59] \n"
    " v_fmac_f32 %[v_c_15w], v63, v239 \n"
    // C: [0~15]M  * [192~255]N  ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[128:129], 0 \n"
    " v_fmac_f32 %[v_c_16x], v56, v248 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[130:131], v[60:63] \n"
    " v_fmac_f32 %[v_c_16y], v57, v248 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[132:133], v[60:63] \n"
    " v_fmac_f32 %[v_c_16z], v58, v248 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[134:135], v[60:63] \n"
    " v_fmac_f32 %[v_c_16w], v59, v248 \n"
    // C: [16~31]M * [128~191]N ----------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[136:137], 0 \n"
    " v_fmac_f32 %[v_c_17x], v60, v248 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[138:139], v[56:59] \n"
    " v_fmac_f32 %[v_c_17y], v61, v248 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[140:141], v[56:59] \n"
    " v_fmac_f32 %[v_c_17z], v62, v248 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[142:143], v[56:59] \n"
    " v_fmac_f32 %[v_c_17w], v63, v248 \n"
    // C: [16~31]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[136:137], 0 \n"
    " v_fmac_f32 %[v_c_18x], v56, v249 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[138:139], v[60:63] \n"
    " v_fmac_f32 %[v_c_18y], v57, v249 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[140:141], v[60:63] \n"
    " v_fmac_f32 %[v_c_18z], v58, v249 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[142:143], v[60:63] \n"
    " v_fmac_f32 %[v_c_18w], v59, v249 \n"
    // C: [32~47]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[144:145], 0 \n"
    " v_fmac_f32 %[v_c_19x], v60, v249 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[146:147], v[56:59] \n"
    " v_fmac_f32 %[v_c_19y], v61, v249 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[148:149], v[56:59] \n"
    " v_fmac_f32 %[v_c_19z], v62, v249 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[150:151], v[56:59] \n"
    " v_fmac_f32 %[v_c_19w], v63, v249 \n"
    // C: [32~47]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[144:145], 0 \n"
    " v_fmac_f32 %[v_c_20x], v56, v250 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[146:147], v[60:63] \n"
    " v_fmac_f32 %[v_c_20y], v57, v250 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[148:149], v[60:63] \n"
    " v_fmac_f32 %[v_c_20z], v58, v250 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[150:151], v[60:63] \n"
    " v_fmac_f32 %[v_c_20w], v59, v250 \n"
    // C: [48~63]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[152:153], 0 \n"
    " v_fmac_f32 %[v_c_21x], v60, v250 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[154:155], v[56:59] \n"
    " v_fmac_f32 %[v_c_21y], v61, v250 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[156:157], v[56:59] \n"
    " v_fmac_f32 %[v_c_21z], v62, v250 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[158:159], v[56:59] \n"
    " v_fmac_f32 %[v_c_21w], v63, v250 \n"
    // C: [48~63]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[152:153], 0 \n"
    " v_fmac_f32 %[v_c_22x], v56, v251 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[154:155], v[60:63] \n"
    " v_fmac_f32 %[v_c_22y], v57, v251 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[156:157], v[60:63] \n"
    " v_fmac_f32 %[v_c_22z], v58, v251 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[158:159], v[60:63] \n"
    " v_fmac_f32 %[v_c_22w], v59, v251 \n"
    // C: [64~79]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[160:161], 0 \n"
    " v_fmac_f32 %[v_c_23x], v60, v251 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[162:163], v[56:59] \n"
    " v_fmac_f32 %[v_c_23y], v61, v251 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[164:165], v[56:59] \n"
    " v_fmac_f32 %[v_c_23z], v62, v251 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[166:167], v[56:59] \n"
    " v_fmac_f32 %[v_c_23w], v63, v251 \n"
    // C: [64~79]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[160:161], 0 \n"
    " v_fmac_f32 %[v_c_24x], v56, v252 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[162:163], v[60:63] \n"
    " v_fmac_f32 %[v_c_24y], v57, v252 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[164:165], v[60:63] \n"
    " v_fmac_f32 %[v_c_24z], v58, v252 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[166:167], v[60:63] \n"
    " v_fmac_f32 %[v_c_24w], v59, v252 \n"
    // C: [80~95]M   * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[168:169], 0 \n"
    " v_fmac_f32 %[v_c_25x], v60, v252 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[170:171], v[56:59] \n"
    " v_fmac_f32 %[v_c_25y], v61, v252 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[172:173], v[56:59] \n"
    " v_fmac_f32 %[v_c_25z], v62, v252 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[174:175], v[56:59] \n"
    " v_fmac_f32 %[v_c_25w], v63, v252 \n"
    // C: [80~95]M  * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[168:169], 0 \n"
    " v_fmac_f32 %[v_c_26x], v56, v253 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[170:171], v[60:63] \n"
    " v_fmac_f32 %[v_c_26y], v57, v253 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[172:173], v[60:63] \n"
    " v_fmac_f32 %[v_c_26z], v58, v253 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[174:175], v[60:63] \n"
    " v_fmac_f32 %[v_c_26w], v59, v253 \n"
    // C: [96~111]M  * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[176:177], 0 \n"
    " v_fmac_f32 %[v_c_27x], v60, v253 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[178:179], v[56:59] \n"
    " v_fmac_f32 %[v_c_27y], v61, v253 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[180:181], v[56:59] \n"
    " v_fmac_f32 %[v_c_27z], v62, v253 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[182:183], v[56:59] \n"
    " v_fmac_f32 %[v_c_27w], v63, v253 \n"
    // C: [96~111]M * [192~255]N ---------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[176:177], 0 \n"
    " v_fmac_f32 %[v_c_28x], v56, v254 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[178:179], v[60:63] \n"
    " v_fmac_f32 %[v_c_28y], v57, v254 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[180:181], v[60:63] \n"
    " v_fmac_f32 %[v_c_28z], v58, v254 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[182:183], v[60:63] \n"
    " v_fmac_f32 %[v_c_28w], v59, v254 \n"
    // C: [112~127]M * [128~191]N --------------------------------------------------------
    _UK_MFMA_ " v[56:59], acc[48:49], acc[184:185], 0 \n"
    " v_fmac_f32 %[v_c_29x], v60, v254 \n"
    _UK_MFMA_ " v[56:59], acc[50:51], acc[186:187], v[56:59] \n"
    " v_fmac_f32 %[v_c_29y], v61, v254 \n"
    _UK_MFMA_ " v[56:59], acc[52:53], acc[188:189], v[56:59] \n"
    " v_fmac_f32 %[v_c_29z], v62, v254 \n"
    _UK_MFMA_ " v[56:59], acc[54:55], acc[190:191], v[56:59] \n"
    " v_fmac_f32 %[v_c_29w], v63, v254 \n"
    // C: [112~127]M * [192~255]N --------------------------------------------------------
    _UK_MFMA_ " v[60:63], acc[56:57], acc[184:185], 0 \n"
    " v_fmac_f32 %[v_c_30x], v56, v255 \n"
    _UK_MFMA_ " v[60:63], acc[58:59], acc[186:187], v[60:63] \n"
    " v_fmac_f32 %[v_c_30y], v57, v255 \n"
    _UK_MFMA_ " v[60:63], acc[60:61], acc[188:189], v[60:63] \n"
    " v_fmac_f32 %[v_c_30z], v58, v255 \n"
    _UK_MFMA_ " v[60:63], acc[62:63], acc[190:191], v[60:63] \n"
    " v_fmac_f32 %[v_c_30w], v59, v255 \n"
    " s_nop 4 \n"
    " v_fmac_f32 %[v_c_31x], v60, v255 \n"
    " v_fmac_f32 %[v_c_31y], v61, v255 \n"
    " v_fmac_f32 %[v_c_31z], v62, v255 \n"
    " v_fmac_f32 %[v_c_31w], v63, v255 \n"
    // -----------------------------------------------------------------------------------
    // move to next A
    " s_cmp_gt_i32  %[s_loop_cnt] 3        \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0 \n"
    " s_add_u32     s16, s86, s16          \n"
    " s_addc_u32    s17, 0, s17            \n"
    // move to next B
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0 \n"
    " s_add_u32     s20, s86, s20          \n"
    " s_addc_u32    s21, 0, s21            \n"
    // move to next SA
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0   \n"
    " s_add_u32     s24, s86, s24          \n"
    " s_addc_u32    s25, 0, s25            \n"
    // move to next SB
    " s_cmp_gt_i32  %[s_loop_cnt] 2        \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0   \n"
    " s_add_u32     s28, s86, s28          \n"
    " s_addc_u32    s29, 0, s29            \n"
    // forloop
    " s_sub_i32    %[s_loop_cnt], %[s_loop_cnt], 1 \n"
    " s_cmp_gt_i32 %[s_loop_cnt] 0                 \n"
    " s_cbranch_scc0 L_end%=   \n"
    " s_branch       L_start%= \n"
#pragma endregion
#endif

"L_end%=: \n"

#undef _UK_MFMA_
#undef _FF_SYNC_
#undef _INIT_ACC_
#undef _SCALE_A_LDS_
#undef _FF_END_
