#include "includes.h"
#define F_CPU 1000000
volatile uint8_t repeatsCount;
volatile bool repeatHappened = false;



int main(void) {
	initAll();

	bool accRange[] = {1, 0};
	bool accMode[] = {1, 0};
	uint8_t x, y, z;
	uint8_t old_x, old_y, old_z;
	uint8_t treshold = 8;
	setMeasuringMode(accRange, accMode);
	while (1) {
		old_x = x;
		old_y = y;
		old_z = z;
		readXYZ(&x, &y, &z);
		if ((abs(x-old_x)<=treshold) & (abs(y-old_y)<=treshold) & (abs(z-old_z)<=treshold))
		{
			PORTD|= (1<<ind);
		}
		else
		{
			PORTD&= ~(1<<ind);
		}
		_delay_ms(500);

	}

}
