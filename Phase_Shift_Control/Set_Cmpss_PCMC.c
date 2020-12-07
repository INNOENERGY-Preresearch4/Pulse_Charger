#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void CMPSS_PCMC_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ*)OBJ;
    EALLOW;
            if(target->CMPSS_PCMC_OBJ_P_INS->port<=5)
                AnalogSubsysRegs.CMPHPMXSEL.all          |= ((Uint32)target->CMPSS_PCMC_OBJ_P_INS->source)<<(3*target->CMPSS_PCMC_OBJ_P_INS->port);
            else
                AnalogSubsysRegs.CMPHPMXSEL.all          |= ((Uint32)target->CMPSS_PCMC_OBJ_P_INS->source)<<((3*target->CMPSS_PCMC_OBJ_P_INS->port)+1);

            CMPSS_PCMC_PTR->COMPCTL.bit.COMPDACE     = 1;//enable
            CMPSS_PCMC_PTR->COMPCTL.bit.ASYNCHEN     = 0;
            CMPSS_PCMC_PTR->COMPCTL.bit.CTRIPOUTHSEL = 2;//digital filter to gpio
            CMPSS_PCMC_PTR->COMPCTL.bit.CTRIPHSEL    = 2;//digital filter to pwm,deliver COMPSTS to CTRIPH, then to TIRPIN4
            CMPSS_PCMC_PTR->COMPCTL.bit.COMPHINV     = 0;//non inverter
            CMPSS_PCMC_PTR->COMPCTL.bit.COMPHSOURCE  = 0;//drived by DAC

            CMPSS_PCMC_PTR->COMPHYSCTL.bit.COMPHYS   = 1;//1* hysteresis


            CMPSS_PCMC_PTR->COMPDACCTL.bit.RAMPLOADSEL = 0;//shadow load RAMPSTS from RAMPMAXREFS
            CMPSS_PCMC_PTR->COMPDACCTL.bit.SELREF    = 0;//DAC's ref is VDDA
            CMPSS_PCMC_PTR->COMPDACCTL.bit.RAMPSOURCE= target->CMPSS_PCMC_OBJ_P_INS->rampsource;//syn on EPWM3
            CMPSS_PCMC_PTR->COMPDACCTL.bit.DACSOURCE = 1;//use RAMPGEN
            CMPSS_PCMC_PTR->COMPDACCTL.bit.SWLOADSEL = 1;
            CMPSS_PCMC_PTR->RAMPMAXREFS              = (Uint16)(65520*target->CMPSS_PCMC_OBJ_P_INS->Itrip);
            CMPSS_PCMC_PTR->RAMPDECVALS              = (Uint16)(65520/500.0*target->CMPSS_PCMC_OBJ_P_INS->Icompramp);

            CMPSS_PCMC_PTR->COMPDACCTL.bit.BLANKEN   = 1;
            CMPSS_PCMC_PTR->COMPDACCTL.bit.BLANKSOURCE=target->PWM_DRV_OBJ_P_INS->port2;

            CMPSS_PCMC_PTR->CTRIPHFILCTL.bit.SAMPWIN = target->CMPSS_PCMC_OBJ_P_INS->filter_win;
            CMPSS_PCMC_PTR->CTRIPHFILCTL.bit.THRESH  = target->CMPSS_PCMC_OBJ_P_INS->filter_vote;
            CMPSS_PCMC_PTR->CTRIPHFILCLKCTL.bit.CLKPRESCALE = 0;
            CMPSS_PCMC_PTR->CTRIPHFILCTL.bit.FILINIT = 1;//initialize sample values

            CMPSS_PCMC_PTR->COMPSTSCLR.bit.HLATCHCLR = 1;//reset comp sts
            CMPSS_PCMC_PTR->COMPSTSCLR.bit.HSYNCCLREN= 1;
    EDIS;
}
