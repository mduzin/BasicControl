/*
 * test_env.h
 *
 *  Created on: Aug 23, 2015
 *      Author: mduzinki
 */

#ifndef TEST_ENV_H_
#define TEST_ENV_H_

/*naglowek ktory grupuje wszystko co zwiazane z
-srodowiskiem symulacji
-sygnalem wejsciowych
-regulatorem
-modelem obiektu*/

#include "common.h"
//#include "input_signal.h"
#include "sim_env.h"
#include "regulator_PID.h"
#include "model_1.h"
#include "file_lib.h"


/** @brief typy funcji zwiazane z sygnalami wejsciowymi */
//typef funkcji sygnalu wejsciowego jako u=f(t)
typedef double (*INPUT_SIGNAL_FUNC)(
		double
   );


/** @brief Struktura inicjalizujaca*/
typedef struct _INIT_TEST_VALUES
{
	INIT_SIMULATION_PARAM sim;
	INIT_PID_PARAM        pid;
	INIT_MODEL_PARAM      model;
	INIT_LOG_PARAM        log;
}INIT_TEST_VALUES;

/** @brief typy funkcji powiazane z regulatorem*/
typedef STATUS (*REGULATOR_RUN_EQUATION)(
		SIMULATION_PARAM *simulation,
		PID_PARAM *regulator,
		MODEL_PARAM *model
   );

typedef STATUS (*REGULATOR_INIT)(
		IN INIT_PID_PARAM *init_values,
		IN SIMULATION_PARAM *SimulationParams,
		OUT PID_PARAM *regulator
   );

typedef STATUS (*REGULATOR_CLOSE)(
		PID_PARAM *regulator
   );


/* @brief typy funkcji powiazane z modelem obiektu*/
typedef STATUS (*MODEL_RUN_EQUATION)(
		SIMULATION_PARAM *simulation,
		PID_PARAM *regulator,
		MODEL_PARAM *model
   );

typedef STATUS (*MODEL_INIT)(
		IN INIT_MODEL_PARAM *init_values,
		IN SIMULATION_PARAM *SimulationParams,
		OUT MODEL_PARAM *model
   );

typedef STATUS (*MODEL_CLOSE)(
		MODEL_PARAM *model
   );



/** @brief typy funkcji powiazane z srodowiskiem symulacji*/
typedef STATUS (*SIM_ENV_INIT)(
		IN INIT_SIMULATION_PARAM *init_values,
		OUT SIMULATION_PARAM *SimulationParams
   );

typedef STATUS (*SIM_ENV_CLOSE)(
		SIMULATION_PARAM *SimulationParams
   );

typedef STATUS (*SIM_ENV_UPDATE)(
		SIMULATION_PARAM *simulation
   );

typedef STATUS (*SIM_ENV_INPUT_SIGNAL)(
		SIMULATION_PARAM *simulation
		);


/** @brief typy funkcji powiazane z logowaniem*/
typedef STATUS (*LOG_INIT)(
		IN INIT_LOG_PARAM init_param,
        IO LOG_PARAM *log
   );

typedef STATUS (*LOG_CLOSE)(
		IN LOG_PARAM *log
   );

typedef STATUS (*LOG_WRITE)(
		IO LOG_PARAM *log,
		IN SIMULATION_PARAM *simulation,
		IN PID_PARAM *regulator,
		IN MODEL_PARAM *model
   );



/**Zbiory typow funcji*/

typedef struct _PID_FUNC
{

 REGULATOR_RUN_EQUATION Run; //glowna funkcja regulatora, zwraca CS na podstawie info o parametrach symulacji
 REGULATOR_INIT Init; //funkcja inicjalizujaca regulator
 REGULATOR_CLOSE Close;	//funkcja zwiajaca regulator

} PID_FUNC;



typedef struct _MODEL_1_FUNC
{

 MODEL_RUN_EQUATION Run; //glowna funkcja modelu obiektu
 MODEL_INIT  Init;        //funkcja inicjalizujaca model obiektu
 MODEL_CLOSE Close;	     //funkcja zwiajaca model obiektu

} MODEL_1_FUNC;


typedef struct _LOG_FUNC
{

 LOG_WRITE Write;   //zapisz wiersza logu
 LOG_INIT  Init;     //funkcja inicjalizujaca log
 LOG_CLOSE Close;	//funkcja zwiajaca log

} LOG_FUNC;


typedef struct _SIM_ENV_FUNC
{

 SIM_ENV_UPDATE       Iter;          // glowna funkcja: update srodowiska symulacji
 SIM_ENV_INPUT_SIGNAL Input;         // funcja sygnalu wejsciowego SP
 SIM_ENV_INIT         Init;          // funkcja inicjalizujaca srodowisko symulacji
 SIM_ENV_CLOSE        Close;	     // funkcja zwiajaca srodowisko symulacji

} SIM_ENV_FUNC;


/**Funckje sygnalow wejsciowych*/
STATUS step_signal(SIMULATION_PARAM *simulation);
STATUS rectangle_signal(SIMULATION_PARAM *simulation);
STATUS saw_signal(SIMULATION_PARAM *simulation);


/** inicjalizacja srodowiska symulacji */
STATUS simulation_init(IN INIT_SIMULATION_PARAM *init_values, OUT SIMULATION_PARAM *simulation);
/** zwijanie srodowiska symulacji */
STATUS simulation_close(SIMULATION_PARAM *simulation);
/**krok symulacji */
STATUS simulation_iter(SIMULATION_PARAM *simulation);



/** inicjalizacja modelu obiektu */
STATUS model_init(IN INIT_MODEL_PARAM *init_values,SIMULATION_PARAM *simulation, OUT MODEL_PARAM *model);
/** zwijanie modelu obiektu */
STATUS model_close(MODEL_PARAM *model);
/** rownanie modelu regulatora */
STATUS model_run(SIMULATION_PARAM *simulation,PID_PARAM *regulator,MODEL_PARAM *model);


/** inicjalizacja regulatora */
STATUS regulator_init (IN INIT_PID_PARAM *init_values,IN SIMULATION_PARAM *SimulationParams,OUT PID_PARAM *regulator);
/** zwijanie regulatora */
STATUS regulator_close(PID_PARAM *regulator);
/** rownanie regulatora */
STATUS regulator_run(SIMULATION_PARAM *simulation,PID_PARAM *regulator,MODEL_PARAM *model);


//strutura z funkcjami srodowiska symulacji
extern SIM_ENV_FUNC sim_func;
//strutura z funkcjami regulatora
extern PID_FUNC regulator_func;
//strutura z funkcjami regulatora
extern MODEL_1_FUNC model_func;


#endif /* TEST_ENV_H_ */
