/*
 * sim_env.h
 *
 *  Created on: Aug 10, 2015
 *      Author: mduzinki
 */

#ifndef SIM_ENV_H_
#define SIM_ENV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

typedef struct _INIT_SIMULATION_PARAM
{
   double Tsym;	     //czas symulacji
   double Tc;	     //okres  calkowania obiektu
}INIT_SIMULATION_PARAM;

typedef struct _RUNTIME_SIMULATION_PARAM
{
	double akt_SP;		//aktualny SetPoint
	double akt_Tsym;	//aktualny czas symulacji
	long   akt_pr_Tsym;	//aktualna probka symulacji

} RUNTIME_SIMULATION_PARAM;

typedef struct _SIMULATION_PARAM
{
	double Tsym;	 //czas symulacji
	double Tc;	     //okres  calkowania obiektu !!!! NAJWAZNIEJSZY PARAMETR !!!!!!
	long il_pr_Tsym;	//ilosc probek do obliczenia symulacji
	unsigned char  Initialized :1,
				   Run 	       :1,
				   Stop		   :1,
				   Error	   :1,
				   Reserved    :5;

	RUNTIME_SIMULATION_PARAM Runtime;

} SIMULATION_PARAM;



#endif /* SIM_ENV_H_ */
