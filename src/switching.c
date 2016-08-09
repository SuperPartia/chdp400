/* adc.c
 *
 *  Created on: 14 cze 2016
 *
*/

#include "includes.h"

bool switchReciever ()
{
	static bool recChange = 0; //reciever change counter (reciever changes when counter is 1)

	//rec= 0, change = 0; 1st in
	//rec= 0, change = 1; 2nd
	//rec= 1, change = 0; 3rd
	//rec= 1, change = 1; 4th
	recChange ^= 1;
	if (recChange == 1)
	{
		return 0;
	}
	return 1; //if all diodes and recievers have been used, we signal it was the last line and we need 'newline' on uar
}

uint8_t doTheSwitching(
		uint8_t *diode,
		bool *reciever,
		uint8_t mode) {

	static uint8_t counter = 0; //diodes counter
	uint8_t diodesCount = ((mode & (1 << 0)) && 1)  + ((mode & (1 << 1)) && 1) + ((mode & (1 << 2)) && 1);
	uint8_t recieverCount = ((mode & (1 << 4)) && 1)  + ((mode & (1 << 5)) && 1);
	//according to mode, bits 0, 1, 2 mean diodes that are used
	uint8_t retval = 0;

	do
		*diode = (*diode + 1) % 3; //0, 1, 2 changing diode value
	while (!((*diode + 1) & mode)); //1, 2, 3
	//if mode == 1 or 2 or 3, it means we can select the diode. otherwise switch again
	counter++;

	if (counter == diodesCount) //endof diodes
		{
		//all diodes used
		counter = 0;
		bool recieverSwitched = 0;
		if (recieverCount == 2) {
			if (mode & (1 << 6)) //recieverchagning per endof diodes (not per repetition)
			{
				retval = 2; // switching recievers, that's for sure
				retval += switchReciever();
				//if all recievers have been used (also diodes) we can return endline and switch recievers (3)
				//if not all recievers, then just switch reciever  (2)
			} else if (repeatHappened) {

				repeatHappened = 0;
				displayString('tata');
				switchReciever();
				retval = 3;
			}
		}
		else
			retval = 1;  //if all diodes have been used, we signal it was the last line and we need 'newline' on uart (1)

	}
	return retval; //not all diodes have been used
	//return !(counter = counter == (diodesCount) ? 0 : counter);
}


