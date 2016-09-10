#include "includes.h"
#define F_CPU 1000000

volatile bool repeatHappened = false;



int main(void) {
	//config data **************//

	uint16_t samplingT = 0;
	int measurementTime = 0;
	uint16_t repeats = 0;
	uint8_t mode = 0;

	bool accRange[] = {1, 0};
	bool accMode[] = {1, 0};
	uint8_t threshold = 8;

	initAll();

	setMeasuringMode(accRange, accMode);

	// this loop allows the uC to work continously, even after finishing the measurement, it will act like restarted
	runConfig(&samplingT, &measurementTime, &repeats, &mode);
	startTimer0(samplingT);
	startTimer1(measurementTime);
	displayString("data = [");
	//starting data string for further processing
	measure_loop(&samplingT, &repeats, &mode, &threshold);

	displayString("];");

}
