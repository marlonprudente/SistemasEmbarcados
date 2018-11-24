#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/rom_map.h"
#include "PWM.h"

#ifndef __SysCtlClockGet
#define __SysCtlClockGet()	\
SysCtlClockFreqSet( 				\
	SYSCTL_XTAL_25MHZ	| 			\
	SYSCTL_OSC_MAIN 	| 			\
	SYSCTL_USE_PLL 		| 			\
	SYSCTL_CFG_VCO_480, 			\
	120000000)
#endif

static uint32_t g_ui32SysClock;
static uint32_t g_ui16Period, g_ui16perMin;

void init_PWM(){
		uint32_t duty_cycle;	
	
	// Configure/Get Clock
	g_ui32SysClock = __SysCtlClockGet();
	
	// Enabling system's peripherals (timer and gpio)
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	
	// Wait system enabling be ready
	while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3) &
				!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));

	// The used port is PM3, we configure it to use the timer 
	MAP_GPIOPinConfigure(GPIO_PM3_T3CCP1);
	MAP_GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_3);

	// PIOSC frequency: 16MHz // SYSTEM: 120Mhz
	MAP_TimerClockSourceSet(TIMER3_BASE, TIMER_CLOCK_SYSTEM);
			
	// Configure timer as split pair (A/B) and PWM
	MAP_TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
	
	MAP_TimerControlLevel(TIMER3_BASE, TIMER_B, true);
	TimerUpdateMode(TIMER3_BASE, TIMER_B, TIMER_UP_MATCH_TIMEOUT);

	// Sets the load register prescale. As the maximum load value is 16bits (65 535)
	// and we need 16Mhz x 20 ms = 320 000, we set the prescale to 4, 
	// and the timer will count 4+1 = 5 times the load register [...]
	MAP_TimerPrescaleSet(TIMER3_BASE, TIMER_B, 0);

	// [...] wich gives 320 000/5 = 64 000 for a 20ms period
	g_ui16Period = 80000;
	// The minimum period is 16Mhz x 1ms = 16 000
	g_ui16perMin = 16000;
	duty_cycle = g_ui16perMin;
		
	// Sets the load (wave period), the match (pulse length) registers
	MAP_TimerLoadSet(TIMER3_BASE, TIMER_B, g_ui16Period);
	MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, duty_cycle);
	// Starts the PWM timer
	MAP_TimerEnable(TIMER3_BASE, TIMER_B);
}

void alterarFrequencia(int frequencia){	
	MAP_TimerLoadSet(TIMER3_BASE, TIMER_B, 160000);
	MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, 80000);
}

void ondaQuadrada(uint16_t angle){
		MAP_TimerLoadSet(TIMER3_BASE, TIMER_B, g_ui16Period);
		MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, 12000000/(angle*256));
}

void ondaSenoidal(uint16_t match, uint16_t period){
	
	MAP_TimerLoadSet(TIMER3_BASE, TIMER_B, period);
	MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, match);
}

void ondaTriangular(uint16_t angle){
	MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, g_ui16perMin*angle/0xFFFF + g_ui16perMin);
}

void ondaDenteSerra(uint16_t angle){
	MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, g_ui16perMin*angle/0xFFFF + g_ui16perMin);
}

void Duty20(){	
	uint16_t match, period;
	match = 120000;
	period = (uint16_t)600000;	
	ondaSenoidal(period, match);
}

void Duty40(){	
	uint16_t match, period;
	match = 32000;
	period = (uint16_t)600000;	
	ondaSenoidal(period, match);
}

void Duty60(){	
	uint16_t match, period;
	match = 48000;
	period = (uint16_t)600000;	
	ondaSenoidal(period, match);
}

void Duty80(){	
	uint16_t match, period;
	match = 64000;
	period = (uint16_t)600000;	
	ondaSenoidal(period, match);
}

// Altera duty-cycle do PWM
void pwmSetDuty(uint16_t duty16){
	// Lê valor atual do Load (valor de contagem máximo que define a frequência do PWM)
	uint16_t loadValue =  MAP_TimerLoadGet(TIMER3_BASE, TIMER_B);
	
	// Calcula a proporção que deve ser aplicada sobre o timer de 16-bits para o duty-cycle desejado
	float fduty = loadValue/(float)0xFFFF;
	
	// Aplica valor de Match, definindo o duty-cycle
  MAP_TimerMatchSet(TIMER3_BASE, TIMER_B, fduty*((float)duty16));
}