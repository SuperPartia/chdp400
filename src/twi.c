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
	while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted

}

void twiStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);  //STOP CONDITION
}

void twiWrite(uint8_t data)
{
	TWDR = data; //write data
	TWCR = (1<<TWINT) | (1<<TWEN); //Clear TWINT bit in TWCR to start transmission of data


	while (!(TWCR & (1<<TWINT))); //Wait for TWINT Flag set. This indicates that data has been transmitted, and ACK/NACK has been received.

}

uint8_t twiRead(uint8_t ack)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
//	TWCR = ack ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) : ((1 << TWINT) | (1 << TWEN)) ;
//	while (!(TWCR & (1<<TWINT)));
//	return TWDR;
}


void twiSendData(uint8_t data, uint8_t regAddress)
{
	//unsigned int length = strlen(data);

	twiStart();
	twiWrite(writeAddr); //send device adress
	twiWrite(regAddress); //send register address

	//unsigned char z;
	//for(z=0; z<length; z++)
	//{
	twiWrite(data);
	twiStop();
}

uint8_t twiGetData(uint8_t regAddress)
{
	twiStart();
//	displayString("1. status po start:");
//	displayInt((TWSR & 0xF8));

	twiWrite(writeAddr); //send device addres for writing
//	displayString("2. status po writeAddr:");
//	displayInt((TWSR & 0xF8));

	twiWrite(regAddress); //send register address

//	displayString("3. status po regAddr:");
//	displayInt((TWSR & 0xF8));
	twiStart();
	twiWrite(readAddr); //send device address for reading

//	displayString("4. status po readAddr:");
//	displayInt((TWSR & 0xF8));


//	displayString("udalo sie kurwa!");
//	displayString("wartosc:");
	return twiRead(0);

}
