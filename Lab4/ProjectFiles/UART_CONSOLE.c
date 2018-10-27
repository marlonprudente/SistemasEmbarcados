#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "UART.h"
#include "cfaf128x128x16.h"
#include "UART_CONSOLE.h"

void pag_ini_uart(){
	UARTprintstring("Embarcados pratica 4\n\r");
	UARTprintstring("1 - Aumentar Frequencia  \n\r");
	UARTprintstring("2 - Diminuir Frequencia  \n\r");
	UARTprintstring("3 - Aumentar Amplitude   \n\r");
	UARTprintstring("4 - Diminuir Amplitude   \n\r");
}