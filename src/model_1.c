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


typedef struct _FIRST_ORDER_MODEL
{
   double k;	  //[-] gain
   double Ts;	  //[s] time constant
   double Tdelay; //[s] delay

   double u;	      //input value
   double y;	      //output value
   double y_delayed;  //output value for model with delay

   double Integral;	            //zmienna wewnetrzna obiektu na ktorej obliczany ca³kowanie

   /*Zmienne z poprzedniej iteracji symulacji obiektu*/
   double Prev_y;			    //sygnal wyjsciowy
   double Buff_y;
   double Prev_Int;	            //zmienna wewnetrzna obiektu na ktorej obliczany ca³kowanie w obiekcie

   double* Delay_array;		    //wskaznik na tablice z opoznionymi probkami
   int     Delay_array_size;	//rozmiar tablicy z opoznionymi probkami

   TIME_SOURCE_CTX_PTR pTimeCtx;
   REG_PID_PTR         pPidCtx;

}FIRST_ORDER_MODEL;

static void   init_delay_array(double *array_ptr,int array_size, double init_val);
static double shift_delay_array(double *array_ptr,int array_size, int shift);
static void   print_delay_array(double *array_ptr,int array_size);

STATUS FirstOrderModelInit(FIRST_ORDER_MODEL_PTR* ppModel)
{
	TIME_EVENT Events;


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
	(*ppModel)->Tdelay = 0.0;     // model delay [s]
	(*ppModel)->u      = 1.0;
	(*ppModel)->y      = 0.0;



    //Internal variables
    (*ppModel)->Prev_y    = 0.0;
    (*ppModel)->Integral  = 0.0;
    (*ppModel)->Prev_Int  = 0.0;
    (*ppModel)->y_delayed = 0.0;
    (*ppModel)->Buff_y    = 0.0;

    //Others
    (*ppModel)->Delay_array = NULL;
    (*ppModel)->Delay_array_size = 0;


    //Register events to time observe
   	Events = (TE_BOT |
   			  TE_10MS);

    //call register API
    CreateObserver((void*)(*ppModel), Events, FirstOrderModelRun);



	return STATUS_SUCCESS;

}

STATUS FirstOrderModelPostInit(IO FIRST_ORDER_MODEL_PTR pModel,
							   IN REG_PID_PTR pPid,
							   IN TIME_SOURCE_CTX_PTR pTimeCtx)
{
	long Tdelay_ms = 0;

    if((NULL == pModel)||(NULL == pPid)||(NULL == pTimeCtx))
    {
    	return STATUS_PTR_ERROR;
    }

    //save handles to necessary ADT
    pModel->pPidCtx = pPid;
	pModel->pTimeCtx = pTimeCtx;

    //Allocate memory for model delay array
    if(NO_DELAY != (pModel)->Tdelay)
    {
        Tdelay_ms =(long)((pModel)->Tdelay* 1000.0);

        (pModel)->Delay_array_size = (int)DIV_ROUND_CLOSEST(Tdelay_ms, TimeSourceGetTc(pTimeCtx));
       	(pModel)->Delay_array = (double*)malloc((pModel)->Delay_array_size * sizeof(double));
        if(NULL == (pModel)->Delay_array)
       	{
       	   return STATUS_PTR_ERROR;
       	}
       	else
       	{
       	   init_delay_array((pModel)->Delay_array,(pModel)->Delay_array_size, 0.0);
   	    }
     }
    else
    {
    	(pModel)->Delay_array_size = 0;
    	(pModel)->Delay_array = NULL;
    }

     return STATUS_SUCCESS;

}


STATUS FirstOrderModelClose(FIRST_ORDER_MODEL_PTR pModel)
{
	if((NO_DELAY != (pModel->Tdelay)) && (NULL != (pModel->Delay_array)) )
	{
	   free(pModel->Delay_array);
	}
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
    pModel->Prev_y   = pModel->y;
    pModel->Prev_Int = pModel->Integral;

   	//model calculations
    pModel->u = RegPidGetCS(pModel->pPidCtx);
   	pModel->Integral = (pModel->k/pModel->Ts)*pModel->u - (1/pModel->Ts)*pModel->y_delayed;
   	pModel->y = pModel->Prev_y + (Tc/2)*(pModel->Integral + pModel->Prev_Int);

   	//delay
   	if(NO_DELAY != pModel->Tdelay)
   	{
   	   pModel->y_delayed = shift_delay_array(pModel->Delay_array,pModel->Delay_array_size,1);
   	   *(pModel->Delay_array) = pModel->y;
   	}
   	else
   	{
   		pModel->y_delayed = pModel->y;
   	}

   //printf("Model output: %f\n",pModel->y_delayed);
}

double FirstOrderModelGetOutput(FIRST_ORDER_MODEL_PTR pModel)
{
   if(NULL == pModel)
   {
	   return 0.0;
   }

   return pModel->y_delayed;


}


char* FirstOrderModelGetHeader(IN FIRST_ORDER_MODEL_PTR pModel)
{
   if(NULL != pModel)
   {
      return "Input[-], Output[-], Output_delayed[-]";
   }
   else
   {
	   return "!!!Error!!!";
   }

}

STATUS FirstOrderModelGetValues(IN FIRST_ORDER_MODEL_PTR pModel, OUT FIRST_ORDER_MODEL_LOG_VALUES* pValues)
{
	if((NULL == pModel)||(NULL == pValues))
	{
		return STATUS_PTR_ERROR;
	}

	pValues->u         = pModel->u;
	pValues->y         = pModel->y;
	pValues->y_delayed = pModel->y_delayed;
	return STATUS_SUCCESS;
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
