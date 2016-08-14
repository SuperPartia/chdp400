#include "includes.h"
#define F_CPU 1000000
volatile uint8_t repeatsCount;
volatile bool repeatHappened = false;



int main(void) {
	uint16_t samplingT = 0;
	int measurementTime = 0;
	int cooldownTime = 0;
	uint8_t repeats = 0;
	uint8_t mode = 0;

	bool currentReciever;
	uint8_t diode = 2;

	uint8_t changeState = 0; //1 last diode, 2 changeRecievers, 3 both
	initAll();

	while (1) {
		runConfig(&samplingT, &measurementTime, &cooldownTime, &mode, &repeats);

		repeatHappened = false;
		_working = false;
		repeatsCount = 0;
		uint8_t previousRepeatsCount = 0;
		bool diodesPerSample = mode & (1 << 3);
		if (diodesPerSample)
			diode = 2;
		else
		{
			if (mode & 1)
				diode = 0;
			else if (mode & 2)
				diode = 1;
			else
				diode = 3;
		}
		//diodes are changing per each sample

		currentReciever = !(mode & (1 << 4));
		bool recieverChange = 0;
		// FIRST reciever is rec0, whenever 4 is set (5 can be set too)
		//if 4 isn't set, 5 must be set, so then FIRST reciever would be rec1.


		startTimer0(samplingT);
		startTimer1(measurementTime, cooldownTime);
		displayString("data = [");
		while (repeatsCount < repeats) { //2* repeats, because repeatsCount changes on each timer1 finish (and is int, can't be devided/2)
			while (!_samplingReady)
				if (diodesPerSample)
					changeState = doTheSwitching(&diode, &currentReciever, mode);
				else {
					if (repeatHappened) {
						repeatHappened = false;
						changeState = doTheSwitching(&diode, &currentReciever, mode);
					}
				}
				; //set in interrupt
			_setup = true;
			PORTD = _working * (1 << ir + diode); //ir + 0, 1, 2
			sendData((diode + 10*currentReciever)* _working, changeState);
			//	sendData(adcRead(currentReciever) * _working, changeState);
			PORTD = 0;
			currentReciever ^= ((changeState & 2) && 1); //&& (mode & 1<<6);
			_samplingReady = false;
			changeState = 0;
			_setup = false;
			previousRepeatsCount = repeatsCount;
		}
/*		while (1) {
			do
				diode = (diode + 1) % 3; //0, 1, 2 changing diode value
			while (!((diode + 1) & mode)); //1, 2, 3
			while (!_samplingReady)
				sendData(0, false);
			if
		}*/
		stopTimer0();
		stopTimer1();
		displayString("];");
	}

}
