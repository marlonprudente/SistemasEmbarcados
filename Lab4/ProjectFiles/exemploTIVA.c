/*============================================================================
 *                    Exemplos de utilização do Kit
 *              EK-TM4C1294XL + Educational BooterPack MKII 
 *---------------------------------------------------------------------------*
 *                    Prof. André Schneider de Oliveira
 *            Universidade Tecnológica Federal do Paraná (UTFPR)
 *===========================================================================
 * Autores das bibliotecas:
 * 		Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 * 		Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 * 		Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 * 		Mariana Carrião        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/
#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "cfaf128x128x16.h"
#include "servo.h"
#include "temp.h"
#include "opt.h"
#include "buttons.h"
#include "buzzer.h"
#include "joy.h"
#include "mic.h"
#include "accel.h"
#include "led.h"
#include "UART.h"
#include "UART_CONSOLE_F.h"


#define m_quantidade 1


/*================================
=========Mail Handler============
=================================*/
typedef struct{
	uint8_t msg_UART;
}UART_read;
/*----------------------------------------
*		Mail
*----------------------------------------*/
osMailQId mid_UART;
osMailQDef(m_UART,m_quantidade,UART_read);
/*================================
=========Msg Handler============
=================================*/
typedef struct{
	uint32_t msg_value;
	uint8_t msg_page;
}msg_generic;
/*----------------------------------------
*		Mail
*----------------------------------------*/
osMessageQId msg_console;
osMessageQDef(msg_console,m_quantidade,msg_generic);
osPoolId poolid_c;
osPoolDef(pool_c,m_quantidade,msg_generic);

//To print on the screen
/*----------------------------------------------------------------------------
 *  Transforming int to string
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	init_UART();
	cfaf128x128x16Init();

}	
/*-----------------------------------------------------------------------------
*      Funcoes de uso exclusivo do programa
*------------------------------------------------------------------------------*/
void UARTIntHandler(void){
	char m;
	UART_read * mailI;
	while((UART0->FR & (1<<4)) != 0);
	m = UART0->DR;
	UART0	->	RIS |= (1<<4);
	mailI = (UART_read*)osMailAlloc(mid_UART,0);
	if(mailI){
		mailI	-> msg_UART = m;
		osMailPut(mid_UART,mailI);
	}
	UARTprintstring("Input:");
	printchar(m);
	UARTprintstring("\n\r");
}
/*-----------------------------------------------------------------------------
*      Threads
*------------------------------------------------------------------------------*/



void Console(const void *args){
	bool ini_uart = true;
	osEvent evt;
	msg_generic *msg = 0;
	while(1){
		if(ini_uart){
			pag_ini_uart();
			ini_uart=false;
		}
		evt = osMessageGet(msg_console,osWaitForever);
		osPoolFree(poolid_c,msg);
		}
}osThreadDef(Console,osPriorityNormal,1,0);
void UART_t(const void *args){
	UART_read *mail=0;
	//msg_generic *msg_g = 0;
	osEvent evento;
	//uint8_t pag = 0;
	char mensagem = NULL;
	while(1){
		evento=osMailGet(mid_UART,osWaitForever);
		if(evento.status==osEventMail){
			mail=evento.value.p;
			if(mail){
				mensagem = mail -> msg_UART;
				osMailFree(mid_UART,mail);
				
						switch(mensagem){
								case '1':
									UARTprintstring("Frequencia aumentda\n\r");
									break;
								case '2':
									UARTprintstring("Frequencia diminuida\n\r");
									break;
								case '3':
									UARTprintstring("Amplitude aumentda\n\r");
									break;
								case '4':
									UARTprintstring("Amplitude diminuida\n\r");
									break;
								case '5':
									UARTprintstring("Onda Quadrada Selecionada\n\r");
									break;
								case '6':
									UARTprintstring("Onda Senoidal Selecionada\n\r");
									break;
								case '7':
									UARTprintstring("Onda Dente-de-serra Selecionada\n\r");
									break;
								case '8':
									UARTprintstring("Onda Triangular Selecionada\n\r");
									break;
								default:
									UARTprintstring("Entrada invalida\n\r");
									break;
							}
						}
					}
				}
			}osThreadDef(UART_t,osPriorityAboveNormal,1,0);
/*----------------------------------------------------------------------------
 *      Main
*---------------------------------------------------------------------------*/
 int main (void) {
	init_all();
	osKernelInitialize();
	mid_UART= osMailCreate(osMailQ(m_UART), NULL);
	msg_console = osMessageCreate(osMessageQ(msg_console),NULL);
	
	//poolid_c = osPoolCreate(osPool(pool_c));
	osThreadCreate(osThread(UART_t),NULL);
	osThreadCreate(osThread(Console),NULL);
	osKernelStart();
	osThreadTerminate(osThreadGetId());
	 while(1){
	 }
}
