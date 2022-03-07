#ifndef UART_H
#define UART_H

#include "uart_defs.h"

int connectUart();
void writeUartGet(int filestream, unsigned char code);
void writeUartSend(int filestream, int value);
Number_type readUart(int filestream, unsigned char code);
void closeUart(int filestream);

#endif /* UART_H */
