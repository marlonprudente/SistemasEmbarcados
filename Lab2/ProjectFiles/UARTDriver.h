#ifndef UARTDRIVER_H
#define UARTDRIVER_H

extern void init_UART_J();
extern void UARTIntHandler(void);
extern void printchar(char c);
extern void UARTprintstring(char * string);
extern char readchar(void);

#endif  //__UARTDRIVER_H__