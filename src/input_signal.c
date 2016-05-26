/*
 * input_signal.c
 *
 *  Created on: 26 maj 2016
 *      Author: M
 */


/Funcje sygnalow wejsciowych sa wywolywane w kazdej iteracji (czyli co okres calkowania)
//skok jednostkowy
STATUS step_signal(SIMULATION_PARAM *simulation)
{
    simulation->Runtime.akt_SP = 1.0;
	return STATUS_SUCCESS;
}

//sygnal prostokatny
STATUS rectangle_signal(SIMULATION_PARAM *simulation)
{
	double period = 25.0;  		 //czas co jaki zmieniamy stan wyjscia
	static double acc_period = 0.0; //zliczony czas trawania stanu
    static int state = 0;
    double output;


    if(0.0 != simulation->Runtime.akt_Tsym)
   	 {
   		 //dzialamy jest czas symulacji jest wiekszy od 0.0

   		 //zliczony czas period jest wiekszy-rowny od trwania period = zmiana stanu
   		 if(acc_period >= period)
   		 {

   			 //zmiana stanu
   			 if(++state > 2)
   			 {
   				//po stanie 4 wracamy do stanu 0
   				 state = 0;
   			 }

   			 //byla zmiana stanu wiec wyzeruj zliczony period(acc_period)
   			 acc_period = 0.0;
   		 }
   		 else
   		 {
   			 //nie ma zmiany stanu tylko zwieksz acc_period
   			 acc_period += simulation->Tc;
   		 }
   	 }
   	 else
   	 {
   		//inicjalizacja maszynki (simulation->Runtime.akt_Tsym == 0)
   		 state = 0;
   	 }
    switch(state)
    	 {
    	 case 0:
    		 output = 4.0;
    		 break;
    	 case 1:
    		 output = 0.0;
    		 break;
    	 default:
    		 output = 0.0;
    		 break;
    	 }

    	//wystaw do globalnej zmiennej symulacji
    	simulation->Runtime.akt_SP = output;


	return STATUS_SUCCESS;
}

//sygnal pila
STATUS saw_signal(SIMULATION_PARAM *simulation)
{
	 double period = 25.0;  		 //czas co jaki zmieniamy stan wyjscia
	 static double acc_period = 0.0; //zliczony czas trawania stanu
	 static int state  = 0;			 //stan maszynki 0,1,2,3
	 double output;					 //zwaracana wartosc



	 if(0.0 != simulation->Runtime.akt_Tsym)
	 {
		 //dzialamy jest czas symulacji jest wiekszy od 0.0

		 //zliczony czas period jest wiekszy-rowny od trwania period = zmiana stanu
		 if(acc_period >= period)
		 {

			 //zmiana stanu
			 if(++state > 4)
			 {
				//po stanie 4 wracamy do stanu 0
				 state = 0;
			 }

			 //byla zmiana stanu wiec wyzeruj zliczony period(acc_period)
			 acc_period = 0.0;
		 }
		 else
		 {
			 //nie ma zmiany stanu tylko zwieksz acc_period
			 acc_period += simulation->Tc;
		 }
	 }
	 else
	 {
		//inicjalizacja maszynki (simulation->Runtime.akt_Tsym == 0)
		 state = 0;
	 }

	 //ustaw wyjscie w zalezenosci od stanu
	 switch(state)
	 {
	 case 0:
	 case 2:
		 output = 0.0;
		 break;
	 case 1:
		 output = 1.0;
		 break;
	 case 3:
		 output = -1.0;
 		 break;
	 default:
		 output = 0.0;
		 break;
	 }

	//wystaw do globalnej zmiennej symulacji
	simulation->Runtime.akt_SP = output;
    return STATUS_SUCCESS;

}

