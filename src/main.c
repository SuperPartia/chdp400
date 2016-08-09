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

		repeatsCount = 0;

		bool diodesPerSample = mode & (1 << 3);
		//diodes are changing per each sample

		currentReciever = !(mode & (1 << 4));
		bool recieverChange = 0;
		// FIRST reciever is rec0, whenever 4 is set (5 can be set too)
		//if 4 isn't set, 5 must be set, so then FIRST reciever would be rec1.


		startTimer0(samplingT);
		startTimer1(measurementTime, cooldownTime);

		displayString("data = [");
		while (repeatsCount <= repeats) { //2* repeats, because repeatsCount changes on each timer1 finish (and is int, can't be devided/2)


			changeState = doTheSwitching(&diode, &currentReciever, mode);


			while (!_samplingReady)
				; //set in interrupt
			_setup = true;
			PORTD = _working * (1 << ir + diode); //ir + 0, 1, 2
				sendData((diode + 10*currentReciever)* _working, changeState);
			//	sendData(adcRead(currentReciever) * _working, changeState);
			PORTD = 0;
			if (changeState & 2)
				currentReciever ^= 1;
			_samplingReady = false;
			_setup = false;
		}
		stopTimer0();
		stopTimer1();
		displayString("];");
	}
}

/*
 * Matrix w matlabie:
 * (working)
 * IR1 R1 G1 IR2 R2 G2; mode dependant length
 * IR1 R1 G1 IR2 R2 G2; mode dependant length
 * (!working)
 * 0   0  0   0  0  0 ; mode dependant length
 * 0   0  0   0  0  0 ; mode dependant length
 * (working)
 * IR1 R1 G1 IR2 R2 G2; mode dependant length
 * IR1 R1 G1 IR2 R2 G2; mode dependant length
 * (!working)
 * 0   0  0   0  0  0 ; all_samples
 * 0   0  0   0  0  0 ; all_samples
 */

void initAll() {

	MCUCSR |= (1 << 7);
	MCUCSR |= (1 << 7);
	sei();

	uartStart();
	adcInit();

	DDRD = (1 << red) | (1 << green) | (1 << ir) | (1 << ind);
	initTimer0();
	initTimer1(15624);

}

void runConfig(uint16_t *samplingT, int *measurementTime, int *cooldownTime,
		uint8_t *mode, uint8_t *repeats) {

	PORTD = (1 << ind);
	*samplingT = wait4input("sampling period (in ms): ");
	*measurementTime = wait4input("measurementTime (in ms):");
	*cooldownTime = wait4input("cooldownTime (in ms):");

	displayString("mode bits:");
	displayString("0 - IR diode used");
	displayString("1 - Red diode used");
	displayString("2 - Green diode used");
	displayString("3 - D per sample/~period");
	displayString("4 - receiver 1 used:");
	displayString("5 - receiver 2 used:");
	displayString("6 - R per diode/~period");
	//30 - r, g, co probkr / jeden rec
	//22 - r, g co repeat
	//62 - r, g co turę, recievery na zmianę co repeat
	//126 - r, g co turę, recievery co obie diody

	while (1) {
		*mode = wait4input("mode:");
		if ((*mode & 7) //at least one diode
		&& (*mode & 48)) //at least one reciever
			break;
		*mode = 0;
		displayString("invalid mode...");
	}
	*repeats = wait4input("repeats:");

}
