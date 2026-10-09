#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_8356721739159858042);
void live_err_fun(double *nom_x, double *delta_x, double *out_3024297532061662983);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_1660304109749409590);
void live_H_mod_fun(double *state, double *out_366535546542209567);
void live_f_fun(double *state, double dt, double *out_6957096699446517531);
void live_F_fun(double *state, double dt, double *out_903611873513124028);
void live_h_4(double *state, double *unused, double *out_8027820106924701449);
void live_H_4(double *state, double *unused, double *out_477841128217789381);
void live_h_9(double *state, double *unused, double *out_7641073242291071365);
void live_H_9(double *state, double *unused, double *out_719030774847380026);
void live_h_10(double *state, double *unused, double *out_839742509670659807);
void live_H_10(double *state, double *unused, double *out_673215584048952417);
void live_h_12(double *state, double *unused, double *out_797769385776473797);
void live_H_12(double *state, double *unused, double *out_5497297536249751176);
void live_h_35(double *state, double *unused, double *out_8130544409350217717);
void live_H_35(double *state, double *unused, double *out_3844503185590396757);
void live_h_32(double *state, double *unused, double *out_2532654400440000411);
void live_H_32(double *state, double *unused, double *out_958021762741942261);
void live_h_13(double *state, double *unused, double *out_371969707737481386);
void live_H_13(double *state, double *unused, double *out_1672006424444033135);
void live_h_14(double *state, double *unused, double *out_7641073242291071365);
void live_H_14(double *state, double *unused, double *out_719030774847380026);
void live_h_33(double *state, double *unused, double *out_380769406781134003);
void live_H_33(double *state, double *unused, double *out_6995060190229254361);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}