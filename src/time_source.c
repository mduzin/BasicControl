/*
 * time_source.c
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "time_source.h"
#include "time_observer.h"

#define OBSERVERS_NUM 3

typedef enum _OBSERVER_TYPE
{
	INPUT_SIGNAL_OBSERVER = 0,
	MODEL_OBSERVER,
	PID_OBSERVER,
	FILE_LOG_OBSERVER,
	OBSERVER_TYPE_LAST
}OBSERVER_TYPE;


typedef struct _TIME_SOURCE_CTX
{
  long Tsym;	  //Simulation time [ms]
  long Tc;	      //Simulation time step [ms] !!!MOST IMPORTANT PARAMETER!!!

  long CurrTsym; //Current Simulation time [ms]

} TIME_SOURCE_CTX;


//GLOBAL VARIABLES
TIME_SOURCE_CTX          gTimeCtx;
static TIME_OBSERVER_PTR gObservers[OBSERVER_TYPE_LAST];		//Array of pointers to observers
static int Index = 0;



STATUS attach(IN TIME_OBSERVER_PTR Observer)
{
  if(NULL == Observer)
  {
	  return STATUS_PTR_ERROR;
  }

  if(OBSERVER_TYPE_LAST <= Index)
  {
	  return STATUS_FAILURE;
  }

  gObservers[Index] = Observer;
  Index++;
  return STATUS_SUCCESS;

}

STATUS dettach(IN TIME_OBSERVER_PTR Observer)
{
  int ii = 0;

  if(NULL == Observer)
  {
	  return STATUS_PTR_ERROR;
  }

  for(ii = 0 ; OBSERVER_TYPE_LAST > ii ; ii++)
  {
	  if(gObservers[ii] == Observer)
	  {
		  gObservers[ii] = NULL;
		  return STATUS_SUCCESS;
	  }
  }

  return STATUS_FAILURE;

}

STATUS TimeSourceInit(TIME_SOURCE_CTX_PTR* ppTimeCtx)
{
   if(NULL == ppTimeCtx)
   {
	  return STATUS_PTR_ERROR;
   }

   *ppTimeCtx = (TIME_SOURCE_CTX_PTR)malloc(sizeof(TIME_SOURCE_CTX));

   if(NULL == *ppTimeCtx)
   {
	   return STATUS_PTR_ERROR;
   }

   (*ppTimeCtx)->Tsym     = 100000;  //Simulation Time [100s]
   (*ppTimeCtx)->Tc       = 10;	     //Time step [10ms]
   (*ppTimeCtx)->CurrTsym = 0;
   return STATUS_SUCCESS;
}


// Time tick and observers notification
// Time tick can be implemented as:
// simple counter
// posix pselect with timeout
// sleep function etc.
void TimeSourceTick(TIME_SOURCE_CTX_PTR pTimeCtx)
{
 TIME_EVENT Events = NO_EVENT;		    //Events that occured in current iteration
 TIME_EVENT EventsFiltered = NO_EVENT;  //Events filtered with those on which client was registered
 TIME_EVENT_NOTIFICATION pCallback = NULL;
 void* pInstance = NULL;
 int index = 0;

 if(NULL == pTimeCtx)
 {
    return;
 }

 //Time tick logic as simple counter
 for(; pTimeCtx->CurrTsym <= pTimeCtx->Tsym ; pTimeCtx->CurrTsym += pTimeCtx->Tc)
 {
	 //printf("Time tick: %lu \n",pTimeCtx->CurrTsym);
	 Events = NO_EVENT;
	 EventsFiltered = NO_EVENT;

	 if(0 == pTimeCtx->CurrTsym)
	 {
		 Events |= IDX_TO_MAP(TE_BOT_IDX);
	 }
	 else
	 {
		 if(0 == (pTimeCtx->CurrTsym % TIME_10MS))
		 {
		 	Events |= IDX_TO_MAP(TE_10MS_IDX);
		 }

		 if(0 == (pTimeCtx->CurrTsym % TIME_20MS))
		 {
		    Events |= IDX_TO_MAP(TE_20MS_IDX);
		 }

		 if(0 == (pTimeCtx->CurrTsym % TIME_100MS))
		 {
		    Events |= IDX_TO_MAP(TE_100MS_IDX);
		 }

		 if(0 == (pTimeCtx->CurrTsym % TIME_500MS))
		 {
		    Events |= IDX_TO_MAP(TE_500MS_IDX);
		 }

		 if(0 == (pTimeCtx->CurrTsym % TIME_1000MS))
		 {
		     Events |= IDX_TO_MAP(TE_1000MS_IDX);
		 }

		 if(pTimeCtx->Tsym == pTimeCtx->CurrTsym )
		 {
			 Events |= IDX_TO_MAP(TE_EOT_IDX);
		 }
	 }


	 //Invoke callbacks for registered observers
	 for(index = 0; index < OBSERVER_TYPE_LAST; index++)
     {
	    if(NULL != gObservers[index])
	    {
    	   EventsFiltered = ObserverGetEvent(gObservers[index]) & Events;
	       if(NO_EVENT != EventsFiltered)
	       {
		      pInstance = ObserverGetInstance(gObservers[index]);
		      pCallback = ObserverGetCallback(gObservers[index]);
		      pCallback(pInstance,EventsFiltered);
	       }
	    }
     }

 }

}

//API for ADT
long TimeSourceGetTc(TIME_SOURCE_CTX_PTR pTimeCtx)
{
  if(NULL != pTimeCtx)
  {
     return pTimeCtx->Tc;
  }
  else
  {
	 return 0;
  }
}

long TimeSourceGetCurrTsym(TIME_SOURCE_CTX_PTR pTimeCtx)
{
  if(NULL != pTimeCtx)
  {
     return pTimeCtx->CurrTsym;
  }
  else
  {
	 return 0;
  }
}

char*  TimeSourceGetHeader(TIME_SOURCE_CTX_PTR pTimeCtx)
{
   if(NULL != pTimeCtx)
   {
      return "Time[ms]";
   }
   else
   {
	   return "!!!Error!!!";
   }
}

STATUS TimeSourceGetLogValues(IN TIME_SOURCE_CTX_PTR pTimeCtx, OUT TIME_SOURCE_LOG_VALUES* pValues)
{
	if((NULL == pTimeCtx)||(NULL == pValues))
	{
		return STATUS_PTR_ERROR;
	}

	*pValues = ((double)(pTimeCtx->CurrTsym)/1000.0);
	return STATUS_SUCCESS;

}
