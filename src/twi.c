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
	twiStart();
	twiWrite(writeAddr); //send device adress
	twiWrite(regAddress); //send register address
	twiWrite(data);
	twiStop();
}

uint8_t twiGetData(uint8_t regAddress)
{
	twiStart();
	twiWrite(writeAddr); //send device addres for writing
	twiWrite(regAddress); //send register address
	twiStart();
	twiWrite(readAddr); //send device address for reading
	return twiRead(0);

}

void setMeasuringMode(bool range[2], bool mode[2])
{
	/*
	range [1:0] g-Range Sensitivity
		00 		8g 		16 LSB/g
		01 		2g 		64 LSB/g
		10 		4g 		32 LSB/g
	 */

	/*
	MODE [1:0] 	Function
		00 		Standby Mode
		01 		Measurement Mode
		10 		Level Detection Mode
		11 		Pulse Detection Mode
	 */
	uint8_t reg16 = 0;
	reg16 = (range[1] << 3) | (range[0] << 2) | mode[1] << 1 | mode[0];
	twiSendData(reg16, 0x16);

}

void readXYZ(uint8_t *x, uint8_t *y, uint8_t *z)
{
	*x = twiGetData(0x06);
	*y = twiGetData(0x07);
	*z = twiGetData(0x08);
}

bool detectMove(uint8_t threshold)
{
	static uint8_t x, y, z;
	static uint8_t old_x, old_y, old_z;
	old_x = x;
	old_y = y;
	old_z = z;
	readXYZ(&x, &y, &z);
	if ((abs(x-old_x)<=threshold) & (abs(y-old_y)<=threshold) & (abs(z-old_z)<=threshold))
	{
		PORTD &= ~(1<<ind);
		return true;
	}
	else
	{
		PORTD |= (1<<ind);
		_delay_ms(1000);
		return false;
	}
}
