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

//	displayInt(twiGetData(0x1A));
//	twiSendData(8, 0x1A);
//	displayInt(twiGetData(0x1A));
	twiSendData(5, 0x16);

	while (1) {
		displayString("X: ");
		displayInt(twiGetData(0x06));
		displayString("Y: ");
		displayInt(twiGetData(0x07));
		displayString("Z: ");
		displayInt(twiGetData(0x08));
		_delay_ms(1000);
	}

}
