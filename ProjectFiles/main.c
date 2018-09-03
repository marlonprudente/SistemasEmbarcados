#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
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
uint32_t mensagemo[140] = 
{0x1a, 0x56, 0xca, 0x88, 0x9e, 0xaa, 0x35, 0x77, 0x49, 0x56, 0xca, 0x88, 0x9d, 0xaa, 0x35, 0x77,
0x40, 0x56, 0xca, 0x88, 0x97, 0xaa, 0x35, 0x77, 0x3b, 0x56, 0xca, 0x88, 0x98, 0xaa, 0x35, 0x77, 
0xf7, 0x55, 0xca, 0x88, 0x8a, 0xaa, 0x35, 0x77, 0xf7, 0x55, 0xca, 0x88, 0x7f, 0xaa, 0x35, 0x77, 
0x40, 0x56, 0xca, 0x88, 0x8d, 0xaa, 0x35, 0x77, 0x38, 0x56, 0xca, 0x88, 0x49, 0xaa, 0x35, 0x77,
0x18, 0x56, 0xca, 0x88, 0x8d, 0xaa, 0x35, 0x77, 0x46, 0x56, 0xca, 0x88, 0x92, 0xaa, 0x35, 0x77,
0x3b, 0x56, 0xca, 0x88, 0x8a, 0xaa, 0x35, 0x77, 0x3b, 0x56, 0xca, 0x88, 0x98, 0xaa, 0x35, 0x77,
0xf7, 0x55, 0xca, 0x88, 0x5a, 0xaa, 0x35, 0x77, 0x10, 0x56, 0xca, 0x88, 0x61, 0xaa, 0x35, 0x77,
0x0d, 0x56, 0xca, 0x88, 0x29, 0xaa, 0x35, 0x77, 0xd7, 0x55, 0xca, 0x88, 0x29, 0xaa, 0x35, 0x77, 
0xd7, 0x55, 0xca, 0x88, 0x49, 0x53, 0xc2, 0x97, 0x7b, 0xfe, 0xa0, 0x65};

/*uint32_t mensagemo[35] = 
{0x43, 0xcc, 0xff, 0xff, 0x66, 0x34, 0x00, 0x00, 0x1f, 0xcc, 0xff, 0xff, 0x57, 0x34, 0x00, 0x00, 
0x6e, 0xcc, 0xff, 0xff, 0x6d, 0x34, 0x00, 0x00, 0x73, 0xcc, 0xff, 0xff, 0x62, 0x34, 0x00, 0x00, 
0x1f, 0xcc, 0xff, 0xff, 0x51, 0x34, 0x00, 0x00, 0x60, 0xcc, 0xff, 0xff, 0x73, 0x34, 0x00, 0x00,
0x60, 0xcc, 0xff, 0xff, 0x21, 0x34, 0x00, 0x00, 0x6e, 0xcc, 0xff, 0xff, 0x21, 0x34, 0x00, 0x00,
0x45, 0xcc, 0xff, 0xff, 0x76, 0x34, 0x00, 0x00, 0x73, 0xcc, 0xff, 0xff, 0x76, 0x34, 0x00, 0x00, 
0x71, 0xcc, 0xff, 0xff, 0x70, 0x34, 0x00, 0x00, 0x1f, 0xcc, 0xff, 0xff, 0x32, 0x34, 0x00, 0x00,
0x38, 0xcc, 0xff, 0xff, 0x39, 0x34, 0x00, 0x00, 0x34, 0xcc, 0xff, 0xff, 0x01, 0x34, 0x00, 0x00,
0xff, 0xcb, 0xff, 0xff, 0x01, 0x34, 0x00, 0x00, 0xff, 0xcb, 0xff, 0xff, 0x01, 0x34, 0x00, 0x00,
0xff, 0xcb, 0xff, 0xff, 0x2b, 0x52, 0x00, 0x00, 0x03, 0x9c, 0x00, 0x00};*/


 uint8_t fluxo; //saber qual thread deve ser a proxima
 bool flag; //saber se deve imprimir ou não na tela
 uint16_t primo = 1; //chave
 uint16_t primoanterior = 1; //chave anterior
 uint32_t antepenultima; //antepenultima word
 uint32_t penultima; //penultima word
 uint32_t ultima; //ultima word
 
 uint32_t mensagemd[140];

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
osThreadId tgeracao, tdecodificacao, tantepenultima, tpenultima, tultima, tprimo, tfibonacci, texibir;
void geracao_thread(void const *args);
void decodificacao_thread(void const *args);
void antepenultima_thread(void const *args);
void penultima_thread(void const *args);
void ultima_thread(void const *args);
void primo_thread(void const *args);
void fibonacci_thread(void const *args);
void exibir_thread(void const *args);
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
//=============Fibbonacci Test==============
bool isPerfectSquare(int x)
{
    int s = sqrt(x);
    return (s*s == x);
}
bool isFibonacci(int n)
{
    // n is Fibinacci if one of 5*n*n + 4 or 5*n*n - 4 or both
    // is a perferct square
    return isPerfectSquare(5*n*n + 4) ||
           isPerfectSquare(5*n*n - 4);
}
//==========================================
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
	//rgb_init();
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
while(true){
osSignalWait(0x0001,osWaitForever);
	primo++; 
osSignalSet(tprimo,0x0002);
}
}

void decodificacao_thread(void const *args){
	uint8_t i;
while(true){
	osSignalWait(0x0003,osWaitForever);
	for(i = 0; i <= 35; i++){
		if(i%2==0)
			mensagemd[i] = mensagemo[i] + primo;
		else
			mensagemd[i] = mensagemo[i] - primo;
	}	
	osSignalSet(tantepenultima, 0x0004);
	flag = 1;
}
}

void antepenultima_thread(void const *args){
while(true){
	osSignalWait(0x0004,osWaitForever);
	antepenultima = mensagemd[137];
	osDelay(1000);	
	osSignalSet(tfibonacci, 0x0005);
}
}

void penultima_thread(void const *args){
while(true){
	osSignalWait(0x0006,osWaitForever);
	if(mensagemd[138] == 2*primo){
		osSignalSet(tultima,0x0007);
	}
	else{
		primoanterior = primo;
		osSignalSet(tgeracao,0x0001);
	}
}
}

void ultima_thread(void const *args){
while(true){
	osSignalWait(0x0007,osWaitForever);
	if(mensagemd[139] ==(primo + primoanterior)/antepenultima){
		osSignalSet(texibir,0x0008);
	}else{
		osSignalSet(tgeracao,0x0001);
		osDelay(osWaitForever);
	}
}
}

void primo_thread(void const *args){
	int aux;
	int cont = 0;
while(true)
	{
		osSignalWait(0x0002,osWaitForever);
		
	for (aux = 1; aux <= primo; aux++){
		if(primo%aux == 0){
			cont++;
		}
	}
	if (cont == 2){	
		osSignalSet(tdecodificacao,0x0003);
	}
	else{
		osSignalSet(tgeracao,0x0001);
	}
}
	
}

void fibonacci_thread(void const *args){
	int num1 = 0,num2 = 1,num3;
while(true){
	osSignalWait(0x0005,osWaitForever);
	while(num3 <= antepenultima){
		num3 = num1 + num2;
		num1 = num2;
		num2 = num3;
		if(num3 == antepenultima){
			osSignalSet(tpenultima,0x0006);
		}
	}
	primoanterior = primo;
	osSignalSet(tgeracao,0x0001);
}
}

void exibir_thread(void const *args){
	osSignalWait(0x0008,osWaitForever);
	init_sidelong_menu();
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
	tgeracao = osThreadCreate(osThread(geracao_thread), NULL);
	tdecodificacao = osThreadCreate(osThread(decodificacao_thread), NULL);
	tantepenultima = osThreadCreate(osThread(antepenultima_thread), NULL);
	tpenultima = osThreadCreate(osThread(penultima_thread), NULL);
	tultima = osThreadCreate(osThread(ultima_thread), NULL);
	tprimo = osThreadCreate(osThread(primo_thread), NULL);
	tfibonacci = osThreadCreate(osThread(fibonacci_thread), NULL);
	texibir = osThreadCreate(osThread(exibir_thread), NULL);
		
	osKernelStart();
	
	osSignalSet(tgeracao,0x0001);
//	while(true){
//		//GrStringDraw(&sContext,"TESTE", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
//	}
	
	osDelay(osWaitForever);
}
