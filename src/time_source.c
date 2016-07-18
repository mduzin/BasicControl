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


typedef struct _TIME_SOURCE_CTX
{
  long Tsym;	  //Simulation time
  long Tc;	      //Simulation time step !!!MOST IMPORTANT PARAMETER!!!

  long CurrTsym; //Current Simulation time

} TIME_SOURCE_CTX;


//GLOBAL VARIABLES
TIME_SOURCE_CTX          gTimeCtx;
static TIME_OBSERVER_PTR gObservers[OBSERVERS_NUM];		//Array of pointers to observers
static int Index = 0;



STATUS attach(IN TIME_OBSERVER_PTR Observer)
{
  if(NULL == Observer)
  {
	  return STATUS_PTR_ERROR;
  }

  if(OBSERVERS_NUM <= Index)
  {
	  return STATUS_FAILURE;
  }

  gObservers[Index] = Observer;
  return STATUS_SUCCESS;

}

STATUS dettach(IN TIME_OBSERVER_PTR Observer)
{
  int ii = 0;

  if(NULL == Observer)
  {
	  return STATUS_PTR_ERROR;
  }

  for(ii = 0 ; OBSERVERS_NUM > ii ; ii++)
  {
	  if(gObservers[ii] == Observer)
	  {
		  gObservers[ii] = NULL;
		  return STATUS_SUCCESS;
	  }
  }

  return STATUS_FAILURE;

}

void TimeSourceInit(void)
{
	  gTimeCtx.Tsym = 100000;  //Simulation Time [100s]
	  gTimeCtx.Tc = 10;		   //Time step [10ms]
	  gTimeCtx.CurrTsym = 0;
}


// Time tick and observers notitication
// Time tick can be implemented as:
// simple counter
// posix pselect with timeout
// sleep function etc.
void TimeSourceTick(void)
{
 TIME_EVENT Events = NO_EVENT;		    //Events that occured in current iteration
 TIME_EVENT EventsFiltered = NO_EVENT;  //Events filtered with those on which client was registered
 TIME_EVENT_NOTIFICATION pCallback = NULL;
 void* pInstance = NULL;
 int index = 0;

 //Time tick logic as simple counter
 for(; gTimeCtx.CurrTsym <= gTimeCtx.Tsym ; gTimeCtx.CurrTsym += gTimeCtx.Tc)
 {
	 Events = NO_EVENT;
	 EventsFiltered = NO_EVENT;

	 if(0 == gTimeCtx.CurrTsym)
	 {
		 Events |= IDX_TO_MAP(TE_BOT_IDX);
	 }
	 else
	 {
		 if(0 == (gTimeCtx.CurrTsym % TIME_10MS))
		 {
		 	Events |= IDX_TO_MAP(TE_10MS_IDX);
		 }

		 if(0 == (gTimeCtx.CurrTsym % TIME_20MS))
		 {
		    Events |= IDX_TO_MAP(TE_20MS_IDX);
		 }

		 if(0 == (gTimeCtx.CurrTsym % TIME_100MS))
		 {
		    Events |= IDX_TO_MAP(TE_100MS_IDX);
		 }

		 if(0 == (gTimeCtx.CurrTsym % TIME_500MS))
		 {
		    Events |= IDX_TO_MAP(TE_500MS_IDX);
		 }

		 if(0 == (gTimeCtx.CurrTsym % TIME_1000MS))
		 {
		     Events |= IDX_TO_MAP(TE_1000MS_IDX);
		 }

		 if(gTimeCtx.Tsym == gTimeCtx.CurrTsym )
		 {
			 Events |= IDX_TO_MAP(TE_EOT_IDX);
		 }
	 }


	 //Invoke callbacks to registered observers
	 for(index = 0; index < OBSERVERS_NUM; index++)
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
