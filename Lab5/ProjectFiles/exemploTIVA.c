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
 *===========================================================================
 * PROJETO FINAL - 2018.2 - Marlon Prudente / Mateus Oliveira
 *
 */
 
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

#define TAM_MAILQUEUE 16  
#define m_quantidade 1

osMailQId id_mail_Gantt; 

//To print on the screen
tContext sContext;
uint32_t primo = 0;
uint32_t fibonacci = 1;
osMutexId mutex_display_id;
osSemaphoreId escalonador;                         // Semaphore ID
osSemaphoreDef(escalonador);                       // Semaphore definition
/*================================
=========Mail Handler============
=================================*/
typedef struct{
	uint8_t msg_UART;
}UART_read;

typedef struct {                                                
  //char Buffer[32];
  uint8_t id;
} struct_Gantt;
/*----------------------------------------
*		Mail
*----------------------------------------*/
osMailQDef(mail_Gantt, TAM_MAILQUEUE, struct_Gantt);
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

void posicao_inicial()
{
	servo_writePosY(1500);
	osDelay(5000);
	servo_writePosX(5000);
	osDelay(5000);
	servo_writeRot(28000);
	osDelay(10000);
	servo_writeRot(28000);
	osDelay(5000);
	servo_writePosX(20000);
	osDelay(5000);
	servo_writePosY(7000);
	osDelay(5000);
}
void desenha_circulo(void)
{
	struct_Gantt *pMailGantt = 0;
	int i, auxR, auxX;
	servo_writePosY(1500);
	osDelay(5000);
	servo_writePosX(5000);
	osDelay(5000);
	servo_writeRot(28000);
	osDelay(10000);

	servo_writePosX(9000);
	osDelay(5000);
	servo_writeRot(23000);
	osDelay(5000);
	servo_writePosY(6000);
	osDelay(10000);
	auxR = 23000;
	auxX = 9000;
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 200;
		auxX+= 150;
		osDelay(5000);
	}
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 150;
		auxX+= 150;
		osDelay(5000);
	}
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 150;
		auxX+= 280;
		osDelay(5000);
	}
	
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 200;
		auxX+= 160;
		osDelay(5000);
	}
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 150;
		auxX+= 200;
		osDelay(5000);
	}
	servo_writePosY(5500);
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 150;
		auxX+= 350;
		osDelay(5000);
	}
	auxX += 500;
	servo_writePosX(auxX);
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 150;
		auxX+= 350;
		osDelay(5000);
	}
	auxX += 500;
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 200;
		auxX-= 160;
		osDelay(5000);
	}
	auxX += 500;
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 150;
		auxX-= 160;
		osDelay(5000);
	}
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR+= 150;
		auxX-= 290;
		osDelay(5000);
	}
	servo_writePosY(6500);
	auxX-= 550;
	servo_writePosX(auxX);
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 200;
		auxX-= 150;
		osDelay(5000);
	}
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 150;
		auxX-= 150;
		osDelay(5000);
	}
	for(i = 0; i < 3; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 150;
		auxX-= 280;
		osDelay(5000);
	}
							
	for(i = 0; i < 4; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 200;
		auxX-= 150;
		osDelay(5000);
	}
	for(i = 0; i < 4; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 150;
		auxX-= 150;
		osDelay(5000);
	}
	for(i = 0; i < 4; i++){
		servo_writeRot(auxR);servo_writePosX(auxX);
		auxR-= 150;
		auxX-= 280;
		osDelay(5000);
	}
		pMailGantt = osMailAlloc(id_mail_Gantt,0);
	pMailGantt->id = 4;
	osMailPut(id_mail_Gantt, pMailGantt); 
						
}
void desenha_quadrado(void)
{
	struct_Gantt *pMailGantt = 0;
	servo_writePosY(8000);
	servo_writePosX(7000);
	servo_writeRot(25000);
	osDelay(10000);
	servo_writePosX(7000);
	servo_writeRot(19000);
	osDelay(10000);
	servo_writePosX(7000);
	servo_writePosY(7000);
	servo_writeRot(20000);
	osDelay(1000);	
	servo_writePosX(21000);
	servo_writeRot(18000);
	osDelay(10000);
	servo_writePosX(20000);
	servo_writeRot(28000);
	osDelay(10000);
	pMailGantt = osMailAlloc(id_mail_Gantt,0);
	pMailGantt->id = 4;
	osMailPut(id_mail_Gantt, pMailGantt); 
}

void desenha_losango()
{
	struct_Gantt *pMailGantt = 0;
	servo_writeRot(26000);
	servo_writePosX(12000);
	//servo_writePosY(7000);
	osDelay(10000);	
	servo_writeRot(22000);
	osDelay(1000);
	servo_writePosX(6500);
	servo_writePosY(7500);
	osDelay(10000);
	servo_writeRot(19000);
	servo_writePosX(10000);
	osDelay(10000);	
	servo_writeRot(23000);
	servo_writePosX(20000);
	osDelay(10000);	
	servo_writeRot(26000);
	servo_writePosX(10500);
	osDelay(10000);	
	pMailGantt = osMailAlloc(id_mail_Gantt,0);
	pMailGantt->id = 4;
	osMailPut(id_mail_Gantt, pMailGantt); 
}
void fibonacci_thread(void const *args){
	osEvent evt;

	uint32_t num1 = 0,num2 = 1,num3;
	int32_t value;
	char fibonacciChar[32];
	struct_Gantt *pMailGantt = 0;
	while(1){
		osDelay(1000);
		value = osSemaphoreWait(escalonador,500);
				if(value > 0){
		num3 = num1 + num2;		
			while(num3 < fibonacci)
				{
						num1 = num2;
						num2 = num3;
						num3 = num1 + num2;
				}
			if(num3 == fibonacci){
			osMutexWait(mutex_display_id,osWaitForever);
			intToString(primo,fibonacciChar,30,10,0);
			GrFlush(&sContext);
			GrContextForegroundSet(&sContext, ClrWhite);
			GrContextBackgroundSet(&sContext, ClrBlack);
			GrStringDraw(&sContext,(char*)fibonacciChar, -1, (sContext.psFont->ui8MaxWidth)*11, (sContext.psFont->ui8Height+2)*1, true);
			osMutexRelease(mutex_display_id);
			}
			else{
				//não pertence	
			}
		}
		fibonacci++;		
				pMailGantt = osMailAlloc(id_mail_Gantt,0);
				pMailGantt->id = 5;				
				//pMailGantt->Buffer = (char*)'Fibonacci';
				// Send Mail
				osMailPut(id_mail_Gantt, pMailGantt);  
			osSemaphoreRelease(escalonador);
		}
}osThreadDef(fibonacci_thread, osPriorityNormal, 1, 0);

void primo_thread(void const *args){
	int32_t value;
	char primoChar[32];
	struct_Gantt *pMailGantt = 0;
	uint32_t aux;
	int div;
	while(1){
		osDelay(200);
		value = osSemaphoreWait(escalonador,60);
		if(value > 0){		
		div = 0;
		for (aux = 1; aux <= primo; aux++){
			if(primo%aux == 0){
				div++;
			}
		}
		if (div == 2){
			osMutexWait(mutex_display_id,osWaitForever);
			intToString(primo,primoChar,32,10,0);
			GrFlush(&sContext);
			GrContextForegroundSet(&sContext, ClrWhite);
			GrContextBackgroundSet(&sContext, ClrBlack);
			GrStringDraw(&sContext,(char*)primoChar, -1, (sContext.psFont->ui8MaxWidth)*8, (sContext.psFont->ui8Height+2)*0, true);
			osMutexRelease(mutex_display_id);
		}
		else{
			
		}
			
		}
		primo =  primo + 1;
						pMailGantt = osMailAlloc(id_mail_Gantt,0);
				pMailGantt->id = 1;				
				//pMailGantt->Buffer = (char*)'Fibonacci';
				// Send Mail
				osMailPut(id_mail_Gantt, pMailGantt);  
		osSemaphoreRelease(escalonador);
	}
}osThreadDef(primo_thread, osPriorityRealtime, 1, 0);


void init_all(){
	init_UART();
	servo_init();
	cfaf128x128x16Init();
}	

/*-----------------------------------------------------------------------------
*      Funcoes de uso exclusivo do programa
*------------------------------------------------------------------------------*/
void UARTIntHandler(void){
	char m;
	struct_Gantt *pMailGantt = 0;
	UART_read * mailI;
	while((UART0->FR & (1<<4)) != 0);
	m = UART0->DR;
	UART0	->	RIS |= (1<<4);
	mailI = (UART_read*)osMailAlloc(mid_UART,0);
	pMailGantt = osMailAlloc(id_mail_Gantt,0);
	if(mailI){ 
		mailI	-> msg_UART = m;
		osMailPut(mid_UART,mailI);
		pMailGantt->id = 2;
		osMailPut(id_mail_Gantt, pMailGantt); 
	}
	UARTprintstring("Input:");
	printchar(m);
	UARTprintstring("\n\r");
}
/*-----------------------------------------------------------------------------
*      Threads
*------------------------------------------------------------------------------*/
void geracao_pontos(const void *args){
	int32_t value;
	char numero[32];
	struct_Gantt *pMailGantt = 0;
	uint32_t aux = 0;
	while(1){
		osDelay(100);
		value = osSemaphoreWait(escalonador,70);
		if(value > 0){			
			osMutexWait(mutex_display_id,osWaitForever);
			intToString(aux,numero,30,10,10);
			GrFlush(&sContext);
			GrContextForegroundSet(&sContext, ClrWhite);
			GrContextBackgroundSet(&sContext, ClrBlack);
			GrStringDraw(&sContext,(char*)numero, -1, (sContext.psFont->ui8MaxWidth)*11, (sContext.psFont->ui8Height+2)*2, true);
			osMutexRelease(mutex_display_id);
		}
		aux++;
		pMailGantt = osMailAlloc(id_mail_Gantt,0);
		pMailGantt->id = 3;				
				//pMailGantt->Buffer = (char*)'Fibonacci';
				// Send Mail
		osMailPut(id_mail_Gantt, pMailGantt);
		osSemaphoreRelease(escalonador);
	}
}osThreadDef(geracao_pontos,osPriorityLow,1,0);

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
}osThreadDef(Console,osPriorityHigh,1,0);

void UART_t(const void *args){
static uint16_t count = 0;
static uint8_t amplitude = 100;
int teste = 1, i, auxR = 0, auxX = 0;
UART_read *mail=0;
osEvent evento;
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
					UARTprintstring("1 - RETANGULO SELECIONADO (6 - p/ desenhar)\n\r");
					desenha_quadrado();
					break;
					case '2':
					UARTprintstring("2 - LOSANGO SELECIONADO (6 - p/ desenhar)\n\r");
						desenha_losango();					
					break;
					case '3':
						UARTprintstring("3 - CIRCULO SELECIONADO (6 - p/ desenhar)\n\r");
					desenha_circulo();
						break;
					case '4':
						UARTprintstring("4 - BANDEIRA SELECIONADO (6 - p/ desenhar)\n\r");
						desenha_quadrado();
						desenha_losango();
						desenha_circulo();
						break;
					case '5':
						UARTprintstring("5 - PARANDO ANDAMENTO DO DESENHO...\n\r");
						break;
					case '6':
						UARTprintstring("6 - DESENHANDO...\n\r");		
						servo_writePosX(7500);					
						break;
					default:
						UARTprintstring("Entrada invalida\n\r");
						break;
				}
			}
		}
	}
}osThreadDef(UART_t,osPriorityHigh,1,0);

void geracao_Gantt(const void *args){
	char inteiro[32];
	osEvent evt;
	osStatus status;
	uint32_t stime, ftime;
	uint8_t i;
	
	struct_Gantt *pMail = 0;
	
	while(1){
		// Mail
		
		//	INICIA	SECAO	CRITICA
		evt = osMailGet(id_mail_Gantt, osWaitForever);
		if(evt.status == osEventMail)
		{
			stime = osKernelSysTick();
			
			pMail = evt.value.p;
      if(pMail) 
			{
				
				// Recebe outros tempos para diagrama de Gantt e envia através da serial (4 tempos, 32 bytes)
				UARTprintstring("Id: ");
				intToString(pMail->id,inteiro,32,10,0);
				UARTprintstring(inteiro);
				UARTprintstring("\n\r");
//        for(i = 0; i < 24; i++){
//					intToString(pMail->Buffer[i],inteiro,32,10,0);
//					UARTprintstring(inteiro);
//				}
//				UARTprintstring("\n");
//				// stime Gantt
//				intToString(stime,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//				intToString(stime >> 8,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//				intToString(stime >> 16,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//				intToString(stime >> 24,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//					
//				ftime = osKernelSysTick();
//					
//				// ftime Gantt
//				intToString(ftime,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//				intToString(ftime >> 8,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//				intToString(ftime >> 16,inteiro,32,10,0);
//				UARTprintstring(inteiro);
//				intToString(ftime >> 24,inteiro,32,10,0);
//				UARTprintstring(inteiro);
					
				// free memory allocated for mail
				osMailFree(id_mail_Gantt, pMail);                      	
			}
		}
		//	FIM	DA	SECAO	CRITICA
	}
}osThreadDef(geracao_Gantt,osPriorityNormal,1,0);

/*----------------------------------------------------------------------------
 *      Main
*---------------------------------------------------------------------------*/


 int main (void) {	 
	init_all();
	osKernelInitialize();
	mid_UART= osMailCreate(osMailQ(m_UART), NULL);
	id_mail_Gantt = osMailCreate(osMailQ(mail_Gantt), NULL);
	msg_console = osMessageCreate(osMessageQ(msg_console),NULL);
	escalonador = osSemaphoreCreate(osSemaphore(escalonador), 1);
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	 //Escreve menu lateral:
	GrStringDraw(&sContext,"Primo:", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"Fibonacci:", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"G.Pontos:", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	//osThreadCreate(osThread(UART_t),NULL);
	osThreadCreate(osThread(Console),NULL);
	osThreadCreate(osThread(primo_thread), NULL);
	osThreadCreate(osThread(fibonacci_thread), NULL);
	osThreadCreate(osThread(geracao_pontos), NULL);
	//osThreadCreate(osThread(geracao_Gantt), NULL);
	osKernelStart();
	osDelay(osWaitForever);
}