#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7736453428140691368);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6034861957144106997);
void pose_H_mod_fun(double *state, double *out_9008000713700484969);
void pose_f_fun(double *state, double dt, double *out_8514240899757441396);
void pose_F_fun(double *state, double dt, double *out_3669342035074408446);
void pose_h_4(double *state, double *unused, double *out_2400270287193189998);
void pose_H_4(double *state, double *unused, double *out_5733778634909535716);
void pose_h_10(double *state, double *unused, double *out_4139302664479765069);
void pose_H_10(double *state, double *unused, double *out_8721481213413243576);
void pose_h_13(double *state, double *unused, double *out_2372409296719316764);
void pose_H_13(double *state, double *unused, double *out_8946052460241868517);
void pose_h_14(double *state, double *unused, double *out_4419510828785612490);
void pose_H_14(double *state, double *unused, double *out_8749724582460531371);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}