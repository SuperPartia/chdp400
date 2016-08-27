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
	startTimer0(*samplingT);
	while(!(samplingReady));
	stopTimer0();
	samplingReady = false;
	return value;
}

void measure_loop(uint16_t *samplingT, int *measurementTime,
		uint8_t *mode)
{
	bool recieverMode = *mode & (1<<6);
	bool diodeMode = *mode & (1<<3);
	bool channelSwitch = false;
	uint8_t diodesAmount = 0;
	uint16_t value;

	for (uint8_t diode = 0; diode < 3; diode++) if (*mode & (1<<diode)) diodesAmount++;
	if(!diodesAmount) return;
	startTimer1(*measurementTime);
	measureFlag = true;

	while(measureFlag){

		if(diodeMode)
		{
			for (uint8_t diode = ir; diode <= red; diode++) {
				if(!(*mode & (1<<(diode-ir)))) continue;

				PORTD |= (1<<diode);

				if(recieverMode)
					value = do_measure(((diode-ir)%2), samplingT);
				else
					value = do_measure(channelSwitch, samplingT);

				PORTD &= (1<<diode);
			}
			if (!recieverMode) channelSwitch = !channelSwitch;
		}



		else
		{
			for (uint8_t diode = ir; diode <= red; diode++) {
				if(!(*mode & (1<<(diode-ir)))) continue;
				for (uint8_t i = 0; i <= diodesAmount; i++) { // tu różnica z poprzednim

					PORTD |= (1<<diode);

					if(recieverMode)
						value = do_measure(((diode-ir)%2), samplingT);
					else
						value = do_measure(channelSwitch, samplingT);

					PORTD &= (1<<diode);
				}
				if (!recieverMode) channelSwitch = !channelSwitch;
			}
		}
	}
	stopTimer1();
}
