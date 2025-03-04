// acc: use 160, free 96 ------------------
// acc[0..15]    (16) : B_ping
// acc[16..31]   (16) : B_pang
// acc[32..63]   (32) : free
// acc[64..127]  (64) : A_ping
// acc[128..191] (64) : A_pang
// acc[192..255] (64) : free
// ----------------------------------------
// vgpr: use 64~195, free 60 --------------
// v[64..127]    (64) : Acc result, float4 c[0..16]
// v[128..143]        : A_ping vmem->vgpr->lds
// v[144..159]        : A_pang vmem->vgpr->lds
// v[160..167]   (8)  : scale A ping
// v[168..175]   (8)  : scale A pang
// v[176]        (1)  : scale B
// v[192..195]        : acc_in_tmp1
// v[196..199]        : acc_in_tmp2
// ----------------------------------------
// s30           (1)  : scale B ping
// s31           (1)  : scale B pang

#define _UK_MFMA_ "v_mfma_f32_16x16x32_fp8_fp8"

#define _FF_SYNC_ " s_waitcnt vmcnt(0)      \n" \
                  " s_waitcnt lgkmcnt(0)    \n" \
                  " s_barrier               \n" \
                  " s_nop 16                \n"
#define _FF_END_  " s_branch L_end%=        \n"

    "s_mov_b32 s16,    %[s_res_a0]      \n"
    "s_mov_b32 s17,    %[s_res_a1]      \n"
    "s_mov_b32 s18,    %[s_res_a2]      \n"
    "s_mov_b32 s19,    %[s_res_a3]      \n"
    "s_mov_b32 s20,    %[s_res_b0]      \n"
    "s_mov_b32 s21,    %[s_res_b1]      \n"
    "s_mov_b32 s22,    %[s_res_b2]      \n"
    "s_mov_b32 s23,    %[s_res_b3]      \n"

    "s_mov_b32 s24,    %[s_res_sa0]     \n"
    "s_mov_b32 s25,    %[s_res_sa1]     \n"
    "s_mov_b32 s26,    %[s_res_sa2]     \n"
    "s_mov_b32 s27,    %[s_res_sa3]     \n"
    "s_mov_b32 s28,    %[s_base_sb_l]   \n"
    "s_mov_b32 s29,    %[s_base_sb_h]   \n"

#pragma region INIT_ACC
    " v_mov_b32 %[v_c_0x],   0          \n"
    " v_mov_b32 %[v_c_0y],   0          \n"
    " v_mov_b32 %[v_c_0z],   0          \n"
    " v_mov_b32 %[v_c_0w],   0          \n"
    " v_mov_b32 %[v_c_1x],   0          \n"
    " v_mov_b32 %[v_c_1y],   0          \n"
    " v_mov_b32 %[v_c_1z],   0          \n"
    " v_mov_b32 %[v_c_1w],   0          \n"
    " v_mov_b32 %[v_c_2x],   0          \n"
    " v_mov_b32 %[v_c_2y],   0          \n"
    " v_mov_b32 %[v_c_2z],   0          \n"
    " v_mov_b32 %[v_c_2w],   0          \n"
    " v_mov_b32 %[v_c_3x],   0          \n"
    " v_mov_b32 %[v_c_3y],   0          \n"
    " v_mov_b32 %[v_c_3z],   0          \n"
    " v_mov_b32 %[v_c_3w],   0          \n"
    " v_mov_b32 %[v_c_4x],   0          \n"
    " v_mov_b32 %[v_c_4y],   0          \n"
    " v_mov_b32 %[v_c_4z],   0          \n"
    " v_mov_b32 %[v_c_4w],   0          \n"
    " v_mov_b32 %[v_c_5x],   0          \n"
    " v_mov_b32 %[v_c_5y],   0          \n"
    " v_mov_b32 %[v_c_5z],   0          \n"
    " v_mov_b32 %[v_c_5w],   0          \n"
    " v_mov_b32 %[v_c_6x],   0          \n"
    " v_mov_b32 %[v_c_6y],   0          \n"
    " v_mov_b32 %[v_c_6z],   0          \n"
    " v_mov_b32 %[v_c_6w],   0          \n"
    " v_mov_b32 %[v_c_7x],   0          \n"
    " v_mov_b32 %[v_c_7y],   0          \n"
    " v_mov_b32 %[v_c_7z],   0          \n"
    " v_mov_b32 %[v_c_7w],   0          \n"
    " v_mov_b32 %[v_c_8x],   0          \n"
    " v_mov_b32 %[v_c_8y],   0          \n"
    " v_mov_b32 %[v_c_8z],   0          \n"
    " v_mov_b32 %[v_c_8w],   0          \n"
    " v_mov_b32 %[v_c_9x],   0          \n"
    " v_mov_b32 %[v_c_9y],   0          \n"
    " v_mov_b32 %[v_c_9z],   0          \n"
    " v_mov_b32 %[v_c_9w],   0          \n"
    " v_mov_b32 %[v_c_10x],  0          \n"
    " v_mov_b32 %[v_c_10y],  0          \n"
    " v_mov_b32 %[v_c_10z],  0          \n"
    " v_mov_b32 %[v_c_10w],  0          \n"
    " v_mov_b32 %[v_c_11x],  0          \n"
    " v_mov_b32 %[v_c_11y],  0          \n"
    " v_mov_b32 %[v_c_11z],  0          \n"
    " v_mov_b32 %[v_c_11w],  0          \n"
    " v_mov_b32 %[v_c_12x],  0          \n"
    " v_mov_b32 %[v_c_12y],  0          \n"
    " v_mov_b32 %[v_c_12z],  0          \n"
    " v_mov_b32 %[v_c_12w],  0          \n"
    " v_mov_b32 %[v_c_13x],  0          \n"
    " v_mov_b32 %[v_c_13y],  0          \n"
    " v_mov_b32 %[v_c_13z],  0          \n"
    " v_mov_b32 %[v_c_13w],  0          \n"
    " v_mov_b32 %[v_c_14x],  0          \n"
    " v_mov_b32 %[v_c_14y],  0          \n"
    " v_mov_b32 %[v_c_14z],  0          \n"
    " v_mov_b32 %[v_c_14w],  0          \n"
    " v_mov_b32 %[v_c_15x],  0          \n"
    " v_mov_b32 %[v_c_15y],  0          \n"
    " v_mov_b32 %[v_c_15z],  0          \n"
    " v_mov_b32 %[v_c_15w],  0          \n"
#pragma endregion
    
#if 0
#pragma region DEBUG_MOVE_TILE
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
#pragma endregion

#pragma region DEBUG_ADDRESS
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
    // debug A lds read 0/2/4/6/8/10/12/14/16 ---------------------------
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]   \n"
    _FF_SYNC_
    // debug A lds read 1/3/5/7/9/11/13/15/17 ---------------------------
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_read_b128  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]   \n"
    _FF_SYNC_
    // debug B vmem -----------------------------------------------------
    " buffer_load_dwordx4  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n"
    " buffer_load_dwordx4  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n"
    " buffer_load_dwordx4  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0 \n"
    " buffer_load_dwordx4  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1 \n"
    _FF_SYNC_
    // debug scale A vmem -----------------------------------------------
    " buffer_load_dword    %[v_c_0x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0   \n"
    " buffer_load_dword    %[v_c_0y], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1   \n"
    " buffer_load_dword    %[v_c_0z], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2   \n"
    " buffer_load_dword    %[v_c_0w], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3   \n"
    " buffer_load_dword    %[v_c_1x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4   \n"
    " buffer_load_dword    %[v_c_1y], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5   \n"
    " buffer_load_dword    %[v_c_1z], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6   \n"
    " buffer_load_dword    %[v_c_1w], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7   \n"
    _FF_SYNC_
    // debug scale B vmem ---------------------------------------------
    " s_load_dword         s30, s[28:29],  0    \n"
    " s_waitcnt lgkmcnt(0)                      \n"
    " v_mov_b32            %[v_c_2x], 1.0       \n"
    " v_mov_b32            %[v_c_2y], 1.0       \n"
    " v_mov_b32            %[v_c_2z], 1.0       \n"
    " v_mov_b32            %[v_c_2w], s30       \n"
    _FF_SYNC_
#pragma endregion

#pragma region DEBUG_MFMA
    _FF_SYNC_
    // A: vmem->vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " s_waitcnt lgkmcnt(0)                                                              \n"
    " s_barrier                                                                         \n"
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
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale A: vmem->vgpr 
    " buffer_load_dword    v160, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v161, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v162, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v163, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v164, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v165, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v166, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v167, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale B: vmem->sgpr->vgpr 
    " s_load_dword         s30, s[28:29],  0                            \n"
    " s_waitcnt vmcnt(0)                                                \n"
    // scale c = scale a * scale b
    " v_mov_b32  v176, s30                                              \n"
    " v_mul_f32  v160, v160, v176                                       \n"
    " v_mul_f32  v161, v161, v176                                       \n"
    " v_mul_f32  v162, v162, v176                                       \n"
    " v_mul_f32  v163, v163, v176                                       \n"
    " v_mul_f32  v164, v164, v176                                       \n"
    " v_mul_f32  v165, v165, v176                                       \n"
    " v_mul_f32  v166, v166, v176                                       \n"
    " v_mul_f32  v167, v167, v176                                       \n"
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
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[64:65], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[66:67], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[68:69], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[70:71], v[192:195]        \n"
    // C = C * scale c
    " s_nop 16                                                          \n"
    " v_mul_f32 %[v_c_0x], v192, v160                                   \n"
    " v_mul_f32 %[v_c_0y], v193, v160                                   \n"
    " v_mul_f32 %[v_c_0z], v194, v160                                   \n"
    " v_mul_f32 %[v_c_0w], v195, v160                                   \n"
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
    _UK_MFMA_   " v[192:195], acc[8:9],   acc[64:65], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[10:11], acc[66:67], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[12:13], acc[68:69], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[14:15], acc[70:71], v[192:195]        \n"
    // C = C * scale c
    " s_nop 16                                                          \n"
    " v_mul_f32 %[v_c_1x], v192, v160                                   \n"
    " v_mul_f32 %[v_c_1y], v193, v160                                   \n"
    " v_mul_f32 %[v_c_1z], v194, v160                                   \n"
    " v_mul_f32 %[v_c_1w], v195, v160                                   \n"
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
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[72:73], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[74:75], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[76:77], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[78:79], v[192:195]        \n"
    // C = C * scale c
    " s_nop 16                                                          \n"
    " v_mul_f32 %[v_c_2x], v192, v161                                   \n"
    " v_mul_f32 %[v_c_2y], v193, v161                                   \n"
    " v_mul_f32 %[v_c_2z], v194, v161                                   \n"
    " v_mul_f32 %[v_c_2w], v195, v161                                   \n"
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
    _UK_MFMA_   " v[192:195], acc[8:9],   acc[120:121], 0               \n"
    _UK_MFMA_   " v[192:195], acc[10:11], acc[122:123], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[12:13], acc[124:125], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[14:15], acc[126:127], v[192:195]      \n"
    // C = C * scale c
    " s_nop 16                                                          \n"
    " v_mul_f32 %[v_c_15x], v192, v167                                  \n"
    " v_mul_f32 %[v_c_15y], v193, v167                                  \n"
    " v_mul_f32 %[v_c_15z], v194, v167                                  \n"
    " v_mul_f32 %[v_c_15w], v195, v167                                  \n"
    _FF_SYNC_
#pragma endregion
    _FF_END_

#endif

#if 0
#pragma region PREFETCH
    // PREFETCH PING =====================================================================
    // prefetch A (ping) -----------------------------------------------------------------
    // A: vmem->vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " s_waitcnt lgkmcnt(0)                                                              \n"
    " s_barrier                                                                         \n"
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

    // prefetch B (ping) -----------------------------------------------------------------
    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                                    \n"

    // prefetch scale (ping) -------------------------------------------------------------
    // scale a
    " buffer_load_dword    v160, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v161, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v162, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v163, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v164, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v165, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v166, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v167, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale b
    " s_load_dword         s30, s[28:29],  0                                                                \n"
    " s_waitcnt lgkmcnt(0)                                                                                  \n"

    // scale c = scale a * scale b
    " v_mov_b32  v176, s30                                              \n"
    " v_mul_f32  v160, v160, v176                                       \n"
    " v_mul_f32  v161, v161, v176                                       \n"
    " v_mul_f32  v162, v162, v176                                       \n"
    " v_mul_f32  v163, v163, v176                                       \n"
    " v_mul_f32  v164, v164, v176                                       \n"
    " v_mul_f32  v165, v165, v176                                       \n"
    " v_mul_f32  v166, v166, v176                                       \n"
    " v_mul_f32  v167, v167, v176                                       \n"

    // move to pang ----------------------------------------------------------------------
    // move to next A
    "s_cmp_gt_i32  %[s_loop_cnt] 1                                      \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0                               \n"
    "s_add_u32     s16, s86, s16                                        \n"
    "s_addc_u32    s17, 0, s17                                          \n"
    // move to next B
    "s_cmp_gt_i32  %[s_loop_cnt] 1                                      \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0                               \n"
    "s_add_u32     s20, s86, s20                                        \n"
    "s_addc_u32    s21, 0, s21                                          \n"
    // move to next SA
    "s_cmp_gt_i32  %[s_loop_cnt] 1                                      \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0                                 \n"
    "s_add_u32     s24, s86, s24                                        \n"
    "s_addc_u32    s25, 0, s25                                          \n"
    // move to next SB
    "s_cmp_gt_i32  %[s_loop_cnt] 1                                      \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0                                 \n"
    "s_add_u32     s28, s86, s28                                        \n"
    "s_addc_u32    s29, 0, s29                                          \n"
#pragma endregion

"L_start%=:                                                             \n"

#pragma region LOOP_1
    // PREFETCH PANG =====================================================================
    // prefetch A (pang) -----------------------------------------------------------------
    // A: vmem->vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen                                   \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen                                   \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen                                   \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen                                   \n"
    " s_waitcnt vmcnt(0)                                                                                        \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3 + %[n_a_lds_pingpang_sz]   \n"
    " s_waitcnt lgkmcnt(0)                                                                                      \n"
    " s_barrier                                                                                                 \n"
    // A: lds->accvgpr
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

    // prefetch B (pang) -----------------------------------------------------------------
    // B0: vmem->acc
    " buffer_load_dwordx4  acc[16:19], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[20:23], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[24:27], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[28:31], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                                    \n"

    // prefetch scale (pang) -------------------------------------------------------------
    // scale a
    " buffer_load_dword    v168, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v169, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v170, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v171, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v172, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v173, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v174, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v175, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale b
    " s_load_dword         s31, s[28:29],  0                                                                \n"
    " s_waitcnt lgkmcnt(0)                                                                                  \n"

    // scale c = scale a * scale b
    " v_mov_b32  v176, s31                                              \n"
    " v_mul_f32  v168, v168, v176                                       \n"
    " v_mul_f32  v169, v169, v176                                       \n"
    " v_mul_f32  v170, v170, v176                                       \n"
    " v_mul_f32  v171, v171, v176                                       \n"
    " v_mul_f32  v172, v172, v176                                       \n"
    " v_mul_f32  v173, v173, v176                                       \n"
    " v_mul_f32  v174, v174, v176                                       \n"
    " v_mul_f32  v175, v175, v176                                       \n"
    
    // move to ping ----------------------------------------------------------------------
    // move to next A
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0                               \n"
    "s_add_u32     s16, s86, s16                                        \n"
    "s_addc_u32    s17, 0, s17                                          \n"
    // move to next B
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0                               \n"
    "s_add_u32     s20, s86, s20                                        \n"
    "s_addc_u32    s21, 0, s21                                          \n"
    // move to next SA
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0                                 \n"
    "s_add_u32     s24, s86, s24                                        \n"
    "s_addc_u32    s25, 0, s25                                          \n"
    // move to next SB
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0                                 \n"
    "s_add_u32     s28, s86, s28                                        \n"
    "s_addc_u32    s29, 0, s29                                          \n"

    // MFMA PING =========================================================================
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[64:65], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[66:67], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[68:69], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[70:71], v[192:195]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_0x],  v192, v160                                 \n"
    " v_fmac_f32 %[v_c_0y],  v193, v160                                 \n"
    " v_fmac_f32 %[v_c_0z],  v194, v160                                 \n"
    " v_fmac_f32 %[v_c_0w],  v195, v160                                 \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[64:65], 0                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[66:67], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[68:69], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[70:71], v[196:199]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_1x],  v196, v160                                 \n"
    " v_fmac_f32 %[v_c_1y],  v197, v160                                 \n"
    " v_fmac_f32 %[v_c_1z],  v198, v160                                 \n"
    " v_fmac_f32 %[v_c_1w],  v199, v160                                 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[72:73], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[74:75], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[76:77], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[78:79], v[192:195]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_2x],  v192, v161                                 \n"
    " v_fmac_f32 %[v_c_2y],  v193, v161                                 \n"
    " v_fmac_f32 %[v_c_2z],  v194, v161                                 \n"
    " v_fmac_f32 %[v_c_2w],  v195, v161                                 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[72:73], 0                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[74:75], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[76:77], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[78:79], v[196:199]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_3x],  v196, v161                                 \n"
    " v_fmac_f32 %[v_c_3y],  v197, v161                                 \n"
    " v_fmac_f32 %[v_c_3z],  v198, v161                                 \n"
    " v_fmac_f32 %[v_c_3w],  v199, v161                                 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[80:81], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[82:83], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[84:85], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[86:87], v[192:195]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_4x],  v192,v162                                  \n"
    " v_fmac_f32 %[v_c_4y],  v193,v162                                  \n"
    " v_fmac_f32 %[v_c_4z],  v194,v162                                  \n"
    " v_fmac_f32 %[v_c_4w],  v195,v162                                  \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[80:81], 0                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[82:83], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[84:85], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[86:87], v[196:199]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_5x],  v196, v162                                 \n"
    " v_fmac_f32 %[v_c_5y],  v197, v162                                 \n"
    " v_fmac_f32 %[v_c_5z],  v198, v162                                 \n"
    " v_fmac_f32 %[v_c_5w],  v199, v162                                 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[88:89], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[90:91], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[92:93], v[192:195]        \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[94:95], v[192:195]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_6x],  v192, v163                                 \n"
    " v_fmac_f32 %[v_c_6y],  v193, v163                                 \n"
    " v_fmac_f32 %[v_c_6z],  v194, v163                                 \n"
    " v_fmac_f32 %[v_c_6w],  v195, v163                                 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[88:89], 0                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[90:91], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[92:93], v[196:199]        \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[94:95], v[196:199]        \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_7x],  v196, v163                                 \n"
    " v_fmac_f32 %[v_c_7y],  v197, v163                                 \n"
    " v_fmac_f32 %[v_c_7z],  v198, v163                                 \n"
    " v_fmac_f32 %[v_c_7w],  v199, v163                                 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[96:97],   0               \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[98:99],   v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[100:101], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[102:103], v[192:195]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_8x],  v192, v164                                 \n"
    " v_fmac_f32 %[v_c_8y],  v193, v164                                 \n"
    " v_fmac_f32 %[v_c_8z],  v194, v164                                 \n"
    " v_fmac_f32 %[v_c_8w],  v195, v164                                 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[96:97],   0               \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[98:99],   v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[100:101], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[102:103], v[196:199]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_9x],  v196, v164                                 \n"
    " v_fmac_f32 %[v_c_9y],  v197, v164                                 \n"
    " v_fmac_f32 %[v_c_9z],  v198, v164                                 \n"
    " v_fmac_f32 %[v_c_9w],  v199, v164                                 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[104:105], 0               \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[106:107], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[108:109], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[110:111], v[192:195]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_10x], v192, v165                                 \n"
    " v_fmac_f32 %[v_c_10y], v193, v165                                 \n"
    " v_fmac_f32 %[v_c_10z], v194, v165                                 \n"
    " v_fmac_f32 %[v_c_10w], v195, v165                                 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[104:105], 0               \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[106:107], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[108:109], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[110:111], v[196:199]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_11x], v196, v165                                 \n"
    " v_fmac_f32 %[v_c_11y], v197, v165                                 \n"
    " v_fmac_f32 %[v_c_11z], v198, v165                                 \n"
    " v_fmac_f32 %[v_c_11w], v199, v165                                 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[112:113], 0               \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[114:115], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[116:117], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[118:119], v[192:195]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_12x], v192, v166                                 \n"
    " v_fmac_f32 %[v_c_12y], v193, v166                                 \n"
    " v_fmac_f32 %[v_c_12z], v194, v166                                 \n"
    " v_fmac_f32 %[v_c_12w], v195, v166                                 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[112:113], 0               \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[114:115], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[116:117], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[118:119], v[196:199]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_13x], v196, v166                                 \n"
    " v_fmac_f32 %[v_c_13y], v197, v166                                 \n"
    " v_fmac_f32 %[v_c_13z], v198, v166                                 \n"
    " v_fmac_f32 %[v_c_13w], v199, v166                                 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[120:121], 0               \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[122:123], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[124:125], v[192:195]      \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[126:127], v[192:195]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_14x], v192, v167                                 \n"
    " v_fmac_f32 %[v_c_14y], v193, v167                                 \n"
    " v_fmac_f32 %[v_c_14z], v194, v167                                 \n"
    " v_fmac_f32 %[v_c_14w], v195, v167                                 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[120:121], 0               \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[122:123], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[124:125], v[196:199]      \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[126:127], v[196:199]      \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_15x], v196, v167                                 \n"
    " v_fmac_f32 %[v_c_15y], v197, v167                                 \n"
    " v_fmac_f32 %[v_c_15z], v198, v167                                 \n"
    " v_fmac_f32 %[v_c_15w], v199, v167                                 \n"
    // -----------------------------------------------------------------------------------
    "  s_sub_i32     %[s_loop_cnt], %[s_loop_cnt], 1                    \n"
    "  s_cmp_gt_i32  %[s_loop_cnt] 0                                    \n"
    "  s_cbranch_scc0 L_end%=                                           \n"
#pragma endregion

#pragma region LOOP_2
    // PREFETCH PING =====================================================================
    // prefetch A (ping) -----------------------------------------------------------------
    // A: vmem->vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " s_waitcnt lgkmcnt(0)                                                              \n"
    " s_barrier                                                                         \n"
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

    // prefetch B (ping) -----------------------------------------------------------------
    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                                    \n"

    // prefetch scale (ping) -------------------------------------------------------------
    // scale a
    " buffer_load_dword    v160, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v161, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v162, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v163, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v164, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v165, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v166, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v167, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale b
    " s_load_dword         s30, s[28:29],  0                                                                \n"
    " s_waitcnt lgkmcnt(0)                                                                                  \n"

    // scale c = scale a * scale b
    " v_mov_b32  v176, s30                                              \n"
    " v_mul_f32  v160, v160, v176                                       \n"
    " v_mul_f32  v161, v161, v176                                       \n"
    " v_mul_f32  v162, v162, v176                                       \n"
    " v_mul_f32  v163, v163, v176                                       \n"
    " v_mul_f32  v164, v164, v176                                       \n"
    " v_mul_f32  v165, v165, v176                                       \n"
    " v_mul_f32  v166, v166, v176                                       \n"
    " v_mul_f32  v167, v167, v176                                       \n"

    // move to pang ----------------------------------------------------------------------
    // move to next A
    "s_cmp_gt_i32  %[s_loop_cnt] 2                                      \n"
    "s_cselect_b32 s86, %[s_tile_os_a], 0                               \n"
    "s_add_u32     s16, s86, s16                                        \n"
    "s_addc_u32    s17, 0, s17                                          \n"
    // move to next B
    "s_cmp_gt_i32  %[s_loop_cnt] 2                                      \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0                               \n"
    "s_add_u32     s20, s86, s20                                        \n"
    "s_addc_u32    s21, 0, s21                                          \n"
    // move to next SA
    "s_cmp_gt_i32  %[s_loop_cnt] 2                                      \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0                                 \n"
    "s_add_u32     s24, s86, s24                                        \n"
    "s_addc_u32    s25, 0, s25                                          \n"
    // move to next SB
    "s_cmp_gt_i32  %[s_loop_cnt] 2                                      \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0                                 \n"
    "s_add_u32     s28, s86, s28                                        \n"
    "s_addc_u32    s29, 0, s29                                          \n"
    
    // MFMA PANG =========================================================================
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[128:129], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[130:131], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[132:133], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[134:135], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_0x],  v192, v168                                 \n"
    " v_fmac_f32 %[v_c_0y],  v193, v168                                 \n"
    " v_fmac_f32 %[v_c_0z],  v194, v168                                 \n"
    " v_fmac_f32 %[v_c_0w],  v195, v168                                 \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[128:129], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[130:131], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[132:133], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[134:135], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_1x],  v196, v168                                 \n"
    " v_fmac_f32 %[v_c_1y],  v197, v168                                 \n"
    " v_fmac_f32 %[v_c_1z],  v198, v168                                 \n"
    " v_fmac_f32 %[v_c_1w],  v199, v168                                 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[136:137], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[138:139], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[140:141], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[142:143], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_2x],  v192, v169                                 \n"
    " v_fmac_f32 %[v_c_2y],  v193, v169                                 \n"
    " v_fmac_f32 %[v_c_2z],  v194, v169                                 \n"
    " v_fmac_f32 %[v_c_2w],  v195, v169                                 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[136:137], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[138:139], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[140:141], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[142:143], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_3x],  v196, v169                                 \n"
    " v_fmac_f32 %[v_c_3y],  v197, v169                                 \n"
    " v_fmac_f32 %[v_c_3z],  v198, v169                                 \n"
    " v_fmac_f32 %[v_c_3w],  v199, v169                                 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[144:145], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[146:147], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[148:149], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[150:151], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_4x],  v192, v170                                 \n"
    " v_fmac_f32 %[v_c_4y],  v193, v170                                 \n"
    " v_fmac_f32 %[v_c_4z],  v194, v170                                 \n"
    " v_fmac_f32 %[v_c_4w],  v195, v170                                 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[144:145], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[146:147], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[148:149], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[150:151], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_5x],  v196, v170                                 \n"
    " v_fmac_f32 %[v_c_5y],  v197, v170                                 \n"
    " v_fmac_f32 %[v_c_5z],  v198, v170                                 \n"
    " v_fmac_f32 %[v_c_5w],  v199, v170                                 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[152:153], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[154:155], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[156:157], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[158:159], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_6x],  v192, v171                                 \n"
    " v_fmac_f32 %[v_c_6y],  v193, v171                                 \n"
    " v_fmac_f32 %[v_c_6z],  v194, v171                                 \n"
    " v_fmac_f32 %[v_c_6w],  v195, v171                                 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[152:153], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[154:155], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[156:157], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[158:159], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_7x],  v196, v171                                 \n"
    " v_fmac_f32 %[v_c_7y],  v197, v171                                 \n"
    " v_fmac_f32 %[v_c_7z],  v198, v171                                 \n"
    " v_fmac_f32 %[v_c_7w],  v199, v171                                 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[160:161], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[162:163], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[164:165], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[166:167], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_8x],  v192, v172                                 \n"
    " v_fmac_f32 %[v_c_8y],  v193, v172                                 \n"
    " v_fmac_f32 %[v_c_8z],  v194, v172                                 \n"
    " v_fmac_f32 %[v_c_8w],  v195, v172                                 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[160:161], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[162:163], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[164:165], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[166:167], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_9x],  v196, v172                                 \n"
    " v_fmac_f32 %[v_c_9y],  v197, v172                                 \n"
    " v_fmac_f32 %[v_c_9z],  v198, v172                                 \n"
    " v_fmac_f32 %[v_c_9w],  v199, v172                                 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[168:169], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[170:171], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[172:173], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[174:175], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_10x], v192, v173                                 \n"
    " v_fmac_f32 %[v_c_10y], v193, v173                                 \n"
    " v_fmac_f32 %[v_c_10z], v194, v173                                 \n"
    " v_fmac_f32 %[v_c_10w], v195, v173                                 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[168:169], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[170:171], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[172:173], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[174:175], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_11x], v196, v173                                 \n"
    " v_fmac_f32 %[v_c_11y], v197, v173                                 \n"
    " v_fmac_f32 %[v_c_11z], v198, v173                                 \n"
    " v_fmac_f32 %[v_c_11w], v199, v173                                 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[176:177], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[178:179], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[180:181], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[182:183], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_12x], v192, v174                                 \n"
    " v_fmac_f32 %[v_c_12y], v193, v174                                 \n"
    " v_fmac_f32 %[v_c_12z], v194, v174                                 \n"
    " v_fmac_f32 %[v_c_12w], v195, v174                                 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[176:177], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[178:179], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[180:181], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[182:183], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_13x], v196, v174                                 \n"
    " v_fmac_f32 %[v_c_13y], v197, v174                                 \n"
    " v_fmac_f32 %[v_c_13z], v198, v174                                 \n"
    " v_fmac_f32 %[v_c_13w], v199, v174                                 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[184:185], 0             \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[186:187], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[188:189], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[190:191], v[192:195]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_14x], v192, v175                                 \n"
    " v_fmac_f32 %[v_c_14y], v193, v175                                 \n"
    " v_fmac_f32 %[v_c_14z], v194, v175                                 \n"
    " v_fmac_f32 %[v_c_14w], v195, v175                                 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[184:185], 0             \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[186:187], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[188:189], v[196:199]    \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[190:191], v[196:199]    \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_15x], v196, v175                                 \n"
    " v_fmac_f32 %[v_c_15y], v197, v175                                 \n"
    " v_fmac_f32 %[v_c_15z], v198, v175                                 \n"
    " v_fmac_f32 %[v_c_15w], v199, v175                                 \n"
    // -----------------------------------------------------------------------------------
    "  s_sub_i32     %[s_loop_cnt], %[s_loop_cnt], 1                    \n"
    "  s_cmp_gt_i32  %[s_loop_cnt] 0                                    \n"
    "  s_cbranch_scc0 L_end%=                                           \n"
    "  s_branch     L_start%=                                           \n"
#pragma endregion

#endif

#pragma region PREFETCH
    // A0: vmem -> vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // move to next A1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"

    // A1: vmem -> vgpr
    " buffer_load_dwordx4  v[144:147], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[148:151], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[152:155], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[156:159], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // move to next A0
    " s_cmp_gt_i32  %[s_loop_cnt] 2                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"
    // A0: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " s_waitcnt lgkmcnt(0)                                                              \n"
    " s_barrier                                                                         \n"

    // A0: vmem -> vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                                \n"
    // move to next A1
    " s_cmp_gt_i32  %[s_loop_cnt] 3                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"
    // A1: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[144:147]  offset: %[n_a_lds_wr_offset] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[148:151]  offset: %[n_a_lds_wr_offset] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[152:155]  offset: %[n_a_lds_wr_offset] * 2 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[156:159]  offset: %[n_a_lds_wr_offset] * 3 + %[n_a_lds_pingpang_sz]   \n"
    " s_waitcnt lgkmcnt(0)                                                                                      \n"
    " s_barrier                                                                                                 \n"
    // A0: lds->accvgpr
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

    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // move to next B1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0                              \n"
    " s_add_u32     s20, s86, s20                                       \n"
    " s_addc_u32    s21, 0, s21                                         \n"

    // prefetch scale (ping) -------------------------------------------------------------
    // scale a0
    " buffer_load_dword    v160, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v161, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v162, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v163, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v164, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v165, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v166, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v167, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                                    \n"
    // scale b0
    " s_load_dword         s30, s[28:29],  0                                                                \n"
    " s_waitcnt lgkmcnt(0)                                                                                  \n"

    // move to next SA1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0                                \n"
    " s_add_u32     s24, s86, s24                                       \n"
    " s_addc_u32    s25, 0, s25                                         \n"
    // move to next SB1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0                                \n"
    " s_add_u32     s28, s86, s28                                       \n"
    " s_addc_u32    s29, 0, s29                                         \n"
#pragma endregion

"L_start%=:                                                             \n"

#pragma region LOOP_1
#if 0
    // PREFETCH ==========================================================================
    // A1: vmem -> vgpr
    " buffer_load_dwordx4  v[144:147], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[148:151], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[152:155], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[156:159], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                \n" // A1: vmem -> vgpr
    // move to next A0
    " s_cmp_gt_i32  %[s_loop_cnt] 4                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"

    // A0: vgpr -> lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " s_waitcnt lgkmcnt(0)                                              \n" // A0: vgpr -> lds
    " s_barrier                                                         \n" // A0: vgpr -> lds

    // A1: lds -> accvgpr
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
    " s_waitcnt lgkmcnt(0)                                              \n" // A1: lds -> accvgpr

    // B1: vmem -> acc
    " buffer_load_dwordx4  acc[16:19], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[20:23], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[24:27], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[28:31], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                \n" // B1: vmem -> acc
    // move to next B0
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0                               \n"
    "s_add_u32     s20, s86, s20                                        \n"
    "s_addc_u32    s21, 0, s21                                          \n"

    // prefetch scale (pang) -------------------------------------------------------------
    // scale a1
    " buffer_load_dword    v168, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v169, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v170, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v171, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v172, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v173, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v174, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v175, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                \n" // scale a1
    // scale b1
    " s_load_dword         s31, s[28:29],  0                                                                \n"
    " s_waitcnt lgkmcnt(0)                                              \n" // scale b1

    // scale c1 = scale a1 * scale b1
    " v_mov_b32  v176, s31                                              \n"
    " v_mul_f32  v168, v168, v176                                       \n"
    " v_mul_f32  v169, v169, v176                                       \n"
    " v_mul_f32  v170, v170, v176                                       \n"
    " v_mul_f32  v171, v171, v176                                       \n"
    " v_mul_f32  v172, v172, v176                                       \n"
    " v_mul_f32  v173, v173, v176                                       \n"
    " v_mul_f32  v174, v174, v176                                       \n"
    " v_mul_f32  v175, v175, v176                                       \n"

    // move to next SA0
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0                                 \n"
    "s_add_u32     s24, s86, s24                                        \n"
    "s_addc_u32    s25, 0, s25                                          \n"
    // move to next SB0
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0                                 \n"
    "s_add_u32     s28, s86, s28                                        \n"
    "s_addc_u32    s29, 0, s29                                          \n"
#endif
    // MFMA PING =========================================================================
    " s_waitcnt vmcnt(0)                                                \n" // B0: vmem -> acc
    " s_waitcnt lgkmcnt(0)                                              \n" // A0: lds -> accvgpr
    " s_waitcnt vmcnt(0)                                                \n" // scale a0
    " s_waitcnt lgkmcnt(0)                                              \n" // scale b0

    // scale c0 = scale a0 * scale b0
    " v_mov_b32  v176, s30                                              \n"
    " v_mul_f32  v160, v160, v176                                       \n"
    " v_mul_f32  v161, v161, v176                                       \n"
    " v_mul_f32  v162, v162, v176                                       \n"
    " v_mul_f32  v163, v163, v176                                       \n"
    " v_mul_f32  v164, v164, v176                                       \n"
    " v_mul_f32  v165, v165, v176                                       \n"
    " v_mul_f32  v166, v166, v176                                       \n"
    " v_mul_f32  v167, v167, v176                                       \n"

    // scale b1 (lgkmcnt) ----------------------------------------------------------------
    " s_load_dword         s31, s[28:29],  0                            \n"

    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[64:65], 0                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[66:67], v[192:195]        \n"
    // A0: vgpr -> lds (lgkmcnt)
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0    \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[68:69], v[192:195]        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1    \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[70:71], v[192:195]        \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[64:65], 0                 \n"
    " v_fmac_f32 %[v_c_0x],  v192, v160                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[66:67], v[196:199]        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2    \n"
    " v_fmac_f32 %[v_c_0y],  v193, v160                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[68:69], v[196:199]        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3    \n"
    " v_fmac_f32 %[v_c_0z],  v194, v160                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[70:71], v[196:199]        \n"
    " v_fmac_f32 %[v_c_0w],  v195, v160                                 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[72:73], 0                 \n"
    // A1: lds -> accvgpr (lgkmcnt)
    " s_waitcnt lgkmcnt(5)                                              \n" // A1: vgpr -> lds
    " s_barrier                                                         \n" // A1: vgpr -> lds
    " ds_read_b128  acc[128:131], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [0~15]M    * [0~63]K
    " v_fmac_f32 %[v_c_1x],  v196, v160                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[74:75], v[192:195]        \n"
    " v_fmac_f32 %[v_c_1y],  v197, v160                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[76:77], v[192:195]        \n"
    " ds_read_b128  acc[132:135], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [0~15]M    * [64~127]K
    " v_fmac_f32 %[v_c_1z],  v198, v160                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[78:79], v[192:195]        \n"
    " v_fmac_f32 %[v_c_1w],  v199, v160                                 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[72:73], 0                 \n"
    " ds_read_b128  acc[136:139], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [16~31]M   * [0~63]K
    " v_fmac_f32 %[v_c_2x],  v192, v161                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[74:75], v[196:199]        \n"
    " v_fmac_f32 %[v_c_2y],  v193, v161                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[76:77], v[196:199]        \n"
    " ds_read_b128  acc[140:143], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [16~31]M   * [64~127]K
    " v_fmac_f32 %[v_c_2z],  v194, v161                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[78:79], v[196:199]        \n"
    " v_fmac_f32 %[v_c_2w],  v195, v161                                 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[80:81], 0                 \n"
    " ds_read_b128  acc[144:147], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [32~47]M   * [0~63]K
    " v_fmac_f32 %[v_c_3x],  v196, v161                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[82:83], v[192:195]        \n"
    " v_fmac_f32 %[v_c_3y],  v197, v161                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[84:85], v[192:195]        \n"
    " v_fmac_f32 %[v_c_3z],  v198, v161                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[86:87], v[192:195]        \n"
    " v_fmac_f32 %[v_c_3w],  v199, v161                                 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[80:81], 0                 \n"
    " ds_read_b128  acc[148:151], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [32~47]M   * [64~127]K
    " v_fmac_f32 %[v_c_4x],  v192,v162                                  \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[82:83], v[196:199]        \n"
    " v_fmac_f32 %[v_c_4y],  v193,v162                                  \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[84:85], v[196:199]        \n"
    " v_fmac_f32 %[v_c_4z],  v194,v162                                  \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[86:87], v[196:199]        \n"
    " v_fmac_f32 %[v_c_4w],  v195,v162                                  \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[88:89], 0                 \n"
    " ds_read_b128  acc[152:155], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [48~63]M   * [0~63]K
    " v_fmac_f32 %[v_c_5x],  v196, v162                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[90:91], v[192:195]        \n"
    " v_fmac_f32 %[v_c_5y],  v197, v162                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[92:93], v[192:195]        \n"
    " v_fmac_f32 %[v_c_5z],  v198, v162                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[94:95], v[192:195]        \n"
    " v_fmac_f32 %[v_c_5w],  v199, v162                                 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[88:89], 0                 \n"
    " ds_read_b128  acc[156:159], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [48~63]M   * [64~127]K
    " v_fmac_f32 %[v_c_6x],  v192, v163                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[90:91], v[196:199]        \n"
    " v_fmac_f32 %[v_c_6y],  v193, v163                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[92:93], v[196:199]        \n"
    " v_fmac_f32 %[v_c_6z],  v194, v163                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[94:95], v[196:199]        \n"
    " v_fmac_f32 %[v_c_6w],  v195, v163                                 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[96:97],   0               \n"
    " ds_read_b128  acc[160:163], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [64~79]M   * [0~63]K
    " v_fmac_f32 %[v_c_7x],  v196, v163                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[98:99],   v[192:195]      \n"
    " v_fmac_f32 %[v_c_7y],  v197, v163                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[100:101], v[192:195]      \n"
    " v_fmac_f32 %[v_c_7z],  v198, v163                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[102:103], v[192:195]      \n"
    " v_fmac_f32 %[v_c_7w],  v199, v163                                 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[96:97],   0               \n"
    " ds_read_b128  acc[164:167], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [64~79]M   * [64~127]K
    " v_fmac_f32 %[v_c_8x],  v192, v164                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[98:99],   v[196:199]      \n"
    " v_fmac_f32 %[v_c_8y],  v193, v164                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[100:101], v[196:199]      \n"
    " v_fmac_f32 %[v_c_8z],  v194, v164                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[102:103], v[196:199]      \n"
    " v_fmac_f32 %[v_c_8w],  v195, v164                                 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[104:105], 0               \n"
    " ds_read_b128  acc[168:171], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [80~95]M   * [0~63]K
    " v_fmac_f32 %[v_c_9x],  v196, v164                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[106:107], v[192:195]      \n"
    " v_fmac_f32 %[v_c_9y],  v197, v164                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[108:109], v[192:195]      \n"
    " v_fmac_f32 %[v_c_9z],  v198, v164                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[110:111], v[192:195]      \n"
    " v_fmac_f32 %[v_c_9w],  v199, v164                                 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[104:105], 0               \n"
    " ds_read_b128  acc[172:175], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [80~95]M   * [64~127]K
    " v_fmac_f32 %[v_c_10x], v192, v165                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[106:107], v[196:199]      \n"
    " v_fmac_f32 %[v_c_10y], v193, v165                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[108:109], v[196:199]      \n"
    " v_fmac_f32 %[v_c_10z], v194, v165                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[110:111], v[196:199]      \n"
    " v_fmac_f32 %[v_c_10w], v195, v165                                 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[112:113], 0               \n"
    " ds_read_b128  acc[176:179], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [96~111]M  * [0~63]K
    " v_fmac_f32 %[v_c_11x], v196, v165                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[114:115], v[192:195]      \n"
    " v_fmac_f32 %[v_c_11y], v197, v165                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[116:117], v[192:195]      \n"
    " v_fmac_f32 %[v_c_11z], v198, v165                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[118:119], v[192:195]      \n"
    " v_fmac_f32 %[v_c_11w], v199, v165                                 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[112:113], 0               \n"
    " ds_read_b128  acc[180:183], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [96~111]M  * [64~128]K
    " v_fmac_f32 %[v_c_12x], v192, v166                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[114:115], v[196:199]      \n"
    " v_fmac_f32 %[v_c_12y], v193, v166                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[116:117], v[196:199]      \n"
    " v_fmac_f32 %[v_c_12z], v194, v166                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[118:119], v[196:199]      \n"
    " v_fmac_f32 %[v_c_12w], v195, v166                                 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[0:1],   acc[120:121], 0               \n"
    " ds_read_b128  acc[184:187], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0 + %[n_a_lds_pingpang_sz]  \n" // [112~127]M * [0~63]K
    " v_fmac_f32 %[v_c_13x], v196, v166                                 \n"
    _UK_MFMA_   " v[192:195], acc[2:3],   acc[122:123], v[192:195]      \n"
    " v_fmac_f32 %[v_c_13y], v197, v166                                 \n"
    _UK_MFMA_   " v[192:195], acc[4:5],   acc[124:125], v[192:195]      \n"
    " v_fmac_f32 %[v_c_13z], v198, v166                                 \n"
    _UK_MFMA_   " v[192:195], acc[6:7],   acc[126:127], v[192:195]      \n"
    " v_fmac_f32 %[v_c_13w], v199, v166                                 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[8:9],   acc[120:121], 0               \n"
    " ds_read_b128  acc[188:191], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1 + %[n_a_lds_pingpang_sz]  \n" // [112~127]M * [64~127]K
    " v_fmac_f32 %[v_c_14x], v192, v167                                 \n"
    _UK_MFMA_   " v[196:199], acc[10:11], acc[122:123], v[196:199]      \n"
    " v_fmac_f32 %[v_c_14y], v193, v167                                 \n"
    _UK_MFMA_   " v[196:199], acc[12:13], acc[124:125], v[196:199]      \n"
    " v_fmac_f32 %[v_c_14z], v194, v167                                 \n"
    _UK_MFMA_   " v[196:199], acc[14:15], acc[126:127], v[196:199]      \n"
    " v_fmac_f32 %[v_c_14w], v195, v167                                 \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_15x], v196, v167                                 \n"
    " v_fmac_f32 %[v_c_15y], v197, v167                                 \n"
    " v_fmac_f32 %[v_c_15z], v198, v167                                 \n"
    " v_fmac_f32 %[v_c_15w], v199, v167                                 \n"

    // B1: vmem -> acc
    " buffer_load_dwordx4  acc[16:19], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[20:23], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[24:27], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[28:31], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    // A1: vmem -> vgpr
    " buffer_load_dwordx4  v[144:147], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[148:151], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[152:155], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[156:159], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    // prefetch scale (pang) -------------------------------------------------------------
    // scale a1
    " buffer_load_dword    v168, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v169, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v170, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v171, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v172, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v173, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v174, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v175, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    // -----------------------------------------------------------------------------------
    // for loop
    "  s_sub_i32     %[s_loop_cnt], %[s_loop_cnt], 1                    \n"
    "  s_cmp_gt_i32  %[s_loop_cnt] 0                                    \n"
    "  s_cbranch_scc0 L_end%=                                           \n"
    // move to next A0
    " s_cmp_gt_i32  %[s_loop_cnt] 4                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"
    // move to next B0
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_os_b], 0                               \n"
    "s_add_u32     s20, s86, s20                                        \n"
    "s_addc_u32    s21, 0, s21                                          \n"
    // move to next SA0
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_sa], 0                                 \n"
    "s_add_u32     s24, s86, s24                                        \n"
    "s_addc_u32    s25, 0, s25                                          \n"
    // move to next SB0
    "  s_cmp_gt_i32  %[s_loop_cnt] 2                                    \n"
    "s_cselect_b32 s86, %[s_tile_sb], 0                                 \n"
    "s_add_u32     s28, s86, s28                                        \n"
    "s_addc_u32    s29, 0, s29                                          \n"
#pragma endregion

#pragma region LOOP_2
#if 0
    // PREFETCH ==========================================================================
    // A0: vmem -> vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    " s_waitcnt vmcnt(0)                                                \n" // A0: vmem -> vgpr
    // move to next A1
    " s_cmp_gt_i32  %[s_loop_cnt] 4                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"

    // A1: vgpr -> lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[144:147]  offset: %[n_a_lds_wr_offset] * 0 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[148:151]  offset: %[n_a_lds_wr_offset] * 1 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[152:155]  offset: %[n_a_lds_wr_offset] * 2 + %[n_a_lds_pingpang_sz]   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[156:159]  offset: %[n_a_lds_wr_offset] * 3 + %[n_a_lds_pingpang_sz]   \n"
    " s_waitcnt lgkmcnt(0)                                              \n" // A1: vgpr -> lds
    " s_barrier                                                         \n" // A1: vgpr -> lds

    // A0: lds -> accvgpr
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
    " s_waitcnt lgkmcnt(0)                                              \n" // A0: lds -> accvgpr

    // B0: vmem -> acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                \n" // B0: vmem -> acc
    // move to next B1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0                              \n"
    " s_add_u32     s20, s86, s20                                       \n"
    " s_addc_u32    s21, 0, s21                                         \n"

    // prefetch scale (ping) -------------------------------------------------------------
    // scale a0
    " buffer_load_dword    v160, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v161, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v162, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v163, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v164, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v165, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v166, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v167, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                \n" // scale a0
    // scale b0
    " s_load_dword         s30, s[28:29],  0                                                                \n"
    " s_waitcnt lgkmcnt(0)                                              \n" // scale b0

    // scale c0 = scale a0 * scale b0
    " v_mov_b32  v176, s30                                              \n"
    " v_mul_f32  v160, v160, v176                                       \n"
    " v_mul_f32  v161, v161, v176                                       \n"
    " v_mul_f32  v162, v162, v176                                       \n"
    " v_mul_f32  v163, v163, v176                                       \n"
    " v_mul_f32  v164, v164, v176                                       \n"
    " v_mul_f32  v165, v165, v176                                       \n"
    " v_mul_f32  v166, v166, v176                                       \n"
    " v_mul_f32  v167, v167, v176                                       \n"
    // move to next SA1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0                                \n"
    " s_add_u32     s24, s86, s24                                       \n"
    " s_addc_u32    s25, 0, s25                                         \n"
    // move to next SB1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0                                \n"
    " s_add_u32     s28, s86, s28                                       \n"
    " s_addc_u32    s29, 0, s29                                         \n"
#endif
    // MFMA PANG =========================================================================
    " s_waitcnt vmcnt(0)                                                \n" // B1: vmem -> acc
    " s_waitcnt lgkmcnt(0)                                              \n" // A1: lds -> accvgpr
    " s_waitcnt vmcnt(0)                                                \n" // scale a1
    " s_waitcnt lgkmcnt(0)                                              \n" // scale b1

    // scale c1 = scale a1 * scale b1
    " s_waitcnt vmcnt(0)                                                \n" // a1
    " v_mov_b32  v176, s31                                              \n"
    " v_mul_f32  v168, v168, v176                                       \n"
    " v_mul_f32  v169, v169, v176                                       \n"
    " v_mul_f32  v170, v170, v176                                       \n"
    " v_mul_f32  v171, v171, v176                                       \n"
    " v_mul_f32  v172, v172, v176                                       \n"
    " v_mul_f32  v173, v173, v176                                       \n"
    " v_mul_f32  v174, v174, v176                                       \n"
    " v_mul_f32  v175, v175, v176                                       \n"

    // scale b0 (lgkmcnt) -------------------------------------------------------------
    " s_load_dword         s30, s[28:29],  0                            \n"
    
    // C: [0~15]M * [0~63]N --------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[128:129], 0             \n"
    // A1: vgpr -> lds (lgkmcnt)
    " ds_write_b128 %[v_a_lds_wr_addr], v[144:147]  offset: %[n_a_lds_wr_offset] * 0 + %[n_a_lds_pingpang_sz]   \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[130:131], v[192:195]    \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[132:133], v[192:195]    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[148:151]  offset: %[n_a_lds_wr_offset] * 1 + %[n_a_lds_pingpang_sz]   \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[134:135], v[192:195]    \n"
    // C: [0~15]M   * [64~127]N  ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[128:129], 0             \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[152:155]  offset: %[n_a_lds_wr_offset] * 2 + %[n_a_lds_pingpang_sz]   \n"
    " v_fmac_f32 %[v_c_0x],  v192, v168                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[130:131], v[196:199]    \n"
    " v_fmac_f32 %[v_c_0y],  v193, v168                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[132:133], v[196:199]    \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[156:159]  offset: %[n_a_lds_wr_offset] * 3 + %[n_a_lds_pingpang_sz]   \n"
    " v_fmac_f32 %[v_c_0z],  v194, v168                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[134:135], v[196:199]    \n"
    " v_fmac_f32 %[v_c_0w],  v195, v168                                 \n"
    // C: [16~31]M * [0~63]N -------------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[136:137], 0             \n"
    // A0: lds -> accvgpr (lgkmcnt)
    " s_waitcnt lgkmcnt(5)                                              \n" // A0: vgpr -> lds
    " s_barrier                                                         \n" // A0: vgpr -> lds
    " ds_read_b128  acc[64:67],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0   \n" // [0~15]M    * [0~63]K
    " v_fmac_f32 %[v_c_1x],  v196, v168                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[138:139], v[192:195]    \n"
    " v_fmac_f32 %[v_c_1y],  v197, v168                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[140:141], v[192:195]    \n"
    " ds_read_b128  acc[68:71],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1   \n" // [0~15]M    * [64~127]K
    " v_fmac_f32 %[v_c_1z],  v198, v168                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[142:143], v[192:195]    \n"
    " v_fmac_f32 %[v_c_1w],  v199, v168                                 \n"
    // C: [16~31]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[136:137], 0             \n"
    " ds_read_b128  acc[72:75],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0   \n" // [16~31]M   * [0~63]K
    " v_fmac_f32 %[v_c_2x],  v192, v169                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[138:139], v[196:199]    \n"
    " v_fmac_f32 %[v_c_2y],  v193, v169                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[140:141], v[196:199]    \n"
    " ds_read_b128  acc[76:79],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1   \n" // [16~31]M   * [64~127]K
    " v_fmac_f32 %[v_c_2z],  v194, v169                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[142:143], v[196:199]    \n"
    " v_fmac_f32 %[v_c_2w],  v195, v169                                 \n"
    // C: [32~47]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[144:145], 0             \n"
    " ds_read_b128  acc[80:83],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0   \n" // [32~47]M   * [0~63]K
    " v_fmac_f32 %[v_c_3x],  v196, v169                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[146:147], v[192:195]    \n"
    " v_fmac_f32 %[v_c_3y],  v197, v169                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[148:149], v[192:195]    \n"
    " v_fmac_f32 %[v_c_3z],  v198, v169                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[150:151], v[192:195]    \n"
    " v_fmac_f32 %[v_c_3w],  v199, v169                                 \n"
    // C: [32~47]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[144:145], 0             \n"
    " ds_read_b128  acc[84:87],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1   \n" // [32~47]M   * [64~127]K
    " v_fmac_f32 %[v_c_4x],  v192, v170                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[146:147], v[196:199]    \n"
    " v_fmac_f32 %[v_c_4y],  v193, v170                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[148:149], v[196:199]    \n"
    " v_fmac_f32 %[v_c_4z],  v194, v170                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[150:151], v[196:199]    \n"
    " v_fmac_f32 %[v_c_4w],  v195, v170                                 \n"
    // C: [48~63]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[152:153], 0             \n"
    " ds_read_b128  acc[88:91],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0   \n" // [48~63]M   * [0~63]K
    " v_fmac_f32 %[v_c_5x],  v196, v170                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[154:155], v[192:195]    \n"
    " v_fmac_f32 %[v_c_5y],  v197, v170                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[156:157], v[192:195]    \n"
    " v_fmac_f32 %[v_c_5z],  v198, v170                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[158:159], v[192:195]    \n"
    " v_fmac_f32 %[v_c_5w],  v199, v170                                 \n"
    // C: [48~63]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[152:153], 0             \n"
    " ds_read_b128  acc[92:95],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1   \n" // [48~63]M   * [64~127]K
    " v_fmac_f32 %[v_c_6x],  v192, v171                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[154:155], v[196:199]    \n"
    " v_fmac_f32 %[v_c_6y],  v193, v171                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[156:157], v[196:199]    \n"
    " v_fmac_f32 %[v_c_6z],  v194, v171                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[158:159], v[196:199]    \n"
    " v_fmac_f32 %[v_c_6w],  v195, v171                                 \n"
    // C: [64~79]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[160:161], 0             \n"
    " ds_read_b128  acc[96:99],   %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0   \n" // [64~79]M   * [0~63]K
    " v_fmac_f32 %[v_c_7x],  v196, v171                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[162:163], v[192:195]    \n"
    " v_fmac_f32 %[v_c_7y],  v197, v171                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[164:165], v[192:195]    \n"
    " v_fmac_f32 %[v_c_7z],  v198, v171                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[166:167], v[192:195]    \n"
    " v_fmac_f32 %[v_c_7w],  v199, v171                                 \n"
    // C: [64~79]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[160:161], 0             \n"
    " ds_read_b128  acc[100:103], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1   \n" // [64~79]M   * [64~127]K
    " v_fmac_f32 %[v_c_8x],  v192, v172                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[162:163], v[196:199]    \n"
    " v_fmac_f32 %[v_c_8y],  v193, v172                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[164:165], v[196:199]    \n"
    " v_fmac_f32 %[v_c_8z],  v194, v172                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[166:167], v[196:199]    \n"
    " v_fmac_f32 %[v_c_8w],  v195, v172                                 \n"
    // C: [80~95]M   * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[168:169], 0             \n"
    " ds_read_b128  acc[104:107], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0   \n" // [80~95]M   * [0~63]K
    " v_fmac_f32 %[v_c_9x],  v196, v172                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[170:171], v[192:195]    \n"
    " v_fmac_f32 %[v_c_9y],  v197, v172                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[172:173], v[192:195]    \n"
    " v_fmac_f32 %[v_c_9z],  v198, v172                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[174:175], v[192:195]    \n"
    " v_fmac_f32 %[v_c_9w],  v199, v172                                 \n"
    // C: [80~95]M   * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[168:169], 0             \n"
    " ds_read_b128  acc[108:111], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1   \n" // [80~95]M   * [64~127]K
    " v_fmac_f32 %[v_c_10x], v192, v173                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[170:171], v[196:199]    \n"
    " v_fmac_f32 %[v_c_10y], v193, v173                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[172:173], v[196:199]    \n"
    " v_fmac_f32 %[v_c_10z], v194, v173                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[174:175], v[196:199]    \n"
    " v_fmac_f32 %[v_c_10w], v195, v173                                 \n"
    // C: [96~111]M  * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[176:177], 0             \n"
    " ds_read_b128  acc[112:115], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0   \n" // [96~111]M  * [0~63]K
    " v_fmac_f32 %[v_c_11x], v196, v173                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[178:179], v[192:195]    \n"
    " v_fmac_f32 %[v_c_11y], v197, v173                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[180:181], v[192:195]    \n"
    " v_fmac_f32 %[v_c_11z], v198, v173                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[182:183], v[192:195]    \n"
    " v_fmac_f32 %[v_c_11w], v199, v173                                 \n"
    // C: [96~111]M  * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[176:177], 0             \n"
    " ds_read_b128  acc[116:119], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1   \n" // [96~111]M  * [64~128]K
    " v_fmac_f32 %[v_c_12x], v192, v174                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[178:179], v[196:199]    \n"
    " v_fmac_f32 %[v_c_12y], v193, v174                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[180:181], v[196:199]    \n"
    " v_fmac_f32 %[v_c_12z], v194, v174                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[182:183], v[196:199]    \n"
    " v_fmac_f32 %[v_c_12w], v195, v174                                 \n"
    // C: [112~127]M * [0~63]N -----------------------------------------------------------
    _UK_MFMA_   " v[192:195], acc[16:17],   acc[184:185], 0             \n"
    " ds_read_b128  acc[120:123], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0   \n" // [112~127]M * [0~63]K
    " v_fmac_f32 %[v_c_13x], v196, v174                                 \n"
    _UK_MFMA_   " v[192:195], acc[18:19],   acc[186:187], v[192:195]    \n"
    " v_fmac_f32 %[v_c_13y], v197, v174                                 \n"
    _UK_MFMA_   " v[192:195], acc[20:21],   acc[188:189], v[192:195]    \n"
    " v_fmac_f32 %[v_c_13z], v198, v174                                 \n"
    _UK_MFMA_   " v[192:195], acc[22:23],   acc[190:191], v[192:195]    \n"
    " v_fmac_f32 %[v_c_13w], v199, v174                                 \n"
    // C: [112~127]M * [64~127]N ---------------------------------------------------------
    _UK_MFMA_   " v[196:199], acc[24:25],   acc[184:185], 0             \n"
    " ds_read_b128  acc[124:127], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1   \n" // [112~127]M * [64~127]K
    " v_fmac_f32 %[v_c_14x], v192, v175                                 \n"
    _UK_MFMA_   " v[196:199], acc[26:27],   acc[186:187], v[196:199]    \n"
    " v_fmac_f32 %[v_c_14y], v193, v175                                 \n"
    _UK_MFMA_   " v[196:199], acc[28:29],   acc[188:189], v[196:199]    \n"
    " v_fmac_f32 %[v_c_14z], v194, v175                                 \n"
    _UK_MFMA_   " v[196:199], acc[30:31],   acc[190:191], v[196:199]    \n"
    " v_fmac_f32 %[v_c_14w], v195, v175                                 \n"
    " s_nop 16                                                          \n"
    " v_fmac_f32 %[v_c_15x], v196, v175                                 \n"
    " v_fmac_f32 %[v_c_15y], v197, v175                                 \n"
    " v_fmac_f32 %[v_c_15z], v198, v175                                 \n"
    " v_fmac_f32 %[v_c_15w], v199, v175                                 \n"

    // B0: vmem -> acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0       \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1       \n" // [64~127]N * [64~127]K
    // A0: vmem -> vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen           \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen           \n"
    // prefetch scale (ping) -------------------------------------------------------------
    // scale a0
    " buffer_load_dword    v160, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0    \n" // [0~15]M
    " buffer_load_dword    v161, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1    \n" // [16~31]M
    " buffer_load_dword    v162, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2    \n" // [32~47]M
    " buffer_load_dword    v163, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3    \n" // [48~63]M
    " buffer_load_dword    v164, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4    \n" // [64~79]M
    " buffer_load_dword    v165, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5    \n" // [80~95]M
    " buffer_load_dword    v166, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6    \n" // [96~111]M
    " buffer_load_dword    v167, %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7    \n" // [112~127]M
    // -----------------------------------------------------------------------------------
    // move to next A1
    " s_cmp_gt_i32  %[s_loop_cnt] 4                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_a], 0                              \n"
    " s_add_u32     s16, s86, s16                                       \n"
    " s_addc_u32    s17, 0, s17                                         \n"
    // move to next B1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_os_b], 0                              \n"
    " s_add_u32     s20, s86, s20                                       \n"
    " s_addc_u32    s21, 0, s21                                         \n"
    // move to next SA1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_sa], 0                                \n"
    " s_add_u32     s24, s86, s24                                       \n"
    " s_addc_u32    s25, 0, s25                                         \n"
    // move to next SB1
    " s_cmp_gt_i32  %[s_loop_cnt] 1                                     \n"
    " s_cselect_b32 s86, %[s_tile_sb], 0                                \n"
    " s_add_u32     s28, s86, s28                                       \n"
    " s_addc_u32    s29, 0, s29                                         \n"
    // forloop
    "  s_sub_i32     %[s_loop_cnt], %[s_loop_cnt], 1                    \n"
    "  s_cmp_gt_i32  %[s_loop_cnt] 0                                    \n"
    "  s_cbranch_scc0 L_end%=                                           \n"
    "  s_branch     L_start%=                                           \n"
#pragma endregion

#pragma region LOOP_OUT_1
"L_loopout1%=:                                                          \n"

#pragma endregion

#pragma region LOOP_OUT_2
"L_loopout2%=:                                                          \n"

#pragma endregion

"L_end%=:                                                               \n"

#undef _UK_MFMA_
#undef _FF_SYNC_
