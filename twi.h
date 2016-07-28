/*
 * twi.h
 *
 *  Created on: Jun 19, 2016
 *      Author: lunta
 */
#define ACK 1
#define NOACK 0

#define writeAddr 0x1D
#define readAddr 0x9D

#ifndef TWI_H_
#define TWI_H_


void twiStart(void);

void twiStop(void);

void twiWrite(char data);

char twiRead(char ack);

void twiSendData(char data, char regAddress);

char twiGetData(char regAddress);

#endif /* TWI_H_ */
