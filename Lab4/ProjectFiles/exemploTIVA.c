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
#include "PWM.h"
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
 *  Transforming int to string
 *---------------------------------------------------------------------------*/
static void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros){
	static const char* pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool n = false;
	int pos = 0, d = 0;
	int64_t tmpValue = value;
	// the buffer must not be null and at least have a length of 2 to handle one
	// digit and null-terminator
	if (pBuf == NULL || len < 2)
			return;
	// a valid base cannot be less than 2 or larger than 36
	// a base value of 2 means binary representation. A value of 1 would mean only zeros
	// a base larger than 36 can only be used if a larger alphabet were used.
	if (base < 2 || base > 36)
			return;
	if (zeros > len)
		return;
	
	// negative value
	if (value < 0)
	{
			tmpValue = -tmpValue;
			value    = -value;
			pBuf[pos++] = '-';
			n = true;
	}

	// calculate the required length of the buffer
	do {
			pos++;
			tmpValue /= base;
	} while(tmpValue > 0);


	if (pos > len)
			// the len parameter is invalid.
			return;

	if(zeros > pos){
		pBuf[zeros] = '\0';
		do{
			pBuf[d++ + (n ? 1 : 0)] = pAscii[0]; 
		}
		while(zeros > d + pos);
	}
	else
		pBuf[pos] = '\0';

	pos += d;
	do {
			pBuf[--pos] = pAscii[value % base];
			value /= base;
	} while(value > 0);
}
/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	init_UART();
	cfaf128x128x16Init();
	init_PWM();
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
	int teste = 320000;
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
									teste+=1000;
									alterarFrequencia(teste);
									//UARTprintstring((char*)teste);
									break;
								case '2':
									UARTprintstring("Frequencia diminuida\n\r");
									teste-=1000;
									alterarFrequencia(teste);
									//UARTprintstring((char*)teste);
									break;
								case '3':
									UARTprintstring("Amplitude aumentda\n\r");
									break;
								case '4':
									UARTprintstring("Amplitude diminuida\n\r");
									break;
								case '5':
									UARTprintstring("Onda Quadrada Selecionada\n\r");
									ondaQuadrada(90);
									break;
								case '6':
									UARTprintstring("Onda Senoidal Selecionada\n\r");
									ondaSenoidal(64000);
									break;
								case '7':
									UARTprintstring("Onda Dente-de-serra Selecionada\n\r");
									ondaDenteSerra(270);
									break;
								case '8':
									UARTprintstring("Onda Triangular Selecionada\n\r");
									ondaTriangular(360);
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
	 
	 ondaQuadrada(90);
	 while(1){
	 }
}
