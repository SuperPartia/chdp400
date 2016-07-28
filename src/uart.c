#include "includes.h"

char* usart_buffer;
volatile uint32_t uartVal = 0;

void usartInit(void)
{
#define BAUD 9600
#include <util/setbaud.h>
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
#if USE_2X
	UCSRA |=  (1<<U2X);
#else
	UCSRA &= ~(1<<U2X);
#endif

	usart_buffer = (char*)malloc(uartBufferSize);
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);  //bitów danych: 8
	//bity stopu:  1
	//parzystość:  brak
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
}


//--------------------------------------------------------------


ISR(USART_RXC_vect)
{
	char receivedByte;
	receivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived"

	if (_readingUart)
	{
//		if (receivedByte == 0x0D || receivedByte == 0x0A || receivedByte == 0x0A)
//		{
//			displayString("");
//			_readingUart = -1;
//			return;
//		}
//		else
		if (1)
		{
			if (receivedByte == 0x08 || receivedByte == 0x1B)
			{
				displayString("");
				_readingUart = false;
			}
			else {

				UDR = receivedByte; // Echo back the received byte back to the computer
			}
			uartVal = 10 * uartVal + (receivedByte - '0');
		}
	}
}


//--------------------------------------------------------------

ISR(USART_UDRE_vect){

	static unsigned int usart_buffer_ind = 0;
	if(usart_buffer[usart_buffer_ind]!= 0){
		UDR = usart_buffer[usart_buffer_ind++];

	}else{
		UCSRB &= ~(1<<UDRIE);
		usart_buffer_ind = 0;
	}
}


//--------------------------------------------------------------


void displayInt(int value)
{
	char* data = (char*) malloc(30);

	//displayString("dec");
	sprintf(data, "%d", value);
	displayString(data);

	//displayString("hex");
	//value = (value/16)*10 + value-(value/16)*16;
	//sprintf(data, "%d", value);
	//displayString(data);

}

void displayString(char* data)
{


	while(tx_flag);
	strncpy(usart_buffer, data, uartBufferSize);
	unsigned char z;
	for(z=0; z<uartBufferSize; z++){
		if(usart_buffer[z]==0){
			usart_buffer[z]   = 13;  //CR (Carrige Return)
			usart_buffer[z+1]  = 10; // LF (Line Feed)
			usart_buffer[z+2]  = 0;  //string end
			break;
		}
	}

	while (!(UCSRA & (1<<UDRE)));

	UCSRB |= (1<<UDRIE);


}

void uartStart(void)
{
	usartInit();
	displayString("*chd400 initialized*");
}

//--------------------------------------------------------------

uint32_t wait4input(char* prompt)
{
	uartVal = 0;
	displayString(prompt);
	_readingUart = true;
	while (_readingUart != 0 );
		if(_readingUart == -1)
		{
		_readingUart = true;
		uartVal = 0;
		displayString(prompt);
		}
	_readingUart = false;
	return uartVal;
}
