/*
 * model_1.c
 *
 *  Created on: Aug 9, 2015
 *      Author: mduzinki
 */


#include "common.h"
#include "model_1.h"
#include "time_source.h"
#include "time_observer.h"
#include "regulator_PID.h"

#define NO_DELAY 0.0

typedef struct _FIRST_ORDER_INTERNAL_VARIABLES
{

  double Integral;	        //zmienna wewnetrzna obiektu na ktorej obliczany ca³kowanie

  /*Zmienne z poprzedniej iteracji symulacji obiektu*/
  double Prev_y;			//sygnal wyjsciowy
  double Buff_y;
  double Prev_Int;	        //zmienna wewnetrzna obiektu na ktorej obliczany ca³kowanie w obiekcie

  double* Delay_array;		//wskaznik na tablice z opoznionymi probkami
  int     Delay_array_size;	//rozmiar tablicy z opoznionymi probkami


} FIRST_ORDER_INTERNAL_VARIABLES;

typedef struct _FIRST_ORDER_MODEL
{
   double k;	  //[-] gain
   double Ts;	  //[s] time constant
   double Tdelay; //[s] delay

   double u;	      //input value
   double y;	      //output value
   double y_delayed;  //output value for model with delay

   FIRST_ORDER_INTERNAL_VARIABLES Internal;
   TIME_SOURCE_CTX_PTR pTimeCtx;
   REG_PID_PTR         pPidCtx;

}FIRST_ORDER_MODEL;

static void init_delay_array(double *array_ptr,int array_size, double init_val);
static double shift_delay_array(double *array_ptr,int array_size, int shift);
static void print_delay_array(double *array_ptr,int array_size);

STATUS FirstOrderModelInit(FIRST_ORDER_MODEL_PTR* ppModel)
{
	TIME_EVENT Events;
	long Tdelay_ms = 0;

	if(NULL == ppModel)
	{
  	   return STATUS_PTR_ERROR;
	}

	*ppModel = (FIRST_ORDER_MODEL_PTR)malloc(sizeof(FIRST_ORDER_MODEL));

	if(NULL == *ppModel)
	{
	   return STATUS_PTR_ERROR;
	}

	//Model parameters
	(*ppModel)->k      = 1.0;	  // gain
	(*ppModel)->Ts     = 4.0;	  // time const [s]
	(*ppModel)->Tdelay = 0.5;     // model delay [s]
	(*ppModel)->u      = 1.0;
	(*ppModel)->y      = 0.0;

	//Allocate memory for model delay array
	/*<TODO:>cross dependency with time source
    if(NO_DELAY != (*ppModel)->Tdelay)
    {
    	Tdelay_ms =((long)(*ppModel)->Tdelay)* 1000;

    	(*ppModel)->Internal.Delay_array_size = (int)DIV_ROUND_CLOSEST(Tdelay_ms, TimeSourceGetTc((*ppModel)->pTimeCtx));
    	(*ppModel)->Internal.Delay_array = (double*)malloc((*ppModel)->Internal.Delay_array_size * sizeof(double));
    	if(NULL == (*ppModel)->Internal.Delay_array)
    	{
    	   return STATUS_PTR_ERROR;
    	}
    	else
    	{
    	   init_delay_array((*ppModel)->Internal.Delay_array,(*ppModel)->Internal.Delay_array_size, 0.0);
  	    }
    }*/


    //Internal variables
    (*ppModel)->Internal.Prev_y   = 0.0;
    (*ppModel)->Internal.Integral = 0.0;
    (*ppModel)->Internal.Prev_Int = 0.0;

    //Register events to time observe
   	Events = (TE_BOT |
   			  TE_10MS);

    //call register API
    CreateObserver((void*)(*ppModel), Events, FirstOrderModelRun);

	return STATUS_SUCCESS;

}


STATUS FirstOrderModelClose(FIRST_ORDER_MODEL_PTR pModel)
{
	free(pModel->Internal.Delay_array);
	free(pModel);
	pModel = NULL;
	return STATUS_SUCCESS;
}

void FirstOrderModelRun(void* pInstance, const TIME_EVENT Events)
{

	FIRST_ORDER_MODEL_PTR pModel = NULL;
	double  Tc;


	if(NULL == pInstance)
	{
	   return;
	}

	pModel = (FIRST_ORDER_MODEL*)pInstance;


    if((NULL == pModel->pTimeCtx) || (NULL == pModel->pPidCtx))
    {
	   return;
    }

    Tc = ((double)TimeSourceGetTc(pModel->pTimeCtx))/1000;

    //save previous values
    pModel->Internal.Prev_y   = pModel->y;
    pModel->Internal.Prev_Int = pModel->Internal.Integral;

   	//model calculations
    pModel->u = 0.0; //PidGetCS(pModel->pPidCtx); //nie input from regulator
   	pModel->Internal.Integral = (pModel->k/pModel->Ts)*pModel->u - (1/pModel->Ts)*pModel->y_delayed;
   	pModel->y = pModel->Internal.Prev_y + (Tc/2)*(pModel->Internal.Integral + pModel->Internal.Prev_Int);

   	//delay
   	if(NO_DELAY != pModel->Tdelay)
   	{
   	   pModel->y_delayed = shift_delay_array(pModel->Internal.Delay_array,pModel->Internal.Delay_array_size,1);
   	   *(pModel->Internal.Delay_array) = pModel->y;
   	}
   	else
   	{
   		pModel->y_delayed = pModel->y;
   	}


}


static void init_delay_array(double *array_ptr,int array_size, double init_val)
{
  int ii;

  if((0 == array_size) || (NULL == array_ptr))
  {
	 printf("Nie powinnienem byc tutaj\n");
	 return;
  }

  for(ii = 0; ii < array_size; ii++)
  {
	  *(array_ptr+ii) = init_val;
  }
}


static double shift_delay_array(double *array_ptr,int array_size, int shift)
{
  //wypluwany element
  int ii;
  double throwed_elem = *(array_ptr + array_size - shift);

  //przesuwanie tablicy
  for(ii = array_size - 1; ii >= shift; ii--)
  {
    *(array_ptr + ii) = *(array_ptr + ii - shift);
  }
  return throwed_elem;
}

static void print_delay_array(double *array_ptr,int array_size)
{
 int ii;
  if((NULL != array_ptr)&&(0 != array_size))
	{
		for(ii = 0 ;ii< array_size; ii++)
		{
			printf("Addr: %p \t delay[%d] = %f\n",(array_ptr + ii),ii, *(array_ptr + ii));
		}
	}
}
