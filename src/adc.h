/*
 * adc.h
 *
 *  Created on: 14 cze 2016
 *
 */

#define analog_switch 1

#ifndef ADC_H_
#define ADC_H_

void adcInit();
unsigned int adcRead(_Bool channel);


#endif /* ADC_H_ */
