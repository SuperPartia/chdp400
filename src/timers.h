/*
 * timer.h
 *
 *  Created on: 28 lip 2016
 *      Author: nkubiak
 */

#ifndef TIMER_H_
#define TIMER_H_

void initTimer0(uint16_t counts);
void startTimer0(uint8_t prescaler);
uint16_t stopTimer0();

void initTimer1(uint16_t counts);
void startTimer1(uint8_t prescaler);
uint16_t stopTimer1();

#endif /* TIMER_H_ */
