#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H

void StepMotor_PWM_Init(void);
void StepMotor_Pulse_Init(void);
void StepMotor_Pulse(void);
void StepMotor_NPulse(uint32_t N,uint32_t time,uint8_t direction);

#endif
