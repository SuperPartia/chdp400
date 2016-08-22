#include "includes.h"
#define F_CPU 1000000
volatile uint8_t repeatsCount;
volatile bool repeatHappened = false;



int main(void) {
	initAll();

	bool accRange[] = {1, 0};
	bool accMode[] = {1, 0};
	uint8_t threshold = 8;
	setMeasuringMode(accRange, accMode);
	while (1) {
		if (detectMove(threshold))
		{
			PORTD|= (1<<ind); //start measuring
		}
		else
		{
			PORTD&= ~(1<<ind); //do nothing, wait for no movement
		}
		_delay_ms(500);

	}

}
