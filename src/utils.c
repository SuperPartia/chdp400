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


void runConfig(uint16_t *samplingT, int *measurementTime,
		uint8_t *mode) {

	PORTD = (1 << ind);
	*samplingT = wait4input("sampling period (in ms): ");

	//admin mode test, will be removed later

	*measurementTime = wait4input("measurementTime (in ms):");
	//*cooldownTime = wait4input("cooldownTime (in ms):");
//	*repeats = wait4input("repeats:");

	displayString("mode bits:");
	displayString("0 - IR diode used");
	displayString("1 - Green diode used");
	displayString("2 - Red diode used");
	displayString("3 - D per sample/~repea");
	displayString("4 - receiver 1 used:");
	displayString("5 - receiver 2 used:");
	displayString("6 - R per allDio/~repea");
	displayString("7 - admin mode test");
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

}





