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
#include "joy.h"
#include "buttons.h"
#include "buzzer.h"
#include <math.h>
#include <stdio.h>
#include "grlib/grlib.h"
#include "driverlib/uart.h"
#include "cenario1.h"
#include "cenario2.h"
#include "aeronave.h"
#include "inimigos.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "cfaf128x128x16.h"
#include "led.h"
#include "Colors.h"
#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7
#define osFeature_SysTick 1

osThreadId veiculo_do_jogador_id;
osThreadId veiculo_obstaculos_id;
osThreadId gerenciador_trajeto_id;
osThreadId painel_de_instrumentos_id;
osThreadId tiro_id;
uint32_t mapa[128][128];
uint8_t pos_x,pos_y;
uint32_t aviao[12][24]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,
0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool flag;
//To print on the screen
tContext sContext;
uint32_t pontos;
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

static void floatToString(float value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros, uint8_t precision){
	static const char* pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	uint8_t start = 0xFF;
	if(len < 2)
		return;
	
	if (base < 2 || base > 36)
		return;
	
	if(zeros + precision + 1 > len)
		return;
	
	intToString((int64_t) value, pBuf, len, base, zeros);
	while(pBuf[++start] != '\0' && start < len); 

	if(start + precision + 1 > len)
		return;
	
	pBuf[start+precision+1] = '\0';
	
	if(value < 0)
		value = -value;
	pBuf[start++] = '.';
	while(precision-- > 0){
		value -= (uint32_t) value;
		value *= (float) base;
		pBuf[start++] = pAscii[(uint32_t) value];
	}
}

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	cfaf128x128x16Init();
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	//GrContextForegroundSet(&sContext, ClrWhite);
	//GrContextBackgroundSet(&sContext, ClrBlack);
	//GrStringDraw(&sContext,"River Raid", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	joy_init();
	buzzer_init(); 
	button_init();
	}

uint32_t saturate(uint8_t r, uint8_t g, uint8_t b){
	uint8_t *max = &r, 
					*mid = &g, 
					*min = &b,
					*aux, 
					div, num;
	if (*mid > *max){ aux = max; max = mid; mid = aux; }
	if (*min > *mid){ aux = mid; mid = min; min = aux; }
	if (*mid > *max){	aux = max; max = mid; mid = aux; }
	if(*max != *min){
		div = *max-*min;
		num = *mid-*min;
		*max = 0xFF;
		*min = 0x00;
		*mid = (uint8_t) num*0xFF/div;
	}
	return 	(((uint32_t) r) << 16) | 
					(((uint32_t) g) <<  8) | 
					( (uint32_t) b       );
}

/*----------------------------------------------------------------------------
 *      Threads
 *---------------------------------------------------------------------------*/
void tiro(void const * args){
	uint16_t k;
	osEvent evento;
	
	while(1){
		
		evento = osSignalWait(0x0001, osWaitForever);
		if(evento.status == osEventSignal && flag == 1){
			for(k = 98; k >= 0; k --){
				if(k == 0)
					flag = 0;
				
				GrContextForegroundSet(&sContext, ClrRed);
				GrPixelDraw(&sContext, pos_x+4 , k);
				GrPixelDraw(&sContext, pos_x+4 , k+1);
				osDelay(1);
				osSignalSet(veiculo_do_jogador_id, 0x0001);
			}
		}
	}
}
//================================================
	void veiculo_do_jogador(void const *args){
	osEvent evento;
	uint16_t x, y,center;
	uint8_t k, aux, i = 0,j = 0,a = 56 , b = 99;
	bool button;

	while(1){
		evento = osSignalWait(0x0001, osWaitForever);
		if(evento.status == osEventSignal)
		{	
			x = joy_read_x();
			y = joy_read_y();
			button = button_read_s1();
		GrFlush(&sContext);
		//GrTransparentImageDraw(&sContext,barco,20,50,ClrBlack);
		GrTransparentImageDraw(&sContext,aeronave,a,99,ClrBlack);
		 		
			if(button == 1)
			{
				pos_x = a;
				pos_y = b;
				flag = 1;
				osSignalSet(tiro_id, 0x0001);
			}		
			if(x > 2800){
//				if(mapa[i+a][j] == 2)
//					a = a;
//				else
				aux = a;
					a++;
			}
			else if (x < 1500){
//				if(mapa[a-1][j] == 2)
//						a = a;
//				else
				aux = a;
					a--;
			}
			if(y > 2800)
				b = b;
				//controle de velocidade
			else if (y < 1500){
				b = b;
				//controle de velocidade
			osDelay(20);
			}
			osSignalSet(gerenciador_trajeto_id, 0x0001);
		}
	}
}


//================================================
void veiculo_obstaculos(void const *args){
	osEvent evento;
	while(1){
		evento = osSignalWait(0x0001, osWaitForever);
		if(evento.status == osEventSignal)
		{

				GrTransparentImageDraw(&sContext,helicoptero,10,30,ClrWhite);
		}
	}
}
//================================================
void gerenciador_trajeto(void const *args){
	uint16_t i, j, cont = 0;
	osEvent evento;
	while(1){
		evento = osSignalWait(0x0001, osWaitForever);
		if(evento.status == osEventSignal){
			
			if(cont > 10){
		GrImageDraw(&sContext,cenario1,4,0);
				cont = 0;
			}
			cont ++;
			osSignalSet(painel_de_instrumentos_id, 0x0001);
			}
		}
	}

//================================================
void painel_de_instrumentos(void const *args){
	char buff_pontos [32];
	osEvent evento;
	
	while(1){
		evento = osSignalWait(0x0001, osWaitForever); 
		if(evento.status == osEventSignal){
		GrContextForegroundSet(&sContext, ClrWhite);
		pontos = 100;
		intToString(pontos,buff_pontos,30,10,10);
		GrStringDraw(&sContext,buff_pontos, -1,  48, (sContext.psFont->ui8Height+2)*11, true);
		GrStringDraw(&sContext,"   E     1/2     F", -1,  0, (sContext.psFont->ui8Height+2)*12, true);
		osSignalSet(veiculo_do_jogador_id, 0x0001);
		}
	}
}
 /*----------------------------------------------------------------------------
 *      ThreadsDef
 *---------------------------------------------------------------------------*/

osThreadDef(veiculo_do_jogador, osPriorityNormal, 1, 0);
osThreadDef(veiculo_obstaculos, osPriorityNormal, 1, 0);
osThreadDef(gerenciador_trajeto, osPriorityNormal, 1, 0);
osThreadDef(painel_de_instrumentos, osPriorityNormal, 1, 0);
osThreadDef(tiro,osPriorityNormal,1 ,0);
/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	
	uint8_t i = 0,j = 0;
	osKernelInitialize();
	init_all();
	veiculo_do_jogador_id = osThreadCreate(osThread(veiculo_do_jogador), NULL);
	veiculo_obstaculos_id = osThreadCreate(osThread(veiculo_obstaculos), NULL);
	gerenciador_trajeto_id = osThreadCreate(osThread(gerenciador_trajeto), NULL);
	painel_de_instrumentos_id = osThreadCreate(osThread(painel_de_instrumentos), NULL);
	tiro_id = osThreadCreate(osThread(tiro),NULL);
	
	osSignalSet(veiculo_do_jogador_id, 0x0001);
	GrImageDraw(&sContext,cenario1,4,0);
	osKernelStart();	
	osDelay(osWaitForever);
}