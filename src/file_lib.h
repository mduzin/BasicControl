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




STATUS output_file_init(IN char* filename, IO FILE **output_file);
STATUS output_file_close(FILE *output_file);
STATUS write_output_line(FILE *output_file,
		double Tsym,
		double SP,
		double output,
		double outputPure,
		double outputRef,
		double CS);


#endif /* LIB_FILE_H_ */
