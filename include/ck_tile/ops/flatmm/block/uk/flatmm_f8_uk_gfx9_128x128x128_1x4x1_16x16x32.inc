// acc[64..127] : A
// acc[0..15]   : B
// v[64..127]   : Acc result, float4 c[0..16]

#define _UK_MFMA_ "v_mfma_f32_16x16x32_fp8_fp8"

#define _FF_SYNC_ " s_waitcnt vmcnt(0) \n" \
                  " s_waitcnt lgkmcnt(0) \n" \
                  " s_barrier \n" \
                  " s_nop 16 \n"

    "s_mov_b32 s16,    %[s_res_a0] \n"
    "s_mov_b32 s17,    %[s_res_a1] \n"
    "s_mov_b32 s18,    %[s_res_a2] \n"
    "s_mov_b32 s19,    %[s_res_a3] \n"
    "s_mov_b32 s20,    %[s_res_b0] \n"
    "s_mov_b32 s21,    %[s_res_b1] \n"
    "s_mov_b32 s22,    %[s_res_b2] \n"
    "s_mov_b32 s23,    %[s_res_b3] \n"

    "s_mov_b32 s24,    %[s_res_sa0] \n"
    "s_mov_b32 s25,    %[s_res_sa1] \n"
    "s_mov_b32 s26,    %[s_res_sa2] \n"
    "s_mov_b32 s27,    %[s_res_sa3] \n"
    "s_mov_b32 s28,    %[s_base_sb_l] \n"
    "s_mov_b32 s29,    %[s_base_sb_h] \n"
    
#if 1
    // ===================================================================================
    _FF_SYNC_
    // debug A vmem ------------------------------------------------------
    " buffer_load_dwordx4  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_vm_ld_addr0],  s[16:19], 0 offen          \n"
    " buffer_load_dwordx4  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_vm_ld_addr1],  s[16:19], 0 offen          \n"
    " buffer_load_dwordx4  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_vm_ld_addr2],  s[16:19], 0 offen          \n"
    " buffer_load_dwordx4  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_vm_ld_addr3],  s[16:19], 0 offen          \n"
    _FF_SYNC_
    // debug A lds write ------------------------------------------------
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]]  offset: %[n_a_lds_wr_offset] * 0   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]]  offset: %[n_a_lds_wr_offset] * 1   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]]  offset: %[n_a_lds_wr_offset] * 2   \n"
    " ds_write_b128 %[v_a_lds_wr_addr], [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]]  offset: %[n_a_lds_wr_offset] * 3   \n"
    _FF_SYNC_
    // debug A lds read 0/2/4/6/8/10/12/14/16 ---------------------------
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 0    \n"
    " ds_read_b128  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 0    \n"
    _FF_SYNC_
    // debug A lds read 1/3/5/7/9/11/13/15/17 ---------------------------
    " ds_read_b128  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 0 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 1 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 2 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 3 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_4x], %[v_c_4y], %[v_c_4z], %[v_c_4w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 4 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_5x], %[v_c_5y], %[v_c_5z], %[v_c_5w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 5 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_6x], %[v_c_6y], %[v_c_6z], %[v_c_6w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 6 + %[n_a_lds_rd_offset_0] * 1    \n"
    " ds_read_b128  [%[v_c_7x], %[v_c_7y], %[v_c_7z], %[v_c_7w]], %[v_a_lds_rd_addr]  offset: %[n_a_lds_rd_offset_1] * 7 + %[n_a_lds_rd_offset_0] * 1    \n"
    _FF_SYNC_
    // debug B vmem -----------------------------------------------------
    " buffer_load_dwordx4  [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0                  \n"
    " buffer_load_dwordx4  [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1                  \n"
    " buffer_load_dwordx4  [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0                  \n"
    " buffer_load_dwordx4  [%[v_c_3x], %[v_c_3y], %[v_c_3z], %[v_c_3w]], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1                  \n"
    _FF_SYNC_
    // debug scale A vmem -----------------------------------------------
    " buffer_load_dword    %[v_c_0x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0          \n"
    " buffer_load_dword    %[v_c_0y], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1          \n"
    " buffer_load_dword    %[v_c_0z], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2          \n"
    " buffer_load_dword    %[v_c_0w], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3          \n"
    " buffer_load_dword    %[v_c_1x], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4          \n"
    " buffer_load_dword    %[v_c_1y], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5          \n"
    " buffer_load_dword    %[v_c_1z], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6          \n"
    " buffer_load_dword    %[v_c_1w], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7          \n"
    _FF_SYNC_
    // debug scale B vmem ---------------------------------------------
    " s_load_dword         %[s_sb], s[28:29],  0                                            \n"
    " s_waitcnt lgkmcnt(0)                                                                  \n"
    " v_mov_b32            %[v_c_2x], %[s_sb]                                               \n"
    _FF_SYNC_
    // debug mfma ========================================================================
    _FF_SYNC_
    // init to 0
    "v_accvgpr_write    acc0,    0                                                          \n"
    "v_accvgpr_write    acc1,    0                                                          \n"
    "v_accvgpr_write    acc2,    0                                                          \n"
    "v_accvgpr_write    acc3,    0                                                          \n"
    "v_mov_b32          v128,    0                                                          \n"
    "v_mov_b32          v129,    0                                                          \n"
    "v_mov_b32          v130,    0                                                          \n"
    "v_mov_b32          v131,    0                                                          \n"
    _FF_SYNC_
    _UK_MFMA_   " [%[v_c_0x],  %[v_c_0y],  %[v_c_0z],  %[v_c_0w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_1x],  %[v_c_1y],  %[v_c_1z],  %[v_c_1w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_2x],  %[v_c_2y],  %[v_c_2z],  %[v_c_2w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_3x],  %[v_c_3y],  %[v_c_3z],  %[v_c_3w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_4x],  %[v_c_4y],  %[v_c_4z],  %[v_c_4w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_5x],  %[v_c_5y],  %[v_c_5z],  %[v_c_5w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_6x],  %[v_c_6y],  %[v_c_6z],  %[v_c_6w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_7x],  %[v_c_7y],  %[v_c_7z],  %[v_c_7w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_8x],  %[v_c_8y],  %[v_c_8z],  %[v_c_8w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_9x],  %[v_c_9y],  %[v_c_9z],  %[v_c_9w]],  acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_10x], %[v_c_10y], %[v_c_10z], %[v_c_10w]], acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_11x], %[v_c_11y], %[v_c_11z], %[v_c_11w]], acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_12x], %[v_c_12y], %[v_c_12z], %[v_c_12w]], acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_13x], %[v_c_13y], %[v_c_13z], %[v_c_13w]], acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_14x], %[v_c_14y], %[v_c_14z], %[v_c_14w]], acc[0:1],  acc[2:3], v[128:131]                     \n"
    _UK_MFMA_   " [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]], acc[0:1],  acc[2:3], v[128:131]                     \n"
    _FF_SYNC_
    // A: vmem->vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen               \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen               \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen               \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen               \n"
    " s_waitcnt vmcnt(0)                                                                    \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3        \n"
    " s_waitcnt lgkmcnt(0)                                                                  \n"
    " s_barrier                                                                             \n"
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
    " s_waitcnt lgkmcnt(0)                                                                  \n"
    // B: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0                   \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1                   \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0                   \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1                   \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                    \n"
    // scale A: vmem->vgpr 
    " buffer_load_dword    %[v_sa_0], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0           \n" // [0~15]M
    " buffer_load_dword    %[v_sa_1], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1           \n" // [16~31]M
    " buffer_load_dword    %[v_sa_2], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2           \n" // [32~47]M
    " buffer_load_dword    %[v_sa_3], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3           \n" // [48~63]M
    " buffer_load_dword    %[v_sa_4], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4           \n" // [64~79]M
    " buffer_load_dword    %[v_sa_5], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5           \n" // [80~95]M
    " buffer_load_dword    %[v_sa_6], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6           \n" // [96~111]M
    " buffer_load_dword    %[v_sa_7], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7           \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                    \n"
    // scale B: vmem->sgpr->vgpr 
    " s_load_dword         %[s_sb], s[28:29],  0                                            \n"
    " s_waitcnt vmcnt(0)                                                                    \n"
    " v_mov_b32            %[v_sb], %[s_sb]                                                 \n"
    // scale c = scale a * scale b
    " v_mul_f32  %[v_sa_0], %[v_sa_0], %[v_sb] \n"
    " v_mul_f32  %[v_sa_1], %[v_sa_1], %[v_sb] \n"
    " v_mul_f32  %[v_sa_2], %[v_sa_2], %[v_sb] \n"
    " v_mul_f32  %[v_sa_3], %[v_sa_3], %[v_sb] \n"
    " v_mul_f32  %[v_sa_4], %[v_sa_4], %[v_sb] \n"
    " v_mul_f32  %[v_sa_5], %[v_sa_5], %[v_sb] \n"
    " v_mul_f32  %[v_sa_6], %[v_sa_6], %[v_sb] \n"
    " v_mul_f32  %[v_sa_7], %[v_sa_7], %[v_sb] \n"
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
    _UK_MFMA_   " [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], acc[0:1],   acc[64:65], [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]]                      \n"
    _UK_MFMA_   " [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], acc[2:3],   acc[66:67], [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]]                      \n"
    _UK_MFMA_   " [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], acc[4:5],   acc[68:69], [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]]                      \n"
    _UK_MFMA_   " [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]], acc[6:7],   acc[70:71], [%[v_c_0x], %[v_c_0y], %[v_c_0z], %[v_c_0w]]                      \n"
    // C = C * scale c
    " s_nop 16 \n"
    " v_mul_f32 %[v_c_0x], %[v_c_0x], %[v_sa_0] \n"
    " v_mul_f32 %[v_c_0y], %[v_c_0y], %[v_sa_0] \n"
    " v_mul_f32 %[v_c_0z], %[v_c_0z], %[v_sa_0] \n"
    " v_mul_f32 %[v_c_0w], %[v_c_0w], %[v_sa_0] \n"
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
    _UK_MFMA_   " [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], acc[8:9],   acc[64:65], [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]]                      \n"
    _UK_MFMA_   " [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], acc[10:11], acc[66:67], [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]]                      \n"
    _UK_MFMA_   " [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], acc[12:13], acc[68:69], [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]]                      \n"
    _UK_MFMA_   " [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]], acc[14:15], acc[70:71], [%[v_c_1x], %[v_c_1y], %[v_c_1z], %[v_c_1w]]                      \n"
    // C = C * scale c
    " s_nop 16 \n"
    " v_mul_f32 %[v_c_1x], %[v_c_1x], %[v_sa_0] \n"
    " v_mul_f32 %[v_c_1y], %[v_c_1y], %[v_sa_0] \n"
    " v_mul_f32 %[v_c_1z], %[v_c_1z], %[v_sa_0] \n"
    " v_mul_f32 %[v_c_1w], %[v_c_1w], %[v_sa_0] \n"
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
    _UK_MFMA_   " [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], acc[0:1],   acc[72:73], [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]]                      \n"
    _UK_MFMA_   " [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], acc[2:3],   acc[74:75], [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]]                      \n"
    _UK_MFMA_   " [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], acc[4:5],   acc[76:77], [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]]                      \n"
    _UK_MFMA_   " [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]], acc[6:7],   acc[78:79], [%[v_c_2x], %[v_c_2y], %[v_c_2z], %[v_c_2w]]                      \n"
    // C = C * scale c
    " s_nop 16 \n"
    " v_mul_f32 %[v_c_2x], %[v_c_2x], %[v_sa_1] \n"
    " v_mul_f32 %[v_c_2y], %[v_c_2y], %[v_sa_1] \n"
    " v_mul_f32 %[v_c_2z], %[v_c_2z], %[v_sa_1] \n"
    " v_mul_f32 %[v_c_2w], %[v_c_2w], %[v_sa_1] \n"
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
    _UK_MFMA_   " [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]], acc[8:9],   acc[120:121], [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]]                    \n"
    _UK_MFMA_   " [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]], acc[10:11], acc[122:123], [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]]                    \n"
    _UK_MFMA_   " [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]], acc[12:13], acc[124:125], [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]]                    \n"
    _UK_MFMA_   " [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]], acc[14:15], acc[126:127], [%[v_c_15x], %[v_c_15y], %[v_c_15z], %[v_c_15w]]                    \n"
    // C = C * scale c
    " s_nop 16 \n"
    " v_mul_f32 %[v_c_15x], %[v_c_15x], %[v_sa_7] \n"
    " v_mul_f32 %[v_c_15y], %[v_c_15y], %[v_sa_7] \n"
    " v_mul_f32 %[v_c_15z], %[v_c_15z], %[v_sa_7] \n"
    " v_mul_f32 %[v_c_15w], %[v_c_15w], %[v_sa_7] \n"
    // ===================================================================================
    _FF_SYNC_
#else

#pragma region PREFETCH_A
    // A0: vmem->vgpr
    // A0: vgpr->lds
    // A1: vmem->vgpr
    // A1: vgpr->lds;
    // A0: lds->vgpr;

    // A: vmem->vgpr
    " buffer_load_dwordx4  v[128:131], %[v_a_vm_ld_addr0],  s[16:19], 0 offen               \n"
    " buffer_load_dwordx4  v[132:135], %[v_a_vm_ld_addr1],  s[16:19], 0 offen               \n"
    " buffer_load_dwordx4  v[136:139], %[v_a_vm_ld_addr2],  s[16:19], 0 offen               \n"
    " buffer_load_dwordx4  v[140:143], %[v_a_vm_ld_addr3],  s[16:19], 0 offen               \n"
    " s_waitcnt vmcnt(0)                                                                    \n"
    // A: vgpr->lds
    " ds_write_b128 %[v_a_lds_wr_addr], v[128:131]  offset: %[n_a_lds_wr_offset] * 0        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[132:135]  offset: %[n_a_lds_wr_offset] * 1        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[136:139]  offset: %[n_a_lds_wr_offset] * 2        \n"
    " ds_write_b128 %[v_a_lds_wr_addr], v[140:143]  offset: %[n_a_lds_wr_offset] * 3        \n"
    " s_waitcnt lgkmcnt(0)                                                                  \n"
    " s_barrier                                                                             \n"
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
    " s_waitcnt lgkmcnt(0)                                                                  \n"
#pragma endregion

#pragma region PREFETCH_B
    // B0: vmem->acc
    " buffer_load_dwordx4  acc[0:3],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0                   \n" // [0~63]N   * [0~63]K
    " buffer_load_dwordx4  acc[4:7],   %[v_os_b0], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1                   \n" // [0~63]N   * [64~127]K
    " buffer_load_dwordx4  acc[8:11],  %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 0                   \n" // [64~127]N * [0~63]K
    " buffer_load_dwordx4  acc[12:15], %[v_os_b1], s[20:23], 0 offen offset: %[n_fb_vm_ld_offset] * 1                   \n" // [64~127]N * [64~127]K
    " s_waitcnt vmcnt(0)                                                                    \n"
#pragma endregion

#pragma region PREFETCH_SCALE_A
    " buffer_load_dword    %[v_sa_0], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 0           \n" // [0~15]M
    " buffer_load_dword    %[v_sa_1], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 1           \n" // [16~31]M
    " buffer_load_dword    %[v_sa_2], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 2           \n" // [32~47]M
    " buffer_load_dword    %[v_sa_3], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 3           \n" // [48~63]M
    " buffer_load_dword    %[v_sa_4], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 4           \n" // [64~79]M
    " buffer_load_dword    %[v_sa_5], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 5           \n" // [80~95]M
    " buffer_load_dword    %[v_sa_6], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 6           \n" // [96~111]M
    " buffer_load_dword    %[v_sa_7], %[v_sa_vm_ld_addr],  s[24:27], 0 offen offset: %[n_sa_vm_ld_offset] * 7           \n" // [112~127]M
    " s_waitcnt vmcnt(0)                                                                    \n"
#pragma endregion

#pragma region PREFETCH_SCALE_B
#pragma endregion

"L_start%=:                                                             \n"

"L_end%=:                                                               \n"
    "  s_nop 2                                                          \n"

#endif

#undef _UK_MFMA_
#undef _FF_SYNC_
