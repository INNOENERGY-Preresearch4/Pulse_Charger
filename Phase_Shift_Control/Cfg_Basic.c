#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void PWM_DAC_CFG(PCP_Driver_OBJ_p OBJ,Uint16 port1,Uint16 port2,float32 out1,float32 out2,float32 out3,float32 out4)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    target->PWM_DAC_OBJ_P_INS->port1   = port1-1;
    target->PWM_DAC_OBJ_P_INS->port2   = port2-1;
    target->PWM_DAC_OBJ_P_INS->DACout1 = out1;
    target->PWM_DAC_OBJ_P_INS->DACout2 = out2;
    target->PWM_DAC_OBJ_P_INS->DACout3 = out3;
    target->PWM_DAC_OBJ_P_INS->DACout4 = out4;
    target->PWM_DAC_OBJ_P_INS->address1= 0x4000 + (port1 - 1)*0x100;
    target->PWM_DAC_OBJ_P_INS->address2= 0x4000 + (port2 - 1)*0x100;
}

void PWM_DRV_CFG(PCP_Driver_OBJ_p OBJ,Uint16 mode,Uint16 port1, Uint16 port2, Uint16 port3, Uint16 socport, Uint16 fkhz, Uint16 dbtime, Uint16 blankwin)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ*)OBJ;
    target->PWM_DRV_OBJ_P_INS->mode    = mode;
    target->PWM_DRV_OBJ_P_INS->port1    = port1 - 1;
    target->PWM_DRV_OBJ_P_INS->port2    = port2 - 1;
    target->PWM_DRV_OBJ_P_INS->port3    = port3 - 1;
    target->PWM_DRV_OBJ_P_INS->socport = socport;
    target->PWM_DRV_OBJ_P_INS->fkhz    = fkhz;
    target->PWM_DRV_OBJ_P_INS->dbtime  = dbtime;
    target->PWM_DRV_OBJ_P_INS->blankwin= blankwin;
    target->PWM_DRV_OBJ_P_INS->address1= 0x4000 + (port1 - 1)*0x100;
    target->PWM_DRV_OBJ_P_INS->address2= 0x4000 + (port2 - 1)*0x100;
    target->PWM_DRV_OBJ_P_INS->address3= 0x4000 + (port3 - 1)*0x100;
}

void CMPSS_CFG(PCP_Driver_OBJ_p OBJ,Uint16* port,Uint16* group,Uint16* source,Uint16* filter_win,Uint16* filter_vote,Uint16* protect_value,Uint16 length)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    target->CMPSS_OBJ_P_INS->length         = length;
    Uint16 num;
    for(num=0;num<target->CMPSS_OBJ_P_INS->length;num++)
    {
        target->CMPSS_OBJ_P_INS->port[num]          = port[num]-1;
        target->CMPSS_OBJ_P_INS->group[num]         = group[num];
        target->CMPSS_OBJ_P_INS->source[num]        = source[num];
        target->CMPSS_OBJ_P_INS->filter_win[num]    = filter_win[num];
        target->CMPSS_OBJ_P_INS->filter_vote[num]   = filter_vote[num];
        target->CMPSS_OBJ_P_INS->protect_value[num] = protect_value[num];
        target->CMPSS_OBJ_P_INS->address[num]       = 0x5C80 + (port[num]-1)* 0x20;
    }
}

void SCI_CFG(PCP_Driver_OBJ_p OBJ,Uint16 port,Uint32 lspclk,Uint32 baudrate)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    target->SCI_DRV_OBJ_P_INS->port     = port;
    target->SCI_DRV_OBJ_P_INS->lspclk   = lspclk;
    target->SCI_DRV_OBJ_P_INS->baudrate = baudrate;
    target->SCI_DRV_OBJ_P_INS->address  = 0x7200 + (port - 1)* 0x10;
}

void CMPSS_PCMC_CFG(PCP_Driver_OBJ_p OBJ,Uint16 port,Uint16 source,Uint16 rampsource,Uint16 filter_win,Uint16 filter_vote,float32 Itrip,float32 Icompramp)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    target->CMPSS_PCMC_OBJ_P_INS->port           = port-1;
    target->CMPSS_PCMC_OBJ_P_INS->source         = source;
    target->CMPSS_PCMC_OBJ_P_INS->rampsource     = rampsource-1;
    target->CMPSS_PCMC_OBJ_P_INS->filter_win     = filter_win;
    target->CMPSS_PCMC_OBJ_P_INS->filter_vote    = filter_vote;
    target->CMPSS_PCMC_OBJ_P_INS->Itrip          = Itrip;
    target->CMPSS_PCMC_OBJ_P_INS->Icompramp      = Icompramp;
    target->CMPSS_PCMC_OBJ_P_INS->address        = 0x5C80 + (port-1)* 0x20;
}

void ADC_DRV_CFG(PCP_Driver_OBJ_p OBJ,Uint16 *port,Uint32* soc,Uint32* source,Uint32* acqps,Uint32* trig,Uint16 length)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    target->ADC_DRV_OBJ_P_INS->length = length;
    Uint16 num;
    for(num=0;num<target->ADC_DRV_OBJ_P_INS->length;num++)
    {
        target->ADC_DRV_OBJ_P_INS->port[num]   = port[num];
        target->ADC_DRV_OBJ_P_INS->soc[num]    = soc[num];
        target->ADC_DRV_OBJ_P_INS->source[num] = source[num];
        target->ADC_DRV_OBJ_P_INS->acqps[num]  = acqps[num];
        target->ADC_DRV_OBJ_P_INS->trig[num]   = trig[num];
        //ADCARegs: 0x7400, ADCBRegs: 0x7480, ADCCRegs: 0x7480
        target->ADC_DRV_OBJ_P_INS->address[num]= 0x7400 + (target->ADC_DRV_OBJ_P_INS->port[num]) * 0x80;
        //ADCSOCxCTL :0x10h + 0x2*N
        if(port[num] == ADCA)
            target->ADC_DRV_OBJ_P_INS->socadd[num] = (0x7410 + (target->ADC_DRV_OBJ_P_INS->soc[num] ) * 0x2);
        if(port[num] == ADCB)
            target->ADC_DRV_OBJ_P_INS->socadd[num] = (0x7490 + (target->ADC_DRV_OBJ_P_INS->soc[num] ) * 0x2);
    }
}

void CLA_DRV_CFG(PCP_Driver_OBJ_p OBJ,Uint16* task,Uint16* source,Uint16* iack,Uint16 length)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    target->CLA_DRV_OBJ_P_INS->length = length;
    Uint16 num;
    for(num=0;num<target->CLA_DRV_OBJ_P_INS->length;num++)
    {
        target->CLA_DRV_OBJ_P_INS->task[num]   = task[num];
        target->CLA_DRV_OBJ_P_INS->source[num] = source[num];
        target->CLA_DRV_OBJ_P_INS->iack[num]   = iack[num];
        target->CLA_DRV_OBJ_P_INS->address[num]= 0x1400 + (task[num]-1);
    }
}

void IO_CFG(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    target->IO_PACK_OBJ_P_INS->FLT1_p->portname      = 11;
    target->IO_PACK_OBJ_P_INS->FLT2_p->portname      = 26;
    target->IO_PACK_OBJ_P_INS->FLT3_p->portname      = 23;
    target->IO_PACK_OBJ_P_INS->FLT4_p->portname      = 10;
    target->IO_PACK_OBJ_P_INS->PSV_p->portname       = 40;
    target->IO_PACK_OBJ_P_INS->PSI_p->portname       = 57;
    target->IO_PACK_OBJ_P_INS->PPV_p->portname       = 34;
    target->IO_PACK_OBJ_P_INS->OIN1_p->portname      = 33;
//    target->IO_PACK_OBJ_P_INS->OIN2_p->portname      = 26;
    target->IO_PACK_OBJ_P_INS->PWM_en_p->portname    = 24;
    target->IO_PACK_OBJ_P_INS->PWM_rst1_p->portname  = 25;
    target->IO_PACK_OBJ_P_INS->PWM_rst2_p->portname  = 39;
    target->IO_PACK_OBJ_P_INS->Relay_in_p->portname  = 27;
    target->IO_PACK_OBJ_P_INS->Relay_out_p->portname = 58;
    target->IO_PACK_OBJ_P_INS->OOUT1_p->portname     = 59;
    target->IO_PACK_OBJ_P_INS->OOUT2_p->portname     = 56;
    target->IO_PACK_OBJ_P_INS->Twinkle_p->portname   = 7;

    target->IO_PACK_OBJ_P_INS->FLT1_p->portdir       = 0;
    target->IO_PACK_OBJ_P_INS->FLT2_p->portdir       = 0;
    target->IO_PACK_OBJ_P_INS->FLT3_p->portdir       = 0;
    target->IO_PACK_OBJ_P_INS->FLT4_p->portdir       = 0;
    target->IO_PACK_OBJ_P_INS->PSV_p->portdir        = 0;
    target->IO_PACK_OBJ_P_INS->PSI_p->portdir        = 0;
    target->IO_PACK_OBJ_P_INS->PPV_p->portdir        = 0;
    target->IO_PACK_OBJ_P_INS->OIN1_p->portdir       = 0;
//    target->IO_PACK_OBJ_P_INS->OIN2_p->portdir       = 0;
    target->IO_PACK_OBJ_P_INS->PWM_en_p->portdir     = 1;
    target->IO_PACK_OBJ_P_INS->PWM_rst1_p->portdir   = 1;
    target->IO_PACK_OBJ_P_INS->PWM_rst2_p->portdir   = 1;
    target->IO_PACK_OBJ_P_INS->Relay_in_p->portdir   = 1;
    target->IO_PACK_OBJ_P_INS->Relay_out_p->portdir  = 1;
    target->IO_PACK_OBJ_P_INS->OOUT1_p->portdir      = 1;
    target->IO_PACK_OBJ_P_INS->OOUT2_p->portdir      = 1;
    target->IO_PACK_OBJ_P_INS->Twinkle_p->portdir    = 1;

    target->IO_PACK_OBJ_P_INS->FLT1_p->portdata      = 0;
    target->IO_PACK_OBJ_P_INS->FLT2_p->portdata      = 0;
    target->IO_PACK_OBJ_P_INS->FLT3_p->portdata      = 0;
    target->IO_PACK_OBJ_P_INS->FLT4_p->portdata      = 0;
    target->IO_PACK_OBJ_P_INS->PSV_p->portdata       = 0;
    target->IO_PACK_OBJ_P_INS->PSI_p->portdata       = 0;
    target->IO_PACK_OBJ_P_INS->PPV_p->portdata       = 0;
    target->IO_PACK_OBJ_P_INS->OIN1_p->portdata      = 0;
//    target->IO_PACK_OBJ_P_INS->OIN2_p->portdata      = 0;
    target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata    = 0;
    target->IO_PACK_OBJ_P_INS->PWM_rst1_p->portdata  = 0;
    target->IO_PACK_OBJ_P_INS->PWM_rst2_p->portdata  = 0;
    target->IO_PACK_OBJ_P_INS->Relay_in_p->portdata  = 0;
    target->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = 0;
    target->IO_PACK_OBJ_P_INS->OOUT1_p->portdata     = 0;
    target->IO_PACK_OBJ_P_INS->OOUT2_p->portdata     = 0;
    target->IO_PACK_OBJ_P_INS->Twinkle_p->portdata   = 0;
}

void PULSE_GEN_CFG(PCP_Driver_OBJ_p OBJ,Uint16 clkdiv,Uint16 hclkdiv,Uint16 pulsefreq, float32 pulseduty, float32 triggertime,Uint16 pwmno)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    target->PCP_PULSEGEN_OBJ_P_INS->ClkDiv       = clkdiv;
    target->PCP_PULSEGEN_OBJ_P_INS->HClkDiv      = hclkdiv;
    target->PCP_PULSEGEN_OBJ_P_INS->Pulse_Freq    = pulsefreq;
    target->PCP_PULSEGEN_OBJ_P_INS->Pulse_Duty    = pulseduty;
    target->PCP_PULSEGEN_OBJ_P_INS->Trigger_Time = triggertime;
    target->PCP_PULSEGEN_OBJ_P_INS->PWM_No       = pwmno;
    target->PCP_PULSEGEN_OBJ_P_INS->address      = 0x4000 + 0x100*(pwmno-1);
}

void InitEpwm1Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;

    EDIS;
}

void InitEpwm2Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;

    EDIS;
}

void InitEpwm3Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;

    EDIS;
}

void InitEpwm4Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;

    EDIS;
}

void InitEpwm5Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;

    EDIS;
}

void InitEpwm6Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;

    EDIS;
}

void InitEpwm7Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;

    EDIS;
}

void InitEpwm8Gpio(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;

    EDIS;
}
