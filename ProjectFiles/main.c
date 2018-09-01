#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "driverlib/uart.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "rgb.h"
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
#include "UARTDriver.h"
#include "UART_CONSOLE_F.h"
#include "Colors.h"

 uint8_t fluxo; //saber qual thread deve ser a proxima
 uint8_t flag; //saber se deve imprimir ou não na tela
 uint16_t primo = 1; //chave
 uint32_t antepenultima; //antepenultima word
 uint32_t penultima; //penultima word
 uint32_t ultima; //ultima word
 uint32_t mensagemo[35];
 uint32_t mensagemd[35];

 //To print on the screen
tContext sContext;
 /*
	fluxo = 1	geração
	fluxo = 2	verifica se é primo
	fluxo = 3	decodifica
	fluxo = 4	verificação da antepenultima word
	fluxo = 5	verificação da serie de fibonacci
	fluxo = 6	verificação da penultima
	fluxo = 7	verificação da ultima 
*/
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
rgb_init();
	init_UART_J();
		cfaf128x128x16Init();
		GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
}

void init_sidelong_menu(){
	uint8_t i;
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);

	//Escreve menu lateral:
	GrStringDraw(&sContext,"Exemplo EK-TM4C1294XL", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	UARTprintstring("TESTE");
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
 

void geracao_thread(void const *args){
	if(fluxo!=1)
	{
		return;
	}
	primo++; 
	//envia para verificação de numero primo
	osDelay(1000);	
	fluxo = 2;
}

void decodificacao_thread(void const *args){
	uint8_t i;
	if(fluxo != 3)
	{
		return;
	}	
	for(i = 0; i <= 35; i++){
		if(i%2==0)
			mensagemo[i] = mensagemd[i] + primo;
		else
			mensagemo[i] = mensagemd[i] - primo;
	}
	osDelay(1000);	
	fluxo = 4;
	flag = 1;
}

void antepenultima_thread(void const *args){
	if(fluxo != 4)
	{
		return;
	}	
	antepenultima = mensagemd[32];
	osDelay(1000);	
	fluxo = 5;
}

void penultima_thread(void const *args){
	if(fluxo != 6)
	{
		return;
	}
	osDelay(1000);	
	fluxo = 7;
}

void ultima_thread(void const *args){
	if(fluxo != 7)
	{
		return;
	}
	osDelay(1000);	
	osDelay(osWaitForever);
}

void primo_thread(void const *args){
	int aux;
	int cont = 0;
	if(fluxo != 2)
	{
		return;
	}
	for (aux = 1; aux <= primo; aux++){
		if(primo%aux == 0){
			cont++;
		}
	}
	if (cont == 2){	
		fluxo = 3;
	}
	else{
		fluxo = 1;
		return;
	}
	osDelay(1000);		
}

void fibonacci_thread(void const *args){
	int num1 = 0,num2 = 1,num3;
	if(fluxo != 5)
	{
		return;
	}
	//num3 = num1 + num2;
	
	while(num3 <= antepenultima){
		num3 = num1 + num2;
		num1 = num2;
		num2 = num3;
		if(num3 == antepenultima){
			fluxo = 6;
			return;
		}
	}
			fluxo = 1;
			return;
	osDelay(1000);
}

void exibir_thread(void const *args){
	if(flag == 0)
	{
		return;
	}
	//colocar função de exibir
	osDelay(1000);	
	flag = 0;
}
 /*----------------------------------------------------------------------------
 *      ThreadsDef
 *---------------------------------------------------------------------------*/
osThreadDef(geracao_thread, osPriorityNormal, 1, 0);
osThreadDef(decodificacao_thread, osPriorityNormal, 1, 0);
osThreadDef(antepenultima_thread, osPriorityNormal, 1, 0);
osThreadDef(penultima_thread, osPriorityNormal, 1, 0);
osThreadDef(ultima_thread, osPriorityNormal, 1, 0);
osThreadDef(primo_thread, osPriorityNormal, 1, 0);
osThreadDef(fibonacci_thread, osPriorityNormal, 1, 0);
osThreadDef(exibir_thread, osPriorityNormal, 1, 0);

/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
		//Initializing all peripherals
	init_all();
	//Sidelong menu creation
	init_sidelong_menu();
	//inicializações
	osKernelInitialize();
	
	
	//criação das threads
	osThreadCreate(osThread(geracao_thread), NULL);
	osThreadCreate(osThread(decodificacao_thread), NULL);
	osThreadCreate(osThread(antepenultima_thread), NULL);
	osThreadCreate(osThread(penultima_thread), NULL);
	osThreadCreate(osThread(ultima_thread), NULL);
	osThreadCreate(osThread(primo_thread), NULL);
	osThreadCreate(osThread(fibonacci_thread), NULL);
	osThreadCreate(osThread(exibir_thread), NULL);
	fluxo = 1;
	
	osKernelStart();
	
	while(true){
		//GrStringDraw(&sContext,"TESTE", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	}
	
	osDelay(osWaitForever);
}
