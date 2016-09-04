#include "includes.h"
#define F_CPU 1000000
volatile uint8_t repeatsCount;
volatile bool repeatHappened = false;



int main(void) {
	//config data **************//

	uint16_t samplingT = 0;
	int measurementTime = 0;
	uint8_t mode = 0;

	bool accRange[] = {1, 0};
	bool accMode[] = {1, 0};
	uint8_t threshold = 8;

	initAll();

	setMeasuringMode(accRange, accMode);

	// this loop allows the uC to work continously, even after finishing the measurement, it will act like restarted
	runConfig(&samplingT, &measurementTime, &mode);
	startTimer0(samplingT);
	displayString("data = [");
	while (1) {

		while(detectMove(threshold))
		{
		 //starting data string for further processing
		measure_loop(&samplingT, &measurementTime, &mode);

		}
	}

}
