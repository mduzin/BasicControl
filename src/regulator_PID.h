/*
 * regulator_PI.h
 *
 *  Created on: Aug 8, 2015
 *      Author: mduzinki
 */

#ifndef REGULATOR_PI_H_
#define REGULATOR_PI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

//ADT
typedef struct _REG_PID* REG_PID_PTR;

//API for ADT
STATUS RegPidInit(REG_PID_PTR*);
STATUS RegPidClose(REG_PID_PTR);
void   RegPidRun(void* pInstance, const TIME_EVENT Events);

typedef enum _ANTI_WINDUP_TYPE
{
	NO_ANTI_WINDUP = 0,
	BACK_CALCULATION,		//Tracking anti-windup, back-calculation
	INTEGRATOR_CLAMPING            //Integrator clamping
}ANTI_WINDUP_TYPE;

/** @brief parametry inicjalizujace regulatora */
typedef struct _INIT_PID_PARAM
{
  BOOL Reg_On;
  BOOL P_sel;
  BOOL I_sel;
  BOOL D_sel;
  int  AntiWindup;
  BOOL AntiWindupV1_sel;
  BOOL AntiWindupV2_sel;
  double kp;
  double Ti;
  double Td;
  double Tp;		//okres probkowania
  double Tt;		//antiwindup tracking time
  double CS_min;	//mininalny Control Signal
  double CS_max;    //maksymalny Control Signal
}INIT_PID_PARAM;



/** @brief parametry runtime regulatora */
typedef struct _RUNTIME_PID_PARAM
{
 double P;
 double I;
 double D;
 double CS_raw;	//Control Signal przed sprawdzaniem limitow wyjscia regulatora
 double CS;	    //aktualny Control Signal podany na obiekt
 long il_pr_CS;	//co ile probek(iteracji symulacji) obliczyc CS
 long zl_il_pr_CS; //biezaca zliczona ilosc probek


 double e; //	aktualny uchyb
 double es; //	aktualny uchyb dla antiwindup
 double calka_e;
 double calka_es;
 double rozniczka_e;
 double prev_e;
 double prev_calka_e;
 double prev_es;
 double prev_calka_es;

} RUNTIME_PID_PARAM;





/** @brief parametry regulatora */
typedef struct _PID_PARAM
{
 BOOL Pid_On;
 BOOL P_sel;
 BOOL I_sel;
 BOOL D_sel;
 BOOL AntiWindupV1_sel;	//Tracking anti-windup, back-calculation
 BOOL AntiWindupV2_sel; //Integrator clamping
 double kp;
 double Ti;
 double Td;
 double Tp;		//okres probkowania
 double Tt;		//windup tracking time
 double CS_min;	//mininalny Control Signal
 double CS_max; //maksymalny Control Signal
 unsigned char     Initialized :1,
				   Run 	       :1,
				   Stop		   :1,
				   Error	   :1,
				   Saturation  :1,
				   Reserved    :4;
 RUNTIME_PID_PARAM Runtime;

} PID_PARAM;









#endif /* REGULATOR_PI_H_ */
