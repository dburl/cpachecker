/* Generated by CIL v. 1.3.7 */
/* print_CIL_Input is true */

#line 44 "../dma_example.h"
struct _control_block {
   unsigned long long in_addr ;
   unsigned long long out_addr ;
   unsigned int num_elements_per_spe ;
   unsigned int id ;
   unsigned int pad[2] ;
};
#line 44 "../dma_example.h"
typedef struct _control_block control_block_t;
#line 339 "/usr/include/stdio.h"
extern int printf(char const   * __restrict  __format  , ...) ;
#line 203 "../spu_mfcio.h"
extern void mfc_put(void volatile   *ls , unsigned int ea , unsigned int size , unsigned int tag ,
                    unsigned int tid , unsigned int rid ) ;
#line 211
extern void mfc_get(void volatile   *ls , unsigned int ea , unsigned int size , unsigned int tag ,
                    unsigned int tid , unsigned int rid ) ;
#line 252
extern void mfc_write_tag_mask(unsigned int mask ) ;
#line 270
extern void mfc_read_tag_status_all() ;
#line 25 "dma_example_spu.c"
float local_buffers[2][4096]  __attribute__((__aligned__(128)))  ;
#line 32 "dma_example_spu.c"
control_block_t control_block  __attribute__((__aligned__(128)))  ;
#line 92
extern int ( /* missing proto */  __CPROVER_ndet_int)() ;
#line 128
extern int ( /* missing proto */  assert)() ;
#line 63 "dma_example_spu.c"
int spu_main(unsigned long long speid  __attribute__((__unused__)) , unsigned long long argp ,
             unsigned long long envp  __attribute__((__unused__)) ) 
{ unsigned int tags[2] ;
  unsigned long long in_addr ;
  unsigned long long out_addr ;
  unsigned int i ;
  unsigned int num_chunks ;
  int cur_buf ;
  int next_buf ;
  int tmp ;
  int tmp___0 ;
  int tmp___1 ;
  int tmp___2 ;
  int tmp___3 ;
  int tmp___4 ;
  int tmp___5 ;
  unsigned long __cil_tmp18 ;
  unsigned long __cil_tmp19 ;
  unsigned long __cil_tmp20 ;
  unsigned long __cil_tmp21 ;
  unsigned long __cil_tmp22 ;
  unsigned long __cil_tmp23 ;
  unsigned int __cil_tmp24 ;
  char const   * __restrict  __cil_tmp25 ;
  unsigned long __cil_tmp26 ;
  unsigned long __cil_tmp27 ;
  unsigned int __cil_tmp28 ;
  char const   * __restrict  __cil_tmp29 ;
  void volatile   *__cil_tmp30 ;
  unsigned int __cil_tmp31 ;
  unsigned int __cil_tmp32 ;
  unsigned long __cil_tmp33 ;
  unsigned long __cil_tmp34 ;
  unsigned int __cil_tmp35 ;
  control_block_t *__cil_tmp36 ;
  unsigned long __cil_tmp37 ;
  unsigned long __cil_tmp38 ;
  unsigned long __cil_tmp39 ;
  unsigned long __cil_tmp40 ;
  unsigned long __cil_tmp41 ;
  unsigned long __cil_tmp42 ;
  unsigned long __cil_tmp43 ;
  unsigned long __cil_tmp44 ;
  unsigned long __cil_tmp45 ;
  unsigned long __cil_tmp46 ;
  unsigned long __cil_tmp47 ;
  unsigned int __cil_tmp48 ;
  int __cil_tmp49 ;
  unsigned int __cil_tmp50 ;
  unsigned long __cil_tmp51 ;
  unsigned int __cil_tmp52 ;
  control_block_t *__cil_tmp53 ;
  unsigned long __cil_tmp54 ;
  unsigned long __cil_tmp55 ;
  unsigned long __cil_tmp56 ;
  unsigned long __cil_tmp57 ;
  float *__cil_tmp58 ;
  void volatile   *__cil_tmp59 ;
  unsigned int __cil_tmp60 ;
  unsigned long __cil_tmp61 ;
  unsigned int __cil_tmp62 ;
  unsigned long __cil_tmp63 ;
  unsigned long __cil_tmp64 ;
  unsigned int __cil_tmp65 ;
  unsigned long __cil_tmp66 ;
  unsigned long long __cil_tmp67 ;
  unsigned long __cil_tmp68 ;
  unsigned long __cil_tmp69 ;
  unsigned long __cil_tmp70 ;
  unsigned int __cil_tmp71 ;
  int __cil_tmp72 ;
  unsigned long __cil_tmp73 ;
  unsigned long __cil_tmp74 ;
  unsigned int __cil_tmp75 ;
  int __cil_tmp76 ;
  unsigned long __cil_tmp77 ;
  unsigned long __cil_tmp78 ;
  unsigned long __cil_tmp79 ;
  unsigned long __cil_tmp80 ;
  float *__cil_tmp81 ;
  void volatile   *__cil_tmp82 ;
  unsigned int __cil_tmp83 ;
  unsigned long __cil_tmp84 ;
  unsigned int __cil_tmp85 ;
  unsigned long __cil_tmp86 ;
  unsigned long __cil_tmp87 ;
  unsigned int __cil_tmp88 ;
  unsigned long __cil_tmp89 ;
  unsigned long __cil_tmp90 ;
  unsigned int __cil_tmp91 ;
  int __cil_tmp92 ;
  unsigned int __cil_tmp93 ;
  unsigned long __cil_tmp94 ;
  unsigned long __cil_tmp95 ;
  unsigned long __cil_tmp96 ;
  unsigned long __cil_tmp97 ;
  float *__cil_tmp98 ;
  void volatile   *__cil_tmp99 ;
  unsigned int __cil_tmp100 ;
  unsigned long __cil_tmp101 ;
  unsigned int __cil_tmp102 ;
  unsigned long __cil_tmp103 ;
  unsigned long __cil_tmp104 ;
  unsigned int __cil_tmp105 ;
  unsigned long __cil_tmp106 ;
  unsigned long long __cil_tmp107 ;
  unsigned long __cil_tmp108 ;
  unsigned long long __cil_tmp109 ;
  unsigned long __cil_tmp110 ;
  unsigned long __cil_tmp111 ;
  unsigned int __cil_tmp112 ;
  int __cil_tmp113 ;
  unsigned int __cil_tmp114 ;
  unsigned long __cil_tmp115 ;
  unsigned long __cil_tmp116 ;
  unsigned long __cil_tmp117 ;
  unsigned long __cil_tmp118 ;
  float *__cil_tmp119 ;
  void volatile   *__cil_tmp120 ;
  unsigned int __cil_tmp121 ;
  unsigned long __cil_tmp122 ;
  unsigned int __cil_tmp123 ;
  unsigned long __cil_tmp124 ;
  unsigned long __cil_tmp125 ;
  unsigned int __cil_tmp126 ;
  unsigned long __cil_tmp127 ;
  unsigned long __cil_tmp128 ;
  unsigned int __cil_tmp129 ;
  int __cil_tmp130 ;
  unsigned int __cil_tmp131 ;

  {
#line 81
  __cil_tmp18 = 0 * 4UL;
#line 81
  __cil_tmp19 = (unsigned long )(tags) + __cil_tmp18;
#line 81
  *((unsigned int *)__cil_tmp19) = 0U;
#line 82
  __cil_tmp20 = 1 * 4UL;
#line 82
  __cil_tmp21 = (unsigned long )(tags) + __cil_tmp20;
#line 82
  *((unsigned int *)__cil_tmp21) = 1U;
  {
#line 84
  __cil_tmp22 = 0 * 4UL;
#line 84
  __cil_tmp23 = (unsigned long )(tags) + __cil_tmp22;
#line 84
  __cil_tmp24 = *((unsigned int *)__cil_tmp23);
#line 84
  if (__cil_tmp24 == 4294967295U) {
    {
#line 86
    __cil_tmp25 = (char const   * __restrict  )"SPU ERROR, unable to reserve tag\n";
#line 86
    printf(__cil_tmp25);
    }
#line 87
    return (1);
  } else {
    {
#line 84
    __cil_tmp26 = 1 * 4UL;
#line 84
    __cil_tmp27 = (unsigned long )(tags) + __cil_tmp26;
#line 84
    __cil_tmp28 = *((unsigned int *)__cil_tmp27);
#line 84
    if (__cil_tmp28 == 4294967295U) {
      {
#line 86
      __cil_tmp29 = (char const   * __restrict  )"SPU ERROR, unable to reserve tag\n";
#line 86
      printf(__cil_tmp29);
      }
#line 87
      return (1);
    } else {

    }
    }
  }
  }
  {
#line 91
  __cil_tmp30 = (void volatile   *)(& control_block);
#line 91
  __cil_tmp31 = (unsigned int )argp;
#line 91
  __cil_tmp32 = (unsigned int )32UL;
#line 91
  __cil_tmp33 = 0 * 4UL;
#line 91
  __cil_tmp34 = (unsigned long )(tags) + __cil_tmp33;
#line 91
  __cil_tmp35 = *((unsigned int *)__cil_tmp34);
#line 91
  mfc_get(__cil_tmp30, __cil_tmp31, __cil_tmp32, __cil_tmp35, 0U, 0U);
#line 92
  tmp = __CPROVER_ndet_int();
#line 92
  __cil_tmp36 = & control_block;
#line 92
  *((unsigned long long *)__cil_tmp36) = (unsigned long long )tmp;
#line 92
  tmp___0 = __CPROVER_ndet_int();
#line 92
  __cil_tmp37 = (unsigned long )(& control_block) + 8;
#line 92
  *((unsigned long long *)__cil_tmp37) = (unsigned long long )tmp___0;
#line 92
  tmp___1 = __CPROVER_ndet_int();
#line 92
  __cil_tmp38 = (unsigned long )(& control_block) + 16;
#line 92
  *((unsigned int *)__cil_tmp38) = (unsigned int )tmp___1;
#line 92
  tmp___2 = __CPROVER_ndet_int();
#line 92
  __cil_tmp39 = (unsigned long )(& control_block) + 20;
#line 92
  *((unsigned int *)__cil_tmp39) = (unsigned int )tmp___2;
#line 92
  tmp___3 = __CPROVER_ndet_int();
#line 92
  __cil_tmp40 = 0 * 4UL;
#line 92
  __cil_tmp41 = 24 + __cil_tmp40;
#line 92
  __cil_tmp42 = (unsigned long )(& control_block) + __cil_tmp41;
#line 92
  *((unsigned int *)__cil_tmp42) = (unsigned int )tmp___3;
#line 92
  tmp___4 = __CPROVER_ndet_int();
#line 92
  __cil_tmp43 = 1 * 4UL;
#line 92
  __cil_tmp44 = 24 + __cil_tmp43;
#line 92
  __cil_tmp45 = (unsigned long )(& control_block) + __cil_tmp44;
#line 92
  *((unsigned int *)__cil_tmp45) = (unsigned int )tmp___4;
#line 95
  __cil_tmp46 = 0 * 4UL;
#line 95
  __cil_tmp47 = (unsigned long )(tags) + __cil_tmp46;
#line 95
  __cil_tmp48 = *((unsigned int *)__cil_tmp47);
#line 95
  __cil_tmp49 = 1 << __cil_tmp48;
#line 95
  __cil_tmp50 = (unsigned int )__cil_tmp49;
#line 95
  mfc_write_tag_mask(__cil_tmp50);
#line 96
  mfc_read_tag_status_all();
#line 99
  cur_buf = 0;
#line 103
  __cil_tmp51 = (unsigned long )(& control_block) + 16;
#line 103
  __cil_tmp52 = *((unsigned int *)__cil_tmp51);
#line 103
  num_chunks = __cil_tmp52 / 4096U;
#line 107
  __cil_tmp53 = & control_block;
#line 107
  in_addr = *((unsigned long long *)__cil_tmp53);
#line 110
  __cil_tmp54 = 0 * 4UL;
#line 110
  __cil_tmp55 = cur_buf * 16384UL;
#line 110
  __cil_tmp56 = __cil_tmp55 + __cil_tmp54;
#line 110
  __cil_tmp57 = (unsigned long )(local_buffers) + __cil_tmp56;
#line 110
  __cil_tmp58 = (float *)__cil_tmp57;
#line 110
  __cil_tmp59 = (void volatile   *)__cil_tmp58;
#line 110
  __cil_tmp60 = (unsigned int )in_addr;
#line 110
  __cil_tmp61 = 4096UL * 4UL;
#line 110
  __cil_tmp62 = (unsigned int )__cil_tmp61;
#line 110
  __cil_tmp63 = cur_buf * 4UL;
#line 110
  __cil_tmp64 = (unsigned long )(tags) + __cil_tmp63;
#line 110
  __cil_tmp65 = *((unsigned int *)__cil_tmp64);
#line 110
  mfc_get(__cil_tmp59, __cil_tmp60, __cil_tmp62, __cil_tmp65, 0U, 0U);
#line 114
  __cil_tmp66 = 4096UL * 4UL;
#line 114
  __cil_tmp67 = (unsigned long long )__cil_tmp66;
#line 114
  in_addr = in_addr + __cil_tmp67;
#line 118
  __cil_tmp68 = (unsigned long )(& control_block) + 8;
#line 118
  out_addr = *((unsigned long long *)__cil_tmp68);
#line 125
  i = 1U;
  }
  {
#line 125
  while (1) {
    while_continue: /* CIL Label */ ;
#line 125
    if (i < num_chunks) {

    } else {
#line 125
      goto while_break;
    }
#line 128
    if (cur_buf == 0) {
#line 128
      tmp___5 = 1;
    } else
#line 128
    if (cur_buf == 1) {
#line 128
      tmp___5 = 1;
    } else {
#line 128
      tmp___5 = 0;
    }
    {
#line 128
    assert(tmp___5);
#line 129
    __cil_tmp69 = 0 * 4UL;
#line 129
    __cil_tmp70 = (unsigned long )(tags) + __cil_tmp69;
#line 129
    __cil_tmp71 = *((unsigned int *)__cil_tmp70);
#line 129
    __cil_tmp72 = __cil_tmp71 == 0U;
#line 129
    assert(__cil_tmp72);
#line 130
    __cil_tmp73 = 1 * 4UL;
#line 130
    __cil_tmp74 = (unsigned long )(tags) + __cil_tmp73;
#line 130
    __cil_tmp75 = *((unsigned int *)__cil_tmp74);
#line 130
    __cil_tmp76 = __cil_tmp75 == 1U;
#line 130
    assert(__cil_tmp76);
#line 134
    next_buf = cur_buf ^ 1;
#line 146
    __cil_tmp77 = 0 * 4UL;
#line 146
    __cil_tmp78 = next_buf * 16384UL;
#line 146
    __cil_tmp79 = __cil_tmp78 + __cil_tmp77;
#line 146
    __cil_tmp80 = (unsigned long )(local_buffers) + __cil_tmp79;
#line 146
    __cil_tmp81 = (float *)__cil_tmp80;
#line 146
    __cil_tmp82 = (void volatile   *)__cil_tmp81;
#line 146
    __cil_tmp83 = (unsigned int )in_addr;
#line 146
    __cil_tmp84 = 4096UL * 4UL;
#line 146
    __cil_tmp85 = (unsigned int )__cil_tmp84;
#line 146
    __cil_tmp86 = next_buf * 4UL;
#line 146
    __cil_tmp87 = (unsigned long )(tags) + __cil_tmp86;
#line 146
    __cil_tmp88 = *((unsigned int *)__cil_tmp87);
#line 146
    mfc_get(__cil_tmp82, __cil_tmp83, __cil_tmp85, __cil_tmp88, 0U, 0U);
#line 152
    __cil_tmp89 = cur_buf * 4UL;
#line 152
    __cil_tmp90 = (unsigned long )(tags) + __cil_tmp89;
#line 152
    __cil_tmp91 = *((unsigned int *)__cil_tmp90);
#line 152
    __cil_tmp92 = 1 << __cil_tmp91;
#line 152
    __cil_tmp93 = (unsigned int )__cil_tmp92;
#line 152
    mfc_write_tag_mask(__cil_tmp93);
#line 153
    mfc_read_tag_status_all();
#line 163
    __cil_tmp94 = 0 * 4UL;
#line 163
    __cil_tmp95 = cur_buf * 16384UL;
#line 163
    __cil_tmp96 = __cil_tmp95 + __cil_tmp94;
#line 163
    __cil_tmp97 = (unsigned long )(local_buffers) + __cil_tmp96;
#line 163
    __cil_tmp98 = (float *)__cil_tmp97;
#line 163
    __cil_tmp99 = (void volatile   *)__cil_tmp98;
#line 163
    __cil_tmp100 = (unsigned int )out_addr;
#line 163
    __cil_tmp101 = 4096UL * 4UL;
#line 163
    __cil_tmp102 = (unsigned int )__cil_tmp101;
#line 163
    __cil_tmp103 = cur_buf * 4UL;
#line 163
    __cil_tmp104 = (unsigned long )(tags) + __cil_tmp103;
#line 163
    __cil_tmp105 = *((unsigned int *)__cil_tmp104);
#line 163
    mfc_put(__cil_tmp99, __cil_tmp100, __cil_tmp102, __cil_tmp105, 0U, 0U);
#line 167
    __cil_tmp106 = 4096UL * 4UL;
#line 167
    __cil_tmp107 = (unsigned long long )__cil_tmp106;
#line 167
    in_addr = in_addr + __cil_tmp107;
#line 168
    __cil_tmp108 = 4096UL * 4UL;
#line 168
    __cil_tmp109 = (unsigned long long )__cil_tmp108;
#line 168
    out_addr = out_addr + __cil_tmp109;
#line 171
    cur_buf = next_buf;
#line 125
    i = i + 1U;
    }
  }
  while_break: /* CIL Label */ ;
  }
  {
#line 175
  __cil_tmp110 = cur_buf * 4UL;
#line 175
  __cil_tmp111 = (unsigned long )(tags) + __cil_tmp110;
#line 175
  __cil_tmp112 = *((unsigned int *)__cil_tmp111);
#line 175
  __cil_tmp113 = 1 << __cil_tmp112;
#line 175
  __cil_tmp114 = (unsigned int )__cil_tmp113;
#line 175
  mfc_write_tag_mask(__cil_tmp114);
#line 176
  mfc_read_tag_status_all();
#line 184
  __cil_tmp115 = 0 * 4UL;
#line 184
  __cil_tmp116 = cur_buf * 16384UL;
#line 184
  __cil_tmp117 = __cil_tmp116 + __cil_tmp115;
#line 184
  __cil_tmp118 = (unsigned long )(local_buffers) + __cil_tmp117;
#line 184
  __cil_tmp119 = (float *)__cil_tmp118;
#line 184
  __cil_tmp120 = (void volatile   *)__cil_tmp119;
#line 184
  __cil_tmp121 = (unsigned int )out_addr;
#line 184
  __cil_tmp122 = 4096UL * 4UL;
#line 184
  __cil_tmp123 = (unsigned int )__cil_tmp122;
#line 184
  __cil_tmp124 = cur_buf * 4UL;
#line 184
  __cil_tmp125 = (unsigned long )(tags) + __cil_tmp124;
#line 184
  __cil_tmp126 = *((unsigned int *)__cil_tmp125);
#line 184
  mfc_put(__cil_tmp120, __cil_tmp121, __cil_tmp123, __cil_tmp126, 0U, 0U);
#line 188
  __cil_tmp127 = cur_buf * 4UL;
#line 188
  __cil_tmp128 = (unsigned long )(tags) + __cil_tmp127;
#line 188
  __cil_tmp129 = *((unsigned int *)__cil_tmp128);
#line 188
  __cil_tmp130 = 1 << __cil_tmp129;
#line 188
  __cil_tmp131 = (unsigned int )__cil_tmp130;
#line 188
  mfc_write_tag_mask(__cil_tmp131);
#line 189
  mfc_read_tag_status_all();
  }
#line 197
  return (0);
}
}
