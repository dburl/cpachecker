/* Generated by CIL v. 1.3.7 */
/* print_CIL_Input is true */

#line 61 "../julia.h"
struct __anonstruct_rendering_context_22 {
   float eyeP[4] ;
   float lightP[4] ;
   float scolor[4] ;
   float epsilonP ;
   unsigned int shadowsP ;
   unsigned int textureP ;
   unsigned int iterationsP ;
   unsigned int wb_addr ;
   unsigned int tex_width ;
   unsigned int tex_height ;
   unsigned int img_width ;
   unsigned int img_height ;
   unsigned int fmbase[6] ;
   unsigned char pad[20] ;
};
#line 61 "../julia.h"
typedef struct __anonstruct_rendering_context_22  __attribute__((__aligned__(128))) rendering_context;
#line 109 "../julia.h"
struct __anonstruct_rendering_region_23 {
   float dir_top_start[4] ;
   float dir_top_stop[4] ;
   float dir_bottom_start[4] ;
   float dir_bottom_stop[4] ;
   float muP[4] ;
   unsigned int fb_start ;
   int column_start ;
   int column_count ;
   unsigned char pad[36] ;
};
#line 109 "../julia.h"
typedef struct __anonstruct_rendering_region_23  __attribute__((__aligned__(128))) rendering_region;
#line 109 "ray.c"
struct float4_s {
   float data[4] ;
};
#line 109 "ray.c"
typedef struct float4_s float4;
#line 71 "/usr/include/assert.h"
extern  __attribute__((__nothrow__, __noreturn__)) void __assert_fail(char const   *__assertion ,
                                                                      char const   *__file ,
                                                                      unsigned int __line ,
                                                                      char const   *__function ) ;
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
#line 152 "../julia.h"
unsigned int fb_start  ;
#line 153 "../julia.h"
int column_start  ;
#line 154 "../julia.h"
int column_count  ;
#line 155 "../julia.h"
unsigned char pad[36]  ;
#line 93 "ray.c"
int FIXED_NUM_COLUMNS  ;
#line 115 "ray.c"
int volatile   mywriteback[4]  __attribute__((__aligned__(128)))  = {      (int volatile   )0,      (int volatile   )0,      (int volatile   )0,      (int volatile   )0};
#line 119 "ray.c"
rendering_context volatile   rc  __attribute__((__aligned__(128)))  ;
#line 120 "ray.c"
rendering_region volatile   region  __attribute__((__aligned__(128)))  ;
#line 121 "ray.c"
float4 volatile   image_buf[2][1024]  __attribute__((__aligned__(128)))  ;
#line 389
extern int ( /* missing proto */  __builtin_si_to_uint)() ;
#line 389
extern int ( /* missing proto */  __builtin_si_rdch)() ;
#line 391
extern int ( /* missing proto */  __CPROVER_assume)() ;
#line 397
extern int __VERIFIER_nondet_float() ;
#line 397
extern int __VERIFIER_nondet_int() ;
#line 326 "ray.c"
int spu_main(void) 
{ unsigned int i ;
  unsigned int ea_low ;
  unsigned int opcode ;
  unsigned int now_tag ;
  unsigned int now_mask ;
  unsigned int sc_tag[2] ;
  unsigned int sc_mask[2] ;
  unsigned int wb_addr ;
  float epsilon ;
  int renderShadows ;
  int maxIterations ;
  int curr_buf ;
  unsigned int fb_ea_low ;
  unsigned int fb_row_stride ;
  unsigned int fb_store ;
  unsigned int num_columns ;
  unsigned int start_column ;
  unsigned int num_rows ;
  float imgWidth ;
  float imgHeight ;
  int state ;
  int tmp ;
  int tmp___0 ;
  int tmp___1 ;
  int tmp___2 ;
  int tmp___3 ;
  int tmp___4 ;
  int tmp___5 ;
  int tmp___6 ;
  int tmp___7 ;
  int tmp___8 ;
  int tmp___9 ;
  int tmp___10 ;
  int tmp___11 ;
  int tmp___12 ;
  int tmp___13 ;
  int tmp___14 ;
  int tmp___15 ;
  int tmp___16 ;
  int tmp___17 ;
  int tmp___18 ;
  int tmp___19 ;
  int tmp___20 ;
  int tmp___21 ;
  int tmp___22 ;
  int tmp___23 ;
  int tmp___24 ;
  int tmp___25 ;
  int tmp___26 ;
  int tmp___27 ;
  int tmp___28 ;
  int finished ;
  int tmp___29 ;
  int tmp___30 ;
  int tmp___31 ;
  int tmp___32 ;
  int tmp___33 ;
  int tmp___34 ;
  int tmp___35 ;
  int tmp___36 ;
  int tmp___37 ;
  int tmp___38 ;
  int tmp___39 ;
  int tmp___40 ;
  int tmp___41 ;
  int tmp___42 ;
  int tmp___43 ;
  int tmp___44 ;
  int tmp___45 ;
  int tmp___46 ;
  int tmp___47 ;
  int tmp___48 ;
  int tmp___49 ;
  int tmp___50 ;
  int tmp___51 ;
  int tmp___52 ;
  int tmp___53 ;
  int tmp___54 ;
  int tmp___55 ;
  int tmp___56 ;
  void *__cil_tmp82 ;
  unsigned long __cil_tmp83 ;
  unsigned long __cil_tmp84 ;
  unsigned long __cil_tmp85 ;
  unsigned long __cil_tmp86 ;
  unsigned long __cil_tmp87 ;
  unsigned long __cil_tmp88 ;
  unsigned int __cil_tmp89 ;
  unsigned long __cil_tmp90 ;
  unsigned long __cil_tmp91 ;
  unsigned int __cil_tmp92 ;
  int __cil_tmp93 ;
  unsigned long __cil_tmp94 ;
  unsigned long __cil_tmp95 ;
  unsigned long __cil_tmp96 ;
  unsigned long __cil_tmp97 ;
  unsigned int __cil_tmp98 ;
  int __cil_tmp99 ;
  unsigned long __cil_tmp100 ;
  unsigned long __cil_tmp101 ;
  unsigned long __cil_tmp102 ;
  unsigned long __cil_tmp103 ;
  unsigned int __cil_tmp104 ;
  int __cil_tmp105 ;
  int __cil_tmp106 ;
  void *__cil_tmp107 ;
  void volatile   *__cil_tmp108 ;
  unsigned int __cil_tmp109 ;
  unsigned long __cil_tmp110 ;
  unsigned long __cil_tmp111 ;
  unsigned long __cil_tmp112 ;
  unsigned long __cil_tmp113 ;
  unsigned long __cil_tmp114 ;
  unsigned long __cil_tmp115 ;
  unsigned long __cil_tmp116 ;
  unsigned long __cil_tmp117 ;
  unsigned long __cil_tmp118 ;
  unsigned long __cil_tmp119 ;
  unsigned long __cil_tmp120 ;
  unsigned long __cil_tmp121 ;
  unsigned long __cil_tmp122 ;
  unsigned long __cil_tmp123 ;
  unsigned long __cil_tmp124 ;
  unsigned long __cil_tmp125 ;
  unsigned long __cil_tmp126 ;
  unsigned long __cil_tmp127 ;
  unsigned long __cil_tmp128 ;
  unsigned long __cil_tmp129 ;
  unsigned long __cil_tmp130 ;
  unsigned long __cil_tmp131 ;
  unsigned long __cil_tmp132 ;
  unsigned long __cil_tmp133 ;
  unsigned long __cil_tmp134 ;
  unsigned long __cil_tmp135 ;
  unsigned long __cil_tmp136 ;
  unsigned long __cil_tmp137 ;
  unsigned long __cil_tmp138 ;
  unsigned long __cil_tmp139 ;
  unsigned long __cil_tmp140 ;
  unsigned long __cil_tmp141 ;
  unsigned long __cil_tmp142 ;
  unsigned long __cil_tmp143 ;
  unsigned long __cil_tmp144 ;
  unsigned long __cil_tmp145 ;
  unsigned long __cil_tmp146 ;
  unsigned long __cil_tmp147 ;
  unsigned long __cil_tmp148 ;
  unsigned long __cil_tmp149 ;
  unsigned long __cil_tmp150 ;
  unsigned long __cil_tmp151 ;
  unsigned long __cil_tmp152 ;
  unsigned long __cil_tmp153 ;
  unsigned long __cil_tmp154 ;
  unsigned long __cil_tmp155 ;
  unsigned long __cil_tmp156 ;
  unsigned long __cil_tmp157 ;
  unsigned long __cil_tmp158 ;
  unsigned long __cil_tmp159 ;
  unsigned long __cil_tmp160 ;
  unsigned long __cil_tmp161 ;
  unsigned long __cil_tmp162 ;
  unsigned long __cil_tmp163 ;
  unsigned long __cil_tmp164 ;
  unsigned long __cil_tmp165 ;
  unsigned long __cil_tmp166 ;
  unsigned long __cil_tmp167 ;
  unsigned long __cil_tmp168 ;
  unsigned long __cil_tmp169 ;
  unsigned long __cil_tmp170 ;
  unsigned long __cil_tmp171 ;
  unsigned long __cil_tmp172 ;
  float volatile   __cil_tmp173 ;
  unsigned long __cil_tmp174 ;
  unsigned int volatile   __cil_tmp175 ;
  unsigned long __cil_tmp176 ;
  unsigned int volatile   __cil_tmp177 ;
  unsigned long __cil_tmp178 ;
  unsigned int volatile   __cil_tmp179 ;
  unsigned long __cil_tmp180 ;
  unsigned int volatile   __cil_tmp181 ;
  unsigned long __cil_tmp182 ;
  unsigned int volatile   __cil_tmp183 ;
  unsigned long __cil_tmp184 ;
  unsigned long __cil_tmp185 ;
  unsigned int __cil_tmp186 ;
  unsigned long __cil_tmp187 ;
  unsigned long __cil_tmp188 ;
  unsigned int __cil_tmp189 ;
  int __cil_tmp190 ;
  unsigned int __cil_tmp191 ;
  unsigned long __cil_tmp192 ;
  unsigned long __cil_tmp193 ;
  unsigned int __cil_tmp194 ;
  int __cil_tmp195 ;
  unsigned int __cil_tmp196 ;
  unsigned long __cil_tmp197 ;
  unsigned long __cil_tmp198 ;
  unsigned int __cil_tmp199 ;
  unsigned long __cil_tmp200 ;
  unsigned long __cil_tmp201 ;
  unsigned int __cil_tmp202 ;
  int __cil_tmp203 ;
  unsigned int __cil_tmp204 ;
  unsigned long __cil_tmp205 ;
  unsigned long __cil_tmp206 ;
  unsigned int __cil_tmp207 ;
  int __cil_tmp208 ;
  void *__cil_tmp209 ;
  void volatile   *__cil_tmp210 ;
  unsigned int __cil_tmp211 ;
  unsigned long __cil_tmp212 ;
  unsigned long __cil_tmp213 ;
  unsigned long __cil_tmp214 ;
  unsigned long __cil_tmp215 ;
  unsigned long __cil_tmp216 ;
  unsigned long __cil_tmp217 ;
  unsigned long __cil_tmp218 ;
  unsigned long __cil_tmp219 ;
  unsigned long __cil_tmp220 ;
  unsigned long __cil_tmp221 ;
  unsigned long __cil_tmp222 ;
  unsigned long __cil_tmp223 ;
  unsigned long __cil_tmp224 ;
  unsigned long __cil_tmp225 ;
  unsigned long __cil_tmp226 ;
  unsigned long __cil_tmp227 ;
  unsigned long __cil_tmp228 ;
  unsigned long __cil_tmp229 ;
  unsigned long __cil_tmp230 ;
  unsigned long __cil_tmp231 ;
  unsigned long __cil_tmp232 ;
  unsigned long __cil_tmp233 ;
  unsigned long __cil_tmp234 ;
  unsigned long __cil_tmp235 ;
  unsigned long __cil_tmp236 ;
  unsigned long __cil_tmp237 ;
  unsigned long __cil_tmp238 ;
  unsigned long __cil_tmp239 ;
  unsigned long __cil_tmp240 ;
  unsigned long __cil_tmp241 ;
  unsigned long __cil_tmp242 ;
  unsigned long __cil_tmp243 ;
  unsigned long __cil_tmp244 ;
  unsigned long __cil_tmp245 ;
  unsigned long __cil_tmp246 ;
  unsigned long __cil_tmp247 ;
  unsigned long __cil_tmp248 ;
  unsigned long __cil_tmp249 ;
  unsigned long __cil_tmp250 ;
  unsigned long __cil_tmp251 ;
  unsigned long __cil_tmp252 ;
  unsigned long __cil_tmp253 ;
  unsigned long __cil_tmp254 ;
  unsigned long __cil_tmp255 ;
  unsigned long __cil_tmp256 ;
  unsigned long __cil_tmp257 ;
  unsigned long __cil_tmp258 ;
  unsigned long __cil_tmp259 ;
  unsigned long __cil_tmp260 ;
  unsigned long __cil_tmp261 ;
  unsigned long __cil_tmp262 ;
  unsigned long __cil_tmp263 ;
  unsigned long __cil_tmp264 ;
  unsigned long __cil_tmp265 ;
  unsigned long __cil_tmp266 ;
  unsigned long __cil_tmp267 ;
  unsigned long __cil_tmp268 ;
  unsigned long __cil_tmp269 ;
  unsigned long __cil_tmp270 ;
  unsigned long __cil_tmp271 ;
  unsigned long __cil_tmp272 ;
  unsigned long __cil_tmp273 ;
  unsigned long __cil_tmp274 ;
  unsigned long __cil_tmp275 ;
  unsigned int volatile   __cil_tmp276 ;
  unsigned long __cil_tmp277 ;
  unsigned long __cil_tmp278 ;
  int volatile   __cil_tmp279 ;
  unsigned int __cil_tmp280 ;
  unsigned long __cil_tmp281 ;
  int volatile   __cil_tmp282 ;
  unsigned int __cil_tmp283 ;
  unsigned int __cil_tmp284 ;
  int __cil_tmp285 ;
  unsigned long __cil_tmp286 ;
  unsigned long __cil_tmp287 ;
  unsigned int __cil_tmp288 ;
  unsigned long __cil_tmp289 ;
  unsigned long __cil_tmp290 ;
  unsigned long __cil_tmp291 ;
  unsigned long __cil_tmp292 ;
  float4 volatile   *__cil_tmp293 ;
  void volatile   *__cil_tmp294 ;
  unsigned long __cil_tmp295 ;
  unsigned long __cil_tmp296 ;
  unsigned int __cil_tmp297 ;

  {
#line 337
  __cil_tmp82 = (void *)0;
#line 337
  wb_addr = (unsigned int )__cil_tmp82;
#line 344
  epsilon = (float )0;
#line 345
  renderShadows = 0;
#line 346
  maxIterations = 0;
#line 356
  curr_buf = 0;
#line 363
  imgWidth = (float )0;
#line 363
  imgHeight = (float )0;
#line 377
  now_tag = 0U;
#line 378
  __cil_tmp83 = 0 * 4UL;
#line 378
  __cil_tmp84 = (unsigned long )(sc_tag) + __cil_tmp83;
#line 378
  *((unsigned int *)__cil_tmp84) = 1U;
#line 379
  __cil_tmp85 = 1 * 4UL;
#line 379
  __cil_tmp86 = (unsigned long )(sc_tag) + __cil_tmp85;
#line 379
  *((unsigned int *)__cil_tmp86) = 2U;
#line 381
  if (now_tag != 4294967295U) {
    {
#line 381
    __cil_tmp87 = 0 * 4UL;
#line 381
    __cil_tmp88 = (unsigned long )(sc_tag) + __cil_tmp87;
#line 381
    __cil_tmp89 = *((unsigned int *)__cil_tmp88);
#line 381
    if (__cil_tmp89 != 4294967295U) {
      {
#line 381
      __cil_tmp90 = 1 * 4UL;
#line 381
      __cil_tmp91 = (unsigned long )(sc_tag) + __cil_tmp90;
#line 381
      __cil_tmp92 = *((unsigned int *)__cil_tmp91);
#line 381
      if (__cil_tmp92 != 4294967295U) {

      } else {
        {
#line 381
        __assert_fail("(now_tag != 0xFFFFFFFF) && (sc_tag[0] != 0xFFFFFFFF) && (sc_tag[1] != 0xFFFFFFFF)",
                      "ray.c", 383U, "spu_main");
        }
      }
      }
    } else {
      {
#line 381
      __assert_fail("(now_tag != 0xFFFFFFFF) && (sc_tag[0] != 0xFFFFFFFF) && (sc_tag[1] != 0xFFFFFFFF)",
                    "ray.c", 383U, "spu_main");
      }
    }
    }
  } else {
    {
#line 381
    __assert_fail("(now_tag != 0xFFFFFFFF) && (sc_tag[0] != 0xFFFFFFFF) && (sc_tag[1] != 0xFFFFFFFF)",
                  "ray.c", 383U, "spu_main");
    }
  }
  {
#line 385
  __cil_tmp93 = 1 << now_tag;
#line 385
  now_mask = (unsigned int )__cil_tmp93;
#line 386
  __cil_tmp94 = 0 * 4UL;
#line 386
  __cil_tmp95 = (unsigned long )(sc_mask) + __cil_tmp94;
#line 386
  __cil_tmp96 = 0 * 4UL;
#line 386
  __cil_tmp97 = (unsigned long )(sc_tag) + __cil_tmp96;
#line 386
  __cil_tmp98 = *((unsigned int *)__cil_tmp97);
#line 386
  __cil_tmp99 = 1 << __cil_tmp98;
#line 386
  *((unsigned int *)__cil_tmp95) = (unsigned int )__cil_tmp99;
#line 387
  __cil_tmp100 = 1 * 4UL;
#line 387
  __cil_tmp101 = (unsigned long )(sc_mask) + __cil_tmp100;
#line 387
  __cil_tmp102 = 1 * 4UL;
#line 387
  __cil_tmp103 = (unsigned long )(sc_tag) + __cil_tmp102;
#line 387
  __cil_tmp104 = *((unsigned int *)__cil_tmp103);
#line 387
  __cil_tmp105 = 1 << __cil_tmp104;
#line 387
  *((unsigned int *)__cil_tmp101) = (unsigned int )__cil_tmp105;
#line 389
  tmp = __builtin_si_rdch(29);
#line 389
  tmp___0 = __builtin_si_to_uint(tmp);
#line 389
  opcode = (unsigned int )tmp___0;
#line 391
  __cil_tmp106 = opcode == 3U;
#line 391
  __CPROVER_assume(__cil_tmp106);
#line 393
  tmp___1 = __builtin_si_rdch(29);
#line 393
  tmp___2 = __builtin_si_to_uint(tmp___1);
#line 393
  ea_low = (unsigned int )tmp___2;
#line 395
  __cil_tmp107 = (void *)(& rc);
#line 395
  __cil_tmp108 = (void volatile   *)__cil_tmp107;
#line 395
  __cil_tmp109 = (unsigned int )128UL;
#line 395
  mfc_get(__cil_tmp108, ea_low, __cil_tmp109, now_tag, 0U, 0U);
#line 397
  tmp___3 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp110 = 0 * 4UL;
#line 397
  __cil_tmp111 = 0 + __cil_tmp110;
#line 397
  __cil_tmp112 = (unsigned long )(& rc) + __cil_tmp111;
#line 397
  *((float volatile   *)__cil_tmp112) = (float )tmp___3;
#line 397
  tmp___4 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp113 = 1 * 4UL;
#line 397
  __cil_tmp114 = 0 + __cil_tmp113;
#line 397
  __cil_tmp115 = (unsigned long )(& rc) + __cil_tmp114;
#line 397
  *((float volatile   *)__cil_tmp115) = (float )tmp___4;
#line 397
  tmp___5 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp116 = 2 * 4UL;
#line 397
  __cil_tmp117 = 0 + __cil_tmp116;
#line 397
  __cil_tmp118 = (unsigned long )(& rc) + __cil_tmp117;
#line 397
  *((float volatile   *)__cil_tmp118) = (float )tmp___5;
#line 397
  tmp___6 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp119 = 3 * 4UL;
#line 397
  __cil_tmp120 = 0 + __cil_tmp119;
#line 397
  __cil_tmp121 = (unsigned long )(& rc) + __cil_tmp120;
#line 397
  *((float volatile   *)__cil_tmp121) = (float )tmp___6;
#line 397
  tmp___7 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp122 = 0 * 4UL;
#line 397
  __cil_tmp123 = 16 + __cil_tmp122;
#line 397
  __cil_tmp124 = (unsigned long )(& rc) + __cil_tmp123;
#line 397
  *((float volatile   *)__cil_tmp124) = (float )tmp___7;
#line 397
  tmp___8 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp125 = 1 * 4UL;
#line 397
  __cil_tmp126 = 16 + __cil_tmp125;
#line 397
  __cil_tmp127 = (unsigned long )(& rc) + __cil_tmp126;
#line 397
  *((float volatile   *)__cil_tmp127) = (float )tmp___8;
#line 397
  tmp___9 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp128 = 2 * 4UL;
#line 397
  __cil_tmp129 = 16 + __cil_tmp128;
#line 397
  __cil_tmp130 = (unsigned long )(& rc) + __cil_tmp129;
#line 397
  *((float volatile   *)__cil_tmp130) = (float )tmp___9;
#line 397
  tmp___10 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp131 = 3 * 4UL;
#line 397
  __cil_tmp132 = 16 + __cil_tmp131;
#line 397
  __cil_tmp133 = (unsigned long )(& rc) + __cil_tmp132;
#line 397
  *((float volatile   *)__cil_tmp133) = (float )tmp___10;
#line 397
  tmp___11 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp134 = 0 * 4UL;
#line 397
  __cil_tmp135 = 32 + __cil_tmp134;
#line 397
  __cil_tmp136 = (unsigned long )(& rc) + __cil_tmp135;
#line 397
  *((float volatile   *)__cil_tmp136) = (float )tmp___11;
#line 397
  tmp___12 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp137 = 1 * 4UL;
#line 397
  __cil_tmp138 = 32 + __cil_tmp137;
#line 397
  __cil_tmp139 = (unsigned long )(& rc) + __cil_tmp138;
#line 397
  *((float volatile   *)__cil_tmp139) = (float )tmp___12;
#line 397
  tmp___13 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp140 = 2 * 4UL;
#line 397
  __cil_tmp141 = 32 + __cil_tmp140;
#line 397
  __cil_tmp142 = (unsigned long )(& rc) + __cil_tmp141;
#line 397
  *((float volatile   *)__cil_tmp142) = (float )tmp___13;
#line 397
  tmp___14 = __VERIFIER_nondet_float();
#line 397
  __cil_tmp143 = 3 * 4UL;
#line 397
  __cil_tmp144 = 32 + __cil_tmp143;
#line 397
  __cil_tmp145 = (unsigned long )(& rc) + __cil_tmp144;
#line 397
  *((float volatile   *)__cil_tmp145) = (float )tmp___14;
#line 397
  tmp___15 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp146 = (unsigned long )(& rc) + 52;
#line 397
  *((unsigned int volatile   *)__cil_tmp146) = (unsigned int volatile   )tmp___15;
#line 397
  tmp___16 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp147 = (unsigned long )(& rc) + 56;
#line 397
  *((unsigned int volatile   *)__cil_tmp147) = (unsigned int volatile   )tmp___16;
#line 397
  tmp___17 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp148 = (unsigned long )(& rc) + 60;
#line 397
  *((unsigned int volatile   *)__cil_tmp148) = (unsigned int volatile   )tmp___17;
#line 397
  tmp___18 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp149 = (unsigned long )(& rc) + 64;
#line 397
  *((unsigned int volatile   *)__cil_tmp149) = (unsigned int volatile   )tmp___18;
#line 397
  tmp___19 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp150 = (unsigned long )(& rc) + 68;
#line 397
  *((unsigned int volatile   *)__cil_tmp150) = (unsigned int volatile   )tmp___19;
#line 397
  tmp___20 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp151 = (unsigned long )(& rc) + 72;
#line 397
  *((unsigned int volatile   *)__cil_tmp151) = (unsigned int volatile   )tmp___20;
#line 397
  tmp___21 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp152 = (unsigned long )(& rc) + 76;
#line 397
  *((unsigned int volatile   *)__cil_tmp152) = (unsigned int volatile   )tmp___21;
#line 397
  tmp___22 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp153 = (unsigned long )(& rc) + 80;
#line 397
  *((unsigned int volatile   *)__cil_tmp153) = (unsigned int volatile   )tmp___22;
#line 397
  tmp___23 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp154 = 0 * 4UL;
#line 397
  __cil_tmp155 = 84 + __cil_tmp154;
#line 397
  __cil_tmp156 = (unsigned long )(& rc) + __cil_tmp155;
#line 397
  *((unsigned int volatile   *)__cil_tmp156) = (unsigned int )tmp___23;
#line 397
  tmp___24 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp157 = 1 * 4UL;
#line 397
  __cil_tmp158 = 84 + __cil_tmp157;
#line 397
  __cil_tmp159 = (unsigned long )(& rc) + __cil_tmp158;
#line 397
  *((unsigned int volatile   *)__cil_tmp159) = (unsigned int )tmp___24;
#line 397
  tmp___25 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp160 = 2 * 4UL;
#line 397
  __cil_tmp161 = 84 + __cil_tmp160;
#line 397
  __cil_tmp162 = (unsigned long )(& rc) + __cil_tmp161;
#line 397
  *((unsigned int volatile   *)__cil_tmp162) = (unsigned int )tmp___25;
#line 397
  tmp___26 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp163 = 3 * 4UL;
#line 397
  __cil_tmp164 = 84 + __cil_tmp163;
#line 397
  __cil_tmp165 = (unsigned long )(& rc) + __cil_tmp164;
#line 397
  *((unsigned int volatile   *)__cil_tmp165) = (unsigned int )tmp___26;
#line 397
  tmp___27 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp166 = 4 * 4UL;
#line 397
  __cil_tmp167 = 84 + __cil_tmp166;
#line 397
  __cil_tmp168 = (unsigned long )(& rc) + __cil_tmp167;
#line 397
  *((unsigned int volatile   *)__cil_tmp168) = (unsigned int )tmp___27;
#line 397
  tmp___28 = __VERIFIER_nondet_int();
#line 397
  __cil_tmp169 = 5 * 4UL;
#line 397
  __cil_tmp170 = 84 + __cil_tmp169;
#line 397
  __cil_tmp171 = (unsigned long )(& rc) + __cil_tmp170;
#line 397
  *((unsigned int volatile   *)__cil_tmp171) = (unsigned int )tmp___28;
#line 398
  mfc_write_tag_mask(now_mask);
#line 399
  mfc_read_tag_status_all();
#line 405
  __cil_tmp172 = (unsigned long )(& rc) + 48;
#line 405
  __cil_tmp173 = *((float volatile   *)__cil_tmp172);
#line 405
  epsilon = (float )__cil_tmp173;
#line 406
  __cil_tmp174 = (unsigned long )(& rc) + 52;
#line 406
  __cil_tmp175 = *((unsigned int volatile   *)__cil_tmp174);
#line 406
  renderShadows = (int )__cil_tmp175;
#line 407
  __cil_tmp176 = (unsigned long )(& rc) + 60;
#line 407
  __cil_tmp177 = *((unsigned int volatile   *)__cil_tmp176);
#line 407
  maxIterations = (int )__cil_tmp177;
#line 408
  __cil_tmp178 = (unsigned long )(& rc) + 64;
#line 408
  __cil_tmp179 = *((unsigned int volatile   *)__cil_tmp178);
#line 408
  wb_addr = (unsigned int )__cil_tmp179;
#line 409
  __cil_tmp180 = (unsigned long )(& rc) + 76;
#line 409
  __cil_tmp181 = *((unsigned int volatile   *)__cil_tmp180);
#line 409
  imgWidth = (float )__cil_tmp181;
#line 410
  __cil_tmp182 = (unsigned long )(& rc) + 80;
#line 410
  __cil_tmp183 = *((unsigned int volatile   *)__cil_tmp182);
#line 410
  imgHeight = (float )__cil_tmp183;
#line 412
  state = 0;
#line 414
  finished = 0;
  }
  {
#line 416
  while (1) {
    while_continue: /* CIL Label */ ;
#line 416
    if (! finished) {

    } else {
#line 416
      goto while_break;
    }
#line 418
    if (now_tag == 0U) {

    } else {
      {
#line 418
      __assert_fail("(now_tag) == 0", "ray.c", 418U, "spu_main");
      }
    }
    {
#line 419
    __cil_tmp184 = 0 * 4UL;
#line 419
    __cil_tmp185 = (unsigned long )(sc_tag) + __cil_tmp184;
#line 419
    __cil_tmp186 = *((unsigned int *)__cil_tmp185);
#line 419
    if (__cil_tmp186 == 1U) {

    } else {
      {
#line 419
      __assert_fail("(sc_tag[0]) == 1", "ray.c", 419U, "spu_main");
      }
    }
    }
    {
#line 420
    __cil_tmp187 = 1 * 4UL;
#line 420
    __cil_tmp188 = (unsigned long )(sc_tag) + __cil_tmp187;
#line 420
    __cil_tmp189 = *((unsigned int *)__cil_tmp188);
#line 420
    if (__cil_tmp189 == 2U) {

    } else {
      {
#line 420
      __assert_fail("(sc_tag[1]) == 2", "ray.c", 420U, "spu_main");
      }
    }
    }
    {
#line 421
    __cil_tmp190 = 1 << now_tag;
#line 421
    __cil_tmp191 = (unsigned int )__cil_tmp190;
#line 421
    if (now_mask == __cil_tmp191) {

    } else {
      {
#line 421
      __assert_fail("(now_mask) == ((1 << now_tag))", "ray.c", 421U, "spu_main");
      }
    }
    }
    {
#line 422
    __cil_tmp192 = 0 * 4UL;
#line 422
    __cil_tmp193 = (unsigned long )(sc_tag) + __cil_tmp192;
#line 422
    __cil_tmp194 = *((unsigned int *)__cil_tmp193);
#line 422
    __cil_tmp195 = 1 << __cil_tmp194;
#line 422
    __cil_tmp196 = (unsigned int )__cil_tmp195;
#line 422
    __cil_tmp197 = 0 * 4UL;
#line 422
    __cil_tmp198 = (unsigned long )(sc_mask) + __cil_tmp197;
#line 422
    __cil_tmp199 = *((unsigned int *)__cil_tmp198);
#line 422
    if (__cil_tmp199 == __cil_tmp196) {

    } else {
      {
#line 422
      __assert_fail("(sc_mask[0]) == ((1 << sc_tag[0]))", "ray.c", 422U, "spu_main");
      }
    }
    }
    {
#line 423
    __cil_tmp200 = 1 * 4UL;
#line 423
    __cil_tmp201 = (unsigned long )(sc_tag) + __cil_tmp200;
#line 423
    __cil_tmp202 = *((unsigned int *)__cil_tmp201);
#line 423
    __cil_tmp203 = 1 << __cil_tmp202;
#line 423
    __cil_tmp204 = (unsigned int )__cil_tmp203;
#line 423
    __cil_tmp205 = 1 * 4UL;
#line 423
    __cil_tmp206 = (unsigned long )(sc_mask) + __cil_tmp205;
#line 423
    __cil_tmp207 = *((unsigned int *)__cil_tmp206);
#line 423
    if (__cil_tmp207 == __cil_tmp204) {

    } else {
      {
#line 423
      __assert_fail("(sc_mask[1]) == ((1 << sc_tag[1]))", "ray.c", 423U, "spu_main");
      }
    }
    }
#line 425
    if (curr_buf == 0) {

    } else
#line 425
    if (curr_buf == 1) {

    } else {
      {
#line 425
      __assert_fail("(curr_buf == 0) || (curr_buf == 1)", "ray.c", 425U, "spu_main");
      }
    }
#line 427
    if (state == 0) {
      {
#line 429
      tmp___29 = __builtin_si_rdch(29);
#line 429
      tmp___30 = __builtin_si_to_uint(tmp___29);
#line 429
      opcode = (unsigned int )tmp___30;
#line 430
      __cil_tmp208 = opcode == 5U;
#line 430
      __CPROVER_assume(__cil_tmp208);
#line 432
      tmp___31 = __builtin_si_rdch(29);
#line 432
      tmp___32 = __builtin_si_to_uint(tmp___31);
#line 432
      ea_low = (unsigned int )tmp___32;
#line 433
      __cil_tmp209 = (void *)(& region);
#line 433
      __cil_tmp210 = (void volatile   *)__cil_tmp209;
#line 433
      __cil_tmp211 = (unsigned int )128UL;
#line 433
      mfc_get(__cil_tmp210, ea_low, __cil_tmp211, now_tag, 0U, 0U);
#line 435
      tmp___33 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp212 = 0 * 4UL;
#line 435
      __cil_tmp213 = 0 + __cil_tmp212;
#line 435
      __cil_tmp214 = (unsigned long )(& region) + __cil_tmp213;
#line 435
      *((float volatile   *)__cil_tmp214) = (float )tmp___33;
#line 435
      tmp___34 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp215 = 1 * 4UL;
#line 435
      __cil_tmp216 = 0 + __cil_tmp215;
#line 435
      __cil_tmp217 = (unsigned long )(& region) + __cil_tmp216;
#line 435
      *((float volatile   *)__cil_tmp217) = (float )tmp___34;
#line 435
      tmp___35 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp218 = 2 * 4UL;
#line 435
      __cil_tmp219 = 0 + __cil_tmp218;
#line 435
      __cil_tmp220 = (unsigned long )(& region) + __cil_tmp219;
#line 435
      *((float volatile   *)__cil_tmp220) = (float )tmp___35;
#line 435
      tmp___36 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp221 = 3 * 4UL;
#line 435
      __cil_tmp222 = 0 + __cil_tmp221;
#line 435
      __cil_tmp223 = (unsigned long )(& region) + __cil_tmp222;
#line 435
      *((float volatile   *)__cil_tmp223) = (float )tmp___36;
#line 435
      tmp___37 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp224 = 0 * 4UL;
#line 435
      __cil_tmp225 = 16 + __cil_tmp224;
#line 435
      __cil_tmp226 = (unsigned long )(& region) + __cil_tmp225;
#line 435
      *((float volatile   *)__cil_tmp226) = (float )tmp___37;
#line 435
      tmp___38 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp227 = 1 * 4UL;
#line 435
      __cil_tmp228 = 16 + __cil_tmp227;
#line 435
      __cil_tmp229 = (unsigned long )(& region) + __cil_tmp228;
#line 435
      *((float volatile   *)__cil_tmp229) = (float )tmp___38;
#line 435
      tmp___39 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp230 = 2 * 4UL;
#line 435
      __cil_tmp231 = 16 + __cil_tmp230;
#line 435
      __cil_tmp232 = (unsigned long )(& region) + __cil_tmp231;
#line 435
      *((float volatile   *)__cil_tmp232) = (float )tmp___39;
#line 435
      tmp___40 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp233 = 3 * 4UL;
#line 435
      __cil_tmp234 = 16 + __cil_tmp233;
#line 435
      __cil_tmp235 = (unsigned long )(& region) + __cil_tmp234;
#line 435
      *((float volatile   *)__cil_tmp235) = (float )tmp___40;
#line 435
      tmp___41 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp236 = 0 * 4UL;
#line 435
      __cil_tmp237 = 32 + __cil_tmp236;
#line 435
      __cil_tmp238 = (unsigned long )(& region) + __cil_tmp237;
#line 435
      *((float volatile   *)__cil_tmp238) = (float )tmp___41;
#line 435
      tmp___42 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp239 = 1 * 4UL;
#line 435
      __cil_tmp240 = 32 + __cil_tmp239;
#line 435
      __cil_tmp241 = (unsigned long )(& region) + __cil_tmp240;
#line 435
      *((float volatile   *)__cil_tmp241) = (float )tmp___42;
#line 435
      tmp___43 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp242 = 2 * 4UL;
#line 435
      __cil_tmp243 = 32 + __cil_tmp242;
#line 435
      __cil_tmp244 = (unsigned long )(& region) + __cil_tmp243;
#line 435
      *((float volatile   *)__cil_tmp244) = (float )tmp___43;
#line 435
      tmp___44 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp245 = 3 * 4UL;
#line 435
      __cil_tmp246 = 32 + __cil_tmp245;
#line 435
      __cil_tmp247 = (unsigned long )(& region) + __cil_tmp246;
#line 435
      *((float volatile   *)__cil_tmp247) = (float )tmp___44;
#line 435
      tmp___45 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp248 = 0 * 4UL;
#line 435
      __cil_tmp249 = 48 + __cil_tmp248;
#line 435
      __cil_tmp250 = (unsigned long )(& region) + __cil_tmp249;
#line 435
      *((float volatile   *)__cil_tmp250) = (float )tmp___45;
#line 435
      tmp___46 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp251 = 1 * 4UL;
#line 435
      __cil_tmp252 = 48 + __cil_tmp251;
#line 435
      __cil_tmp253 = (unsigned long )(& region) + __cil_tmp252;
#line 435
      *((float volatile   *)__cil_tmp253) = (float )tmp___46;
#line 435
      tmp___47 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp254 = 2 * 4UL;
#line 435
      __cil_tmp255 = 48 + __cil_tmp254;
#line 435
      __cil_tmp256 = (unsigned long )(& region) + __cil_tmp255;
#line 435
      *((float volatile   *)__cil_tmp256) = (float )tmp___47;
#line 435
      tmp___48 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp257 = 3 * 4UL;
#line 435
      __cil_tmp258 = 48 + __cil_tmp257;
#line 435
      __cil_tmp259 = (unsigned long )(& region) + __cil_tmp258;
#line 435
      *((float volatile   *)__cil_tmp259) = (float )tmp___48;
#line 435
      tmp___49 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp260 = 0 * 4UL;
#line 435
      __cil_tmp261 = 64 + __cil_tmp260;
#line 435
      __cil_tmp262 = (unsigned long )(& region) + __cil_tmp261;
#line 435
      *((float volatile   *)__cil_tmp262) = (float )tmp___49;
#line 435
      tmp___50 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp263 = 1 * 4UL;
#line 435
      __cil_tmp264 = 64 + __cil_tmp263;
#line 435
      __cil_tmp265 = (unsigned long )(& region) + __cil_tmp264;
#line 435
      *((float volatile   *)__cil_tmp265) = (float )tmp___50;
#line 435
      tmp___51 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp266 = 2 * 4UL;
#line 435
      __cil_tmp267 = 64 + __cil_tmp266;
#line 435
      __cil_tmp268 = (unsigned long )(& region) + __cil_tmp267;
#line 435
      *((float volatile   *)__cil_tmp268) = (float )tmp___51;
#line 435
      tmp___52 = __VERIFIER_nondet_float();
#line 435
      __cil_tmp269 = 3 * 4UL;
#line 435
      __cil_tmp270 = 64 + __cil_tmp269;
#line 435
      __cil_tmp271 = (unsigned long )(& region) + __cil_tmp270;
#line 435
      *((float volatile   *)__cil_tmp271) = (float )tmp___52;
#line 435
      tmp___53 = __VERIFIER_nondet_int();
#line 435
      __cil_tmp272 = (unsigned long )(& region) + 80;
#line 435
      *((unsigned int volatile   *)__cil_tmp272) = (unsigned int volatile   )tmp___53;
#line 435
      tmp___54 = __VERIFIER_nondet_int();
#line 435
      __cil_tmp273 = (unsigned long )(& region) + 84;
#line 435
      *((int volatile   *)__cil_tmp273) = (int volatile   )tmp___54;
#line 435
      tmp___55 = __VERIFIER_nondet_int();
#line 435
      __cil_tmp274 = (unsigned long )(& region) + 88;
#line 435
      *((int volatile   *)__cil_tmp274) = (int volatile   )tmp___55;
#line 436
      mfc_write_tag_mask(now_mask);
#line 437
      mfc_read_tag_status_all();
#line 444
      __cil_tmp275 = (unsigned long )(& region) + 80;
#line 444
      __cil_tmp276 = *((unsigned int volatile   *)__cil_tmp275);
#line 444
      fb_ea_low = (unsigned int )__cil_tmp276;
#line 445
      __cil_tmp277 = 1024UL * 16UL;
#line 445
      fb_row_stride = (unsigned int )__cil_tmp277;
#line 446
      num_rows = (unsigned int )imgHeight;
#line 447
      __cil_tmp278 = (unsigned long )(& region) + 88;
#line 447
      __cil_tmp279 = *((int volatile   *)__cil_tmp278);
#line 447
      num_columns = (unsigned int )__cil_tmp279;
      }
#line 449
      if (num_columns > 0U) {
        {
#line 449
        __cil_tmp280 = (unsigned int )FIXED_NUM_COLUMNS;
#line 449
        if (num_columns <= __cil_tmp280) {
#line 449
          tmp___56 = 1;
        } else {
#line 449
          tmp___56 = 0;
        }
        }
      } else {
#line 449
        tmp___56 = 0;
      }
      {
#line 449
      __CPROVER_assume(tmp___56);
#line 450
      __cil_tmp281 = (unsigned long )(& region) + 84;
#line 450
      __cil_tmp282 = *((int volatile   *)__cil_tmp281);
#line 450
      start_column = (unsigned int )__cil_tmp282;
#line 452
      __cil_tmp283 = start_column * fb_row_stride;
#line 452
      fb_store = fb_ea_low + __cil_tmp283;
#line 470
      i = 0U;
#line 472
      state = 1;
      }
    } else {

    }
#line 476
    if (state == 1) {
#line 478
      if (num_columns > 0U) {
        {
#line 478
        __cil_tmp284 = (unsigned int )FIXED_NUM_COLUMNS;
#line 478
        if (num_columns <= __cil_tmp284) {

        } else {
          {
#line 478
          __assert_fail("(num_columns>0) && (num_columns<=(FIXED_NUM_COLUMNS))", "ray.c",
                        478U, "spu_main");
          }
        }
        }
      } else {
        {
#line 478
        __assert_fail("(num_columns>0) && (num_columns<=(FIXED_NUM_COLUMNS))", "ray.c",
                      478U, "spu_main");
        }
      }
      {
#line 479
      __cil_tmp285 = i < num_columns;
#line 479
      if (! __cil_tmp285) {
#line 481
        state = 0;
      } else {
        {
#line 489
        __cil_tmp286 = curr_buf * 4UL;
#line 489
        __cil_tmp287 = (unsigned long )(sc_mask) + __cil_tmp286;
#line 489
        __cil_tmp288 = *((unsigned int *)__cil_tmp287);
#line 489
        mfc_write_tag_mask(__cil_tmp288);
#line 513
        __cil_tmp289 = 0 * 16UL;
#line 513
        __cil_tmp290 = curr_buf * 16384UL;
#line 513
        __cil_tmp291 = __cil_tmp290 + __cil_tmp289;
#line 513
        __cil_tmp292 = (unsigned long )(image_buf) + __cil_tmp291;
#line 513
        __cil_tmp293 = (float4 volatile   *)__cil_tmp292;
#line 513
        __cil_tmp294 = (void volatile   *)__cil_tmp293;
#line 513
        __cil_tmp295 = curr_buf * 4UL;
#line 513
        __cil_tmp296 = (unsigned long )(sc_tag) + __cil_tmp295;
#line 513
        __cil_tmp297 = *((unsigned int *)__cil_tmp296);
#line 513
        mfc_put(__cil_tmp294, fb_store, fb_row_stride, __cil_tmp297, 0U, 0U);
#line 515
        curr_buf = 1 - curr_buf;
#line 519
        fb_store = fb_store + fb_row_stride;
#line 521
        i = i + 1U;
        }
      }
      }
    } else {

    }
  }
  while_break: /* CIL Label */ ;
  }
#line 525
  return (0);
}
}
