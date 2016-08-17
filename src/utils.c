/* utils.c
 *
 *  Created on: 14 cze 2016
 *
 */

#include "includes.h"

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
	displayString("3 - D per sample/~repea");
	displayString("4 - receiver 1 used:");
	displayString("5 - receiver 2 used:");
	displayString("6 - R per allDio/~repea");
	//30 - r, g, co probkr / jeden rec
	//22 - r, g co repeat
	//62 - r, g co repeat, recievery na zmianę co diodę
	//126 - r, g co turę, recievery co obie diody

	while (1) {
		*mode = wait4input("mode:");
		if ((*mode & 7) //at least one diode
		&& (*mode & 48)) //at least one reciever
			break;
		*mode = 0;
		displayString("invalid mode...");
	}
	*repeats = 10;//wait4input("repeats:");

}

uint8_t switchReciever (uint8_t recieverCount)
{
	uint8_t retval = recieverCount;
	static bool rCounter = 1; //diodes counter

	//*reciever ^= recieverCount;
	if (rCounter == recieverCount)
	{
		rCounter = 0;
		retval |= 1; //endline
	}
	rCounter++;

	return retval;
}

uint8_t doTheSwitching(uint8_t *diode, bool *reciever, uint8_t mode) {

	static uint8_t dCounter = 0; //diodes counter
	uint8_t diodesCount = ((mode & (1 << 0)) && 1) + ((mode & (1 << 1)) && 1)
			+ ((mode & (1 << 2)) && 1);
	uint8_t recieverCount = ((mode & (1 << 4)) && 1) + ((mode & (1 << 5)) && 1);
	//according to mode, bits 0, 1, 2 mean diodes that are used


	do
		*diode = (*diode + 1) % 3; //0, 1, 2 changing diode value
	while (!((1 << *diode) & mode)); //1, 2, 3 (*diode + 1) & mode
	//if mode == 1 or 2 or 3, it means we can select the diode. otherwise switch again
	dCounter++;

	uint8_t retval = 0;
	//if (didoesPerSample)
	if ((mode & (1 << 6)))
	{
		retval = switchReciever(recieverCount);
		retval &= ~1; //no endline here
	}

	if (dCounter == diodesCount)
	{
		if ( (!(mode & (1 << 6))) )
		{
			retval = switchReciever(recieverCount);
		}
		else retval |= 1;
		dCounter = 0;
	}


	return retval; //not all diodes have been used
}



