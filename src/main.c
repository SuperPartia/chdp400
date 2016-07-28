
#include "includes.h"
#define F_CPU 1000000


int main(void)
{
	uint16_t freq = 0;
	uint8_t mode = 0;
	int count = 0;

	initAll();
	runConfig(&freq, &count, &mode);

	while(1){
	}
}


void initAll()
{

	MCUCSR|= (1<<7);
	MCUCSR|= (1<<7);
	sei();

	uartStart();
	adcInit();
	//DDR na diodki, nie pamiętam na jakim porcie
}

void runConfig(uint16_t *freq, int *count, uint8_t *mode)
{

	freq = wait4input("sample frequency (in kHz):");
	count = wait4input("sample count (in k):");
	mode = wait4input("mode:");

	displayString("freq =");
	displayInt(freq);
	displayString("count =");
	displayInt(count);
	displayString("mode =");
	displayInt(mode);

}
