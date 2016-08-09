#include "includes.h"

volatile char* usart_buffer;
volatile uint8_t _readingUart = false;
volatile uint32_t uartVal = 0;
volatile unsigned int usart_buffer_ind = 0;

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
		if (receivedByte == 0x08 || receivedByte == 0x1B)
		{
			displayString("");
			_readingUart = 2;
			return;
		}
		else {

			if (receivedByte == '\r')
			{
				displayString("");
				_readingUart = false;
				return;
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

		//static unsigned int usart_buffer_ind = 0;
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
		//char* data = (char*) malloc(30);

		//displayString("dec");
		while(usart_buffer_ind);
		sprintf(usart_buffer, "%d", value);
		sendToUc(true);

		//displayString("hex");
		//value = (value/16)*10 + value-(value/16)*16;
		//sprintf(data, "%d", value);
		//displayString(data);

	}
	void displayString(char* data)
	{
		while(usart_buffer_ind);
		strncpy(usart_buffer, data, uartBufferSize);
		sendToUc(true);
	}

	void sendData(uint16_t data, uint8_t endline)
	{
		while(usart_buffer_ind);
		sprintf(usart_buffer, "%d", data);
		sendToUc(false);
		while(usart_buffer_ind);
		if (endline & 1) //if it's 2, then not. but if it's 3 it's ok
		strncpy(usart_buffer, ";", uartBufferSize);
		else
		strncpy(usart_buffer, " ", uartBufferSize);

		sendToUc(endline); //with end line
	}

	void sendToUc(bool newLine)
	{


		while(tx_flag);
		unsigned char z;
		for(z=0; z<uartBufferSize; z++){
			if(usart_buffer[z]==0){
				if (newLine)
				{
					usart_buffer[z]   = 13;  //CR (Carrige Return)
					usart_buffer[z+1]  = 10; // LF (Line Feed)
					usart_buffer[z+2]  = 0;  //string end
				}
				else
				{
					//usart_buffer[z]   = 32;  //SPACE
					usart_buffer[z+1]  = 0;  //string end
				}
				break;
			}
		}

		while (!(UCSRA & (1<<UDRE)));

		usart_buffer_ind = 0;
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
		while (_readingUart != 0 )
		{
			if(_readingUart == 2)
			{
				displayString(prompt);
				uartVal = 0;
				_readingUart = true;
			}
		}
		_readingUart = false;
		return uartVal;
	}
