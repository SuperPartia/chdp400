/*
 * timer.h
 *
 *  Created on: 28 lip 2016
 *      Author: nkubiakCiota
 */

#ifndef TIMER_H_
#define TIMER_H_

void initTimer0();
void startTimer0(uint16_t samplingT);
uint16_t stopTimer0();

void initTimer1();
void startTimer1(int measurementPeriod, int cooldownT);
uint16_t stopTimer1();

#endif /* TIMER_H_ */