#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include "rgb_j.h"
 												// Number of clock pulses per second.

volatile uint32_t flags = FLAG_NONE;            // A variable flags is declared which gets modified by interrupt(hence volatile)

void TIMER0A_Handler( void );                   // Prototype for the TIMER0A_Handler function.

void rgb_j_init(){
	GPIOF_AHB->DIR = (1<<1)|(1<<2)|(1<<3);            // make PF1,PF2, PF3 output
  GPIOF_AHB->DEN = (1<<1)|(1<<2)|(1<<3);            // digital i/o enable PF1, PF2, PF3
  
/*11.4 Initialization and Configuration */
  SYSCTL->RCGCTIMER |= (1<<0);                  // To use a GPTM, the appropriate TIMERn bit must be set in the RCGCTIMER
  
/*11.4.1 One-shot/Periodic Timer Mode with Interrupt generation */
  TIMER0->CTL = ~(1<<0);                        // Timer0 is disabled (the TnEN bit in the GPTMCTL register is cleared)
  TIMER0->CFG = 0x00000000;                     // Write the GPTMCFG with a value of 0x0000.0000.
  TIMER0->TAMR |= (0x2<<0);                     // Periodic timer mode in the TnMR field in the GPTMTnMR.
  TIMER0->TAMR &= ~(1<<4);                      // TnCDIR bits in the GPTMTnMR register set for count down direction.
  TIMER0->TAILR = SECONDS;                      // Load the start value into the GPTMTnILR.
  TIMER0->CTL = (1<<0);                         // Set the TnEN bit in the GPTMCTL register to enable the timer.
  TIMER0->IMR |= (1<<0);                        // Set the appropriate bits in the (GPTMIMR) to enable the interrupt mask
  NVIC_EnableIRQ(TIMER0A_IRQn);                 // Enable the TIMER0A interrupt request in NVIC.
  TIMER0->CTL = (1<<0);                         // Set the TnEN bit in the GPTMCTL register to enable the timer.
}

void TIMER0A_Handler( void ){                   
  if(flags == FLAG_TOGGLE_LED)flags = FLAG_NONE;                      // The flag is modified to FLAG_TOGGLE_LED so it can be detected in Main.
  else flags = FLAG_TOGGLE_LED;
	TIMER0->ICR |= (1<<0);                    
}