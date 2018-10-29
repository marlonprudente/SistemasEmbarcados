#ifndef PWM_H
#define PWM_H

#define PI 3.14159265359

extern void init_PWM();
extern void ondaQuadrada(uint16_t angulo);
extern void ondaSenoidal(uint16_t angulo);
extern void ondaDenteSerra(uint16_t angulo);
extern void ondaTriangular(uint16_t angulo);

#endif