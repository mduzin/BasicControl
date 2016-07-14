/*
 * input_signal.c
 *
 *  Created on: 26 maj 2016
 *      Author: M
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "time_observer.h"

typedef struct _INPUT_SIGNAL
{
   double Value;				//aktualna wartosc sygnalu

} INPUT_SIGNAL;

//global
INPUT_SIGNAL Input;

//callbacki
void rect_singal(const TIME_EVENT Events)
{
	if(IS_IDX_SET_IN_MAP(TE_500MS_IDX,Events))
	{
		Input.Value = Input.Value * (-1.0);
	}
}


//tworzenie obiektu Input Signal
STATUS InputSignalInit(IN INPUT_SIGNAL* Input)
{
	STATUS Status = STATUS_SUCCESS;
	TIME_EVENT Events;

	Input->Value = 0.0;
	Events = (TE_20MS |
			  TE_500MS);

	CreateObserver((void*)Input,Events,rect_singal);

	return Status;

}

