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
	//unsigned int length = strlen(data);

	twiStart();
	twiWrite(writeAddr); //send device adress
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
	displayString("1. status po start:");
	displayInt((TWSR & 0xF8));

	twiWrite(writeAddr); //send device addres for writing
	displayString("2. status po writeAddr:");
	displayInt((TWSR & 0xF8));

	twiWrite(regAddress); //send register address

	displayString("3. status po regAddr:");
	displayInt((TWSR & 0xF8));

	twiWrite(readAddr); //send device address for reading

	displayString("4. status po readAddr:");
	displayInt((TWSR & 0xF8));


	displayString("udalo sie kurwa!");
	displayString("wartosc:");
	return twiRead(1);

}


char test()
{
	int i = 0;

	displayString("try adresy z zakresu 0-FF:");
	displayString(".");
	_delay_ms(2000);
	int propperAddr = -1;

	for(i=0; i<=0xFF; i++)
	{

	twiStart();
	twiWrite(writeAddr); //send device adress

	//displayString(".");
	if ((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x20 || (TWSR & 0xF8) == 0x38)
	{
	displayString("pieknie kurwa!");
	displayString("udalo sie na adresie:");
	displayInt(i);
	propperAddr = i;

	displayString("status:");
	displayInt((TWSR & 0xF8));
	displayString("to oznacza:");
	switch ((TWSR & 0xF8))
	{
	case 0x18 : { displayString("adres wyszedl,"); displayString("jest ack,"); }
	case 0x20 : { displayString("adres wyszedl,"); displayString("nie ma ack,"); }
	case 0x38 : { displayString("adres wyszed,"); displayString("ale cos sie zjebalo,"); }
	default: { displayString("sprawdz w doku"); }
	}
	twiStop();
	break;
	}

	twiStop();
	}

	if (propperAddr == -1)
	{
	displayString("kupa :(");
	displayString("ale dla pewnosci,");
	displayString("read na 0x0D");
	int a = twiGetData(0x0D);
	displayInt(a);


	}
	else
	{
		displayString("no to tera, operacja read");
		displayString("4 kroki i wartosc ");

		twiStart();
		displayString("1. status po start:");
		displayInt((TWSR & 0xF8));

		twiWrite(propperAddr); //send device addres for writing
		displayString("2. status po writeAddr:");
		displayInt((TWSR & 0xF8));

		twiWrite(0x0D); //send register address (np ten rejestr)

		displayString("3. status po regAddr:");
		displayInt((TWSR & 0xF8));

		twiWrite(propperAddr ^ 1); //send device address for reading

		displayString("4. status po readAddr:");
		displayInt((TWSR & 0xF8));


		displayString("wartosc rejestru:");
		displayInt(twiRead(1));

	}
}
