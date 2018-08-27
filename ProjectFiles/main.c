#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
 float primo = 0;
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
 *      Threads
 *---------------------------------------------------------------------------*/
 

void geracao_thread(void const *args){
	primo++; 
	//envia para verificação de numero primo
	osDelay(1000);	
}

void decodificacao_thread(void const *args){
	
	osDelay(1000);	
}

void antepenultima_thread(void const *args){
	osDelay(1000);	
}

void penultima_thread(void const *args){
	osDelay(1000);	
}

void ultima_thread(void const *args){
	osDelay(1000);	
}

void primo_thread(void const *args){
	float aux;
	int cont = 0;
	for (aux = 2; aux < primo; aux++){
		if(primo%aux == 0){
			cont++;
		}
	}
	if (cont > 0){
		//retorna geração	
	}
		
	osDelay(1000);	
	
}

void fibonacci_thread(void const *args){
	int num1 = 0,num2 = 1,num3;
	bool flagcorreto = true;
	num3 = num1 + num2;
	
	while}(){
		num3 = num1 + num2;
		num1 = num2;
		num2 = num3;
		if(num3 = antepenultima){
			flagcorreto = true;
			break;
		}
		else if(num3 > antepenultima)
			flagcorreto = false;
			break;
		
	}
	osDelay(1000);	
}

void exibir_thread(void const *args){
	osDelay(1000);	
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
	osThreadCreate(osThread(geracao_thread), NULL);
	osThreadCreate(osThread(decodificacao_thread), NULL);
	osThreadCreate(osThread(antepenultima_thread), NULL);
	osThreadCreate(osThread(penultima_thread), NULL);
	osThreadCreate(osThread(ultima_thread), NULL);
	osThreadCreate(osThread(primo_thread), NULL);
	osThreadCreate(osThread(fibonacci_thread), NULL);
	osThreadCreate(osThread(exibir_thread), NULL);
}
