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
#include <math.h>
#include <stdio.h>
#include "grlib/grlib.h"
#include "driverlib/uart.h"

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

uint32_t mensagemo[36] = 
{ 0xffffcc43, 0x00003466, 0xffffcc1f, 0x00003457, 0xffffcc6e, 0x0000346d,
  0xffffcc73, 0x00003462, 0xffffcc1f, 0x00003451, 0xffffcc60, 0x00003473,
	0xffffcc60, 0x00003421, 0xffffcc6e, 0x00003421, 0xffffcc45, 0x00003476,
	0xffffcc73, 0x00003476, 0xffffcc71, 0x00003470, 0xffffcc1f, 0x00003421,
	0xffffcc1f, 0x00003421, 0xffffcc30, 0x0000343a, 0xffffcc37, 0x00003436,
	0x0000811f, 0x00009c03, 0xffffcbff };

/*uint32_t mensagemo[36] = 
{ 0x88ca561a, 0x7735aa9e, 0x88ca5649, 0x7735aa9d, 0x88ca5640, 0x7735aa97, 
	0x88ca563b, 0x7735aa98, 0x88ca55f7, 0x7735aa8a, 0x88ca55f7, 0x7735aa7f, 
  0x88ca5640, 0x7735aa8d, 0x88ca5638, 0x7735aa49, 0x88ca5618, 0x7735aa8d, 
  0x88ca5646, 0x7735aa92, 0x88ca563b, 0x7735aa8a, 0x88ca563b, 0x7735aa98, 
  0x88ca55f7, 0x7735aa49, 0x88ca5608, 0x7735aa62, 0x88ca560f, 0x7735aa5f, 
  0x98c2620f, 0x65a0fe7b, 0x88ca55e5};*/

 uint8_t fluxo; //saber qual thread deve ser a proxima
 bool flag; //saber se deve imprimir ou não na tela
 uint16_t primo = 2; //chave	
 uint16_t primoanterior = 1; //chave anterior
 uint32_t antepenultima; //antepenultima word
 uint32_t penultima; //penultima word
 uint32_t ultima; //ultima word
 
 uint32_t mensagemd[36];
//To print on the screen
tContext sContext;

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

void init_display(){
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
	/*GrStringDraw(&sContext,"Exemplo EK-TM4C1294XL", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"RGB", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"ACC", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"TMP", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"OPT", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"MIC", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"JOY", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"BUT", -1, 0, (sContext.psFont->ui8Height+2)*8, true);*/

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

/*----------------------------------------------------------------------------
 *      Threads
 *---------------------------------------------------------------------------*/
void geracao_thread(void const *args){
	if(fluxo == 1){
		primo++; 
		//envia para verificação de numero primo
		
		fluxo = 2;
		osThreadYield();
	}
}

void decodificacao_thread(void const *args){	
	uint8_t i;
	if(fluxo == 3){
		for(i = 0; i <= 35; i++){
			if(i%2==0)
				mensagemd[i] = mensagemo[i] + primo;
			else
				mensagemd[i] = mensagemo[i] - primo;
		}
		fluxo = 4;
		flag = 1;
		osThreadYield();
	}
}

void antepenultima_thread(void const *args){
	if(fluxo == 4){
		antepenultima = mensagemd[33];
		fluxo = 5;
	}
	osThreadYield();
}

void penultima_thread(void const *args){
	if(fluxo == 6){
		if(mensagemd[34] == 2*primo){
			fluxo = 7;
		}
		else{
			fluxo = 1;
			primoanterior = primo;
		}
	}
		osThreadYield();
}

void ultima_thread(void const *args){
	
	if(fluxo == 7){
		if(mensagemd[35] == (primo + primoanterior)/antepenultima )
		{
			flag = 1;
			fluxo = 8;
			osDelay(osWaitForever);
		}
		else
			fluxo = 1;
	}
	osThreadYield();
}

void primo_thread(void const *args){
	uint32_t aux;
	int cont = 0;
	//UARTprintstring("PrimoThread");	
	if(fluxo == 2){
		for (aux = 1; aux <= primo; aux++){
			if(primo%aux == 0)
				cont++;
		}
		if (cont == 2){	
			fluxo = 3;
		}
		else{
			fluxo = 1;
		}
	}
	osThreadYield();
}

void fibonacci_thread(void const *args){
	uint32_t num1 = 0,num2 = 1,num3;
	num3 = num1 + num2;
	if(fluxo == 5){
		while(num3 < antepenultima)
			{
					num1 = num2;
					num2 = num3;
					num3 = num1 + num2;
			}
		if(num3 == antepenultima)
		{
			fluxo = 6;
		}
		else{
			fluxo = 1;
			primoanterior = primo;			
		}
		/*if(isFibonacci(antepenultima)){
			fluxo = 6;
		}else{
			fluxo = 1;
			primoanterior = primo;
		}	*/
	}
	osThreadYield();
}

void exibir_thread(void const *args){
	uint8_t n;
	uint8_t i;
	char c[2];
	
	if(flag == 0)
	{
		osThreadYield();
	}
	init_display();
	//Sidelong menu creation
	init_sidelong_menu();

	for(i = 0; i<36; i++){
	c[0] = (char)(mensagemd[i])%256;
	c[1] = '\0';
	GrStringDraw(&sContext,(char*)c, -1,  (sContext.psFont->ui8MaxWidth)*(i%20), (sContext.psFont->ui8Height+2)*(2 + i/20), true);
	}
	GrStringDraw(&sContext,"Mensagem", -1,  0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"antepenultima", -1,  0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"penultima", -1,  0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"ultima", -1,  0, (sContext.psFont->ui8Height+2)*8, true);
	
	flag = 0;
}
 /*----------------------------------------------------------------------------
 *      ThreadsDef
 *---------------------------------------------------------------------------*/
osThreadDef(geracao_thread, osPriorityNormal, 1, 0);
osThreadDef(primo_thread, osPriorityNormal, 1, 0);
osThreadDef(decodificacao_thread, osPriorityNormal, 1, 0);
osThreadDef(antepenultima_thread, osPriorityNormal, 1, 0);
osThreadDef(fibonacci_thread, osPriorityNormal, 1, 0);
osThreadDef(penultima_thread, osPriorityNormal, 1, 0);
osThreadDef(ultima_thread, osPriorityNormal, 1, 0);
osThreadDef(exibir_thread, osPriorityNormal, 1, 0);
/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	osKernelInitialize();
	//Initializing all peripherals
	init_display();
	//Sidelong menu creation
	init_sidelong_menu();
	osThreadCreate(osThread(geracao_thread), NULL);
	osThreadCreate(osThread(primo_thread), NULL);
	osThreadCreate(osThread(decodificacao_thread), NULL);
	osThreadCreate(osThread(antepenultima_thread), NULL);
	osThreadCreate(osThread(fibonacci_thread), NULL);
	osThreadCreate(osThread(penultima_thread), NULL);
	osThreadCreate(osThread(ultima_thread), NULL);
	osThreadCreate(osThread(exibir_thread), NULL);
	fluxo = 1;
	
	osKernelStart();
	osDelay(osWaitForever);			
	}	

