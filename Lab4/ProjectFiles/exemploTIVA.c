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
#include "ondas.h"


#define m_quantidade 1
osThreadId id_PWM_sen;
osThreadId id_PWM_quad;
osThreadId id_PWM_den;
osThreadId id_PWM_tri;
osThreadId id_ondas;
osThreadId id_loop;

uint16_t onda_tipo;
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
	init_PWM();
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
static uint16_t count = 0;
static uint8_t amplitude = 100;
int teste = 1;
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
						Duty80();
						//while(1){																							// Aplica duty-cycle para sinal
							//pwmSetDuty(0xFFFE - ((float)square[count] * (amplitude/100.0)));
						 // Itera sobre a tabela com os valores para a geração de sinais

						break;
					case '2':
						UARTprintstring("Frequencia diminuida\n\r");
						Duty40();
						break;
					case '3':
						UARTprintstring("Amplitude aumentda\n\r");
						Duty60();
						break;
					case '4':
						UARTprintstring("Amplitude diminuida\n\r");
						Duty80();
						break;
					case '5':
						UARTprintstring("Onda Quadrada Selecionada\n\r");
						//onda_tipo = 1;
						//osSignalSet(id_ondas, 0x0007);
						pwmSetDuty(0xFFFE - ((float)square[count] * (amplitude/100.0)));
						break;
					case '6':
						UARTprintstring("Onda Senoidal Selecionada\n\r");	
						//onda_tipo = 2;
						//osSignalSet(id_ondas, 0x0007);					
						pwmSetDuty(0xFFFE - ((float)sine[count] * (amplitude/100.0)));
						// Itera sobre a tabela com os valores para a geração de sinais
						//osSignalSet(id_PWM_sen, 0x0003);						
						break;
					case '7':						
						UARTprintstring("Onda Dente-de-serra Selecionada\n\r");	
						//onda_tipo = 3;
						//osSignalSet(id_ondas, 0x0007);					
						pwmSetDuty(0xFFFE - ((float)sawtooth[count] * (amplitude/100.0)));
						// Itera sobre a tabela com os valores para a geração de sinais
						//osSignalSet(id_PWM_den, 0x0004);						
						break;
					case '8':
						UARTprintstring("Onda Triangular Selecionada\n\r");	
						//onda_tipo = 4;
						//osSignalSet(id_ondas, 0x0007);					
						pwmSetDuty(0xFFFE - ((float)triangular[count] * (amplitude/100.0)));
													 // Itera sobre a tabela com os valores para a geração de sinais

						//osSignalSet(id_PWM_tri, 0x0005);
						
						break;
					default:
						UARTprintstring("Entrada invalida\n\r");
						break;
				}
			}
		}
	}
}osThreadDef(UART_t,osPriorityAboveNormal,1,0);
/*-----------------------------------------------------------------------------
*      Threads de PWM
*------------------------------------------------------------------------------*/
void ondas_PWM(const void *args){
	osEvent evt;
	osStatus status;
	while(1){
		evt = osSignalWait(0x0007, osWaitForever);
		if (evt.status == osEventSignal)  
		{
			if(onda_tipo == 1)
				pwmSetDuty(0xFFFE - ((float)square[0])); //Quadrada
			if(onda_tipo == 2)
				pwmSetDuty(0xFFFE - ((float)sine[0]));
			if(onda_tipo == 3)
				pwmSetDuty(0xFFFE - ((float)sawtooth[0]));
			if(onda_tipo == 4)
				pwmSetDuty(0xFFFE - ((float)triangular[0]));
		}
		
	}
}osThreadDef(ondas_PWM,osPriorityAboveNormal,1,0);

void loop_ondas(const void *args){
	uint16_t count = 0;
	while(1){
			if(onda_tipo == 1)
				pwmSetDuty(0xFFFE - ((float)square[count]));
			if(onda_tipo == 2)
				pwmSetDuty(0xFFFE - ((float)sine[count]));
			if(onda_tipo == 3)
				pwmSetDuty(0xFFFE - ((float)sawtooth[count]));
			if(onda_tipo == 4)
				pwmSetDuty(0xFFFE - ((float)triangular[count]));
			
		count+= 4;
		if(count>1023){
			count = 0;
		}
		
	}
}osThreadDef(loop_ondas,osPriorityAboveNormal,1,0);

void PWM_sen(const void *args){
	osEvent evt;
	osStatus status;
	uint8_t onda = 0;
	while(1){
		evt = osSignalWait(0x0003, osWaitForever);
		if (evt.status == osEventSignal)  
		{
			ondaSenoidal(90,90);
		}
	}
	
}osThreadDef(PWM_sen,osPriorityAboveNormal,1,0);

void PWM_quad(const void *args){
	osEvent evt;
	osStatus status;
	uint8_t onda = 0;
	while(1){
		evt = osSignalWait(0x0002, osWaitForever);
		if (evt.status == osEventSignal)  
		{
			ondaQuadrada(90);
		}
	}
	
}osThreadDef(PWM_quad,osPriorityAboveNormal,1,0);

void PWM_tri(const void *args){
	osEvent evt;
	osStatus status;
	uint8_t onda = 0;
	while(1){
		evt = osSignalWait(0x0005, osWaitForever);
		if (evt.status == osEventSignal)  
		{
			ondaTriangular(360);
		}
	}
	
}osThreadDef(PWM_tri,osPriorityAboveNormal,1,0);

void PWM_den(const void *args){
	osEvent evt;
	osStatus status;
	uint8_t onda = 0;
	while(1){
		evt = osSignalWait(0x0004, osWaitForever);
		if (evt.status == osEventSignal)  
		{
			ondaDenteSerra(270);
		}
	}
	
}osThreadDef(PWM_den,osPriorityAboveNormal,1,0);
/*----------------------------------------------------------------------------
 *      Main
*---------------------------------------------------------------------------*/


 int main (void) {	 
	init_all();
	osKernelInitialize();
	mid_UART= osMailCreate(osMailQ(m_UART), NULL);
	msg_console = osMessageCreate(osMessageQ(msg_console),NULL);
	onda_tipo = 0;
	 	 
	osThreadCreate(osThread(UART_t),NULL);	 
  //id_PWM_sen = osThreadCreate(osThread(PWM_sen),NULL);
  //id_PWM_quad = osThreadCreate(osThread(PWM_quad),NULL);
  //id_PWM_tri = osThreadCreate(osThread(PWM_tri),NULL);
  //id_PWM_den = osThreadCreate(osThread(PWM_den),NULL);
	osThreadCreate(osThread(Console),NULL);
	//id_ondas = osThreadCreate(osThread(ondas_PWM),NULL);
	//id_loop = osThreadCreate(osThread(loop_ondas),NULL);
	osKernelStart();
	osDelay(osWaitForever);
}