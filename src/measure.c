/*
 * measure.c
 *
 *  Created on: 25 sie 2016
 *      Author: nkubiak
 */
#include "includes.h"

uint16_t do_measure(bool receiver, uint16_t *samplingT)
{
	uint16_t value;
	value = adcRead(receiver);
	while(!(samplingReady));
	samplingReady = false;
	sendData(value, true);
	return value;
}

void measure_loop(uint16_t *samplingT, int *measurementTime,
		uint8_t *mode)
{
	bool recieverMode = *mode & (1<<6);
	bool diodeMode = *mode & (1<<3);
	bool channelSwitch = !(*mode & (1<<4));
	uint8_t diodesAmount = 0;
	uint8_t transistorsAmount = 0;
	uint16_t value;

	for (uint8_t diode = 0; diode < 3; diode++) if (*mode & (1<<diode)) diodesAmount++;
	transistorsAmount = (*mode & (1<<4)) && (*mode & (1<<5));
	if(!diodesAmount) return;
	if(!(*mode & (1<<4)) && !(*mode & (1<<5))) return; // no transistors
	startTimer1(*measurementTime);
	measureFlag = true;

	while(measureFlag){

		if(diodeMode)
		{
			for (uint8_t diode = ir; diode <= red; diode++) {
				if(!(*mode & (1<<(diode-ir)))) continue;

				sendData(diode-ir, false);
				PORTD |= (1<<diode);

				sendData(channelSwitch, false);
				value = do_measure(channelSwitch, samplingT);

				if(recieverMode && transistorsAmount)
					channelSwitch = ! channelSwitch;

				PORTD &= (1<<diode);
			}
			if (!recieverMode && transistorsAmount) channelSwitch = !channelSwitch;
		}



		else
		{
			for (uint8_t diode = ir; diode <= red; diode++) {
				if(!(*mode & (1<<(diode-ir)))) continue;
				for (uint8_t i = 0; i < diodesAmount; i++) { // tu różnica z poprzednim

					sendData(diode-ir, false);
					PORTD |= (1<<diode);

					sendData(channelSwitch, false);
					value = do_measure(channelSwitch, samplingT);

					if(recieverMode && transistorsAmount)
						channelSwitch = ! channelSwitch;

					PORTD &= (1<<diode);
				}
				if (!recieverMode && transistorsAmount) channelSwitch = !channelSwitch;
			}
		}
	}
	stopTimer1();
}
