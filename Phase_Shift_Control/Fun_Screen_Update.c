#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

float PulseDuty;
float PulseVally;
float PulseFreq;

//Update Kp and Ki for PI controller
void PI_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->CPa_data != 0)
    {
    Kp = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->CPa_data)/RX_SC_General*Kp_Scaling;
    Ki = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->CIa_data)/RX_SC_General*Ki_Scaling;
    }
    else
    {
    Kp = (float)Kp_Init;
    Ki = (float)Ki_Init;
    }
}

//Update reference output current (pulse high)
void Iref_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_ref_data != 0)
    {
    iref = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_ref_data)/RX_SC_General*Iref_Scaling;
    }
    else
    {
    iref = 0.0f;
    }
}

//Update duty cycle, frequency for pulse current
void Pulse_Update(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 prescale_count;

    //pulse frequency saturation and scaling, PulseFreqMin = 0.001 -> 10Hz£¬ 1¡£0 -> 10kHz
    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->fre_set_data < (Uint16)(PulseFreqMin*RX_SC_Freq))
    {
        PulseFreq = PulseFreqMin*PulseFreq_Scaling;
    }
    else
    {
        PulseFreq = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->fre_set_data)/RX_SC_Freq*PulseFreq_Scaling;
    }
    //pulse period calculation
    Tpulse = (float)(1.0f/PulseFreq);
    //calculate MCUCLK counts for one pulse period
    prescale_count                     = (Uint16)(McuClk/(((1<<target->PCP_PULSEGEN_OBJ_P_INS->ClkDiv)*2*target->PCP_PULSEGEN_OBJ_P_INS->HClkDiv*PulseFreq)));
    PULSE_GEN_PTR->TBPRD               = prescale_count;
    //calculate MCUCLK counts for triggering CLA
    PULSE_GEN_PTR->CMPA.bit.CMPA       = (Uint16)(prescale_count * target->PCP_PULSEGEN_OBJ_P_INS->Trigger_Time);
    //pulse duty cycle saturation and scaling
    if (target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data == 0)
    {
    PulseDuty = 0.0f;
    }
    else if (target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data < (Uint16)(PulseDuty_Max*RX_SC_General) )
    {
    PulseDuty = ((float)target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data)/RX_SC_General;
    }
    else
    {
    PulseDuty = 1.0f;
    }
    //scaling and calculate MCUCLK counts for "PulseOff"
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
// low speed protection
void Soft_LSpd_Prt_Update(PCP_Driver_OBJ_p OBJ)
{
    //Over Temperature protection update
    if(PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data < T_Prt_Min )
        PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data = T_Prt_Min;
    else if(PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data > T_Prt_Max)
        PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data = T_Prt_Max;
    else {}
    //Execute Protection
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


    if ((target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data >= 1.0f/Tmt_scaling*RX_SC_General) && (target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data <= RX_SC_General))
    {
    target->CMPSS_OBJ_P_INS->protect_value[0] = (Uint16)(target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data*105.90f*Tmt_scaling/RX_SC_General);
    }

    if ((target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data >= 1.0f/Tmt_scaling*RX_SC_General) && (target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data <= RX_SC_General))
    {
    target->CMPSS_OBJ_P_INS->protect_value[1] = (Uint16)(target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data*65.45f*Tmt_scaling/RX_SC_General);
    }

    if ((target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data >= 1.0f/Tmt_scaling*RX_SC_General) && (target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data <= RX_SC_General))
    {
    target->CMPSS_OBJ_P_INS->protect_value[2] = (Uint16)(target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data*67.88f*Tmt_scaling/RX_SC_General);
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
