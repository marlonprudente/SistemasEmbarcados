#ifndef PWM_H
#define PWM_H

#define PI 3.14159265359

extern void init_PWM();
extern void alterarFrequencia(int frequencia);
extern void alterarAmplitude(uint16_t amplitude);
extern void ondaQuadrada(uint16_t angulo);
extern void ondaSenoidal(uint16_t match, uint16_t period);
extern void ondaDenteSerra(uint16_t angulo);
extern void ondaTriangular(uint16_t angulo);
extern void pwmSetDuty(uint16_t duty);
extern void Duty20();
extern void Duty40();
extern void Duty60();
extern void Duty80();


#endif