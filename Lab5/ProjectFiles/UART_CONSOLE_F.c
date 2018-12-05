#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "UART.h"
#include "cfaf128x128x16.h"
#include "UART_CONSOLE_F.h"

void pag_ini_uart(){
	UARTprintstring("Embarcados Lab 5\n\r");
	UARTprintstring("1 - RETANGULO\n\r");
	UARTprintstring("2 - LOSANGO\n\r");
	UARTprintstring("3 - CIRCULO\n\r");
	UARTprintstring("4 - BANDEIRA\n\r");
	UARTprintstring("5 - PARAR DESENHO EM ANDAMENTO\n\r");	
}