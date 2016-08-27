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

volatile uint8_t cycles = 0;
volatile uint8_t sampleResid;

volatile uint8_t measureCycles = 1;
volatile uint16_t measOCR1A = 0;
volatile uint8_t cooldownCycles = 1;
volatile uint16_t coolOCR1A = 0;

//TIMER 0
void initTimer0()
{
	TIMSK |= (1 << TOIE0);
	//Set interrupt on compare match
}

void startTimer0(uint16_t samplingT)
{
	//t = 1/frequ 100 us 500m
	//t = TCNT0*presc / 1 000 000  0,065536

	float t = samplingT;
	while (floor(t) >= 262)
	{
		t -= 262.1;
		cycles++;
	}
	sampleResid = floor(t*0xFF/262.1);
	//resid - t
	//256   - 262.1
	TCNT0 = 0xFF - sampleResid;
	TCCR0 |= 5; //0,262144

}

uint16_t stopTimer0()
{
	TCCR0 &= ~(1 << CS00) & ~(1 << CS01) & ~(1 << CS02);
	TCNT0 = 0;
	return TCNT0;
}


ISR (TIMER0_OVF_vect)  // timer0 overflow interrupt
{

	static uint8_t subCounter = 0;
	if(subCounter == cycles)
	{
		TCNT0 = 0xFF - sampleResid;
		subCounter = 0;
//		if (_working == true)
//		{
			if (samplingReady == false)
			{
				samplingReady = true;
			}
			else
			{
				displayString("error!");
			}
//		}
		return;
	}
	subCounter ++;
}

//TIMER 1

void initTimer1()
{
	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A
	TIMSK |= (1 << OCIE1A);
	//Set interrupt on compare match
}

void startTimer1(int measurementT)
{
	float T = measurementT;
	while (floor(T) >= 4194)
	{
		T /= 2;
		measureCycles *= 2;
	}
	T /= 4194.5;
	OCR1A = floor(T*0xFFFF);
	TCCR1B |= 3;
}

uint16_t stopTimer1()
{
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10) & ~(1 << CS11);
	OCR1A = 0;
	return OCR1A;
}

ISR (TIMER1_COMPA_vect)
{
measureFlag = false;
}



