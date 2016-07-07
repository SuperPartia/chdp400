#include "includes.h"

char* usart_buffer;

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
	char ReceivedByte;
	ReceivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived"
	UDR = ReceivedByte; // Echo back the received byte back to the computer
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


void ucToPcSend(char* data){

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
	ucToPcSend("*chd400 initialized*");
}

//--------------------------------------------------------------

