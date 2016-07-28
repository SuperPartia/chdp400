
#include "includes.h"
#define F_CPU 1000000

volatile uint8_t _readingUart = false;
volatile uint16_t freq = 0;
volatile uint16_t count = 0;
volatile uint8_t mode = 0;


int main(void)
{

	initAll();
	runConfig();

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

void runConfig()
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
