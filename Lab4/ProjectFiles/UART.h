#ifndef UART_H
#define UART_H

extern void init_UART_J();
extern void UARTIntHandler(void);
extern void printchar(char c);
extern void UARTprintstring(char * string);
extern char readchar(void);

#endif