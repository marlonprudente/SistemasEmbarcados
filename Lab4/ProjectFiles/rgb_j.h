#ifndef RGB_J_H
#define RGB_J_H

#define FLAG_NONE 0x00000000                    // The flag will be reset to this when Interrupt is detected in the Main.
#define FLAG_TOGGLE_LED 0x00000001              // The flag will be set to this when Interrupt is generated and Handler function executes.
#define SECONDS 16000000

extern volatile uint32_t flags;
extern void rgb_j_init();
extern void TIMER0A_Handler(void);

#endif  //__RGB_J_H__