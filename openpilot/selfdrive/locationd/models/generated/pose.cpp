#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7736453428140691368) {
   out_7736453428140691368[0] = delta_x[0] + nom_x[0];
   out_7736453428140691368[1] = delta_x[1] + nom_x[1];
   out_7736453428140691368[2] = delta_x[2] + nom_x[2];
   out_7736453428140691368[3] = delta_x[3] + nom_x[3];
   out_7736453428140691368[4] = delta_x[4] + nom_x[4];
   out_7736453428140691368[5] = delta_x[5] + nom_x[5];
   out_7736453428140691368[6] = delta_x[6] + nom_x[6];
   out_7736453428140691368[7] = delta_x[7] + nom_x[7];
   out_7736453428140691368[8] = delta_x[8] + nom_x[8];
   out_7736453428140691368[9] = delta_x[9] + nom_x[9];
   out_7736453428140691368[10] = delta_x[10] + nom_x[10];
   out_7736453428140691368[11] = delta_x[11] + nom_x[11];
   out_7736453428140691368[12] = delta_x[12] + nom_x[12];
   out_7736453428140691368[13] = delta_x[13] + nom_x[13];
   out_7736453428140691368[14] = delta_x[14] + nom_x[14];
   out_7736453428140691368[15] = delta_x[15] + nom_x[15];
   out_7736453428140691368[16] = delta_x[16] + nom_x[16];
   out_7736453428140691368[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6034861957144106997) {
   out_6034861957144106997[0] = -nom_x[0] + true_x[0];
   out_6034861957144106997[1] = -nom_x[1] + true_x[1];
   out_6034861957144106997[2] = -nom_x[2] + true_x[2];
   out_6034861957144106997[3] = -nom_x[3] + true_x[3];
   out_6034861957144106997[4] = -nom_x[4] + true_x[4];
   out_6034861957144106997[5] = -nom_x[5] + true_x[5];
   out_6034861957144106997[6] = -nom_x[6] + true_x[6];
   out_6034861957144106997[7] = -nom_x[7] + true_x[7];
   out_6034861957144106997[8] = -nom_x[8] + true_x[8];
   out_6034861957144106997[9] = -nom_x[9] + true_x[9];
   out_6034861957144106997[10] = -nom_x[10] + true_x[10];
   out_6034861957144106997[11] = -nom_x[11] + true_x[11];
   out_6034861957144106997[12] = -nom_x[12] + true_x[12];
   out_6034861957144106997[13] = -nom_x[13] + true_x[13];
   out_6034861957144106997[14] = -nom_x[14] + true_x[14];
   out_6034861957144106997[15] = -nom_x[15] + true_x[15];
   out_6034861957144106997[16] = -nom_x[16] + true_x[16];
   out_6034861957144106997[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_9008000713700484969) {
   out_9008000713700484969[0] = 1.0;
   out_9008000713700484969[1] = 0.0;
   out_9008000713700484969[2] = 0.0;
   out_9008000713700484969[3] = 0.0;
   out_9008000713700484969[4] = 0.0;
   out_9008000713700484969[5] = 0.0;
   out_9008000713700484969[6] = 0.0;
   out_9008000713700484969[7] = 0.0;
   out_9008000713700484969[8] = 0.0;
   out_9008000713700484969[9] = 0.0;
   out_9008000713700484969[10] = 0.0;
   out_9008000713700484969[11] = 0.0;
   out_9008000713700484969[12] = 0.0;
   out_9008000713700484969[13] = 0.0;
   out_9008000713700484969[14] = 0.0;
   out_9008000713700484969[15] = 0.0;
   out_9008000713700484969[16] = 0.0;
   out_9008000713700484969[17] = 0.0;
   out_9008000713700484969[18] = 0.0;
   out_9008000713700484969[19] = 1.0;
   out_9008000713700484969[20] = 0.0;
   out_9008000713700484969[21] = 0.0;
   out_9008000713700484969[22] = 0.0;
   out_9008000713700484969[23] = 0.0;
   out_9008000713700484969[24] = 0.0;
   out_9008000713700484969[25] = 0.0;
   out_9008000713700484969[26] = 0.0;
   out_9008000713700484969[27] = 0.0;
   out_9008000713700484969[28] = 0.0;
   out_9008000713700484969[29] = 0.0;
   out_9008000713700484969[30] = 0.0;
   out_9008000713700484969[31] = 0.0;
   out_9008000713700484969[32] = 0.0;
   out_9008000713700484969[33] = 0.0;
   out_9008000713700484969[34] = 0.0;
   out_9008000713700484969[35] = 0.0;
   out_9008000713700484969[36] = 0.0;
   out_9008000713700484969[37] = 0.0;
   out_9008000713700484969[38] = 1.0;
   out_9008000713700484969[39] = 0.0;
   out_9008000713700484969[40] = 0.0;
   out_9008000713700484969[41] = 0.0;
   out_9008000713700484969[42] = 0.0;
   out_9008000713700484969[43] = 0.0;
   out_9008000713700484969[44] = 0.0;
   out_9008000713700484969[45] = 0.0;
   out_9008000713700484969[46] = 0.0;
   out_9008000713700484969[47] = 0.0;
   out_9008000713700484969[48] = 0.0;
   out_9008000713700484969[49] = 0.0;
   out_9008000713700484969[50] = 0.0;
   out_9008000713700484969[51] = 0.0;
   out_9008000713700484969[52] = 0.0;
   out_9008000713700484969[53] = 0.0;
   out_9008000713700484969[54] = 0.0;
   out_9008000713700484969[55] = 0.0;
   out_9008000713700484969[56] = 0.0;
   out_9008000713700484969[57] = 1.0;
   out_9008000713700484969[58] = 0.0;
   out_9008000713700484969[59] = 0.0;
   out_9008000713700484969[60] = 0.0;
   out_9008000713700484969[61] = 0.0;
   out_9008000713700484969[62] = 0.0;
   out_9008000713700484969[63] = 0.0;
   out_9008000713700484969[64] = 0.0;
   out_9008000713700484969[65] = 0.0;
   out_9008000713700484969[66] = 0.0;
   out_9008000713700484969[67] = 0.0;
   out_9008000713700484969[68] = 0.0;
   out_9008000713700484969[69] = 0.0;
   out_9008000713700484969[70] = 0.0;
   out_9008000713700484969[71] = 0.0;
   out_9008000713700484969[72] = 0.0;
   out_9008000713700484969[73] = 0.0;
   out_9008000713700484969[74] = 0.0;
   out_9008000713700484969[75] = 0.0;
   out_9008000713700484969[76] = 1.0;
   out_9008000713700484969[77] = 0.0;
   out_9008000713700484969[78] = 0.0;
   out_9008000713700484969[79] = 0.0;
   out_9008000713700484969[80] = 0.0;
   out_9008000713700484969[81] = 0.0;
   out_9008000713700484969[82] = 0.0;
   out_9008000713700484969[83] = 0.0;
   out_9008000713700484969[84] = 0.0;
   out_9008000713700484969[85] = 0.0;
   out_9008000713700484969[86] = 0.0;
   out_9008000713700484969[87] = 0.0;
   out_9008000713700484969[88] = 0.0;
   out_9008000713700484969[89] = 0.0;
   out_9008000713700484969[90] = 0.0;
   out_9008000713700484969[91] = 0.0;
   out_9008000713700484969[92] = 0.0;
   out_9008000713700484969[93] = 0.0;
   out_9008000713700484969[94] = 0.0;
   out_9008000713700484969[95] = 1.0;
   out_9008000713700484969[96] = 0.0;
   out_9008000713700484969[97] = 0.0;
   out_9008000713700484969[98] = 0.0;
   out_9008000713700484969[99] = 0.0;
   out_9008000713700484969[100] = 0.0;
   out_9008000713700484969[101] = 0.0;
   out_9008000713700484969[102] = 0.0;
   out_9008000713700484969[103] = 0.0;
   out_9008000713700484969[104] = 0.0;
   out_9008000713700484969[105] = 0.0;
   out_9008000713700484969[106] = 0.0;
   out_9008000713700484969[107] = 0.0;
   out_9008000713700484969[108] = 0.0;
   out_9008000713700484969[109] = 0.0;
   out_9008000713700484969[110] = 0.0;
   out_9008000713700484969[111] = 0.0;
   out_9008000713700484969[112] = 0.0;
   out_9008000713700484969[113] = 0.0;
   out_9008000713700484969[114] = 1.0;
   out_9008000713700484969[115] = 0.0;
   out_9008000713700484969[116] = 0.0;
   out_9008000713700484969[117] = 0.0;
   out_9008000713700484969[118] = 0.0;
   out_9008000713700484969[119] = 0.0;
   out_9008000713700484969[120] = 0.0;
   out_9008000713700484969[121] = 0.0;
   out_9008000713700484969[122] = 0.0;
   out_9008000713700484969[123] = 0.0;
   out_9008000713700484969[124] = 0.0;
   out_9008000713700484969[125] = 0.0;
   out_9008000713700484969[126] = 0.0;
   out_9008000713700484969[127] = 0.0;
   out_9008000713700484969[128] = 0.0;
   out_9008000713700484969[129] = 0.0;
   out_9008000713700484969[130] = 0.0;
   out_9008000713700484969[131] = 0.0;
   out_9008000713700484969[132] = 0.0;
   out_9008000713700484969[133] = 1.0;
   out_9008000713700484969[134] = 0.0;
   out_9008000713700484969[135] = 0.0;
   out_9008000713700484969[136] = 0.0;
   out_9008000713700484969[137] = 0.0;
   out_9008000713700484969[138] = 0.0;
   out_9008000713700484969[139] = 0.0;
   out_9008000713700484969[140] = 0.0;
   out_9008000713700484969[141] = 0.0;
   out_9008000713700484969[142] = 0.0;
   out_9008000713700484969[143] = 0.0;
   out_9008000713700484969[144] = 0.0;
   out_9008000713700484969[145] = 0.0;
   out_9008000713700484969[146] = 0.0;
   out_9008000713700484969[147] = 0.0;
   out_9008000713700484969[148] = 0.0;
   out_9008000713700484969[149] = 0.0;
   out_9008000713700484969[150] = 0.0;
   out_9008000713700484969[151] = 0.0;
   out_9008000713700484969[152] = 1.0;
   out_9008000713700484969[153] = 0.0;
   out_9008000713700484969[154] = 0.0;
   out_9008000713700484969[155] = 0.0;
   out_9008000713700484969[156] = 0.0;
   out_9008000713700484969[157] = 0.0;
   out_9008000713700484969[158] = 0.0;
   out_9008000713700484969[159] = 0.0;
   out_9008000713700484969[160] = 0.0;
   out_9008000713700484969[161] = 0.0;
   out_9008000713700484969[162] = 0.0;
   out_9008000713700484969[163] = 0.0;
   out_9008000713700484969[164] = 0.0;
   out_9008000713700484969[165] = 0.0;
   out_9008000713700484969[166] = 0.0;
   out_9008000713700484969[167] = 0.0;
   out_9008000713700484969[168] = 0.0;
   out_9008000713700484969[169] = 0.0;
   out_9008000713700484969[170] = 0.0;
   out_9008000713700484969[171] = 1.0;
   out_9008000713700484969[172] = 0.0;
   out_9008000713700484969[173] = 0.0;
   out_9008000713700484969[174] = 0.0;
   out_9008000713700484969[175] = 0.0;
   out_9008000713700484969[176] = 0.0;
   out_9008000713700484969[177] = 0.0;
   out_9008000713700484969[178] = 0.0;
   out_9008000713700484969[179] = 0.0;
   out_9008000713700484969[180] = 0.0;
   out_9008000713700484969[181] = 0.0;
   out_9008000713700484969[182] = 0.0;
   out_9008000713700484969[183] = 0.0;
   out_9008000713700484969[184] = 0.0;
   out_9008000713700484969[185] = 0.0;
   out_9008000713700484969[186] = 0.0;
   out_9008000713700484969[187] = 0.0;
   out_9008000713700484969[188] = 0.0;
   out_9008000713700484969[189] = 0.0;
   out_9008000713700484969[190] = 1.0;
   out_9008000713700484969[191] = 0.0;
   out_9008000713700484969[192] = 0.0;
   out_9008000713700484969[193] = 0.0;
   out_9008000713700484969[194] = 0.0;
   out_9008000713700484969[195] = 0.0;
   out_9008000713700484969[196] = 0.0;
   out_9008000713700484969[197] = 0.0;
   out_9008000713700484969[198] = 0.0;
   out_9008000713700484969[199] = 0.0;
   out_9008000713700484969[200] = 0.0;
   out_9008000713700484969[201] = 0.0;
   out_9008000713700484969[202] = 0.0;
   out_9008000713700484969[203] = 0.0;
   out_9008000713700484969[204] = 0.0;
   out_9008000713700484969[205] = 0.0;
   out_9008000713700484969[206] = 0.0;
   out_9008000713700484969[207] = 0.0;
   out_9008000713700484969[208] = 0.0;
   out_9008000713700484969[209] = 1.0;
   out_9008000713700484969[210] = 0.0;
   out_9008000713700484969[211] = 0.0;
   out_9008000713700484969[212] = 0.0;
   out_9008000713700484969[213] = 0.0;
   out_9008000713700484969[214] = 0.0;
   out_9008000713700484969[215] = 0.0;
   out_9008000713700484969[216] = 0.0;
   out_9008000713700484969[217] = 0.0;
   out_9008000713700484969[218] = 0.0;
   out_9008000713700484969[219] = 0.0;
   out_9008000713700484969[220] = 0.0;
   out_9008000713700484969[221] = 0.0;
   out_9008000713700484969[222] = 0.0;
   out_9008000713700484969[223] = 0.0;
   out_9008000713700484969[224] = 0.0;
   out_9008000713700484969[225] = 0.0;
   out_9008000713700484969[226] = 0.0;
   out_9008000713700484969[227] = 0.0;
   out_9008000713700484969[228] = 1.0;
   out_9008000713700484969[229] = 0.0;
   out_9008000713700484969[230] = 0.0;
   out_9008000713700484969[231] = 0.0;
   out_9008000713700484969[232] = 0.0;
   out_9008000713700484969[233] = 0.0;
   out_9008000713700484969[234] = 0.0;
   out_9008000713700484969[235] = 0.0;
   out_9008000713700484969[236] = 0.0;
   out_9008000713700484969[237] = 0.0;
   out_9008000713700484969[238] = 0.0;
   out_9008000713700484969[239] = 0.0;
   out_9008000713700484969[240] = 0.0;
   out_9008000713700484969[241] = 0.0;
   out_9008000713700484969[242] = 0.0;
   out_9008000713700484969[243] = 0.0;
   out_9008000713700484969[244] = 0.0;
   out_9008000713700484969[245] = 0.0;
   out_9008000713700484969[246] = 0.0;
   out_9008000713700484969[247] = 1.0;
   out_9008000713700484969[248] = 0.0;
   out_9008000713700484969[249] = 0.0;
   out_9008000713700484969[250] = 0.0;
   out_9008000713700484969[251] = 0.0;
   out_9008000713700484969[252] = 0.0;
   out_9008000713700484969[253] = 0.0;
   out_9008000713700484969[254] = 0.0;
   out_9008000713700484969[255] = 0.0;
   out_9008000713700484969[256] = 0.0;
   out_9008000713700484969[257] = 0.0;
   out_9008000713700484969[258] = 0.0;
   out_9008000713700484969[259] = 0.0;
   out_9008000713700484969[260] = 0.0;
   out_9008000713700484969[261] = 0.0;
   out_9008000713700484969[262] = 0.0;
   out_9008000713700484969[263] = 0.0;
   out_9008000713700484969[264] = 0.0;
   out_9008000713700484969[265] = 0.0;
   out_9008000713700484969[266] = 1.0;
   out_9008000713700484969[267] = 0.0;
   out_9008000713700484969[268] = 0.0;
   out_9008000713700484969[269] = 0.0;
   out_9008000713700484969[270] = 0.0;
   out_9008000713700484969[271] = 0.0;
   out_9008000713700484969[272] = 0.0;
   out_9008000713700484969[273] = 0.0;
   out_9008000713700484969[274] = 0.0;
   out_9008000713700484969[275] = 0.0;
   out_9008000713700484969[276] = 0.0;
   out_9008000713700484969[277] = 0.0;
   out_9008000713700484969[278] = 0.0;
   out_9008000713700484969[279] = 0.0;
   out_9008000713700484969[280] = 0.0;
   out_9008000713700484969[281] = 0.0;
   out_9008000713700484969[282] = 0.0;
   out_9008000713700484969[283] = 0.0;
   out_9008000713700484969[284] = 0.0;
   out_9008000713700484969[285] = 1.0;
   out_9008000713700484969[286] = 0.0;
   out_9008000713700484969[287] = 0.0;
   out_9008000713700484969[288] = 0.0;
   out_9008000713700484969[289] = 0.0;
   out_9008000713700484969[290] = 0.0;
   out_9008000713700484969[291] = 0.0;
   out_9008000713700484969[292] = 0.0;
   out_9008000713700484969[293] = 0.0;
   out_9008000713700484969[294] = 0.0;
   out_9008000713700484969[295] = 0.0;
   out_9008000713700484969[296] = 0.0;
   out_9008000713700484969[297] = 0.0;
   out_9008000713700484969[298] = 0.0;
   out_9008000713700484969[299] = 0.0;
   out_9008000713700484969[300] = 0.0;
   out_9008000713700484969[301] = 0.0;
   out_9008000713700484969[302] = 0.0;
   out_9008000713700484969[303] = 0.0;
   out_9008000713700484969[304] = 1.0;
   out_9008000713700484969[305] = 0.0;
   out_9008000713700484969[306] = 0.0;
   out_9008000713700484969[307] = 0.0;
   out_9008000713700484969[308] = 0.0;
   out_9008000713700484969[309] = 0.0;
   out_9008000713700484969[310] = 0.0;
   out_9008000713700484969[311] = 0.0;
   out_9008000713700484969[312] = 0.0;
   out_9008000713700484969[313] = 0.0;
   out_9008000713700484969[314] = 0.0;
   out_9008000713700484969[315] = 0.0;
   out_9008000713700484969[316] = 0.0;
   out_9008000713700484969[317] = 0.0;
   out_9008000713700484969[318] = 0.0;
   out_9008000713700484969[319] = 0.0;
   out_9008000713700484969[320] = 0.0;
   out_9008000713700484969[321] = 0.0;
   out_9008000713700484969[322] = 0.0;
   out_9008000713700484969[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_8514240899757441396) {
   out_8514240899757441396[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_8514240899757441396[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_8514240899757441396[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_8514240899757441396[3] = dt*state[12] + state[3];
   out_8514240899757441396[4] = dt*state[13] + state[4];
   out_8514240899757441396[5] = dt*state[14] + state[5];
   out_8514240899757441396[6] = state[6];
   out_8514240899757441396[7] = state[7];
   out_8514240899757441396[8] = state[8];
   out_8514240899757441396[9] = state[9];
   out_8514240899757441396[10] = state[10];
   out_8514240899757441396[11] = state[11];
   out_8514240899757441396[12] = state[12];
   out_8514240899757441396[13] = state[13];
   out_8514240899757441396[14] = state[14];
   out_8514240899757441396[15] = state[15];
   out_8514240899757441396[16] = state[16];
   out_8514240899757441396[17] = state[17];
}
void F_fun(double *state, double dt, double *out_3669342035074408446) {
   out_3669342035074408446[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3669342035074408446[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3669342035074408446[2] = 0;
   out_3669342035074408446[3] = 0;
   out_3669342035074408446[4] = 0;
   out_3669342035074408446[5] = 0;
   out_3669342035074408446[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3669342035074408446[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3669342035074408446[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_3669342035074408446[9] = 0;
   out_3669342035074408446[10] = 0;
   out_3669342035074408446[11] = 0;
   out_3669342035074408446[12] = 0;
   out_3669342035074408446[13] = 0;
   out_3669342035074408446[14] = 0;
   out_3669342035074408446[15] = 0;
   out_3669342035074408446[16] = 0;
   out_3669342035074408446[17] = 0;
   out_3669342035074408446[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3669342035074408446[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3669342035074408446[20] = 0;
   out_3669342035074408446[21] = 0;
   out_3669342035074408446[22] = 0;
   out_3669342035074408446[23] = 0;
   out_3669342035074408446[24] = 0;
   out_3669342035074408446[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3669342035074408446[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_3669342035074408446[27] = 0;
   out_3669342035074408446[28] = 0;
   out_3669342035074408446[29] = 0;
   out_3669342035074408446[30] = 0;
   out_3669342035074408446[31] = 0;
   out_3669342035074408446[32] = 0;
   out_3669342035074408446[33] = 0;
   out_3669342035074408446[34] = 0;
   out_3669342035074408446[35] = 0;
   out_3669342035074408446[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3669342035074408446[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3669342035074408446[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3669342035074408446[39] = 0;
   out_3669342035074408446[40] = 0;
   out_3669342035074408446[41] = 0;
   out_3669342035074408446[42] = 0;
   out_3669342035074408446[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3669342035074408446[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_3669342035074408446[45] = 0;
   out_3669342035074408446[46] = 0;
   out_3669342035074408446[47] = 0;
   out_3669342035074408446[48] = 0;
   out_3669342035074408446[49] = 0;
   out_3669342035074408446[50] = 0;
   out_3669342035074408446[51] = 0;
   out_3669342035074408446[52] = 0;
   out_3669342035074408446[53] = 0;
   out_3669342035074408446[54] = 0;
   out_3669342035074408446[55] = 0;
   out_3669342035074408446[56] = 0;
   out_3669342035074408446[57] = 1;
   out_3669342035074408446[58] = 0;
   out_3669342035074408446[59] = 0;
   out_3669342035074408446[60] = 0;
   out_3669342035074408446[61] = 0;
   out_3669342035074408446[62] = 0;
   out_3669342035074408446[63] = 0;
   out_3669342035074408446[64] = 0;
   out_3669342035074408446[65] = 0;
   out_3669342035074408446[66] = dt;
   out_3669342035074408446[67] = 0;
   out_3669342035074408446[68] = 0;
   out_3669342035074408446[69] = 0;
   out_3669342035074408446[70] = 0;
   out_3669342035074408446[71] = 0;
   out_3669342035074408446[72] = 0;
   out_3669342035074408446[73] = 0;
   out_3669342035074408446[74] = 0;
   out_3669342035074408446[75] = 0;
   out_3669342035074408446[76] = 1;
   out_3669342035074408446[77] = 0;
   out_3669342035074408446[78] = 0;
   out_3669342035074408446[79] = 0;
   out_3669342035074408446[80] = 0;
   out_3669342035074408446[81] = 0;
   out_3669342035074408446[82] = 0;
   out_3669342035074408446[83] = 0;
   out_3669342035074408446[84] = 0;
   out_3669342035074408446[85] = dt;
   out_3669342035074408446[86] = 0;
   out_3669342035074408446[87] = 0;
   out_3669342035074408446[88] = 0;
   out_3669342035074408446[89] = 0;
   out_3669342035074408446[90] = 0;
   out_3669342035074408446[91] = 0;
   out_3669342035074408446[92] = 0;
   out_3669342035074408446[93] = 0;
   out_3669342035074408446[94] = 0;
   out_3669342035074408446[95] = 1;
   out_3669342035074408446[96] = 0;
   out_3669342035074408446[97] = 0;
   out_3669342035074408446[98] = 0;
   out_3669342035074408446[99] = 0;
   out_3669342035074408446[100] = 0;
   out_3669342035074408446[101] = 0;
   out_3669342035074408446[102] = 0;
   out_3669342035074408446[103] = 0;
   out_3669342035074408446[104] = dt;
   out_3669342035074408446[105] = 0;
   out_3669342035074408446[106] = 0;
   out_3669342035074408446[107] = 0;
   out_3669342035074408446[108] = 0;
   out_3669342035074408446[109] = 0;
   out_3669342035074408446[110] = 0;
   out_3669342035074408446[111] = 0;
   out_3669342035074408446[112] = 0;
   out_3669342035074408446[113] = 0;
   out_3669342035074408446[114] = 1;
   out_3669342035074408446[115] = 0;
   out_3669342035074408446[116] = 0;
   out_3669342035074408446[117] = 0;
   out_3669342035074408446[118] = 0;
   out_3669342035074408446[119] = 0;
   out_3669342035074408446[120] = 0;
   out_3669342035074408446[121] = 0;
   out_3669342035074408446[122] = 0;
   out_3669342035074408446[123] = 0;
   out_3669342035074408446[124] = 0;
   out_3669342035074408446[125] = 0;
   out_3669342035074408446[126] = 0;
   out_3669342035074408446[127] = 0;
   out_3669342035074408446[128] = 0;
   out_3669342035074408446[129] = 0;
   out_3669342035074408446[130] = 0;
   out_3669342035074408446[131] = 0;
   out_3669342035074408446[132] = 0;
   out_3669342035074408446[133] = 1;
   out_3669342035074408446[134] = 0;
   out_3669342035074408446[135] = 0;
   out_3669342035074408446[136] = 0;
   out_3669342035074408446[137] = 0;
   out_3669342035074408446[138] = 0;
   out_3669342035074408446[139] = 0;
   out_3669342035074408446[140] = 0;
   out_3669342035074408446[141] = 0;
   out_3669342035074408446[142] = 0;
   out_3669342035074408446[143] = 0;
   out_3669342035074408446[144] = 0;
   out_3669342035074408446[145] = 0;
   out_3669342035074408446[146] = 0;
   out_3669342035074408446[147] = 0;
   out_3669342035074408446[148] = 0;
   out_3669342035074408446[149] = 0;
   out_3669342035074408446[150] = 0;
   out_3669342035074408446[151] = 0;
   out_3669342035074408446[152] = 1;
   out_3669342035074408446[153] = 0;
   out_3669342035074408446[154] = 0;
   out_3669342035074408446[155] = 0;
   out_3669342035074408446[156] = 0;
   out_3669342035074408446[157] = 0;
   out_3669342035074408446[158] = 0;
   out_3669342035074408446[159] = 0;
   out_3669342035074408446[160] = 0;
   out_3669342035074408446[161] = 0;
   out_3669342035074408446[162] = 0;
   out_3669342035074408446[163] = 0;
   out_3669342035074408446[164] = 0;
   out_3669342035074408446[165] = 0;
   out_3669342035074408446[166] = 0;
   out_3669342035074408446[167] = 0;
   out_3669342035074408446[168] = 0;
   out_3669342035074408446[169] = 0;
   out_3669342035074408446[170] = 0;
   out_3669342035074408446[171] = 1;
   out_3669342035074408446[172] = 0;
   out_3669342035074408446[173] = 0;
   out_3669342035074408446[174] = 0;
   out_3669342035074408446[175] = 0;
   out_3669342035074408446[176] = 0;
   out_3669342035074408446[177] = 0;
   out_3669342035074408446[178] = 0;
   out_3669342035074408446[179] = 0;
   out_3669342035074408446[180] = 0;
   out_3669342035074408446[181] = 0;
   out_3669342035074408446[182] = 0;
   out_3669342035074408446[183] = 0;
   out_3669342035074408446[184] = 0;
   out_3669342035074408446[185] = 0;
   out_3669342035074408446[186] = 0;
   out_3669342035074408446[187] = 0;
   out_3669342035074408446[188] = 0;
   out_3669342035074408446[189] = 0;
   out_3669342035074408446[190] = 1;
   out_3669342035074408446[191] = 0;
   out_3669342035074408446[192] = 0;
   out_3669342035074408446[193] = 0;
   out_3669342035074408446[194] = 0;
   out_3669342035074408446[195] = 0;
   out_3669342035074408446[196] = 0;
   out_3669342035074408446[197] = 0;
   out_3669342035074408446[198] = 0;
   out_3669342035074408446[199] = 0;
   out_3669342035074408446[200] = 0;
   out_3669342035074408446[201] = 0;
   out_3669342035074408446[202] = 0;
   out_3669342035074408446[203] = 0;
   out_3669342035074408446[204] = 0;
   out_3669342035074408446[205] = 0;
   out_3669342035074408446[206] = 0;
   out_3669342035074408446[207] = 0;
   out_3669342035074408446[208] = 0;
   out_3669342035074408446[209] = 1;
   out_3669342035074408446[210] = 0;
   out_3669342035074408446[211] = 0;
   out_3669342035074408446[212] = 0;
   out_3669342035074408446[213] = 0;
   out_3669342035074408446[214] = 0;
   out_3669342035074408446[215] = 0;
   out_3669342035074408446[216] = 0;
   out_3669342035074408446[217] = 0;
   out_3669342035074408446[218] = 0;
   out_3669342035074408446[219] = 0;
   out_3669342035074408446[220] = 0;
   out_3669342035074408446[221] = 0;
   out_3669342035074408446[222] = 0;
   out_3669342035074408446[223] = 0;
   out_3669342035074408446[224] = 0;
   out_3669342035074408446[225] = 0;
   out_3669342035074408446[226] = 0;
   out_3669342035074408446[227] = 0;
   out_3669342035074408446[228] = 1;
   out_3669342035074408446[229] = 0;
   out_3669342035074408446[230] = 0;
   out_3669342035074408446[231] = 0;
   out_3669342035074408446[232] = 0;
   out_3669342035074408446[233] = 0;
   out_3669342035074408446[234] = 0;
   out_3669342035074408446[235] = 0;
   out_3669342035074408446[236] = 0;
   out_3669342035074408446[237] = 0;
   out_3669342035074408446[238] = 0;
   out_3669342035074408446[239] = 0;
   out_3669342035074408446[240] = 0;
   out_3669342035074408446[241] = 0;
   out_3669342035074408446[242] = 0;
   out_3669342035074408446[243] = 0;
   out_3669342035074408446[244] = 0;
   out_3669342035074408446[245] = 0;
   out_3669342035074408446[246] = 0;
   out_3669342035074408446[247] = 1;
   out_3669342035074408446[248] = 0;
   out_3669342035074408446[249] = 0;
   out_3669342035074408446[250] = 0;
   out_3669342035074408446[251] = 0;
   out_3669342035074408446[252] = 0;
   out_3669342035074408446[253] = 0;
   out_3669342035074408446[254] = 0;
   out_3669342035074408446[255] = 0;
   out_3669342035074408446[256] = 0;
   out_3669342035074408446[257] = 0;
   out_3669342035074408446[258] = 0;
   out_3669342035074408446[259] = 0;
   out_3669342035074408446[260] = 0;
   out_3669342035074408446[261] = 0;
   out_3669342035074408446[262] = 0;
   out_3669342035074408446[263] = 0;
   out_3669342035074408446[264] = 0;
   out_3669342035074408446[265] = 0;
   out_3669342035074408446[266] = 1;
   out_3669342035074408446[267] = 0;
   out_3669342035074408446[268] = 0;
   out_3669342035074408446[269] = 0;
   out_3669342035074408446[270] = 0;
   out_3669342035074408446[271] = 0;
   out_3669342035074408446[272] = 0;
   out_3669342035074408446[273] = 0;
   out_3669342035074408446[274] = 0;
   out_3669342035074408446[275] = 0;
   out_3669342035074408446[276] = 0;
   out_3669342035074408446[277] = 0;
   out_3669342035074408446[278] = 0;
   out_3669342035074408446[279] = 0;
   out_3669342035074408446[280] = 0;
   out_3669342035074408446[281] = 0;
   out_3669342035074408446[282] = 0;
   out_3669342035074408446[283] = 0;
   out_3669342035074408446[284] = 0;
   out_3669342035074408446[285] = 1;
   out_3669342035074408446[286] = 0;
   out_3669342035074408446[287] = 0;
   out_3669342035074408446[288] = 0;
   out_3669342035074408446[289] = 0;
   out_3669342035074408446[290] = 0;
   out_3669342035074408446[291] = 0;
   out_3669342035074408446[292] = 0;
   out_3669342035074408446[293] = 0;
   out_3669342035074408446[294] = 0;
   out_3669342035074408446[295] = 0;
   out_3669342035074408446[296] = 0;
   out_3669342035074408446[297] = 0;
   out_3669342035074408446[298] = 0;
   out_3669342035074408446[299] = 0;
   out_3669342035074408446[300] = 0;
   out_3669342035074408446[301] = 0;
   out_3669342035074408446[302] = 0;
   out_3669342035074408446[303] = 0;
   out_3669342035074408446[304] = 1;
   out_3669342035074408446[305] = 0;
   out_3669342035074408446[306] = 0;
   out_3669342035074408446[307] = 0;
   out_3669342035074408446[308] = 0;
   out_3669342035074408446[309] = 0;
   out_3669342035074408446[310] = 0;
   out_3669342035074408446[311] = 0;
   out_3669342035074408446[312] = 0;
   out_3669342035074408446[313] = 0;
   out_3669342035074408446[314] = 0;
   out_3669342035074408446[315] = 0;
   out_3669342035074408446[316] = 0;
   out_3669342035074408446[317] = 0;
   out_3669342035074408446[318] = 0;
   out_3669342035074408446[319] = 0;
   out_3669342035074408446[320] = 0;
   out_3669342035074408446[321] = 0;
   out_3669342035074408446[322] = 0;
   out_3669342035074408446[323] = 1;
}
void h_4(double *state, double *unused, double *out_2400270287193189998) {
   out_2400270287193189998[0] = state[6] + state[9];
   out_2400270287193189998[1] = state[7] + state[10];
   out_2400270287193189998[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_5733778634909535716) {
   out_5733778634909535716[0] = 0;
   out_5733778634909535716[1] = 0;
   out_5733778634909535716[2] = 0;
   out_5733778634909535716[3] = 0;
   out_5733778634909535716[4] = 0;
   out_5733778634909535716[5] = 0;
   out_5733778634909535716[6] = 1;
   out_5733778634909535716[7] = 0;
   out_5733778634909535716[8] = 0;
   out_5733778634909535716[9] = 1;
   out_5733778634909535716[10] = 0;
   out_5733778634909535716[11] = 0;
   out_5733778634909535716[12] = 0;
   out_5733778634909535716[13] = 0;
   out_5733778634909535716[14] = 0;
   out_5733778634909535716[15] = 0;
   out_5733778634909535716[16] = 0;
   out_5733778634909535716[17] = 0;
   out_5733778634909535716[18] = 0;
   out_5733778634909535716[19] = 0;
   out_5733778634909535716[20] = 0;
   out_5733778634909535716[21] = 0;
   out_5733778634909535716[22] = 0;
   out_5733778634909535716[23] = 0;
   out_5733778634909535716[24] = 0;
   out_5733778634909535716[25] = 1;
   out_5733778634909535716[26] = 0;
   out_5733778634909535716[27] = 0;
   out_5733778634909535716[28] = 1;
   out_5733778634909535716[29] = 0;
   out_5733778634909535716[30] = 0;
   out_5733778634909535716[31] = 0;
   out_5733778634909535716[32] = 0;
   out_5733778634909535716[33] = 0;
   out_5733778634909535716[34] = 0;
   out_5733778634909535716[35] = 0;
   out_5733778634909535716[36] = 0;
   out_5733778634909535716[37] = 0;
   out_5733778634909535716[38] = 0;
   out_5733778634909535716[39] = 0;
   out_5733778634909535716[40] = 0;
   out_5733778634909535716[41] = 0;
   out_5733778634909535716[42] = 0;
   out_5733778634909535716[43] = 0;
   out_5733778634909535716[44] = 1;
   out_5733778634909535716[45] = 0;
   out_5733778634909535716[46] = 0;
   out_5733778634909535716[47] = 1;
   out_5733778634909535716[48] = 0;
   out_5733778634909535716[49] = 0;
   out_5733778634909535716[50] = 0;
   out_5733778634909535716[51] = 0;
   out_5733778634909535716[52] = 0;
   out_5733778634909535716[53] = 0;
}
void h_10(double *state, double *unused, double *out_4139302664479765069) {
   out_4139302664479765069[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_4139302664479765069[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_4139302664479765069[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8721481213413243576) {
   out_8721481213413243576[0] = 0;
   out_8721481213413243576[1] = 9.8100000000000005*cos(state[1]);
   out_8721481213413243576[2] = 0;
   out_8721481213413243576[3] = 0;
   out_8721481213413243576[4] = -state[8];
   out_8721481213413243576[5] = state[7];
   out_8721481213413243576[6] = 0;
   out_8721481213413243576[7] = state[5];
   out_8721481213413243576[8] = -state[4];
   out_8721481213413243576[9] = 0;
   out_8721481213413243576[10] = 0;
   out_8721481213413243576[11] = 0;
   out_8721481213413243576[12] = 1;
   out_8721481213413243576[13] = 0;
   out_8721481213413243576[14] = 0;
   out_8721481213413243576[15] = 1;
   out_8721481213413243576[16] = 0;
   out_8721481213413243576[17] = 0;
   out_8721481213413243576[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8721481213413243576[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8721481213413243576[20] = 0;
   out_8721481213413243576[21] = state[8];
   out_8721481213413243576[22] = 0;
   out_8721481213413243576[23] = -state[6];
   out_8721481213413243576[24] = -state[5];
   out_8721481213413243576[25] = 0;
   out_8721481213413243576[26] = state[3];
   out_8721481213413243576[27] = 0;
   out_8721481213413243576[28] = 0;
   out_8721481213413243576[29] = 0;
   out_8721481213413243576[30] = 0;
   out_8721481213413243576[31] = 1;
   out_8721481213413243576[32] = 0;
   out_8721481213413243576[33] = 0;
   out_8721481213413243576[34] = 1;
   out_8721481213413243576[35] = 0;
   out_8721481213413243576[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8721481213413243576[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8721481213413243576[38] = 0;
   out_8721481213413243576[39] = -state[7];
   out_8721481213413243576[40] = state[6];
   out_8721481213413243576[41] = 0;
   out_8721481213413243576[42] = state[4];
   out_8721481213413243576[43] = -state[3];
   out_8721481213413243576[44] = 0;
   out_8721481213413243576[45] = 0;
   out_8721481213413243576[46] = 0;
   out_8721481213413243576[47] = 0;
   out_8721481213413243576[48] = 0;
   out_8721481213413243576[49] = 0;
   out_8721481213413243576[50] = 1;
   out_8721481213413243576[51] = 0;
   out_8721481213413243576[52] = 0;
   out_8721481213413243576[53] = 1;
}
void h_13(double *state, double *unused, double *out_2372409296719316764) {
   out_2372409296719316764[0] = state[3];
   out_2372409296719316764[1] = state[4];
   out_2372409296719316764[2] = state[5];
}
void H_13(double *state, double *unused, double *out_8946052460241868517) {
   out_8946052460241868517[0] = 0;
   out_8946052460241868517[1] = 0;
   out_8946052460241868517[2] = 0;
   out_8946052460241868517[3] = 1;
   out_8946052460241868517[4] = 0;
   out_8946052460241868517[5] = 0;
   out_8946052460241868517[6] = 0;
   out_8946052460241868517[7] = 0;
   out_8946052460241868517[8] = 0;
   out_8946052460241868517[9] = 0;
   out_8946052460241868517[10] = 0;
   out_8946052460241868517[11] = 0;
   out_8946052460241868517[12] = 0;
   out_8946052460241868517[13] = 0;
   out_8946052460241868517[14] = 0;
   out_8946052460241868517[15] = 0;
   out_8946052460241868517[16] = 0;
   out_8946052460241868517[17] = 0;
   out_8946052460241868517[18] = 0;
   out_8946052460241868517[19] = 0;
   out_8946052460241868517[20] = 0;
   out_8946052460241868517[21] = 0;
   out_8946052460241868517[22] = 1;
   out_8946052460241868517[23] = 0;
   out_8946052460241868517[24] = 0;
   out_8946052460241868517[25] = 0;
   out_8946052460241868517[26] = 0;
   out_8946052460241868517[27] = 0;
   out_8946052460241868517[28] = 0;
   out_8946052460241868517[29] = 0;
   out_8946052460241868517[30] = 0;
   out_8946052460241868517[31] = 0;
   out_8946052460241868517[32] = 0;
   out_8946052460241868517[33] = 0;
   out_8946052460241868517[34] = 0;
   out_8946052460241868517[35] = 0;
   out_8946052460241868517[36] = 0;
   out_8946052460241868517[37] = 0;
   out_8946052460241868517[38] = 0;
   out_8946052460241868517[39] = 0;
   out_8946052460241868517[40] = 0;
   out_8946052460241868517[41] = 1;
   out_8946052460241868517[42] = 0;
   out_8946052460241868517[43] = 0;
   out_8946052460241868517[44] = 0;
   out_8946052460241868517[45] = 0;
   out_8946052460241868517[46] = 0;
   out_8946052460241868517[47] = 0;
   out_8946052460241868517[48] = 0;
   out_8946052460241868517[49] = 0;
   out_8946052460241868517[50] = 0;
   out_8946052460241868517[51] = 0;
   out_8946052460241868517[52] = 0;
   out_8946052460241868517[53] = 0;
}
void h_14(double *state, double *unused, double *out_4419510828785612490) {
   out_4419510828785612490[0] = state[6];
   out_4419510828785612490[1] = state[7];
   out_4419510828785612490[2] = state[8];
}
void H_14(double *state, double *unused, double *out_8749724582460531371) {
   out_8749724582460531371[0] = 0;
   out_8749724582460531371[1] = 0;
   out_8749724582460531371[2] = 0;
   out_8749724582460531371[3] = 0;
   out_8749724582460531371[4] = 0;
   out_8749724582460531371[5] = 0;
   out_8749724582460531371[6] = 1;
   out_8749724582460531371[7] = 0;
   out_8749724582460531371[8] = 0;
   out_8749724582460531371[9] = 0;
   out_8749724582460531371[10] = 0;
   out_8749724582460531371[11] = 0;
   out_8749724582460531371[12] = 0;
   out_8749724582460531371[13] = 0;
   out_8749724582460531371[14] = 0;
   out_8749724582460531371[15] = 0;
   out_8749724582460531371[16] = 0;
   out_8749724582460531371[17] = 0;
   out_8749724582460531371[18] = 0;
   out_8749724582460531371[19] = 0;
   out_8749724582460531371[20] = 0;
   out_8749724582460531371[21] = 0;
   out_8749724582460531371[22] = 0;
   out_8749724582460531371[23] = 0;
   out_8749724582460531371[24] = 0;
   out_8749724582460531371[25] = 1;
   out_8749724582460531371[26] = 0;
   out_8749724582460531371[27] = 0;
   out_8749724582460531371[28] = 0;
   out_8749724582460531371[29] = 0;
   out_8749724582460531371[30] = 0;
   out_8749724582460531371[31] = 0;
   out_8749724582460531371[32] = 0;
   out_8749724582460531371[33] = 0;
   out_8749724582460531371[34] = 0;
   out_8749724582460531371[35] = 0;
   out_8749724582460531371[36] = 0;
   out_8749724582460531371[37] = 0;
   out_8749724582460531371[38] = 0;
   out_8749724582460531371[39] = 0;
   out_8749724582460531371[40] = 0;
   out_8749724582460531371[41] = 0;
   out_8749724582460531371[42] = 0;
   out_8749724582460531371[43] = 0;
   out_8749724582460531371[44] = 1;
   out_8749724582460531371[45] = 0;
   out_8749724582460531371[46] = 0;
   out_8749724582460531371[47] = 0;
   out_8749724582460531371[48] = 0;
   out_8749724582460531371[49] = 0;
   out_8749724582460531371[50] = 0;
   out_8749724582460531371[51] = 0;
   out_8749724582460531371[52] = 0;
   out_8749724582460531371[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_7736453428140691368) {
  err_fun(nom_x, delta_x, out_7736453428140691368);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6034861957144106997) {
  inv_err_fun(nom_x, true_x, out_6034861957144106997);
}
void pose_H_mod_fun(double *state, double *out_9008000713700484969) {
  H_mod_fun(state, out_9008000713700484969);
}
void pose_f_fun(double *state, double dt, double *out_8514240899757441396) {
  f_fun(state,  dt, out_8514240899757441396);
}
void pose_F_fun(double *state, double dt, double *out_3669342035074408446) {
  F_fun(state,  dt, out_3669342035074408446);
}
void pose_h_4(double *state, double *unused, double *out_2400270287193189998) {
  h_4(state, unused, out_2400270287193189998);
}
void pose_H_4(double *state, double *unused, double *out_5733778634909535716) {
  H_4(state, unused, out_5733778634909535716);
}
void pose_h_10(double *state, double *unused, double *out_4139302664479765069) {
  h_10(state, unused, out_4139302664479765069);
}
void pose_H_10(double *state, double *unused, double *out_8721481213413243576) {
  H_10(state, unused, out_8721481213413243576);
}
void pose_h_13(double *state, double *unused, double *out_2372409296719316764) {
  h_13(state, unused, out_2372409296719316764);
}
void pose_H_13(double *state, double *unused, double *out_8946052460241868517) {
  H_13(state, unused, out_8946052460241868517);
}
void pose_h_14(double *state, double *unused, double *out_4419510828785612490) {
  h_14(state, unused, out_4419510828785612490);
}
void pose_H_14(double *state, double *unused, double *out_8749724582460531371) {
  H_14(state, unused, out_8749724582460531371);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
