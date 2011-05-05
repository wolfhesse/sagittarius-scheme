/* This file is automatically generated. DO NOT EDIT!*/
#define LIBSAGITTARIUS_BODY
#include "sagittarius.h"
#define WORD(x)       SG_WORD(x)
#define SYMBOL(x)     Sg_Intern(Sg_MakeString(UC(x), SG_LITERAL_STRING))
#define SYMBOLW(x)    WORD(SYMBOL(x))
#define STRING(x)     Sg_MakeString(UC(x), SG_LITERAL_STRING)
#define STRINGW(x)    WORD(STRING(x))
#define KEYWORD(x)    Sg_MakeKeyword(STRING(x))
#define KEYWORDW(x)    WORD(KEYWORD(x))
#define IDENT(x, lib) WORD(Sg_MakeIdentifier(SYMBOL(x), SG_NIL, (lib)))
static struct sg__wcRec {
  SgCodeBuilder cb[9];
  SgWord        w[517];
} sg__wc = {
  { /* code builder */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[26], NULL, 1, TRUE, 0, 16, 62), /* gcd */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[88], NULL, 1, FALSE, 1, 10, 29), /* #f */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[117], NULL, 1, TRUE, 0, 21, 96), /* lcm */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[213], NULL, 1, FALSE, 1, 10, 29), /* #f */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[242], NULL, 2, FALSE, 0, 11, 20), /* div-and-mod */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[262], NULL, 2, FALSE, 0, 11, 20), /* div0-and-mod0 */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[282], NULL, 4, FALSE, 0, 32, 113), /* bitwise-rotate-bit-field */
    SG_STATIC_CODE_BUILDER(&sg__wc.w[395], NULL, 3, FALSE, 0, 29, 122), /* bitwise-reverse-bit-field */
  },
  { /* compiled code */
  /*     0 */        0x0000002A           /* 0      (CLOSURE) */,
  /*     1 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*     2 */        0x00000037           /* 2      (DEFINE) */,
  /*     3 */        WORD(SG_UNDEF)  /* identifier#gcd */,
  /*     4 */        0x0000002A           /* 4      (CLOSURE) */,
  /*     5 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*     6 */        0x00000037           /* 6      (DEFINE) */,
  /*     7 */        WORD(SG_UNDEF)  /* identifier#lcm */,
  /*     8 */        0x0000002A           /* 8      (CLOSURE) */,
  /*     9 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*    10 */        0x00000037           /* 10     (DEFINE) */,
  /*    11 */        WORD(SG_UNDEF)  /* identifier#div-and-mod */,
  /*    12 */        0x0000002A           /* 12     (CLOSURE) */,
  /*    13 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*    14 */        0x00000037           /* 14     (DEFINE) */,
  /*    15 */        WORD(SG_UNDEF)  /* identifier#div0-and-mod0 */,
  /*    16 */        0x0000002A           /* 16     (CLOSURE) */,
  /*    17 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*    18 */        0x00000037           /* 18     (DEFINE) */,
  /*    19 */        WORD(SG_UNDEF)  /* identifier#bitwise-rotate-bit-field */,
  /*    20 */        0x0000002A           /* 20     (CLOSURE) */,
  /*    21 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*    22 */        0x00000037           /* 22     (DEFINE) */,
  /*    23 */        WORD(SG_UNDEF)  /* identifier#bitwise-reverse-bit-field */,
  /*    24 */        0x00000002           /* 24     (UNDEF) */,
  /*    25 */        0x00000001           /* 25     (HALT) */,
  /*    26 */        0x00000232           /* 0      (LET_FRAME) */,
  /*    27 */        0x00000047           /* 1      (LREF_PUSH) */,
  /*    28 */        0x00000134           /* 2      (DISPLAY) */,
  /*    29 */        0x00000031           /* 3      (FRAME) */,
  /*    30 */        WORD(SG_MAKE_INT(8)),
  /*    31 */        0x00000047           /* 5      (LREF_PUSH) */,
  /*    32 */        0x0000002A           /* 6      (CLOSURE) */,
  /*    33 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*    34 */        0x0000000B           /* 8      (PUSH) */,
  /*    35 */        0x00000047           /* 9      (LREF_PUSH) */,
  /*    36 */        0x0000024C           /* 10     (GREF_CALL) */,
  /*    37 */        WORD(SG_UNDEF)  /* identifier#map */,
  /*    38 */        0x0000000B           /* 12     (PUSH) */,
  /*    39 */        0x00000133           /* 13     (POP_LET_FRAME) */,
  /*    40 */        0x00000005           /* 14     (LREF) */,
  /*    41 */        0x00000022           /* 15     (BNNULL) */,
  /*    42 */        WORD(SG_MAKE_INT(5)),
  /*    43 */        0x00000003           /* 17     (CONST) */,
  /*    44 */        WORD(SG_MAKE_INT(0)),
  /*    45 */        0x00000018           /* 19     (JUMP) */,
  /*    46 */        WORD(SG_MAKE_INT(41)),
  /*    47 */        0x00000005           /* 21     (LREF) */,
  /*    48 */        0x00000039           /* 22     (CDR) */,
  /*    49 */        0x00000022           /* 23     (BNNULL) */,
  /*    50 */        WORD(SG_MAKE_INT(5)),
  /*    51 */        0x00000005           /* 25     (LREF) */,
  /*    52 */        0x00000038           /* 26     (CAR) */,
  /*    53 */        0x00000018           /* 27     (JUMP) */,
  /*    54 */        WORD(SG_MAKE_INT(33)),
  /*    55 */        0x00000332           /* 29     (LET_FRAME) */,
  /*    56 */        0x00000047           /* 30     (LREF_PUSH) */,
  /*    57 */        0x00000134           /* 31     (DISPLAY) */,
  /*    58 */        0x00000005           /* 32     (LREF) */,
  /*    59 */        0x00000038           /* 33     (CAR) */,
  /*    60 */        0x0000000B           /* 34     (PUSH) */,
  /*    61 */        0x00000005           /* 35     (LREF) */,
  /*    62 */        0x00000039           /* 36     (CDR) */,
  /*    63 */        0x0000000B           /* 37     (PUSH) */,
  /*    64 */        0x00000233           /* 38     (POP_LET_FRAME) */,
  /*    65 */        0x0000001A           /* 39     (MARK) */,
  /*    66 */        0x00000105           /* 40     (LREF) */,
  /*    67 */        0x00000022           /* 41     (BNNULL) */,
  /*    68 */        WORD(SG_MAKE_INT(4)),
  /*    69 */        0x00000005           /* 43     (LREF) */,
  /*    70 */        0x00000018           /* 44     (JUMP) */,
  /*    71 */        WORD(SG_MAKE_INT(16)),
  /*    72 */        0x00000031           /* 46     (FRAME) */,
  /*    73 */        WORD(SG_MAKE_INT(7)),
  /*    74 */        0x00000047           /* 48     (LREF_PUSH) */,
  /*    75 */        0x00000105           /* 49     (LREF) */,
  /*    76 */        0x00000038           /* 50     (CAR) */,
  /*    77 */        0x0000000B           /* 51     (PUSH) */,
  /*    78 */        0x0000024C           /* 52     (GREF_CALL) */,
  /*    79 */        WORD(SG_UNDEF)  /* identifier#%gcd */,
  /*    80 */        0x0000000B           /* 54     (PUSH) */,
  /*    81 */        0x00000105           /* 55     (LREF) */,
  /*    82 */        0x00000039           /* 56     (CDR) */,
  /*    83 */        0x0000000B           /* 57     (PUSH) */,
  /*    84 */        0x00000219           /* 58     (SHIFTJ) */,
  /*    85 */        0x00000018           /* 59     (JUMP) */,
  /*    86 */        WORD(SG_MAKE_INT(-20)),
  /*    87 */        0x00000030           /* 61     (RET) */,
  /*    88 */        0x00000031           /* 0      (FRAME) */,
  /*    89 */        WORD(SG_MAKE_INT(4)),
  /*    90 */        0x00000047           /* 2      (LREF_PUSH) */,
  /*    91 */        0x0000014C           /* 3      (GREF_CALL) */,
  /*    92 */        WORD(SG_UNDEF)  /* identifier#integer? */,
  /*    93 */        0x00000017           /* 5      (TEST) */,
  /*    94 */        WORD(SG_MAKE_INT(4)),
  /*    95 */        0x00000002           /* 7      (UNDEF) */,
  /*    96 */        0x00000018           /* 8      (JUMP) */,
  /*    97 */        WORD(SG_MAKE_INT(16)),
  /*    98 */        0x00000031           /* 10     (FRAME) */,
  /*    99 */        WORD(SG_MAKE_INT(14)),
  /*   100 */        0x0000004A           /* 12     (CONST_PUSH) */,
  /*   101 */        WORD(SG_UNDEF), /* gcd */
  /*   102 */        0x00000031           /* 14     (FRAME) */,
  /*   103 */        WORD(SG_MAKE_INT(6)),
  /*   104 */        0x0000004A           /* 16     (CONST_PUSH) */,
  /*   105 */        WORD(SG_UNDEF), /* "integer" */
  /*   106 */        0x00000047           /* 18     (LREF_PUSH) */,
  /*   107 */        0x0000024C           /* 19     (GREF_CALL) */,
  /*   108 */        WORD(SG_UNDEF)  /* identifier#wrong-type-argument-message */,
  /*   109 */        0x0000000B           /* 21     (PUSH) */,
  /*   110 */        0x00000048           /* 22     (FREF_PUSH) */,
  /*   111 */        0x0000034C           /* 23     (GREF_CALL) */,
  /*   112 */        WORD(SG_UNDEF)  /* identifier#assertion-violation */,
  /*   113 */        0x00000047           /* 25     (LREF_PUSH) */,
  /*   114 */        0x0000014D           /* 26     (GREF_TAIL_CALL) */,
  /*   115 */        WORD(SG_UNDEF)  /* identifier#abs */,
  /*   116 */        0x00000030           /* 28     (RET) */,
  /*   117 */        0x00000232           /* 0      (LET_FRAME) */,
  /*   118 */        0x00000047           /* 1      (LREF_PUSH) */,
  /*   119 */        0x00000134           /* 2      (DISPLAY) */,
  /*   120 */        0x00000031           /* 3      (FRAME) */,
  /*   121 */        WORD(SG_MAKE_INT(8)),
  /*   122 */        0x00000047           /* 5      (LREF_PUSH) */,
  /*   123 */        0x0000002A           /* 6      (CLOSURE) */,
  /*   124 */        WORD(SG_UNDEF)  /* <code-builder> */,
  /*   125 */        0x0000000B           /* 8      (PUSH) */,
  /*   126 */        0x00000047           /* 9      (LREF_PUSH) */,
  /*   127 */        0x0000024C           /* 10     (GREF_CALL) */,
  /*   128 */        WORD(SG_UNDEF)  /* identifier#map */,
  /*   129 */        0x0000000B           /* 12     (PUSH) */,
  /*   130 */        0x00000133           /* 13     (POP_LET_FRAME) */,
  /*   131 */        0x00000005           /* 14     (LREF) */,
  /*   132 */        0x00000022           /* 15     (BNNULL) */,
  /*   133 */        WORD(SG_MAKE_INT(5)),
  /*   134 */        0x00000003           /* 17     (CONST) */,
  /*   135 */        WORD(SG_MAKE_INT(1)),
  /*   136 */        0x00000018           /* 19     (JUMP) */,
  /*   137 */        WORD(SG_MAKE_INT(75)),
  /*   138 */        0x00000005           /* 21     (LREF) */,
  /*   139 */        0x00000039           /* 22     (CDR) */,
  /*   140 */        0x00000022           /* 23     (BNNULL) */,
  /*   141 */        WORD(SG_MAKE_INT(5)),
  /*   142 */        0x00000005           /* 25     (LREF) */,
  /*   143 */        0x00000038           /* 26     (CAR) */,
  /*   144 */        0x00000018           /* 27     (JUMP) */,
  /*   145 */        WORD(SG_MAKE_INT(67)),
  /*   146 */        0x00000332           /* 29     (LET_FRAME) */,
  /*   147 */        0x00000047           /* 30     (LREF_PUSH) */,
  /*   148 */        0x00000134           /* 31     (DISPLAY) */,
  /*   149 */        0x00000005           /* 32     (LREF) */,
  /*   150 */        0x00000038           /* 33     (CAR) */,
  /*   151 */        0x0000000B           /* 34     (PUSH) */,
  /*   152 */        0x00000005           /* 35     (LREF) */,
  /*   153 */        0x00000039           /* 36     (CDR) */,
  /*   154 */        0x0000000B           /* 37     (PUSH) */,
  /*   155 */        0x00000233           /* 38     (POP_LET_FRAME) */,
  /*   156 */        0x0000001A           /* 39     (MARK) */,
  /*   157 */        0x00000105           /* 40     (LREF) */,
  /*   158 */        0x00000022           /* 41     (BNNULL) */,
  /*   159 */        WORD(SG_MAKE_INT(4)),
  /*   160 */        0x00000005           /* 43     (LREF) */,
  /*   161 */        0x00000018           /* 44     (JUMP) */,
  /*   162 */        WORD(SG_MAKE_INT(50)),
  /*   163 */        0x00000232           /* 46     (LET_FRAME) */,
  /*   164 */        0x00000047           /* 47     (LREF_PUSH) */,
  /*   165 */        0x00000134           /* 48     (DISPLAY) */,
  /*   166 */        0x00000105           /* 49     (LREF) */,
  /*   167 */        0x00000038           /* 50     (CAR) */,
  /*   168 */        0x0000000B           /* 51     (PUSH) */,
  /*   169 */        0x00000135           /* 52     (ENTER) */,
  /*   170 */        0x00000432           /* 53     (LET_FRAME) */,
  /*   171 */        0x00000048           /* 54     (FREF_PUSH) */,
  /*   172 */        0x00000047           /* 55     (LREF_PUSH) */,
  /*   173 */        0x00000048           /* 56     (FREF_PUSH) */,
  /*   174 */        0x00000334           /* 57     (DISPLAY) */,
  /*   175 */        0x00000031           /* 58     (FRAME) */,
  /*   176 */        WORD(SG_MAKE_INT(5)),
  /*   177 */        0x00000048           /* 60     (FREF_PUSH) */,
  /*   178 */        0x00000047           /* 61     (LREF_PUSH) */,
  /*   179 */        0x0000024C           /* 62     (GREF_CALL) */,
  /*   180 */        WORD(SG_UNDEF)  /* identifier#%gcd */,
  /*   181 */        0x0000000B           /* 64     (PUSH) */,
  /*   182 */        0x00000135           /* 65     (ENTER) */,
  /*   183 */        0x00000031           /* 66     (FRAME) */,
  /*   184 */        WORD(SG_MAKE_INT(4)),
  /*   185 */        0x00000048           /* 68     (FREF_PUSH) */,
  /*   186 */        0x0000014C           /* 69     (GREF_CALL) */,
  /*   187 */        WORD(SG_UNDEF)  /* identifier#zero? */,
  /*   188 */        0x00000017           /* 71     (TEST) */,
  /*   189 */        WORD(SG_MAKE_INT(5)),
  /*   190 */        0x00000003           /* 73     (CONST) */,
  /*   191 */        WORD(SG_MAKE_INT(0)),
  /*   192 */        0x00000018           /* 75     (JUMP) */,
  /*   193 */        WORD(SG_MAKE_INT(10)),
  /*   194 */        0x00000031           /* 77     (FRAME) */,
  /*   195 */        WORD(SG_MAKE_INT(5)),
  /*   196 */        0x00000048           /* 79     (FREF_PUSH) */,
  /*   197 */        0x00000047           /* 80     (LREF_PUSH) */,
  /*   198 */        0x0000024C           /* 81     (GREF_CALL) */,
  /*   199 */        WORD(SG_UNDEF)  /* identifier#quotient */,
  /*   200 */        0x0000000B           /* 83     (PUSH) */,
  /*   201 */        0x00000107           /* 84     (FREF) */,
  /*   202 */        0x00000012           /* 85     (MUL) */,
  /*   203 */        0x00000036           /* 86     (LEAVE) */,
  /*   204 */        0x00000036           /* 87     (LEAVE) */,
  /*   205 */        0x0000000B           /* 88     (PUSH) */,
  /*   206 */        0x00000105           /* 89     (LREF) */,
  /*   207 */        0x00000039           /* 90     (CDR) */,
  /*   208 */        0x0000000B           /* 91     (PUSH) */,
  /*   209 */        0x00000219           /* 92     (SHIFTJ) */,
  /*   210 */        0x00000018           /* 93     (JUMP) */,
  /*   211 */        WORD(SG_MAKE_INT(-54)),
  /*   212 */        0x00000030           /* 95     (RET) */,
  /*   213 */        0x00000031           /* 0      (FRAME) */,
  /*   214 */        WORD(SG_MAKE_INT(4)),
  /*   215 */        0x00000047           /* 2      (LREF_PUSH) */,
  /*   216 */        0x0000014C           /* 3      (GREF_CALL) */,
  /*   217 */        WORD(SG_UNDEF)  /* identifier#integer? */,
  /*   218 */        0x00000017           /* 5      (TEST) */,
  /*   219 */        WORD(SG_MAKE_INT(4)),
  /*   220 */        0x00000002           /* 7      (UNDEF) */,
  /*   221 */        0x00000018           /* 8      (JUMP) */,
  /*   222 */        WORD(SG_MAKE_INT(16)),
  /*   223 */        0x00000031           /* 10     (FRAME) */,
  /*   224 */        WORD(SG_MAKE_INT(14)),
  /*   225 */        0x0000004A           /* 12     (CONST_PUSH) */,
  /*   226 */        WORD(SG_UNDEF), /* lcm */
  /*   227 */        0x00000031           /* 14     (FRAME) */,
  /*   228 */        WORD(SG_MAKE_INT(6)),
  /*   229 */        0x0000004A           /* 16     (CONST_PUSH) */,
  /*   230 */        WORD(SG_UNDEF), /* "integer" */
  /*   231 */        0x00000047           /* 18     (LREF_PUSH) */,
  /*   232 */        0x0000024C           /* 19     (GREF_CALL) */,
  /*   233 */        WORD(SG_UNDEF)  /* identifier#wrong-type-argument-message */,
  /*   234 */        0x0000000B           /* 21     (PUSH) */,
  /*   235 */        0x00000048           /* 22     (FREF_PUSH) */,
  /*   236 */        0x0000034C           /* 23     (GREF_CALL) */,
  /*   237 */        WORD(SG_UNDEF)  /* identifier#assertion-violation */,
  /*   238 */        0x00000047           /* 25     (LREF_PUSH) */,
  /*   239 */        0x0000014D           /* 26     (GREF_TAIL_CALL) */,
  /*   240 */        WORD(SG_UNDEF)  /* identifier#abs */,
  /*   241 */        0x00000030           /* 28     (RET) */,
  /*   242 */        0x00000232           /* 0      (LET_FRAME) */,
  /*   243 */        0x00000031           /* 1      (FRAME) */,
  /*   244 */        WORD(SG_MAKE_INT(5)),
  /*   245 */        0x00000047           /* 3      (LREF_PUSH) */,
  /*   246 */        0x00000147           /* 4      (LREF_PUSH) */,
  /*   247 */        0x0000024C           /* 5      (GREF_CALL) */,
  /*   248 */        WORD(SG_UNDEF)  /* identifier#div */,
  /*   249 */        0x0000000B           /* 7      (PUSH) */,
  /*   250 */        0x00000031           /* 8      (FRAME) */,
  /*   251 */        WORD(SG_MAKE_INT(5)),
  /*   252 */        0x00000047           /* 10     (LREF_PUSH) */,
  /*   253 */        0x00000147           /* 11     (LREF_PUSH) */,
  /*   254 */        0x0000024C           /* 12     (GREF_CALL) */,
  /*   255 */        WORD(SG_UNDEF)  /* identifier#mod */,
  /*   256 */        0x0000000B           /* 14     (PUSH) */,
  /*   257 */        0x00000233           /* 15     (POP_LET_FRAME) */,
  /*   258 */        0x00000047           /* 16     (LREF_PUSH) */,
  /*   259 */        0x00000105           /* 17     (LREF) */,
  /*   260 */        0x0000023C           /* 18     (VALUES) */,
  /*   261 */        0x00000030           /* 19     (RET) */,
  /*   262 */        0x00000232           /* 0      (LET_FRAME) */,
  /*   263 */        0x00000031           /* 1      (FRAME) */,
  /*   264 */        WORD(SG_MAKE_INT(5)),
  /*   265 */        0x00000047           /* 3      (LREF_PUSH) */,
  /*   266 */        0x00000147           /* 4      (LREF_PUSH) */,
  /*   267 */        0x0000024C           /* 5      (GREF_CALL) */,
  /*   268 */        WORD(SG_UNDEF)  /* identifier#div0 */,
  /*   269 */        0x0000000B           /* 7      (PUSH) */,
  /*   270 */        0x00000031           /* 8      (FRAME) */,
  /*   271 */        WORD(SG_MAKE_INT(5)),
  /*   272 */        0x00000047           /* 10     (LREF_PUSH) */,
  /*   273 */        0x00000147           /* 11     (LREF_PUSH) */,
  /*   274 */        0x0000024C           /* 12     (GREF_CALL) */,
  /*   275 */        WORD(SG_UNDEF)  /* identifier#mod0 */,
  /*   276 */        0x0000000B           /* 14     (PUSH) */,
  /*   277 */        0x00000233           /* 15     (POP_LET_FRAME) */,
  /*   278 */        0x00000047           /* 16     (LREF_PUSH) */,
  /*   279 */        0x00000105           /* 17     (LREF) */,
  /*   280 */        0x0000023C           /* 18     (VALUES) */,
  /*   281 */        0x00000030           /* 19     (RET) */,
  /*   282 */        0x00000532           /* 0      (LET_FRAME) */,
  /*   283 */        0x00000347           /* 1      (LREF_PUSH) */,
  /*   284 */        0x00000047           /* 2      (LREF_PUSH) */,
  /*   285 */        0x00000147           /* 3      (LREF_PUSH) */,
  /*   286 */        0x00000247           /* 4      (LREF_PUSH) */,
  /*   287 */        0x00000434           /* 5      (DISPLAY) */,
  /*   288 */        0x00000247           /* 6      (LREF_PUSH) */,
  /*   289 */        0x00000105           /* 7      (LREF) */,
  /*   290 */        0x00000010           /* 8      (SUB) */,
  /*   291 */        0x0000000B           /* 9      (PUSH) */,
  /*   292 */        0x00000133           /* 10     (POP_LET_FRAME) */,
  /*   293 */        0x00000031           /* 11     (FRAME) */,
  /*   294 */        WORD(SG_MAKE_INT(4)),
  /*   295 */        0x00000047           /* 13     (LREF_PUSH) */,
  /*   296 */        0x0000014C           /* 14     (GREF_CALL) */,
  /*   297 */        WORD(SG_UNDEF)  /* identifier#positive? */,
  /*   298 */        0x00000017           /* 16     (TEST) */,
  /*   299 */        WORD(SG_MAKE_INT(94)),
  /*   300 */        0x00000632           /* 18     (LET_FRAME) */,
  /*   301 */        0x00000248           /* 19     (FREF_PUSH) */,
  /*   302 */        0x00000148           /* 20     (FREF_PUSH) */,
  /*   303 */        0x00000048           /* 21     (FREF_PUSH) */,
  /*   304 */        0x00000047           /* 22     (LREF_PUSH) */,
  /*   305 */        0x00000348           /* 23     (FREF_PUSH) */,
  /*   306 */        0x00000534           /* 24     (DISPLAY) */,
  /*   307 */        0x00000031           /* 25     (FRAME) */,
  /*   308 */        WORD(SG_MAKE_INT(5)),
  /*   309 */        0x00000048           /* 27     (FREF_PUSH) */,
  /*   310 */        0x00000047           /* 28     (LREF_PUSH) */,
  /*   311 */        0x0000024C           /* 29     (GREF_CALL) */,
  /*   312 */        WORD(SG_UNDEF)  /* identifier#mod */,
  /*   313 */        0x0000000B           /* 31     (PUSH) */,
  /*   314 */        0x00000133           /* 32     (POP_LET_FRAME) */,
  /*   315 */        0x00000932           /* 33     (LET_FRAME) */,
  /*   316 */        0x00000047           /* 34     (LREF_PUSH) */,
  /*   317 */        0x00000148           /* 35     (FREF_PUSH) */,
  /*   318 */        0x00000448           /* 36     (FREF_PUSH) */,
  /*   319 */        0x00000348           /* 37     (FREF_PUSH) */,
  /*   320 */        0x00000248           /* 38     (FREF_PUSH) */,
  /*   321 */        0x00000448           /* 39     (FREF_PUSH) */,
  /*   322 */        0x00000348           /* 40     (FREF_PUSH) */,
  /*   323 */        0x00000248           /* 41     (FREF_PUSH) */,
  /*   324 */        0x00000834           /* 42     (DISPLAY) */,
  /*   325 */        0x00000031           /* 43     (FRAME) */,
  /*   326 */        WORD(SG_MAKE_INT(6)),
  /*   327 */        0x00000248           /* 45     (FREF_PUSH) */,
  /*   328 */        0x00000148           /* 46     (FREF_PUSH) */,
  /*   329 */        0x00000048           /* 47     (FREF_PUSH) */,
  /*   330 */        0x0000034C           /* 48     (GREF_CALL) */,
  /*   331 */        WORD(SG_UNDEF)  /* identifier#bitwise-bit-field */,
  /*   332 */        0x0000000B           /* 50     (PUSH) */,
  /*   333 */        0x00000133           /* 51     (POP_LET_FRAME) */,
  /*   334 */        0x00000832           /* 52     (LET_FRAME) */,
  /*   335 */        0x00000047           /* 53     (LREF_PUSH) */,
  /*   336 */        0x00000648           /* 54     (FREF_PUSH) */,
  /*   337 */        0x00000748           /* 55     (FREF_PUSH) */,
  /*   338 */        0x00000248           /* 56     (FREF_PUSH) */,
  /*   339 */        0x00000148           /* 57     (FREF_PUSH) */,
  /*   340 */        0x00000048           /* 58     (FREF_PUSH) */,
  /*   341 */        0x00000748           /* 59     (FREF_PUSH) */,
  /*   342 */        0x00000734           /* 60     (DISPLAY) */,
  /*   343 */        0x00000031           /* 61     (FRAME) */,
  /*   344 */        WORD(SG_MAKE_INT(5)),
  /*   345 */        0x00000047           /* 63     (LREF_PUSH) */,
  /*   346 */        0x00000048           /* 64     (FREF_PUSH) */,
  /*   347 */        0x0000024C           /* 65     (GREF_CALL) */,
  /*   348 */        WORD(SG_UNDEF)  /* identifier#bitwise-arithmetic-shift-left */,
  /*   349 */        0x0000000B           /* 67     (PUSH) */,
  /*   350 */        0x00000133           /* 68     (POP_LET_FRAME) */,
  /*   351 */        0x00000832           /* 69     (LET_FRAME) */,
  /*   352 */        0x00000047           /* 70     (LREF_PUSH) */,
  /*   353 */        0x00000348           /* 71     (FREF_PUSH) */,
  /*   354 */        0x00000248           /* 72     (FREF_PUSH) */,
  /*   355 */        0x00000148           /* 73     (FREF_PUSH) */,
  /*   356 */        0x00000648           /* 74     (FREF_PUSH) */,
  /*   357 */        0x00000548           /* 75     (FREF_PUSH) */,
  /*   358 */        0x00000048           /* 76     (FREF_PUSH) */,
  /*   359 */        0x00000734           /* 77     (DISPLAY) */,
  /*   360 */        0x00000031           /* 78     (FRAME) */,
  /*   361 */        WORD(SG_MAKE_INT(8)),
  /*   362 */        0x00000248           /* 80     (FREF_PUSH) */,
  /*   363 */        0x00000148           /* 81     (FREF_PUSH) */,
  /*   364 */        0x00000007           /* 82     (FREF) */,
  /*   365 */        0x00000010           /* 83     (SUB) */,
  /*   366 */        0x0000000B           /* 84     (PUSH) */,
  /*   367 */        0x0000024C           /* 85     (GREF_CALL) */,
  /*   368 */        WORD(SG_UNDEF)  /* identifier#bitwise-arithmetic-shift-right */,
  /*   369 */        0x0000000B           /* 87     (PUSH) */,
  /*   370 */        0x00000133           /* 88     (POP_LET_FRAME) */,
  /*   371 */        0x00000532           /* 89     (LET_FRAME) */,
  /*   372 */        0x00000548           /* 90     (FREF_PUSH) */,
  /*   373 */        0x00000448           /* 91     (FREF_PUSH) */,
  /*   374 */        0x00000348           /* 92     (FREF_PUSH) */,
  /*   375 */        0x00000648           /* 93     (FREF_PUSH) */,
  /*   376 */        0x00000434           /* 94     (DISPLAY) */,
  /*   377 */        0x00000031           /* 95     (FRAME) */,
  /*   378 */        WORD(SG_MAKE_INT(5)),
  /*   379 */        0x00000048           /* 97     (FREF_PUSH) */,
  /*   380 */        0x00000047           /* 98     (LREF_PUSH) */,
  /*   381 */        0x0000024C           /* 99     (GREF_CALL) */,
  /*   382 */        WORD(SG_UNDEF)  /* identifier#bitwise-ior */,
  /*   383 */        0x0000000B           /* 101    (PUSH) */,
  /*   384 */        0x00000133           /* 102    (POP_LET_FRAME) */,
  /*   385 */        0x00000348           /* 103    (FREF_PUSH) */,
  /*   386 */        0x00000248           /* 104    (FREF_PUSH) */,
  /*   387 */        0x00000148           /* 105    (FREF_PUSH) */,
  /*   388 */        0x00000047           /* 106    (LREF_PUSH) */,
  /*   389 */        0x0000044D           /* 107    (GREF_TAIL_CALL) */,
  /*   390 */        WORD(SG_UNDEF)  /* identifier#bitwise-copy-bit-field */,
  /*   391 */        0x00000018           /* 109    (JUMP) */,
  /*   392 */        WORD(SG_MAKE_INT(2)),
  /*   393 */        0x00000207           /* 111    (FREF) */,
  /*   394 */        0x00000030           /* 112    (RET) */,
  /*   395 */        0x00000432           /* 0      (LET_FRAME) */,
  /*   396 */        0x00000047           /* 1      (LREF_PUSH) */,
  /*   397 */        0x00000147           /* 2      (LREF_PUSH) */,
  /*   398 */        0x00000247           /* 3      (LREF_PUSH) */,
  /*   399 */        0x00000334           /* 4      (DISPLAY) */,
  /*   400 */        0x00000247           /* 5      (LREF_PUSH) */,
  /*   401 */        0x00000105           /* 6      (LREF) */,
  /*   402 */        0x00000010           /* 7      (SUB) */,
  /*   403 */        0x0000000B           /* 8      (PUSH) */,
  /*   404 */        0x00000133           /* 9      (POP_LET_FRAME) */,
  /*   405 */        0x00000031           /* 10     (FRAME) */,
  /*   406 */        WORD(SG_MAKE_INT(4)),
  /*   407 */        0x00000047           /* 12     (LREF_PUSH) */,
  /*   408 */        0x0000014C           /* 13     (GREF_CALL) */,
  /*   409 */        WORD(SG_UNDEF)  /* identifier#positive? */,
  /*   410 */        0x00000017           /* 15     (TEST) */,
  /*   411 */        WORD(SG_MAKE_INT(104)),
  /*   412 */        0x00000732           /* 17     (LET_FRAME) */,
  /*   413 */        0x00000248           /* 18     (FREF_PUSH) */,
  /*   414 */        0x00000148           /* 19     (FREF_PUSH) */,
  /*   415 */        0x00000048           /* 20     (FREF_PUSH) */,
  /*   416 */        0x00000047           /* 21     (LREF_PUSH) */,
  /*   417 */        0x00000434           /* 22     (DISPLAY) */,
  /*   418 */        0x0000004A           /* 23     (CONST_PUSH) */,
  /*   419 */        WORD(SG_MAKE_INT(0)),
  /*   420 */        0x00000031           /* 25     (FRAME) */,
  /*   421 */        WORD(SG_MAKE_INT(6)),
  /*   422 */        0x00000348           /* 27     (FREF_PUSH) */,
  /*   423 */        0x00000248           /* 28     (FREF_PUSH) */,
  /*   424 */        0x00000148           /* 29     (FREF_PUSH) */,
  /*   425 */        0x0000034C           /* 30     (GREF_CALL) */,
  /*   426 */        WORD(SG_UNDEF)  /* identifier#bitwise-bit-field */,
  /*   427 */        0x0000000B           /* 32     (PUSH) */,
  /*   428 */        0x00000047           /* 33     (LREF_PUSH) */,
  /*   429 */        0x00000333           /* 34     (POP_LET_FRAME) */,
  /*   430 */        0x0000001A           /* 35     (MARK) */,
  /*   431 */        0x00000031           /* 36     (FRAME) */,
  /*   432 */        WORD(SG_MAKE_INT(4)),
  /*   433 */        0x00000247           /* 38     (LREF_PUSH) */,
  /*   434 */        0x0000014C           /* 39     (GREF_CALL) */,
  /*   435 */        WORD(SG_UNDEF)  /* identifier#zero? */,
  /*   436 */        0x00000017           /* 41     (TEST) */,
  /*   437 */        WORD(SG_MAKE_INT(9)),
  /*   438 */        0x00000348           /* 43     (FREF_PUSH) */,
  /*   439 */        0x00000248           /* 44     (FREF_PUSH) */,
  /*   440 */        0x00000148           /* 45     (FREF_PUSH) */,
  /*   441 */        0x00000047           /* 46     (LREF_PUSH) */,
  /*   442 */        0x0000044D           /* 47     (GREF_TAIL_CALL) */,
  /*   443 */        WORD(SG_UNDEF)  /* identifier#bitwise-copy-bit-field */,
  /*   444 */        0x00000018           /* 49     (JUMP) */,
  /*   445 */        WORD(SG_MAKE_INT(68)),
  /*   446 */        0x00000031           /* 51     (FRAME) */,
  /*   447 */        WORD(SG_MAKE_INT(11)),
  /*   448 */        0x00000031           /* 53     (FRAME) */,
  /*   449 */        WORD(SG_MAKE_INT(6)),
  /*   450 */        0x00000147           /* 55     (LREF_PUSH) */,
  /*   451 */        0x0000004A           /* 56     (CONST_PUSH) */,
  /*   452 */        WORD(SG_MAKE_INT(1)),
  /*   453 */        0x0000024C           /* 58     (GREF_CALL) */,
  /*   454 */        WORD(SG_UNDEF)  /* identifier#bitwise-and */,
  /*   455 */        0x0000000B           /* 60     (PUSH) */,
  /*   456 */        0x0000014C           /* 61     (GREF_CALL) */,
  /*   457 */        WORD(SG_UNDEF)  /* identifier#zero? */,
  /*   458 */        0x00000017           /* 63     (TEST) */,
  /*   459 */        WORD(SG_MAKE_INT(25)),
  /*   460 */        0x00000031           /* 65     (FRAME) */,
  /*   461 */        WORD(SG_MAKE_INT(6)),
  /*   462 */        0x00000047           /* 67     (LREF_PUSH) */,
  /*   463 */        0x0000004A           /* 68     (CONST_PUSH) */,
  /*   464 */        WORD(SG_MAKE_INT(1)),
  /*   465 */        0x0000024C           /* 70     (GREF_CALL) */,
  /*   466 */        WORD(SG_UNDEF)  /* identifier#bitwise-arithmetic-shift */,
  /*   467 */        0x0000000B           /* 72     (PUSH) */,
  /*   468 */        0x00000031           /* 73     (FRAME) */,
  /*   469 */        WORD(SG_MAKE_INT(6)),
  /*   470 */        0x00000147           /* 75     (LREF_PUSH) */,
  /*   471 */        0x0000004A           /* 76     (CONST_PUSH) */,
  /*   472 */        WORD(SG_MAKE_INT(1)),
  /*   473 */        0x0000024C           /* 78     (GREF_CALL) */,
  /*   474 */        WORD(SG_UNDEF)  /* identifier#bitwise-arithmetic-shift-right */,
  /*   475 */        0x0000000B           /* 80     (PUSH) */,
  /*   476 */        0x00000205           /* 81     (LREF) */,
  /*   477 */        -0x000000F1          /* 82     (ADDI) */,
  /*   478 */        0x0000000B           /* 83     (PUSH) */,
  /*   479 */        0x00000319           /* 84     (SHIFTJ) */,
  /*   480 */        0x00000018           /* 85     (JUMP) */,
  /*   481 */        WORD(SG_MAKE_INT(-50)),
  /*   482 */        0x00000018           /* 87     (JUMP) */,
  /*   483 */        WORD(SG_MAKE_INT(30)),
  /*   484 */        0x00000031           /* 89     (FRAME) */,
  /*   485 */        WORD(SG_MAKE_INT(13)),
  /*   486 */        0x00000031           /* 91     (FRAME) */,
  /*   487 */        WORD(SG_MAKE_INT(6)),
  /*   488 */        0x00000047           /* 93     (LREF_PUSH) */,
  /*   489 */        0x0000004A           /* 94     (CONST_PUSH) */,
  /*   490 */        WORD(SG_MAKE_INT(1)),
  /*   491 */        0x0000024C           /* 96     (GREF_CALL) */,
  /*   492 */        WORD(SG_UNDEF)  /* identifier#bitwise-arithmetic-shift */,
  /*   493 */        0x0000000B           /* 98     (PUSH) */,
  /*   494 */        0x0000004A           /* 99     (CONST_PUSH) */,
  /*   495 */        WORD(SG_MAKE_INT(1)),
  /*   496 */        0x0000024C           /* 101    (GREF_CALL) */,
  /*   497 */        WORD(SG_UNDEF)  /* identifier#bitwise-ior */,
  /*   498 */        0x0000000B           /* 103    (PUSH) */,
  /*   499 */        0x00000031           /* 104    (FRAME) */,
  /*   500 */        WORD(SG_MAKE_INT(6)),
  /*   501 */        0x00000147           /* 106    (LREF_PUSH) */,
  /*   502 */        0x0000004A           /* 107    (CONST_PUSH) */,
  /*   503 */        WORD(SG_MAKE_INT(1)),
  /*   504 */        0x0000024C           /* 109    (GREF_CALL) */,
  /*   505 */        WORD(SG_UNDEF)  /* identifier#bitwise-arithmetic-shift-right */,
  /*   506 */        0x0000000B           /* 111    (PUSH) */,
  /*   507 */        0x00000205           /* 112    (LREF) */,
  /*   508 */        -0x000000F1          /* 113    (ADDI) */,
  /*   509 */        0x0000000B           /* 114    (PUSH) */,
  /*   510 */        0x00000319           /* 115    (SHIFTJ) */,
  /*   511 */        0x00000018           /* 116    (JUMP) */,
  /*   512 */        WORD(SG_MAKE_INT(-81)),
  /*   513 */        0x00000018           /* 118    (JUMP) */,
  /*   514 */        WORD(SG_MAKE_INT(2)),
  /*   515 */        0x00000207           /* 120    (FREF) */,
  /*   516 */        0x00000030           /* 121    (RET) */,
  }
};

static SgCodeBuilder toplevel_core_arithmetic = SG_STATIC_CODE_BUILDER(&sg__wc.w[0], SG_FALSE, 0, FALSE, 0, 0, 26);
void Sg__Init_core_arithmetic()
{
  SgLibrary *lib = Sg_FindLibrary(SYMBOL("(core arithmetic)"), TRUE);
  sg__wc.w[497] = IDENT("bitwise-ior", lib);
  sg__wc.w[255] = IDENT("mod", lib);
  sg__wc.w[180] = IDENT("%gcd", lib);
  sg__wc.w[13] = WORD(&sg__wc.cb[5]);
sg__wc.cb[5].name = SYMBOL("div0-and-mod0");
  sg__wc.w[112] = IDENT("assertion-violation", lib);
  sg__wc.w[382] = IDENT("bitwise-ior", lib);
  sg__wc.w[5] = WORD(&sg__wc.cb[2]);
sg__wc.cb[2].name = SYMBOL("lcm");
  sg__wc.w[115] = IDENT("abs", lib);
  sg__wc.w[331] = IDENT("bitwise-bit-field", lib);
  sg__wc.w[3] = IDENT("gcd", lib);
  sg__wc.w[108] = IDENT("wrong-type-argument-message", lib);
  sg__wc.w[435] = IDENT("zero?", lib);
  sg__wc.w[466] = IDENT("bitwise-arithmetic-shift", lib);
  sg__wc.w[237] = IDENT("assertion-violation", lib);
  sg__wc.w[199] = IDENT("quotient", lib);
  sg__wc.w[240] = IDENT("abs", lib);
  sg__wc.w[37] = IDENT("map", lib);
  sg__wc.w[348] = IDENT("bitwise-arithmetic-shift-left", lib);
  sg__wc.w[268] = IDENT("div0", lib);
  sg__wc.w[233] = IDENT("wrong-type-argument-message", lib);
  sg__wc.w[390] = IDENT("bitwise-copy-bit-field", lib);
  sg__wc.w[105] = STRINGW("integer");
  sg__wc.w[17] = WORD(&sg__wc.cb[6]);
sg__wc.cb[6].name = SYMBOL("bitwise-rotate-bit-field");
  sg__wc.w[248] = IDENT("div", lib);
  sg__wc.w[454] = IDENT("bitwise-and", lib);
  sg__wc.w[443] = IDENT("bitwise-copy-bit-field", lib);
  sg__wc.w[124] = WORD(&sg__wc.cb[3]);
sg__wc.cb[3].name = SG_MAKE_BOOL(FALSE);
  sg__wc.w[474] = IDENT("bitwise-arithmetic-shift-right", lib);
  sg__wc.w[128] = IDENT("map", lib);
  sg__wc.w[101] = SYMBOLW("gcd");
  sg__wc.w[1] = WORD(&sg__wc.cb[0]);
sg__wc.cb[0].name = SYMBOL("gcd");
  sg__wc.w[19] = IDENT("bitwise-rotate-bit-field", lib);
  sg__wc.w[492] = IDENT("bitwise-arithmetic-shift", lib);
  sg__wc.w[187] = IDENT("zero?", lib);
  sg__wc.w[409] = IDENT("positive?", lib);
  sg__wc.w[312] = IDENT("mod", lib);
  sg__wc.w[230] = STRINGW("integer");
  sg__wc.w[297] = IDENT("positive?", lib);
  sg__wc.w[7] = IDENT("lcm", lib);
  sg__wc.w[15] = IDENT("div0-and-mod0", lib);
  sg__wc.w[79] = IDENT("%gcd", lib);
  sg__wc.w[11] = IDENT("div-and-mod", lib);
  sg__wc.w[21] = WORD(&sg__wc.cb[7]);
sg__wc.cb[7].name = SYMBOL("bitwise-reverse-bit-field");
  sg__wc.w[426] = IDENT("bitwise-bit-field", lib);
  sg__wc.w[9] = WORD(&sg__wc.cb[4]);
sg__wc.cb[4].name = SYMBOL("div-and-mod");
  sg__wc.w[226] = SYMBOLW("lcm");
  sg__wc.w[23] = IDENT("bitwise-reverse-bit-field", lib);
  sg__wc.w[368] = IDENT("bitwise-arithmetic-shift-right", lib);
  sg__wc.w[33] = WORD(&sg__wc.cb[1]);
sg__wc.cb[1].name = SG_MAKE_BOOL(FALSE);
  sg__wc.w[505] = IDENT("bitwise-arithmetic-shift-right", lib);
  sg__wc.w[92] = IDENT("integer?", lib);
  sg__wc.w[457] = IDENT("zero?", lib);
  sg__wc.w[275] = IDENT("mod0", lib);
  sg__wc.w[217] = IDENT("integer?", lib);
  Sg_ImportLibrary(lib, SG_OBJ(SYMBOL("(core errors)")));
  Sg_ImportLibrary(lib, SG_OBJ(SYMBOL("null")));
  Sg_LibraryExportedSet(lib, Sg_Cons(Sg_Cons(KEYWORD("all"), SG_NIL), SG_NIL));
  Sg_VMExecute(SG_OBJ(&toplevel_core_arithmetic));
}
