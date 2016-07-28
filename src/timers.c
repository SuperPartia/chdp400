/*
 * timers.c
 *
 *  Created on: 28 lip 2016
 *      Author: nkubiak
 */

#include "includes.h"

/*
0 0 0 No clock source. (Timer/Counter stopped)
0 0 1 clk /1 (No prescaling)
0 1 0 clk /8 (From prescaler)
0 1 1 clk /64 (From prescaler)
1 0 0 clk /256 (From prescaler)
1 0 1 clk /1024 (From prescaler)
*/

void initTimer1(uint16_t counts)
{
	OCR1A = counts; //15624 for 1s
	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A
	TIMSK |= (1 << OCIE1A);
	//Set interrupt on compare match
}

void startTimer1(uint8_t prescaler)
{
	TCCR1B |= prescaler;
}

uint16_t stopTimer1()
{
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10) & ~(1 << CS11);
	return OCR1A;
}

ISR (TIMER1_COMPA_vect)
{
    displayString("ło kurwa, tajmer!");
}



