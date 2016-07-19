/*
 * time_source.h
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#ifndef TIME_SOURCE_H_
#define TIME_SOURCE_H_

#include "common.h"
#include "time_observer.h"

//ADT for time source contetx
typedef struct _TIME_SOURCE_CTX* TIME_SOURCE_CTX_PTR;

//API for ADT
void TimeSourceInit(TIME_SOURCE_CTX_PTR);
void TimeSourceTick(TIME_SOURCE_CTX_PTR);
long TimeSourceGetTc(TIME_SOURCE_CTX_PTR);
long TimeSourceGetCurrTsym(TIME_SOURCE_CTX_PTR);


//Dodanie do listy nastepnego obserwatora
STATUS attach(IN TIME_OBSERVER_PTR Observer);
//Usuniecie z listy obserwatora
STATUS detach(IN TIME_OBSERVER_PTR Observer);


#endif /* TIME_SOURCE_H_ */
