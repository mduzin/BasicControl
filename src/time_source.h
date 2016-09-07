/*
 * time_source.h
 *
 *  Created on: 20 cze 2016
 *      Author: M
 */

#ifndef TIME_SOURCE_H_
#define TIME_SOURCE_H_

#include "common.h"
#include "adt.h"
#include "time_observer.h"

typedef double TIME_SOURCE_LOG_VALUES;

//API for ADT
STATUS TimeSourceInit(TIME_SOURCE_CTX_PTR*);
void   TimeSourceTick(TIME_SOURCE_CTX_PTR);
long   TimeSourceGetTc(TIME_SOURCE_CTX_PTR);
long   TimeSourceGetCurrTsym(TIME_SOURCE_CTX_PTR);

char*  TimeSourceGetHeader(TIME_SOURCE_CTX_PTR);
STATUS TimeSourceGetLogValues(IN TIME_SOURCE_CTX_PTR, OUT TIME_SOURCE_LOG_VALUES*);


//Add new observer
STATUS attach(IN TIME_OBSERVER_PTR Observer);
//Remove observer
STATUS detach(IN TIME_OBSERVER_PTR Observer);


#endif /* TIME_SOURCE_H_ */
