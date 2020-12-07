#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void (*PWM_GPIO_CFG2[])(void) = {InitEpwm1Gpio,InitEpwm2Gpio,InitEpwm3Gpio,InitEpwm4Gpio,InitEpwm5Gpio,InitEpwm6Gpio,InitEpwm7Gpio,InitEpwm8Gpio};

void PWM_PSFB_VCM_FUN(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    EALLOW;
           CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
           CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
           EPwm2Regs.TZFRC.bit.OST = 1;
           EPwm1Regs.TZFRC.bit.OST = 1;
    EDIS;

    EALLOW;
    //PWM Clock Enable
    CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DRV_OBJ_P_INS->port1);
    CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DRV_OBJ_P_INS->port2);
    //HRPWM Clock Enable
    CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
    //Init EPWM GPIO
    PWM_GPIO_CFG2[target->PWM_DRV_OBJ_P_INS->port1]();
    PWM_GPIO_CFG2[target->PWM_DRV_OBJ_P_INS->port2]();

    Uint32 PRD_cal,deadtime_cal;
    Uint32 duty_cal;
    //HR period calculation: 256(8 digit)*100MHz/fs -> 256(8 digit)*100000/fkHz
    PRD_cal = 25600000/target->PWM_DRV_OBJ_P_INS->fkhz;

    //HR period
    PWM_DRV_PTR1->TBPRD               = (PRD_cal >> 8)-1;
    PWM_DRV_PTR1->TBPRDHR             = ((PRD_cal & 0xff))<<8;
    //Init phase shift
    PWM_DRV_PTR1->TBPHS.all           = 0;
    PWM_DRV_PTR1->TBCTL.bit.FREE_SOFT = 0b11;
    PWM_DRV_PTR1->TBCTL.bit.PHSDIR    = 1;//up count after syn
    PWM_DRV_PTR1->TBCTL.bit.CLKDIV    = 0;
    PWM_DRV_PTR1->TBCTL.bit.HSPCLKDIV = 0;//no divide
    PWM_DRV_PTR1->TBCTL.bit.SYNCOSEL  = 3;//syn out defined by TBCTL2.SYNCOSELX
    PWM_DRV_PTR1->TBCTL2.bit.SYNCOSELX= 1;//syn out = CMPC

    PWM_DRV_PTR1->CMPC                = (PRD_cal >> 8)-1;
    PWM_DRV_PTR1->TBCTL.bit.PRDLD     = 0;//period is loaded from shadow register when the TBCTR is equal to zero or a sync event as determined by TBCTL2.PRDLDSYNC bit
    PWM_DRV_PTR1->TBCTL.bit.PHSEN     = 0;//epwm1 is master
    PWM_DRV_PTR1->TBCTL.bit.CTRMODE   = 0;//up count mode


    PWM_DRV_PTR1->CMPCTL.bit.SHDWAMODE= 0;//CMPA shadow mode
    PWM_DRV_PTR1->CMPCTL.bit.LOADAMODE= 0;//load CMPA at CTR=ZRO

    //setting duty cycle
    duty_cal = (Uint32)(PRD_cal * 0.5);
    PWM_DRV_PTR1->CMPA.bit.CMPA       = duty_cal >> 8;
    PWM_DRV_PTR1->CMPA.bit.CMPAHR     = (duty_cal & 0xff)<<8;
    PWM_DRV_PTR1->AQCTLA.bit.CAU      = 0b01;//CMPA is clear
    PWM_DRV_PTR1->AQCTLA.bit.ZRO      = 0b10;//ZRO is set
    PWM_DRV_PTR1->AQSFRC.bit.RLDCSF   = 0b01;//Load on event counter equals period

    EALLOW;
    //HR PWM config
    PWM_DRV_PTR1->HRCNFG.all          = 0;
    PWM_DRV_PTR1->HRCNFG.bit.HRLOAD   = 0;//load at PRD
    PWM_DRV_PTR1->HRCNFG.bit.CTLMODE  = 0;//phase control mode
    PWM_DRV_PTR1->HRCNFG.bit.EDGMODE  = 3;//both edges
    PWM_DRV_PTR1->HRCNFG.bit.HRLOADB  = 0;//load at PRD
    PWM_DRV_PTR1->HRCNFG.bit.CTLMODEB = 0;//phase control mode
    PWM_DRV_PTR1->HRCNFG.bit.EDGMODEB = 3;//both edges
    PWM_DRV_PTR1->HRCNFG.bit.AUTOCONV = 1;//auto scaling is enable
    PWM_DRV_PTR1->HRPCTL.bit.TBPHSHRLOADE = 1;
    PWM_DRV_PTR1->HRPCTL.bit.HRPE     = 0;//HRPRD is enable

    //deadtime mode config
    PWM_DRV_PTR1->DBCTL.bit.HALFCYCLE = 1;//enable half deadtime
    PWM_DRV_PTR1->DBCTL.bit.SHDWDBFEDMODE = 1;
    PWM_DRV_PTR1->DBCTL.bit.SHDWDBREDMODE = 1;
    PWM_DRV_PTR1->DBCTL.bit.POLSEL    = 0b10;//AHC
    PWM_DRV_PTR1->DBCTL.bit.OUT_MODE  = 0b11;//AHC

    deadtime_cal = target->PWM_DRV_OBJ_P_INS->dbtime * 256 / 5;
    PWM_DRV_PTR1->DBFED.bit.DBFED     = deadtime_cal >> 8;
    PWM_DRV_PTR1->DBRED.bit.DBRED     = deadtime_cal >> 8;
    PWM_DRV_PTR1->DBFEDHR.bit.DBFEDHR = deadtime_cal & 0xff;//HR deadtime falling mode
    PWM_DRV_PTR1->DBREDHR.bit.DBREDHR = deadtime_cal & 0xff;//HR deadtime rising mode
    PWM_DRV_PTR1->HRCNFG2.bit.CTLMODEDBFED = 0;//DBFEDHR shadow value is loaded on CTR = Zero
    PWM_DRV_PTR1->HRCNFG2.bit.CTLMODEDBRED = 0;//DBREDHR shadow value is loaded on CTR = Zero
    PWM_DRV_PTR1->HRCNFG2.bit.EDGMODEDB = 0b11;//MEP control of both edges

//epwmno+1
    PWM_DRV_PTR2->TBCTL.bit.FREE_SOFT = 0b10;
    PWM_DRV_PTR2->TBCTL.bit.PHSDIR    = 1;//up after syn
    PWM_DRV_PTR2->TBCTL.bit.CLKDIV    = 0;
    PWM_DRV_PTR2->TBCTL.bit.HSPCLKDIV = 0;//no divide
    PWM_DRV_PTR2->TBCTL.bit.SYNCOSEL  = 1;//syn out when CTR=0
    PWM_DRV_PTR2->TBCTL.bit.PRDLD     = 0;//shadow mode
    PWM_DRV_PTR2->TBCTL.bit.PHSEN     = 1;//epwmno+1 is slaver
    PWM_DRV_PTR2->TBCTL.bit.CTRMODE   = 0;//up count

    PWM_DRV_PTR2->TBPRD               = (PRD_cal >> 8)-1;
    PWM_DRV_PTR2->TBPRDHR             = (PRD_cal & 0xff)<<8;

    PWM_DRV_PTR2->CMPCTL.bit.SHDWAMODE= 0;//shadow mode
    PWM_DRV_PTR2->CMPCTL.bit.LOADAMODE= 1;//load at CTR=PRD

    PWM_DRV_PTR2->CMPA.bit.CMPA       = duty_cal >> 8;
    PWM_DRV_PTR2->CMPA.bit.CMPAHR     = (duty_cal & 0xff)<<8;
    PWM_DRV_PTR2->AQCTLA.bit.CAU      = 0b01;//CMPA is clear
    PWM_DRV_PTR2->AQCTLA.bit.ZRO      = 0b10;//ZRO is set
    PWM_DRV_PTR2->AQSFRC.bit.RLDCSF   = 0b01;

    PWM_DRV_PTR2->TBPHS.bit.TBPHS     = 0;
    PWM_DRV_PTR2->TBPHS.bit.TBPHSHR   = 0;

    EALLOW;
    PWM_DRV_PTR2->HRCNFG.all          = 0;
    PWM_DRV_PTR2->HRCNFG.bit.HRLOAD   = 0;//load at PRD
    PWM_DRV_PTR2->HRCNFG.bit.CTLMODE  = 1;//phase control mode
    PWM_DRV_PTR2->HRCNFG.bit.EDGMODE  = 3;//both edges
    PWM_DRV_PTR2->HRCNFG.bit.HRLOADB  = 0;//load at PRD
    PWM_DRV_PTR2->HRCNFG.bit.CTLMODEB = 1;//phase control mode
    PWM_DRV_PTR2->HRCNFG.bit.EDGMODEB = 3;//both edges
    PWM_DRV_PTR2->HRCNFG.bit.AUTOCONV = 1;//auto scaling is enable
    PWM_DRV_PTR2->HRPCTL.bit.TBPHSHRLOADE = 1;
    PWM_DRV_PTR2->HRPCTL.bit.HRPE     = 0;//HRPRD is enable

    PWM_DRV_PTR2->DBCTL.bit.HALFCYCLE = 1;//enable half cycle
    PWM_DRV_PTR2->DBCTL.bit.SHDWDBFEDMODE = 1;
    PWM_DRV_PTR2->DBCTL.bit.SHDWDBREDMODE = 1;
    PWM_DRV_PTR2->DBCTL.bit.POLSEL    = 0b10;//AHC
    PWM_DRV_PTR2->DBCTL.bit.OUT_MODE  = 0b11;//AHC
    PWM_DRV_PTR2->DBCTL.bit.OUTSWAP   = 0b00;

    PWM_DRV_PTR2->DBFED.bit.DBFED     = deadtime_cal >> 8;
    PWM_DRV_PTR2->DBRED.bit.DBRED     = deadtime_cal >> 8;
    PWM_DRV_PTR2->DBFEDHR.bit.DBFEDHR = deadtime_cal & 0xff;
    PWM_DRV_PTR2->DBREDHR.bit.DBREDHR = deadtime_cal & 0xff;

    PWM_DRV_PTR2->HRCNFG2.bit.CTLMODEDBFED = 0;
    PWM_DRV_PTR2->HRCNFG2.bit.CTLMODEDBRED = 0;
    PWM_DRV_PTR2->HRCNFG2.bit.EDGMODEDB = 0b11;

    switch(target->PWM_DRV_OBJ_P_INS->socport)
    {
    case ePWM1SOCA:
        //Trigger ADC Start of Conversion A EPWM1
        PWM_DRV_PTR1->ETSEL.bit.SOCAEN      = 1;//enable socA
        PWM_DRV_PTR1->ETSEL.bit.SOCASEL     = 2;//soc = TBCTR=PRD
        PWM_DRV_PTR1->ETPS.bit.SOCAPRD      = 1;//Generate the EPWMxSOCA pulse on the first SOC event (each period)
        break;
    case ePWM2SOCA:
        //Trigger ADC ADC Start of Conversion A EPWM2
        PWM_DRV_PTR2->ETSEL.bit.SOCAEN      = 1;//enable socA
        PWM_DRV_PTR2->ETSEL.bit.SOCASEL     = 2;//soc = TBCTR=PRD
        PWM_DRV_PTR2->ETPS.bit.SOCAPRD      = 1;//Generate the EPWMxSOCA pulse on the first SOC event (each period)
        break;
    default:
        break;
    }


    EDIS;
    EALLOW;
           CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
           PWM_DRV_PTR1->TBCTL.bit.SWFSYNC = 1;
           PWM_DRV_PTR2->TBCTL.bit.SWFSYNC = 1;
    EDIS;
}
