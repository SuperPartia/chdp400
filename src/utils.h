/*
 * utils.h
 *
 *  Created on: 14 cze 2016
 *
 */


#ifndef UTILS_H_
#define UTILS_H_

#define endLine 1
#define switchRec 2

void initAll();
uint8_t initialdiode (uint8_t mode);

void runConfig(uint16_t *samplingT, int *measurementTime, int *cooldownTime,
		uint8_t *mode, uint8_t *repeats);

bool switchReciever (uint8_t recieverCount);
uint8_t doTheSwitching(uint8_t *diode, bool *reciever, uint8_t mode);



#endif /* UTILS_H_ */
