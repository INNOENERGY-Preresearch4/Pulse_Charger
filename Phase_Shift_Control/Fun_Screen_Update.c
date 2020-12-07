#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

float PulseDuty;
float PulseVally;
float PulseFreq;

void PI_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->CPa_data != 0)
    {
    Kp = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->CPa_data)/65535.0f/100.0f;
    Ki = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->CIa_data)/65535.0f*10.0f;
    }
    else
    {
    Kp = (float)Kp_Init;
    Ki = (float)Ki_Init;
    }
}

void Iref_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_ref_data != 0)
    {
    iref = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_ref_data)/65535.0f*50.0f;
    }
    else
    {
    iref = 0.0f;
    }
}

void Pulse_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 prescale_count;

    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->fre_set_data < (Uint16)(PulseFreqMin*65535.0f))
    {
        PulseFreq = PulseFreqMin*10000.0f;
    }
    else
    {
        PulseFreq = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->fre_set_data)*10000.0f/65535.0f;
    }
    Tpulse = (float)(1.0f/PulseFreq);
    prescale_count                     = (Uint16)(McuClk/(((1<<target->PCP_PULSEGEN_OBJ_P_INS->ClkDiv)*2*target->PCP_PULSEGEN_OBJ_P_INS->HClkDiv*PulseFreq)));
    PULSE_GEN_PTR->TBPRD               = prescale_count;
    PULSE_GEN_PTR->CMPA.bit.CMPA       = (Uint16)(prescale_count * target->PCP_PULSEGEN_OBJ_P_INS->Trigger_Time);

    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data == 0)
    {
    PulseDuty = 0.0f;
    }
    else if (target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data < (Uint16)(0.99f*65535.0f) )
    {
    PulseDuty = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data)/65535.0f;
    }
    else
    {
    PulseDuty = 1.0f;
    }

    if (PulseDuty < 1.0f)
    {
    PulseVally = 1.0f - PulseDuty;
    PULSE_GEN_PTR->CMPB.bit.CMPB       = (Uint16)(prescale_count * PulseVally);
    }
    else
    {
    PULSE_GEN_PTR->CMPB.bit.CMPB       = 0;
    }
}

void Soft_LSpd_Prt_Update(PCP_Driver_OBJ_p OBJ)
{

    if(PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data < T_Launch_Prt )
            PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data = T_Launch;
    else if(PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data < T_Prt_Min )
        PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data = T_Prt_Min;
    else if(PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data > T_Prt_Max)
        PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data = T_Prt_Max;
    else {}


    if(PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_dis_data > PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data || Prt_flag == 1 )
    {
        EALLOW;
        EPwm2Regs.TZFRC.bit.OST = 1;
        EPwm1Regs.TZFRC.bit.OST = 1;
        EDIS;

        PCP_P_handle->IO_PACK_OBJ_P_INS->Relay_in_p->portdata = 0;
        PCP_P_handle->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = 0;
        Prt_flag = 1;
        otflag = 1;
    }
    else
    {
        Prt_flag = 0;
        //otflag = 0;
    }


}

void Soft_Prt_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 num;

    if ((target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data >= 1.0f/60.0f*65535.0f) && (target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data <= 60.0f/60.0f*65535.0f))
    {
    target->CMPSS_OBJ_P_INS->protect_value[0] = (Uint16)(target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data*65.45f*60.0f/65535.0f);
    }

    if ((target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data >= 1.0f/60.0f*65535.0f) && (target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data <= 60.0f/60.0f*65535.0f))
    {
    target->CMPSS_OBJ_P_INS->protect_value[1] = (Uint16)(target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data*105.90f*60.0f/65535.0f);
    }

    if ((target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data >= 1.0f/60.0f*65535.0f) && (target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data <= 60.0f/60.0f*65535.0f))
    {
    target->CMPSS_OBJ_P_INS->protect_value[2] = (Uint16)(target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data*67.88f*60.0f/65535.0f);
    }

    EALLOW;
    for(num=0;num<target->CMPSS_OBJ_P_INS->length;num++)
    {
        //0x12h DACLVALS (Low DAC Value Shadow Register)
        //0x06h DACHVALS (High DAC Value Shadow Register)(0xC may be wrong)
        (*(volatile Uint32*)(target->CMPSS_OBJ_P_INS->address[num]+0x12-(0xC*target->CMPSS_OBJ_P_INS->group[num]))) = (Uint16)(target->CMPSS_OBJ_P_INS->protect_value[num]);
    }
    EDIS;
}
