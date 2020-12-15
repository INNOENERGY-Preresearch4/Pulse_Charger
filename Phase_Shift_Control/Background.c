#include "Global.h"
#include "Cla_Interrupts.h"
#include "background.h"
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "user_def.h"
#include "emavg.h"

extern void(*backgroundpfa[4])(void)={backgroundfa1,backgroundfa2,backgroundfa3,backgroundfa4};
extern void(*backgroundpfb[4])(void)={backgroundfb1,backgroundfb2,backgroundfb3,backgroundfb4};
extern void(*backgroundpfc[4])(void)={backgroundfc1,backgroundfc2,backgroundfc3,backgroundfc4};
Uint16 fva=0,fvb=0,fvc=0;
Uint16 BeatCount=0;
float Vin;
Uint16 In_Relay_flag;
Uint16 otflag=0;
EMAVG emavgTemp;
EMAVG emavgVin;
EMAVG emavgVo;
EMAVG emavgIo;

float temp_emavg,vin_emavg,vo_emavg,io_emavg;

//CpuTimer0Regs.TCR.bit.TIF gets set when the CPU-timer decrements to zero
//Writing a 1 to this bit clears the flag

//Run 4 background functions sequentially at 1-millisecond period (4ms in total): backgroundfa1,backgroundfa2,backgroundfa3,backgroundfa4
void backgroundfa(void)
{
    if(CpuTimer0Regs.TCR.bit.TIF == 1)
    {
        CpuTimer0Regs.TCR.bit.TIF = 1;
        backgroundpfa[fva++]();
        if(fva >= 4) fva = 0;
    }

}

//Run 4 background functions sequentially at 10-millisecond period (40ms in total): backgroundfb1,backgroundfb2,backgroundfb3,backgroundfb4
void backgroundfb(void)
{
    if(CpuTimer1Regs.TCR.bit.TIF == 1)
    {
        CpuTimer1Regs.TCR.bit.TIF = 1;
        backgroundpfb[fvb++]();
        if(fvb >= 4) fvb = 0;
    }

}

//Run 4 background functions sequentially at 100-millisecond period (400ms in total): backgroundfc1,backgroundfc2,backgroundfc3,backgroundfc4
void backgroundfc(void)
{
    if(CpuTimer2Regs.TCR.bit.TIF == 1)
    {
        CpuTimer2Regs.TCR.bit.TIF = 1;
        backgroundpfc[fvc++]();
        if(fvc >= 4) fvc = 0;
    }
}

//4ms background function 1
//Calibrate Sample Value of Vin
//If Vin > 40V, Enable Input Relay
//disable input and output relay if any of protections were triggered.
void backgroundfa1(void)
{
    Vin = AdcbResultRegs.ADCRESULT4*V_scaling/ADC_Cnt_Max;
    if(Vin >= In_Relay_Thrd)
    {
        PCP_P_handle->IO_PACK_OBJ_P_INS->Relay_in_p->portdata = 1;
    }

    In_Relay_flag = XbarRegs.XBARFLG2.bit.INPUT3 || XbarRegs.XBARFLG2.bit.INPUT2 || Cmpss1Regs.COMPSTS.bit.COMPLLATCH || Cmpss1Regs.COMPSTS.bit.COMPHLATCH || Cmpss2Regs.COMPSTS.bit.COMPHLATCH;

    if (In_Relay_flag == 1)
    {
    PCP_P_handle->IO_PACK_OBJ_P_INS->Relay_in_p->portdata = 0;
    PCP_P_handle->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = 0;
    }
}

//4ms background function 2
//Calibrate Sample Value of Temperature, Vin, Vo (Io in Cla_Fun.asm : Cla_Avg_Run)
//Save Protection Flags for Display
void backgroundfa2(void)
{
    temp_emavg = AdcbResultRegs.ADCRESULT3*0.03533+7.193;
    EMAVG_run(&emavgTemp, temp_emavg);
    PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_dis_data = emavgTemp.out;
    vin_emavg  = AdcbResultRegs.ADCRESULT4*RX_SC_General/ADC_Cnt_Max;
    EMAVG_run(&emavgVin, vin_emavg);
    PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->Vin_dis_data = (Uint16)emavgVin.out;
//    vo_emavg = AdcaResultRegs.ADCRESULT2*17.031998*;
    vo_emavg = AdcaResultRegs.ADCRESULT2*RX_SC_General/ADC_Cnt_Max;
    EMAVG_run(&emavgVo, vo_emavg);
    PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_dis_data = (Uint16)emavgVo.out;
    PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->Io_dis_data = (Uint16)io;

    PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->Prt_Status_H = 0;
    PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->Prt_Status_L = XbarRegs.XBARFLG2.bit.INPUT3\
            +(XbarRegs.XBARFLG2.bit.INPUT2 << 1)+(0 << 2)\
            +(0 << 3)+(Cmpss1Regs.COMPSTS.bit.COMPLLATCH << 4)\
            +(Cmpss1Regs.COMPSTS.bit.COMPHLATCH << 5)+(Cmpss2Regs.COMPSTS.bit.COMPHLATCH << 6)\
            +(otflag << 7);
}

//4ms background function 3
//Update(Write)/Save(Read) GPIO States
void backgroundfa3(void)
{
    IO_HANDLE_FUN(PCP_P_handle);
}

//1ms background function 4
void backgroundfa4(void)
{

}

//40ms background function 1

void backgroundfb1(void)
{
    //Transmit Saved Values and Flags for Display
    SCI_HANDLE_FUN(PCP_P_handle);
    //State Machine to identify Operating Mode
    SM_FUN(PCP_P_handle);
}

//40ms background function 2
//Update PWM duty for Hardware Protection
void backgroundfb2(void)
{
}

//40ms background function 3
//Update Reference Value of Output Current (not used).
void backgroundfb3(void)
{

}

void backgroundfb4(void)
{
    //NotifyLight, speed up with increasing temperature
    if (BeatCount >= (12 - PCP_P_handle->PCP_DATAEXCHANGE_OBJ_P_INS->T_dis_data/10))
    {
        BeatCount = 0;
        PCP_P_handle->IO_PACK_OBJ_P_INS->Twinkle_p->portdata   = (1 - PCP_P_handle->IO_PACK_OBJ_P_INS->Twinkle_p->portdata);
    }
    else
    {
        BeatCount += 1;
    }
}
//400ms background function 1
void backgroundfc1(void)
{
    //PI update function
//    PI_Update(PCP_P_handle);
    //Reference current update function
    Iref_Update(PCP_P_handle);
    //Pulse Parameters(Duty/Frequency/)
    Pulse_Update(PCP_P_handle);
    //Software protection threshold update(Iin/Io/Vin/Vo)
    Soft_Prt_Update(PCP_P_handle);
    //Software protection threshold update(T)
    Soft_LSpd_Prt_Update(PCP_P_handle);

}

//400ms background function 2
void backgroundfc2(void)
{

}

//400ms background function 3
void backgroundfc3(void)
{

}

//400ms background function 4
//SFO(Scale Factor Optimizer Function) is used for driving the micro-edge positioner (MEP) calibration module to run SFO diagnostics and determine the appropriate MEP scale factor
//Pg.1927
void backgroundfc4(void)
{
    SFO_status = SFO_Running;//cal HRPWM scale
    while(SFO_status == SFO_Running)
    {
        SFO_status = SFO();
        if(SFO_status == SFO_Error)
        {
            ESTOP0;
        }
    }
}
