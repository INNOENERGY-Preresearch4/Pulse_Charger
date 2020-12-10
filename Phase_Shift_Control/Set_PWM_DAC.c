#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void (*PWM_GPIO_CFG1[])(void) = {InitEpwm1Gpio,InitEpwm2Gpio,InitEpwm3Gpio,InitEpwm4Gpio,InitEpwm5Gpio,InitEpwm6Gpio,InitEpwm7Gpio,InitEpwm8Gpio};

//Hardware Protection PWM Setting
//HW Protection Threshold is controlled by PWM duty cycle. HW Threshold = 3.3V*d
void PWM_DAC_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ *)OBJ;
    EALLOW;
    //PWM Clock Enable
    CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DAC_OBJ_P_INS->port1);
    CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DAC_OBJ_P_INS->port2);
    //Init EPWM GPIO
    PWM_GPIO_CFG1[target->PWM_DAC_OBJ_P_INS->port1]();
    PWM_GPIO_CFG1[target->PWM_DAC_OBJ_P_INS->port2]();
    PWM_DAC_PTR1->TBCTL.bit.FREE_SOFT  = 0b10;
    PWM_DAC_PTR1->TBCTL.bit.PHSDIR     = 1;
    PWM_DAC_PTR1->TBCTL.bit.CLKDIV     = 0;
    PWM_DAC_PTR1->TBCTL.bit.HSPCLKDIV  = 0;
    PWM_DAC_PTR1->TBCTL.bit.PRDLD      = 0;
    PWM_DAC_PTR1->TBCTL.bit.PHSEN      = 0;
    PWM_DAC_PTR1->TBCTL.bit.CTRMODE    = 0;
    PWM_DAC_PTR1->CMPCTL.bit.SHDWAMODE = 0;
    PWM_DAC_PTR1->CMPCTL.bit.SHDWBMODE = 0;
    PWM_DAC_PTR1->CMPCTL.bit.LOADAMODE = 1;
    PWM_DAC_PTR1->CMPCTL.bit.LOADBMODE = 1;
    PWM_DAC_PTR1->AQCTL.bit.SHDWAQAMODE= 1;
    PWM_DAC_PTR1->AQCTL.bit.SHDWAQBMODE= 1;
    PWM_DAC_PTR1->AQCTL.bit.LDAQAMODE  = 1;
    PWM_DAC_PTR1->AQCTL.bit.LDAQBMODE  = 1;
    PWM_DAC_PTR1->AQCTLA.bit.ZRO       = 0b10;
    PWM_DAC_PTR1->AQCTLA.bit.CAU       = 0b01;
    PWM_DAC_PTR1->AQCTLA.bit.PRD       = 0b01;
    PWM_DAC_PTR1->AQCTLB.bit.ZRO       = 0b10;
    PWM_DAC_PTR1->AQCTLB.bit.CBU       = 0b01;
    PWM_DAC_PTR1->AQCTLB.bit.PRD       = 0b01;
    PWM_DAC_PTR1->TBPHS.bit.TBPHS      = 0;
    PWM_DAC_PTR1->TBPRD                = 9999;
    PWM_DAC_PTR1->CMPA.bit.CMPA        = (Uint16)((target->PWM_DAC_OBJ_P_INS->DACout1)/V_scaling*10000);
    PWM_DAC_PTR1->CMPB.bit.CMPB        = (Uint16)((target->PWM_DAC_OBJ_P_INS->DACout2)/I_scaling*10000);

    PWM_DAC_PTR2->TBCTL.bit.FREE_SOFT  = 0b10;
    PWM_DAC_PTR2->TBCTL.bit.PHSDIR     = 1;
    PWM_DAC_PTR2->TBCTL.bit.CLKDIV     = 0;
    PWM_DAC_PTR2->TBCTL.bit.HSPCLKDIV  = 0;
    PWM_DAC_PTR2->TBCTL.bit.PRDLD      = 0;
    PWM_DAC_PTR2->TBCTL.bit.PHSEN      = 0;
    PWM_DAC_PTR2->TBCTL.bit.CTRMODE    = 0;
    PWM_DAC_PTR2->CMPCTL.bit.SHDWAMODE = 0;
    PWM_DAC_PTR2->CMPCTL.bit.SHDWBMODE = 0;
    PWM_DAC_PTR2->CMPCTL.bit.LOADAMODE = 1;
    PWM_DAC_PTR2->CMPCTL.bit.LOADBMODE = 1;
    PWM_DAC_PTR2->AQCTL.bit.SHDWAQAMODE= 1;
    PWM_DAC_PTR2->AQCTL.bit.SHDWAQBMODE= 1;
    PWM_DAC_PTR2->AQCTL.bit.LDAQAMODE  = 1;
    PWM_DAC_PTR2->AQCTL.bit.LDAQBMODE  = 1;
    PWM_DAC_PTR2->AQCTLA.bit.ZRO       = 0b10;
    PWM_DAC_PTR2->AQCTLA.bit.CAU       = 0b01;
    PWM_DAC_PTR2->AQCTLA.bit.PRD       = 0b01;
    PWM_DAC_PTR2->AQCTLB.bit.ZRO       = 0b10;
    PWM_DAC_PTR2->AQCTLB.bit.CBU       = 0b01;
    PWM_DAC_PTR2->AQCTLB.bit.PRD       = 0b01;
    PWM_DAC_PTR2->TBPHS.bit.TBPHS      = 0;
    PWM_DAC_PTR2->TBPRD                = 9999;
    PWM_DAC_PTR2->CMPA.bit.CMPA        = (Uint16)((target->PWM_DAC_OBJ_P_INS->DACout3)/I_scaling*10000);
    PWM_DAC_PTR2->CMPB.bit.CMPB        = (Uint16)((target->PWM_DAC_OBJ_P_INS->DACout4)/V_scaling*10000);

    EDIS;
}
