#include "F28x_Project.h"

#ifndef CLA_INTERRUPTS_H_
#define CLA_INTERRUPTS_H_
//#define Kp 0.003f//0.1f
//#define Ki 1.0f




extern float io;
extern float iref;
extern float Kp;
extern float Ki;
extern float Tpulse;
extern bool Prt_flag;

__attribute__((interrupt)) void claTask1(void);
__attribute__((interrupt)) void claTask2(void);
__attribute__((interrupt)) void claTask3(void);
__attribute__((interrupt)) void claTask4(void);
__attribute__((interrupt)) void claTask5(void);
__attribute__((interrupt)) void claTask6(void);
__attribute__((interrupt)) void claTask7(void);
__attribute__((interrupt)) void claTask8(void);

typedef volatile struct {
    float32 out;
    float32 mult;
    float32 mult2;
}Cla_Evag;

typedef volatile struct{
    float32 b0;
    float32 b1;
    float32 a1;
    float32 ek1;
    float32 uk1;
    float32 umax;
    float32 umin;
    float32 uimax;
    float32 uimin;
}Cla_Control_N;
extern void Cla_Avg_Run(Cla_Evag *p);
extern void Cla_PS_Control_Run(Cla_Evag *p,Cla_Control_N *q,float32 ref);
extern void Cla_Pulse_Run(void);
#endif
