/*
 * twi.c
 *
 *  Created on: Jun 19, 2016
 *      Author: lunta
 */


#include "includes.h"

void twiStart(void)
{
TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWSTA)); //START CONDITION

int a = TWCR;
char* data = (char*) malloc(30);

sprintf(data, "%d", a);
ucToPcSend(data);

while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted

a=TWCR;
sprintf(data, "%d", a);
ucToPcSend(data);
ucToPcSend("tata2");

}

void twiStop(void)
{
TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);  //STOP CONDITION
}

void twiWrite(char data)
{
TWDR = data; //write data
TWCR = (1<<TWINT) | (1<<TWEN); //Clear TWINT bit in TWCR to start transmission of data
while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that data has been transmitted, and ACK/NACK has been received.
}

char twiRead(char ack)
{
TWCR = ack ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) : ((1 << TWINT) | (1 << TWEN)) ;
while (!(TWCR & (1<<TWINT)));
return TWDR;
}

void twiSendData(char data, char regAddress)
{
	unsigned int length = strlen(data);

	twiStart();
	twiWrite(accReadAddr); //send device adress
	twiWrite(regAddress); //send register address

	//unsigned char z;
	//for(z=0; z<length; z++)
	//{
		twiWrite(data);
	//}
	twiWrite(data);
	twiStop();
}

char twiGetData(char regAddress)
{
	twiStart();
	ucToPcSend("mama");
	twiWrite(accReadAddr); //send device adress
	ucToPcSend("mama2");
	twiWrite(regAddress); //send register address
	ucToPcSend("mama3");
	twiWrite(accWriteAddr); //send device adress
	return twiRead(1);

}
