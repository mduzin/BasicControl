/*
 * file_log.c
 *
 *  Created on: 19 lip 2016
 *      Author: M
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_log.h"

typedef struct _FILE_LOG
{
	char* filename;
	FILE *file;

	//pointery ADT do danych regulatora i obiektu
}FILE_LOG;


