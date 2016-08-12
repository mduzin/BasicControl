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

typedef struct _INIT_LOG_PARAM
{
	char* filename;
}INIT_LOG_PARAM;

typedef struct _LOG_PARAM
{
	char* filename;
	FILE *file;
}LOG_PARAM;

/*
//Inicjalizacja pliku z logowaniem danych
STATUS log_init(IN INIT_LOG_PARAM init_param,
		        IO LOG_PARAM *log);
//zapis do pliku z danymi
STATUS log_write(IO LOG_PARAM *log,
		         IN SIMULATION_PARAM *simulation,
				 IN PID_PARAM *regulator,
				 IN MODEL_PARAM *model);
//zamykanie pliku z logiem
STATUS log_close(IN LOG_PARAM *log);
*/
#endif /* LIB_FILE_H_ */
