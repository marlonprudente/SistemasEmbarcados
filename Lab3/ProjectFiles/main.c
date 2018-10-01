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
#include "cenario.h"
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
osMutexId mutex_display_id;
//osMutexId mutex_tiro_id;
//uint32_t mapa[128][128];
uint8_t pos_x,pos_y, hel_x, hel_y, bar_x, bar_y, pon_x, pon_y;
//uint32_t aux[120][110];

bool flag, helicoptero_status, barco_status, ponte_status;
int cont_cenario;
//To print on the screen
tContext sContext;
uint32_t pontos;
int cont_cenario = -990;
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
	buzzer_vol_set(4000);
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
	osStatus status;
	int k;	
	while(1){
		osSignalWait(0x0005, osWaitForever);		
		osMutexWait(mutex_display_id,osWaitForever);
		GrFlush(&sContext);
		GrContextForegroundSet(&sContext, ClrRed);
		GrContextBackgroundSet(&sContext, ClrBlack);
		buzzer_write(true);
		
		//GrLineDrawH(&sContext, pos_x+4 , k+1, 0);
		
		for(k = 98; k > 0; k --){			
				buzzer_per_set(6002 -(2*k));
				GrPixelDraw(&sContext, pos_x+4 , k);
				GrPixelDraw(&sContext, pos_x+4 , k+1);
		}
		
		if(bar_x <= pos_x+4 && bar_x + 28 >= pos_x+4)
			barco_status = false;
		if(hel_x <= pos_x+4 && hel_x + 11 >= pos_x+4)
			helicoptero_status = false;
		if(pon_x <= pos_x+4 && pon_x + 120 >= pos_x+4)
			ponte_status = false;
		
		osMutexRelease(mutex_display_id);
		//osDelay(10);
		buzzer_write(false);
		osSignalSet(veiculo_do_jogador_id, 0x0001);
		osDelay(60);
	}
}
//================================================
	void veiculo_do_jogador(void const *args){
	uint16_t x, y,center;
	uint8_t k, aux, i = 0,j = 0,a = 56 , b = 99;
	bool button;
	while(1){
		osSignalWait(0x0001, osWaitForever);
		//GrContextBackgroundSet(&sContext, ClrBlack);
		//leitura do joystick
		x = joy_read_x();
		y = joy_read_y();
		button = button_read_s1();
		//mutex para display
		osMutexWait(mutex_display_id,osWaitForever);
		GrFlush(&sContext);
		GrContextForegroundSet(&sContext, ClrWhite);
		GrContextBackgroundSet(&sContext, ClrBlack);
		GrTransparentImageDraw(&sContext,aeronave,a,99, ClrBlack);
		 		
			if(button == 1)
			{
				pos_x = a;
				pos_y = b;
				button = 0;
				osSignalSet(tiro_id, 0x0005);
				osMutexRelease(mutex_display_id);
			}
			if(x > 2800){
				aux = a;
				a = a + 5;
			}
			else if (x < 1500){
				aux = a;
				a = a - 5;
			}
			if(y > 2800)
				b = b;
				//controle de velocidade
			else if (y < 1500){
				b = b;
				//controle de velocidade
			osDelay(30);
			}
			osMutexRelease(mutex_display_id);
			osSignalSet(gerenciador_trajeto_id, 0x0003);
			osDelay(30);
		}
	
}


//================================================
void veiculo_obstaculos(void const *args){
	barco_status = true;
	bar_x = 50, bar_y = 50;
	helicoptero_status = true;
	hel_x = 50, hel_y = 25;
	ponte_status = true;
	pon_x = 4, pon_y = 0;
	
	while(1){
		osSignalWait(0x0002, osWaitForever);
		osMutexWait(mutex_display_id,osWaitForever);
		pos_y_obstaculo = 25;
		pos_x_obstaculo = 25;
		//area critica
		GrFlush(&sContext);
		if(ponte_status)
			GrImageDraw(&sContext,ponte,pon_x,pon_y);
		if(barco_status)
			GrTransparentImageDraw(&sContext,barco,bar_x,bar_y,ClrBlack);//colocar coordenada que varie
		if(helicoptero_status)
			GrTransparentImageDraw(&sContext,helicoptero,hel_x,hel_y,ClrWhite);//colocar coordenada que varie
		
		osSignalSet(veiculo_do_jogador_id,0x0001);
		osMutexRelease(mutex_display_id);
		osDelay(60);
	}
}
//================================================
void gerenciador_trajeto(void const *args){
	cont_cenario = -990;
	while(1){
		osSignalWait(0x0003, osWaitForever);
		osMutexWait(mutex_display_id,osWaitForever);
		GrFlush(&sContext);
		GrImageDraw(&sContext,cenario,4,cont_cenario);
		GrFlush(&sContext);
		GrImageDraw(&sContext,painel,4,110);
		cont_cenario = cont_cenario + 5;
			if(cont_cenario == 0)
				cont_cenario = -990;
			osSignalSet(painel_de_instrumentos_id, 0x0004);
			osMutexRelease(mutex_display_id);
			osDelay(60);
		}
	}

//================================================
void painel_de_instrumentos(void const *args){
	char buff_pontos [32];
	pontos = 100;
	while(1){
		osSignalWait(0x0004, osWaitForever);
		osMutexWait(mutex_display_id,osWaitForever);
		GrFlush(&sContext);
		GrContextForegroundSet(&sContext, ClrWhite);
		GrContextBackgroundSet(&sContext, ClrBlack);		
		pontos++;
		intToString(pontos,buff_pontos,30,10,10);
		GrStringDraw(&sContext,buff_pontos, -1,  48, (sContext.psFont->ui8Height+2)*11, true);
		GrStringDraw(&sContext,"   E     1/2     F", -1,  0, (sContext.psFont->ui8Height+2)*12, true);
		osMutexRelease(mutex_display_id);
		osSignalSet(veiculo_obstaculos_id, 0x0002);
		osDelay(60);
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
osMutexDef(mutex_painel);
osMutexDef(mutex_tiro);
/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	bool start = false;
	uint8_t i = 0,j = 0, k = 15;
	//int c = -990;
	osKernelInitialize();
	init_all();
	veiculo_do_jogador_id = osThreadCreate(osThread(veiculo_do_jogador), NULL); //Sinal 0x001
	veiculo_obstaculos_id = osThreadCreate(osThread(veiculo_obstaculos), NULL); //sinal 0x002
	gerenciador_trajeto_id = osThreadCreate(osThread(gerenciador_trajeto), NULL); //Sinal 0x003
	painel_de_instrumentos_id = osThreadCreate(osThread(painel_de_instrumentos), NULL); //Sinal 0x004
	tiro_id = osThreadCreate(osThread(tiro),NULL); //sinal 0x005
  //mutex_tiro_id = osMutexCreate(osMutex(mutex_tiro));
	mutex_display_id = osMutexCreate(osMutex(mutex_painel));
	osKernelStart();
	
while(!start){
	GrFlush(&sContext);	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
  GrStringDraw(&sContext,"PRESS START", -1,  0, (sContext.psFont->ui8Height+2)*1, true);	
			start = button_read_s2();
	}
GrStringDraw(&sContext,"           ", -1,  0, (sContext.psFont->ui8Height+2)*1, true);	
	osSignalSet(veiculo_do_jogador_id, 0x0001);	
	osDelay(osWaitForever);
}