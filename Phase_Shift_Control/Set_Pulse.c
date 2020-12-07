#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"



void PULSE_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    Uint16 prescale_count;

    EALLOW;
    //TBCLK = EPWMCLK/(HSPCLKDIV * CLKDIV)
    PULSE_GEN_PTR->TBCTL.bit.CLKDIV    = target->PCP_PULSEGEN_OBJ_P_INS->ClkDiv;
    PULSE_GEN_PTR->TBCTL.bit.HSPCLKDIV = target->PCP_PULSEGEN_OBJ_P_INS->HClkDiv; // divide by 2
    PULSE_GEN_PTR->TBCTL.bit.CTRMODE   = 0;
    prescale_count                     = (Uint16)(McuClk/(2*(1<<target->PCP_PULSEGEN_OBJ_P_INS->ClkDiv)*target->PCP_PULSEGEN_OBJ_P_INS->HClkDiv*target->PCP_PULSEGEN_OBJ_P_INS->Pulse_Freq));
    PULSE_GEN_PTR->TBPRD               = prescale_count;
    PULSE_GEN_PTR->CMPB.bit.CMPB       = (Uint16)(prescale_count * (1 - target->PCP_PULSEGEN_OBJ_P_INS->Pulse_Duty));
    PULSE_GEN_PTR->CMPA.bit.CMPA       = (Uint16)(prescale_count * target->PCP_PULSEGEN_OBJ_P_INS->Trigger_Time);
    PULSE_GEN_PTR->ETSEL.bit.INTSEL    = 0b100;//Select CMPA as Interupt Source
    PULSE_GEN_PTR->ETPS.bit.INTPRD     = 1;
    PULSE_GEN_PTR->ETSEL.bit.INTEN     = 1;
    EDIS;
}


