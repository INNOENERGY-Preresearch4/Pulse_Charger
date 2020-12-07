#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"


void (*PWM_GPIO_CFG3[])(void) = {InitEpwm1Gpio,InitEpwm2Gpio,InitEpwm3Gpio,InitEpwm4Gpio,InitEpwm5Gpio,InitEpwm6Gpio,InitEpwm7Gpio,InitEpwm8Gpio};

void PWM_PSFB_PCMC_FUN(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    EALLOW;
           CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
           CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
           EPwm2Regs.TZFRC.bit.OST = 1;
           EPwm1Regs.TZFRC.bit.OST = 1;
    EDIS;
    EALLOW;
        CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DRV_OBJ_P_INS->port1);
        CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DRV_OBJ_P_INS->port2);
        CpuSysRegs.PCLKCR2.all |= 1<<(target->PWM_DRV_OBJ_P_INS->port3);
        CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
        PWM_GPIO_CFG3[target->PWM_DRV_OBJ_P_INS->port1]();
        PWM_GPIO_CFG3[target->PWM_DRV_OBJ_P_INS->port2]();

    Uint32 PRD_cal,deadtime_cal,duty_cal;
    PRD_cal = 25600000/target->PWM_DRV_OBJ_P_INS->fkhz;
    EALLOW;
    PWM_DRV_PTR1->TBPRD               = (PRD_cal >> 9);
    PWM_DRV_PTR1->TBPRDHR             = ((PRD_cal & 0x7f))<<8;
    PWM_DRV_PTR1->TBPHS.all           = 0;
    PWM_DRV_PTR1->TBCTL.bit.FREE_SOFT = 0b11;
    PWM_DRV_PTR1->TBCTL.bit.PHSDIR    = 1;//up after syn
    PWM_DRV_PTR1->TBCTL.bit.CLKDIV    = 0;
    PWM_DRV_PTR1->TBCTL.bit.HSPCLKDIV = 0;//no divide
    PWM_DRV_PTR1->TBCTL.bit.SYNCOSEL  = 1;//syn out = CMPC
    PWM_DRV_PTR1->TBCTL2.bit.SYNCOSELX= 1;
    PWM_DRV_PTR1->CMPC                = (PRD_cal >> 8)-2;
    PWM_DRV_PTR1->TBCTL.bit.PRDLD     = 0;//shadow mode
    PWM_DRV_PTR1->TBCTL.bit.PHSEN     = 0;//epwmno is master
    PWM_DRV_PTR1->TBCTL.bit.CTRMODE   = 2;//up down count


    PWM_DRV_PTR1->CMPCTL.bit.SHDWAMODE= 0;//shadow mode
    PWM_DRV_PTR1->CMPCTL.bit.LOADAMODE= 0;//load at CTR=ZRO

    duty_cal = (Uint32)(PRD_cal * 0.5);
    PWM_DRV_PTR1->CMPA.bit.CMPA       = duty_cal >> 8;
    PWM_DRV_PTR1->CMPA.bit.CMPAHR     = (duty_cal & 0xff)<<8;
    //PWM_DRV_PTR1->AQCTLA.bit.CAU      = 0b01;//CMPA is clear
    PWM_DRV_PTR1->AQCTLA.bit.PRD      = 0b01;
    PWM_DRV_PTR1->AQCTLA.bit.ZRO      = 0b10;//ZRO is set
    PWM_DRV_PTR1->AQSFRC.bit.RLDCSF   = 0b01;

    PWM_DRV_PTR1->HRCNFG.all          = 0;
    PWM_DRV_PTR1->HRCNFG.bit.HRLOAD   = 0;//load at PRD
    PWM_DRV_PTR1->HRCNFG.bit.CTLMODE  = 0;//phase control mode
    PWM_DRV_PTR1->HRCNFG.bit.EDGMODE  = 3;//both edges
    PWM_DRV_PTR1->HRCNFG.bit.HRLOADB  = 0;//load at PRD
    PWM_DRV_PTR1->HRCNFG.bit.CTLMODEB = 0;//phase control mode
    PWM_DRV_PTR1->HRCNFG.bit.EDGMODEB = 3;//both edges
    PWM_DRV_PTR1->HRCNFG.bit.AUTOCONV = 1;//auto scaling is enable
    PWM_DRV_PTR1->HRPCTL.bit.TBPHSHRLOADE = 1;
    PWM_DRV_PTR1->HRPCTL.bit.HRPE     = 1;//HRPRD is enable


    PWM_DRV_PTR1->DBCTL.bit.HALFCYCLE = 1;//enable half cycle
    PWM_DRV_PTR1->DBCTL.bit.SHDWDBFEDMODE = 1;
    PWM_DRV_PTR1->DBCTL.bit.SHDWDBREDMODE = 1;
    PWM_DRV_PTR1->DBCTL.bit.POLSEL    = 0b10;//AHC
    PWM_DRV_PTR1->DBCTL.bit.OUT_MODE  = 0b11;//AHC

    deadtime_cal = target->PWM_DRV_OBJ_P_INS->dbtime * 256 / 5;
    PWM_DRV_PTR1->DBFED.bit.DBFED     = deadtime_cal >> 8;
    PWM_DRV_PTR1->DBRED.bit.DBRED     = deadtime_cal >> 8;
    PWM_DRV_PTR1->DBFEDHR.bit.DBFEDHR = deadtime_cal & 0xff;
    PWM_DRV_PTR1->DBREDHR.bit.DBREDHR = deadtime_cal & 0xff;
    PWM_DRV_PTR1->HRCNFG2.bit.CTLMODEDBFED = 0;
    PWM_DRV_PTR1->HRCNFG2.bit.CTLMODEDBRED = 0;
    PWM_DRV_PTR1->HRCNFG2.bit.EDGMODEDB = 0b11;

//PWM+1
    PWM_DRV_PTR2->TBCTL.bit.FREE_SOFT = 0b10;
    PWM_DRV_PTR2->TBCTL.bit.PHSDIR    = 1;//up after syn
    PWM_DRV_PTR2->TBCTL.bit.CLKDIV    = 0;
    PWM_DRV_PTR2->TBCTL.bit.HSPCLKDIV = 0;//no divide
    PWM_DRV_PTR2->TBCTL.bit.SYNCOSEL  = 0;//syn out when CTR=0
    PWM_DRV_PTR2->TBCTL.bit.PRDLD     = 0;//shadow mode
    PWM_DRV_PTR2->TBCTL.bit.PHSEN     = 1;//epwmno+1 is slaver
    PWM_DRV_PTR2->TBCTL.bit.CTRMODE   = 2;//up down count

    PWM_DRV_PTR2->TBPRD               = (PRD_cal >> 9);
    PWM_DRV_PTR2->TBPRDHR             = (PRD_cal & 0x7f)<<8;

    PWM_DRV_PTR2->AQCTLA.bit.ZRO      = 0b10;//ZRO is set
    PWM_DRV_PTR2->AQCTLA.bit.PRD      = 0b01;//PRD is clr
    PWM_DRV_PTR2->AQCTLA2.bit.T1U     = 0b01;//T1 up is clr
    PWM_DRV_PTR2->AQCTLA2.bit.T1D     = 0b10;//T1 down is set
    PWM_DRV_PTR2->AQTSRCSEL.bit.T1SEL = 0b1000;
//    PWM_DRV_PTR2->AQTSRCSEL.bit.T1SEL = 0b0001;//DCAEVT2
    PWM_DRV_PTR2->AQSFRC.bit.RLDCSF   = 0b01;

    PWM_DRV_PTR2->TBPHS.bit.TBPHS     = 0;
    PWM_DRV_PTR2->TRREM.bit.TRREM     = 0;

    PWM_DRV_PTR2->HRCNFG.all          = 0;
    PWM_DRV_PTR2->HRCNFG.bit.HRLOAD   = 0;//load at PRD
    PWM_DRV_PTR2->HRCNFG.bit.CTLMODE  = 1;//phase control mode
    PWM_DRV_PTR2->HRCNFG.bit.EDGMODE  = 3;//both edges
    PWM_DRV_PTR2->HRCNFG.bit.HRLOADB  = 0;//load at PRD
    PWM_DRV_PTR2->HRCNFG.bit.CTLMODEB = 1;//phase control mode
    PWM_DRV_PTR2->HRCNFG.bit.EDGMODEB = 3;//both edges
    PWM_DRV_PTR2->HRCNFG.bit.AUTOCONV = 1;//auto scaling is enable
    PWM_DRV_PTR2->HRPCTL.bit.TBPHSHRLOADE = 1;
    PWM_DRV_PTR2->HRPCTL.bit.HRPE     = 1;//HRPRD is enable

    PWM_DRV_PTR2->DBCTL.bit.IN_MODE = 0b00;
    PWM_DRV_PTR2->DBCTL.bit.DEDB_MODE = 0;
    PWM_DRV_PTR2->DBCTL.bit.HALFCYCLE = 1;//enable half cycle
    PWM_DRV_PTR2->DBCTL.bit.SHDWDBFEDMODE = 1;
    PWM_DRV_PTR2->DBCTL.bit.SHDWDBREDMODE = 1;
    PWM_DRV_PTR2->DBCTL.bit.POLSEL    = 0b10;//AHC
    PWM_DRV_PTR2->DBCTL.bit.OUT_MODE  = 0b11;//AHC
    PWM_DRV_PTR2->DBCTL.bit.OUTSWAP   = 0b11;

    PWM_DRV_PTR2->DBFED.bit.DBFED     = deadtime_cal >> 8;
    PWM_DRV_PTR2->DBRED.bit.DBRED     = deadtime_cal >> 8;
    PWM_DRV_PTR2->DBFEDHR.bit.DBFEDHR = deadtime_cal & 0xff;
    PWM_DRV_PTR2->DBREDHR.bit.DBREDHR = deadtime_cal & 0xff;

    PWM_DRV_PTR2->HRCNFG2.bit.CTLMODEDBFED = 0;
    PWM_DRV_PTR2->HRCNFG2.bit.CTLMODEDBRED = 0;
    PWM_DRV_PTR2->HRCNFG2.bit.EDGMODEDB = 0b11;

//PWM+2
    PWM_DRV_PTR3->TBCTL.bit.FREE_SOFT = 0b10;
    PWM_DRV_PTR3->TBCTL.bit.PHSDIR    = 1;//up after syn
    PWM_DRV_PTR3->TBCTL.bit.CLKDIV    = 0;
    PWM_DRV_PTR3->TBCTL.bit.HSPCLKDIV = 0;//no divide
    PWM_DRV_PTR3->TBCTL.bit.SYNCOSEL  = 0;//syn out when CTR=0
    PWM_DRV_PTR3->TBCTL.bit.PRDLD     = 0;//shadow mode
    PWM_DRV_PTR3->TBCTL.bit.PHSEN     = 1;//epwmno+1 is slaver
    PWM_DRV_PTR3->TBCTL.bit.CTRMODE   = 0;//up count

    PWM_DRV_PTR3->TBPRD               = (PRD_cal >> 9)-1;

    PWM_DRV_PTR3->TBPHS.bit.TBPHS     = 0;
    PWM_DRV_PTR3->TRREM.bit.TRREM     = 0;

    PWM_DRV_PTR3->HRPCTL.bit.PWMSYNCSEL = 1;

//  PWM + 1 as pcmc
    (*(volatile Uint32*)(0x7A00+0x20)) = (1<<(2*(target->CMPSS_PCMC_OBJ_P_INS->port)));
    PWM_DRV_PTR2->DCFCTL.bit.PULSESEL  = 2;//Blanking for both CTR = 0 and CTR = PRD
    PWM_DRV_PTR2->DCFCTL.bit.BLANKE    = 1;//Blanking Enable
//    PWM_DRV_PTR2->DCFCTL.bit.BLANKE    = 0;
    PWM_DRV_PTR2->DCFCTL.bit.SRCSEL    = 0b01;
    PWM_DRV_PTR2->DCFWINDOW            = target->PWM_DRV_OBJ_P_INS->blankwin;
    PWM_DRV_PTR2->DCTRIPSEL.bit.DCAHCOMPSEL = 0b0011;//DCAH Trip Select: 0011 = TRIPIN4
    PWM_DRV_PTR2->DCACTL.bit.EVT2SRCSEL= 1;//DCAEVT2 Source Signal Select: 1= DCEVTFILT Signal, 0 = DCAEVT2;
    PWM_DRV_PTR2->DCACTL.bit.EVT2FRCSYNCSEL = 1;//FRC Synchro
    PWM_DRV_PTR2->TZDCSEL.bit.DCAEVT2  = 0b010;//DCAH high

    switch(target->PWM_DRV_OBJ_P_INS->socport)
    {
    case 1:
        PWM_DRV_PTR1->ETSEL.bit.SOCAEN      = 1;//enable soc
        PWM_DRV_PTR1->ETSEL.bit.SOCASEL     = 2;//soc = TBCTR=PRD
        PWM_DRV_PTR1->ETPS.bit.SOCAPRD      = 1;//per period
        break;
    case 2:
        PWM_DRV_PTR2->ETSEL.bit.SOCAEN      = 1;//enable soc
        PWM_DRV_PTR2->ETSEL.bit.SOCASEL     = 2;//soc = TBCTR=PRD
        PWM_DRV_PTR2->ETPS.bit.SOCAPRD      = 1;//per period
        break;
    default:
        break;
    }
    EDIS;

    EALLOW;
           CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
           PWM_DRV_PTR1->TBCTL.bit.SWFSYNC = 1;
           PWM_DRV_PTR2->TBCTL.bit.SWFSYNC = 1;
           PWM_DRV_PTR3->TBCTL.bit.SWFSYNC = 1;
    EDIS;
}
