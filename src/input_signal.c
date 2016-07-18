/*
 * input_signal.c
 *
 *  Created on: 26 maj 2016
 *      Author: MD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "common.h"
#include "input_signal.h"
#include "time_observer.h"

#define STEP_SIGNAL_HIGH 1.0
#define STEP_SIGNAL_LOW  0.0


//callbacki
void RectangleSignal(void* pInstance, const TIME_EVENT Events)
{
	//w rect_signal co 5sekund zmieniamy z 1 na 0 i odwrotnie
	INPUT_SIGNAL_RECT* pInput = NULL;

	if(NULL == pInstance)
	{
		return;
	}

	pInput = (INPUT_SIGNAL_RECT*)pInstance;

	if(TRUE == (IS_IDX_SET_IN_MAP(TE_BOT_IDX,Events)))
	{
		pInput->InternalCounter = 0;
		pInput->Value = pInput->InitValue;
	}

	if(TRUE == (IS_IDX_SET_IN_MAP(TE_1000MS_IDX,Events)))
	{
		pInput->InternalCounter++;

		if(pInput->SwitchTime == pInput->InternalCounter)
		{
			if(STEP_SIGNAL_HIGH == pInput->Value)
			{
				pInput->Value = STEP_SIGNAL_LOW;
			}
			else
			{
				pInput->Value = STEP_SIGNAL_HIGH;
			}

			pInput->InternalCounter = 0;
		}
	}

}


//tworzenie obiektu Input Signal
STATUS RectangleSignalInit(IN INPUT_SIGNAL_RECT* pInput)
{
	STATUS Status = STATUS_SUCCESS;
	TIME_EVENT Events;

	if(NULL == pInput)
	{
		return STATUS_PTR_ERROR;
	}

	//init internal values
	pInput->Value = STEP_SIGNAL_LOW;
	pInput->InitValue = STEP_SIGNAL_HIGH;
	pInput->SwitchTime = 5;


	//Register events to time observe
	Events = (TE_BOT |
			  TE_1000MS);

	//call register API
	CreateObserver((void*)pInput, Events, RectangleSignal);

	return Status;

}

STATUS StepSignalInit(IN INPUT_SIGNAL_STEP* pInput)
{
	STATUS Status = STATUS_SUCCESS;
	TIME_EVENT Events;

	if(NULL == pInput)
	{
		return STATUS_PTR_ERROR;
	}

	//init internal values
	pInput->Value = STEP_SIGNAL_HIGH;
	pInput->InitValue = STEP_SIGNAL_HIGH;

	//Register events to time observe
	Events = TE_BOT;

	//call register API
	CreateObserver((void*)pInput, Events, StepSignal);

	return Status;
}
void StepSignal(void* pInstance, const TIME_EVENT Events)
{
	INPUT_SIGNAL_RECT* pInput = NULL;

	if(NULL == pInstance)
	{
		return;
	}

	pInput = (INPUT_SIGNAL_RECT*)pInstance;

	if(TRUE == (IS_IDX_SET_IN_MAP(TE_BOT_IDX,Events)))
	{
   	    pInput->Value = pInput->InitValue;
	}


}

