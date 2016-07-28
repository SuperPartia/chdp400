#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "adc.h"
#include "uart.h"
#include "twi.h"

//defines
#define red 7
#define green 6
#define ir 5
#define ind 4

//enums
typedef enum {false, true } bool;

//globals
volatile extern uint8_t _readingUart;
volatile extern uint32_t uartVal;

volatile extern uint16_t freq;
volatile extern uint16_t count;
volatile extern uint8_t mode;

