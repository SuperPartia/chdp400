/*
 * measure.c
 *
 *  Created on: 25 sie 2016
 *      Author: nkubiak
 */
#include "includes.h"

uint16_t do_measure(bool receiver, uint16_t *samplingT, bool *DetectMove)
{

	blockFlag = false;
	while(!(samplingFlag));
	samplingFlag = false;

	uint16_t value;
	value = adcRead(receiver);

	if(*DetectMove)
		sendData(value, true);
	else
		sendData(0, true);

	blockFlag = true;
	return value;
}

void measure_loop(uint16_t *samplingT, uint16_t *repeats, uint8_t *mode, uint8_t *threshold)
{
	uint16_t repeatsCounter = 0;
	bool recieverMode = *mode & (1<<6);
	bool diodeMode = *mode & (1<<3);
	bool receiver;// 0 red receiver
	if(*mode & (1<<4))
	{
		receiver = (*mode & (1<<0)) && (*mode & (1<<5));
	}
	else //czerwony nie zezwolony
	{
		receiver = 1;
	}

	uint8_t diodesAmount = 0;
	uint8_t transistorsAmount = 0;

	for (uint8_t diode = 0; diode < 3; diode++)
		if (*mode & (1<<diode)) diodesAmount++;

	transistorsAmount = (*mode & (1<<4)) && (*mode & (1<<5));
	//TODO move above to init measurement

	bool DetectMove = true;

	do { //measureFlag
		for (uint8_t diode = ir; diode <= red; diode++) {
			if(!(*mode & (1<<(diode-ir)))) continue;

			measureFlag = true;
			do { // tu różnica z poprzednim

				PORTD |= (1<<diode);

				sendData(diode-ir, false);
				sendData(receiver, false);

				do_measure(receiver, samplingT, &DetectMove);

				if(recieverMode && transistorsAmount)
					receiver = ! receiver;

				PORTD &= ~(1<<diode);
			} while (measureFlag && !diodeMode);
			if (!recieverMode && transistorsAmount) receiver = !receiver;
		}
		repeatsCounter++;
	} while(repeatsCounter != *repeats);
}
