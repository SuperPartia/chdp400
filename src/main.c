#include "includes.h"
#define F_CPU 1000000

bool doTheSwitching (uint8_t *diode, uint8_t mode, uint8_t diodesCount, uint8_t receiversCount)
{
	static uint8_t counter = 0;
	do
		*diode = (*diode + 1)%3; //0, 1, 2
	while (!((*diode+1) & mode)); //1, 2, 3
	counter++;
	//return !(counter = counter == diodesCount ? 0 : counter);
}
void initAll()
{

	MCUCSR|= (1<<7);
	MCUCSR|= (1<<7);
	sei();

	uartStart();
	adcInit();

	DDRD = (1<<red) | (1<<green) | (1<<ir) | (1<<ind);
	initTimer0();
	initTimer1(15624);

}

void runConfig(uint16_t *samplingT, int *measurementTime, int *cooldownTime, uint8_t *mode, uint8_t *repeats)
{

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


	while(1)
	{
	*mode = wait4input("mode:");


	if ( (*mode & 7) //at least one diode
			&& (*mode & 48) ) //at least one reciever
		break;
	*mode = 0;
	displayString("invalid mode...");
	}
	*repeats = wait4input("repeats:");

}


int main(void)
{
	uint16_t samplingT = 0;
	int measurementTime = 0;
	int cooldownTime = 0;
	uint8_t repeats = 0;
	uint8_t mode = 0;

	initAll();
	runConfig(&samplingT, &measurementTime, &cooldownTime, &mode, &repeats);

	uint8_t diodesCount = (mode & (1<<0))&&1 + (mode & (1<<1))&&1 + (mode & (1<<2))&&1;
	bool diodesPerSample = mode & (1<<3);

	uint8_t receiversCount = (mode & (1<<4))&&1 + (mode & (1<<5))&&1;
	bool receiversPerDiodes = mode & (1<<6);

	bool currentReciever = 0;
	uint8_t diode = 0;
	bool lastDiode = false;

	startTimer0(samplingT);
	startTimer1(measurementTime, cooldownTime);

	while(1) {
			if (!diodesPerSample)
				{
				//doTheSwitching(&diode, mode, diodesCount, receiversCount);
				lastDiode = true;
				}
			while(!_working);

			if (diodesPerSample)
			{
			//	lastDiode = doTheSwitching(&diode, mode, diodesCount, receiversCount);
			}
			while(!_samplingReady); //set in interrupt
			_setup = true;
			PORTD = (1<<ir + diode); //ir + 0, 1, 2
			sendData(adcRead(currentReciever), lastDiode);
			//displayInt(adcRead(currentReciever));
			PORTD = 0;
			_samplingReady = false;
			_setup = false;
	}
}

/*
 * Matrix w matlabie:
 * (working)
 * -1  -1 -1 -1  -1 -1 ; modes
 * IR1 R1 G1 IR2 R2 G2; modes
 * IR1 R1 G1 IR2 R2 G2; modes
 * (!working)
 * 0   0  0   0  0  0 ; modes
 * 0   0  0   0  0  0 ; modes
 * (working)
 * -1  -1 -1 -1  -1 -1 ; modes
 * IR1 R1 G1 IR2 R2 G2; modes
 * IR1 R1 G1 IR2 R2 G2; modes
 * (!working)
 * 0   0  0   0  0  0 ; all_samples
 * 0   0  0   0  0  0 ; all_samples
 * M[modeChange][sample/modeChange
*/


