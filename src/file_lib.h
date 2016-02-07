/*
 * lib_file.h
 *
 *  Created on: Aug 8, 2015
 *      Author: mduzinki
 */

#ifndef LIB_FILE_H_
#define LIB_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sim_env.h"
#include "regulator_PID.h"
#include "model_1.h"




STATUS output_file_init(IN char* filename, IO FILE **output_file);
STATUS output_file_close(FILE *output_file);
STATUS output_file_write(FILE *output_file,
		double Tsym,
		double SP,
		double output,
		double outputPure,
		double outputRef,
		double CS);


STATUS log_file_init(IN char* filename,
		             IO FILE **log_file);

STATUS log_file_write(IO FILE *output_file,
		              IN SIMULATION_PARAM *simulation,
					  IN PID_PARAM *regulator,
					  IN MODEL_PARAM *model);

STATUS log_file_close(IN FILE *log_file);

#endif /* LIB_FILE_H_ */
