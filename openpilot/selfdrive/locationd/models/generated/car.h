#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_3043335726999534980);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_4356260062798740363);
void car_H_mod_fun(double *state, double *out_8572103577084980982);
void car_f_fun(double *state, double dt, double *out_7242561094882829948);
void car_F_fun(double *state, double dt, double *out_3878615744521220760);
void car_h_25(double *state, double *unused, double *out_2632853913076272353);
void car_H_25(double *state, double *unused, double *out_5049122927095489305);
void car_h_24(double *state, double *unused, double *out_6419488108494770939);
void car_H_24(double *state, double *unused, double *out_9031446078493257203);
void car_h_30(double *state, double *unused, double *out_4137981313274078583);
void car_H_30(double *state, double *unused, double *out_1867567414396127450);
void car_h_26(double *state, double *unused, double *out_9139654580934924228);
void car_H_26(double *state, double *unused, double *out_8790626245969545529);
void car_h_27(double *state, double *unused, double *out_444566270375282095);
void car_H_27(double *state, double *unused, double *out_307195897404297461);
void car_h_29(double *state, double *unused, double *out_287379602024152950);
void car_H_29(double *state, double *unused, double *out_2020558624273848494);
void car_h_28(double *state, double *unused, double *out_2292659944851087435);
void car_H_28(double *state, double *unused, double *out_7102957641343379068);
void car_h_31(double *state, double *unused, double *out_8418790473714065202);
void car_H_31(double *state, double *unused, double *out_5018476965218528877);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}