/* utils.c
 *
 *  Created on: 14 cze 2016
 *
 */

#include "includes.h"

#define endline 1
#define switchRec 2

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

uint8_t initialdiode (uint8_t mode)
{
	if (diodesPerSample)
	{
		//if diodes are switched per sample
		//diodes switch BEFORE first measurement, so it's set to the last diode, and then try switching
		//switching procedures will assure, that only the diodes selected in the mode will be used
		diode = 2;
	}
	else
	{
		// if diodes change per repetition, the first diode change will be after the first one.
		// therefore, the valid first diode needs to be selected from the start
		if (mode & 1) //0 bit of the mode - use diode 0 IR diode
			diode = 0;
		else if (mode & 2)
			diode = 1;
		else
			diode = 2;
	}
}

void runConfig(uint16_t *samplingT, int *measurementTime, int *cooldownTime,
		uint8_t *mode, uint8_t *repeats) {

	PORTD = (1 << ind);
	*samplingT = wait4input("sampling period (in ms): ");

	//admin mode test, will be removed later

	*measurementTime = wait4input("measurementTime (in ms):");
	*cooldownTime = wait4input("cooldownTime (in ms):");
	*repeats = wait4input("repeats:");

	displayString("mode bits:");
	displayString("0 - IR diode used");
	displayString("1 - Red diode used");
	displayString("2 - Green diode used");
	displayString("3 - D per sample/~repea");
	displayString("4 - receiver 1 used:");
	displayString("5 - receiver 2 used:");
	displayString("6 - R per allDio/~repea");
	displayString("7 - admin mode test")
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

bool canSwitchReciever (uint8_t recieverCount) {
	//reciever can be changed per alldiodes change (1 reciever measures all diodes, then changes)
	//or per repetition (regardless of the diodes)

	static bool rCounter = 0; //receiverscounter (only 2 receivers, so bool is fine)

	if (rCounter == (recieverCount - 1) ) {
		rCounter = 0;
		return 1; //endline
	}
	rCounter++;
	return 0;
}

uint8_t doTheSwitching(uint8_t *diode, bool *reciever, uint8_t mode) {

	//the (do the switching method can be run in two situations : per sample, or per repetition
	//the method works for both, depending on mode

	//this code is mode independent,
	//whether the method was run on next sample, or on start of the repetition,
	//diode changes ***********

	static uint8_t dCounter = 0; //diodes counter
	uint8_t diodesCount =
			((mode & (1 << 0)) && 1) //by using && 1 I ensure, that the value will be equal 1, if true
			+ ((mode & (1 << 1)) && 1) //so that I can add 3 boolan value to get an int from 0 to 3
			+ ((mode & (1 << 2)) && 1);

	uint8_t recieverCount =
			((mode & (1 << 4)) && 1)
			+ ((mode & (1 << 5)) && 1);

	do									//do the diode switching, until encountered a one that was allowed in 'mode'
		*diode = (*diode + 1) % 3; 	 	//diode values changing from 0 to 2
	while (!((1 << *diode) & mode)); 	//checking if current diode was allowed
										// (1 << diode) gives a corresponding bit in the 'mode' byte

	dCounter++; //after successful diode switch, add to diodes counter

	//*****************************************//

	uint8_t changeState = 0; //changeState for processing in main


	if ((mode & (1 << 6))) //if a reviec
	{
		if (canSwitchReciever(recieverCount)) //check if can switch receivers
			retval = switchRec; //switchingReceivers allowed
	}

	if (dCounter == diodesCount)
	{
		if ( (!(mode & (1 << 6))) )
		{
			retval = endline;
			if (canSwitchReciever(recieverCount))
				retval |= switchRec; //endline here
		}
		else retval |= 1;
		dCounter = 0;
	}


	return retval; //not all diodes have been used
}



