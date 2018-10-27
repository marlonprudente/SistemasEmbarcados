#include "UART.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"

//defines



//função de inicialização
void initUART(void)
{
	SYSCTL-> RCGCUART |= (1<<0);   				//Habilita registradores da para uso da UART0
	SYSCTL-> RCGCGPIO |= (1<<0);					//Habilita a porta A do GPIO
	GPIOA_AHB -> AFSEL = (1<<1)|(1<<0);		//Habilita o usa de funcoes especiais do GPIO
	GPIOA_AHB -> PCTL = (1<<0)|(1<<4);
	GPIOA_AHB -> DEN = (1<<0)|(1<<1);
	UART0 ->	CTL &= ~(1<<0);							//Disable a UART, para fazer as configuracoes 
	UART0 ->	IBRD = 65;									//Baudrate 115200
	UART0 ->	FBRD = 7;										//Relativo a parte decimal do baud
	UART0 ->	LCRH = (0x3<<5);						//palavra de 8 bits sem paridade 
	UART0 ->	CC	= 0x0;									//Usa o clock do sistema 
	UART0 ->	IFLS |=(1<<4);							//Uso default 
	UART0 ->	IM |=(1<<4);								//Habilita interrupcoes 
	UART0 ->	CTL = (1<<0)|(1<<8)|(1<<9);	//Habilita a UART com Rx&Tx
	NVIC -> ISER[0] |= (1<<5);						//Habilita interrupcoes de UART0 no vetor de interrupcoes
	__NVIC_SetPriority(UART0_IRQn,1<<5);
}
//função de envio
char writeUART(void)
{
	char c;
	while((UART0->FR & (1<<4)) != 0);//Enquanto estiver enviado algo NAO se le
	c = UART0->DR;//Recebe caracteres para a saida
	return c;
}
char * UARTreadstring(){
	char c = writeUART();
	char* texto = "";
	while(c){
		texto += c;
	}
	
	return texto;
}

/*Envio de elementos*/
void printchar(char c){//Envio UM caracter
	while((UART0->FR & (1<<5)) != 0);//Enquanto estiver recebendo algo NAO escreve
	UART0->DR = c;	//Escreve na saida o conteudo de c
}
void UARTprintstring(char * string){//Envia uma String
	while(*string){//enquanto houver elementos na string
		printchar(*(string++));//Envia caracteres para a saida
	}
}

//tratamento de interrupções