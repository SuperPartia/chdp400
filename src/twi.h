/*
 * twi.h
 *
 *  Created on: Jun 19, 2016
 *      Author: lunta
 */
#define ACK 1
#define NOACK 0

#define writeAddr 0x3A
#define readAddr 0x3B

#ifndef TWI_H_
#define TWI_H_


void twiStart(void);

void twiStop(void);

void twiWrite(uint8_t data);

uint8_t twiRead(uint8_t ack);

void twiSendData(uint8_t data, uint8_t regAddress);

uint8_t twiGetData(uint8_t regAddress);

void setAccelerometerMode(bool range[2], bool mode[2]);
void readXYZ(uint8_t *x, uint8_t *y, uint8_t *z);
bool detectMove(uint8_t threshold);

#endif /* TWI_H_ */
