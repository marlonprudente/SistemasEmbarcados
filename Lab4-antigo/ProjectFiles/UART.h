//implementar UART.h
#ifndef UART_H
#define UART_H

extern void initUART(void);
extern void UARTIntHandler(void);
extern char writeUART(void);

extern void printchar(char c);
extern void UARTprintstring(char * string);

#endif 