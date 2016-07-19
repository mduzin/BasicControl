/*
 * model_1.h
 *
 *  Created on: Aug 9, 2015
 *      Author: mduzinki
 */

#ifndef MODEL_1_H_
#define MODEL_1_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "sim_env.h"

//ADT
typedef struct _FIRST_ORDER_MODEL* FIRST_ORDER_MODEL_PTR;

//API for ADT
STATUS FirstOrderModelInit(FIRST_ORDER_MODEL_PTR*);
STATUS FirstOrderModelRun(FIRST_ORDER_MODEL_PTR);
STATUS FirstOrderModelClose(FIRST_ORDER_MODEL_PTR);

typedef struct _INIT_MODEL_PARAM
{
	double k;	   //[-]
	double Ts;	   //[s]
	double Tdelay; //[s]
}INIT_MODEL_PARAM;


typedef struct _RUNTIME_MODEL_PARAM
{
  double u;					//sygnal wejsciowy
  double y;					//sygnal wyjsciowy
  double calka;	            //zmienna wewnetrzna obiektu na ktorej obliczany ca³kowanie

  /*Zmienne z poprzedniej iteracji symulacji obiektu*/
  double prev_y;					//sygnal wyjsciowy
  double buff_y;
  double prev_calka;	        //zmienna wewnetrzna obiektu na ktorej obliczany ca³kowanie w obiekcie

  double *delay_array;				//wskaznik na tablice z opoznionymi probkami
  int delay_array_size;				//rozmiar tablicy z opoznionymi probkami


} RUNTIME_MODEL_PARAM;

typedef struct _MODEL_PARAM
{
  double k;		 //[-]
  double Ts;	 //[s]
  double Tdelay; //[s]
  unsigned char  Initialized :1,
				 Run 	     :1,
				 Stop		 :1,
				 Error	     :1,
				 Reserved    :5;
  RUNTIME_MODEL_PARAM Runtime;

} MODEL_PARAM;








#endif /* MODEL_1_H_ */
