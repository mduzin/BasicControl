/*
 * test_env.c
 *
 *  Created on: Aug 23, 2015
 *      Author: mduzinki
 */

#include "common.h"
#include "test_env.h"

//strutura z funkcjami srodowiska symulacji
SIM_ENV_FUNC sim_func = {simulation_iter,rectangle_signal, simulation_init, simulation_close};
//strutura z funkcjami regulatora
PID_FUNC regulator_func = {regulator_run, regulator_init, regulator_close};
//strutura z funkcjami modelu
MODEL_1_FUNC model_func = {model_run, model_init, model_close};
//strutura z funkcjami log
LOG_FUNC log_func = {log_write, log_init, log_close};




