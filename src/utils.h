/*
 * utils.h
 *
 *  Created on: 14 cze 2016
 *
 */


#ifndef UTILS_H_
#define UTILS_H_

void initAll();
void runConfig(uint16_t *samplingT, int *measurementTime, int *cooldownTime,
		uint8_t *mode, uint8_t *repeats);

uint8_t switchReciever (uint8_t recieverCount);
bool doTheSwitching(uint8_t *diode, bool *reciever, mode);



#endif /* UTILS_H_ */
