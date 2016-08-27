#include "includes.h"
#define F_CPU 1000000
volatile uint8_t repeatsCount;
volatile bool repeatHappened = false;



int main(void) {
	//config data **************//

	uint16_t samplingT = 0;
	int measurementTime = 0;
	//uint8_t repeats = 0;
	uint8_t mode = 0;
	//chagne state is the supportive value, that allows deciding what is the next step
	//0 - normal. Switch diode,
	//1 - send endline to data
	//2 - switch receivers,
	//3 - switch receivers end send endline

	initAll();

	// this loop allows the uC to work continously, even after finishing the measurement, it will act like restarted
	while (1) {
		//user config getter method
		runConfig(&samplingT, &measurementTime, &mode);
		//***************************//


		displayString("data = ["); //starting data string for further processing

		displayString("];");
	}

}
