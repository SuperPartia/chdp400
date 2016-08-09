/*
 * uart.h
 *
 *  Created on: 14 cze 2016
 *
 */
#define uartBufferSize 30
#define tx_flag UCSRB & (1<<UDRIE)

#ifndef UART_H_
#define UART_H_

void usartInit(void);
void uartStart(void);
void sendToUc(bool newLine);
void sendData(uint16_t data, uint8_t endline);
void displayString(char* data);
void displayInt(int value);
uint32_t wait4input(char* prompt);


//m

#endif /* UART_H_ */
