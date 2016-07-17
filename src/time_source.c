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
  long Tsym;	  //czas symulacji
  long Tc;	  //okres  calkowania obiektu !!!! NAJWAZNIEJSZY PARAMETR !!!!!!

  long CurrTsym;//aktualny czas symulacji

} TIME_SOURCE_CTX;


//globalne zmienne
TIME_SOURCE_CTX gTimeCtx;
static TIME_OBSERVER_PTR gObservers[OBSERVERS_NUM];		//Lista z handles do poszczegolnych obserwatorow
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
	  gTimeCtx.Tsym = 1000;	 //czas symulacji [ms]
	  gTimeCtx.Tc = 10;		 //krok zegara [ms]
	  gTimeCtx.CurrTsym = 0;
}


// Tik zegara, ktory notyfikuje obserwatorow
// w zaleznosci od implementacji moze to byc licznik
// moze to byc select z timeoutem
// moze to byc sleep itp..
void TimeSourceTick(void)
{
 TIME_EVENT Events = NO_EVENT;		    //zmienna lokalna z wszystkimi time eventami jakie wystapily w tym wywolaniu
 TIME_EVENT EventsFiltered = NO_EVENT;  //time eventy zmaskowane z observatorem ktorego bedziemy notyfikowac (filtrujemy po to zeby nie informawac o eventach na ktore observawtor sie nie rejestrowal)
 TIME_EVENT_NOTIFICATION pCallback = NULL;
 void* pInstance = NULL;
 int index = 0;


 //Logika tick'a zegar
 for(; gTimeCtx.CurrTsym < gTimeCtx.Tsym ; gTimeCtx.CurrTsym += gTimeCtx.Tc)
 {
	 if(0 == (gTimeCtx.CurrTsym%TIME_20MS))
	 {
		 Events |= IDX_TO_MAP(TE_20MS_IDX);
	 }

	 if(0 == (gTimeCtx.CurrTsym%TIME_500MS))
	 {
		 Events |= IDX_TO_MAP(TE_500MS_IDX);
	 }

	 //wywo�ywanie callbackow do zajestrowanych obserwatorow
     for(index = 0; index < OBSERVERS_NUM; index++)
     {
	    EventsFiltered = ObserverGetEvent(gObservers[index]) & Events;
	    if(NO_EVENT != EventsFiltered)
	    {
		   pInstance = ObserverGetInstance(gObservers[index]);
		   pCallback = ObserverGetCallback(gObservers[index]);
		   pCallback(pInstance,EventsFiltered);
	    }
	    else
	    {
		   //no need to notify observer
	    }
     }

 }

}
