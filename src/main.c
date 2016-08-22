#include "includes.h"
#define F_CPU 1000000
volatile uint8_t repeatsCount;
volatile bool repeatHappened = false;



int main(void) {
	//config data **************//

	uint16_t samplingT = 0;
	int measurementTime = 0;
	int cooldownTime = 0;
	uint8_t repeats = 0;
	uint8_t mode = 0;

	//***************************//

	//starting diode and receiver
	bool currentReciever;
	uint8_t diode;
	//**************************//

	uint8_t changeState = 0;
	//chagne state is the supportive value, that allows deciding what is the next step
	//0 - normal. Switch diode,
	//1 - send endline to data
	//2 - switch receivers,
	//3 - switch receivers end send endline

	initAll();

	// this loop allows the uC to work continously, even after finishing the measurement, it will act like restarted
	while (1) {
		//user config getter method
		runConfig(&samplingT, &measurementTime, &cooldownTime, &mode, &repeats);
		//***************************//

		//initial values of control variables
		repeatHappened = false;
		_working = false;
		repeatsCount = 0;
		bool diodesPerSample = mode & (1 << 3);
		//***************************//

		diode = initialDiode(mode);

		currentReciever = !(mode & (1 << 4)); // 0 if rec0 is anabled
											  // 1 if rec0 is not anabled (then rec1 has to be active, and therefore "first"

		startTimer0(samplingT);
		startTimer1(measurementTime, cooldownTime);
		displayString("data = ["); //starting data string for further processing


		while (repeatsCount < repeats) { //repeats loop
			while (!_samplingReady); //breaks, if the sampling period has passed (set in timer0 interrupt)
			_samplingReady = false;
			_setup = true;			 //this assures, that if the sampling period is too low
									 //and the timer counted down another sample, before the measurement was done,
									 //an error notification appears and the circuit should be restarted.

				if (diodesPerSample) //if diodes are changed per sample, start switching procedures
				{
					changeState = doTheSwitching(&diode, &currentReciever, mode);
				}
				else {					  //if diodes are switched per repetition,
					if (repeatHappened) { //start switching only if a a repeat just happened (set in timer1 interrupt)
						repeatHappened = false;
						changeState = doTheSwitching(&diode, &currentReciever, mode);
					}
				}
										 	 	 	 	 //diodes are placed on high bits of the port D, starting from diode IR
			PORTD = _(1 << (ir + diode))  * _working 	 //IR + 0, 1, 2 gives the current diode
					      	  	  	  	  	  	  	  	 //if currently not working (_working == 0), don't light up diodes


			if ( !(mode & (1<<7)) ) //if not mode admin test
			{
						//send data method sends the int value via uart in the syntax appropiate for further processing
						//if (changestate & 1) == 1, that means an endline sign will be sent, so the method separates data by ';'
						//else, the method separates data by ' '
				sendData(adcRead(currentReciever) * _working, changeState);
				//if currently not working (_working == 0), send 0 as part of data (cooldown is needed for processing too
			}
			else //mode admin test sends, instead of the values, the number
			{
				//which shows the current diode and the reciever that would be used in the measurement
				sendData((diode + 10*(1+currentReciever))* _working, changeState); //format DR, diodes 0-2, receivers 1-2
			}
			PORTD = 0;
			currentReciever ^= ((changeState & 2) && 1); //(changeState & 2) implifies if the receiver should be changed ( &&1 to ensure boolan )

			changeState = 0;
			_setup = false;
		}
		stopTimer0();
		stopTimer1();
		displayString("];");
	}

}
