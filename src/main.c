
#include "includes.h"
#define F_CPU 1000000


int main(void)
{
	MCUCSR|= (1<<7);
	MCUCSR|= (1<<7); // JTAG off
	sei();
	uartStart();
	_delay_ms(500);
	//adcInit();
	//int a = 0;//adcRead(0);
	//char* data = (char*) malloc(30);

	/*DDRC = 0;
	PORTC = 0;
	a = PINC;
	sprintf(data, "%d", a);
	ucToPcSend(data);
	DDRD=0xF0;*/
	twiStart();

	//a = twiGetData(0x00);
	//sprintf(data, "%d", a);

	//ucToPcSend(data);


	while(1){
	//	a = readAdc(0);
	//	sprintf(data, "%d", a);
		//ucToPcSend(data);
	}
}
