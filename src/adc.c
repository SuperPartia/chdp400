/* adc.c
 *
 *  Created on: 14 cze 2016
 *
*/

#include "includes.h"
void adcInit()
{
	ADMUX = 0 | (1<<REFS0); //channel 0, Vref = Vcc
	ADCSRA = (1<<ADEN); //| (1<<ADIE);
	DDRC |= 1 << analog_switch;
}

unsigned int adcRead(bool channel)
{
	PORTC = channel<<analog_switch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;

}
//ISR(ADC_vect)
//{
//	ADCSRA &= ~(1<<ADIE);
//	ADCSRA &= ~(1<<ADSC);
//}


