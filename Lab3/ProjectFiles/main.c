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
#include "rgb.h"
#include "buzzer.h"
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
#define osFeature_SysTick 1

uint32_t nave[21][16] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0,
0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0,
0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,
0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,
0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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

void init_all(){
	cfaf128x128x16Init();
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	//GrStringDraw(&sContext,"River Raid", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	joy_init();
	buzzer_init(); 
	button_init();
	//rgb_init();
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
/*bool isPerfectSquare(int x)
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
}*/

/*----------------------------------------------------------------------------
 *      Threads
 *---------------------------------------------------------------------------*/

	
 /*----------------------------------------------------------------------------
 *      ThreadsDef
 *---------------------------------------------------------------------------*/

//osThreadDef(thread, osPriorityNormal, 1, 0);
/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	uint16_t x, y,center;
	uint8_t i = 0,j = 0,a = 0 , b = 0;
	init_all();

	while(1){
	x = joy_read_x();
	y = joy_read_y();
	
			for(i = 0; i < 16; i++){
				for(j = 0; j < 21; j++){
					if(nave[j][i] == 1 ){
						GrContextForegroundSet(&sContext, ClrWhite);
						GrPixelDraw(&sContext, i+a , j+b);}
					else{
						GrContextForegroundSet(&sContext, ClrBlack);
						GrPixelDraw(&sContext, i+a , j+b);}
									}
			}
			if(x > 2800)
				a++;
			else if (x < 1500)
				a--;
			if(y > 2800)
				b--;
			else if (y < 1500)
				b++;
			osDelay(20);
				}
			}