#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <math.h>

//defines
#define red 7
#define green 6
#define ir 5
#define ind 4

//enums
typedef enum {false, true } bool;

#include "adc.h"
#include "uart.h"
#include "twi.h"
#include "timers.h"

//globals
volatile extern uint8_t _readingUart;
volatile uint32_t uartVal;
volatile extern bool _samplingReady;
volatile extern bool _setup;
volatile extern bool _working;


