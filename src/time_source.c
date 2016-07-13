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

//Dodanie do listy nastepnego obserwatora
STATUS attach(IN TIME_OBSERVER_PTR Observer);
//Usuniecie z listy obserwatora
STATUS detach(IN TIME_OBSERVER_PTR Observer);

//deklaracje funkcji
//Tik zegara, ktory notyfikuje obserwatorow
static void TimeTick(void);

//globalne zmienne
static TIME_OBSERVER_PTR gObservers[OBSERVERS_NUM];		//Lista z handles do poszczegolnych obserwatorow





// Tik zegara, ktory notyfikuje obserwatorow
// w zaleznosci od implementacji moze to byc licznik
// moze to byc select z timeoutem
// moze to byc sleep itp..
static void TimeTick(void)
{
 TIME_EVENT Events = NO_EVENT;		    //zmienna lokalna z wszystkimi time eventami jakie wystapily w tym wywolaniu
 TIME_EVENT EventsFiltered = NO_EVENT;  //time eventy zmaskowane z observatorem ktorego bedziemy notyfikowac (filtrujemy po to zeby nie informawac o eventach na ktore observawtor sie nie rejestrowal)
 TIME_EVENT_NOTIFICATION Callback = NULL;
 int index = 0;

 //<TODO:>logika tick'a zegar

 //na podstawie internal danych w zmiennej Event zapalamy bity, jaki event wystapil
 //zeby wiedziec ktorych obserwatorow notyfikowac
 //Events = xxx | yyy | zzz;


 for(index = 0; index < OBSERVERS_NUM; index++)
 {
	 EventsFiltered = ObserverGetEvent(gObservers[index]) & Events;
	 if(NO_EVENT != EventsFiltered)
	 {
		 Callback = ObserverGetCallback(gObservers[index]);
		 Callback(EventsFiltered);
	 }
	 else
	 {
		 //no need to notify observer
	 }
 }



}
