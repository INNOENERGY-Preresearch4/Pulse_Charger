#include "F28x_Project.h"
#include "SFO_V8.h"
#include "user_def.h"
#include "background.h"
#ifndef GLOBAL_H_
#define GLOBAL_H_

#define CMPSS_PTR    ((volatile struct CMPSS_REGS *)target->CMPSS_OBJ_P_INS->address[num])
#define CMPSS_PCMC_PTR    ((volatile struct CMPSS_REGS *)target->CMPSS_PCMC_OBJ_P_INS->address)
#define PWM_DAC_PTR1 ((volatile struct EPWM_REGS *)target->PWM_DAC_OBJ_P_INS->address1)
#define PWM_DAC_PTR2 ((volatile struct EPWM_REGS *)target->PWM_DAC_OBJ_P_INS->address2)
#define PWM_DRV_PTR1 ((volatile struct EPWM_REGS *)target->PWM_DRV_OBJ_P_INS->address1)
#define PWM_DRV_PTR2 ((volatile struct EPWM_REGS *)target->PWM_DRV_OBJ_P_INS->address2)
#define PWM_DRV_PTR3 ((volatile struct EPWM_REGS *)target->PWM_DRV_OBJ_P_INS->address3)
#define ADC_DRV_PTR  ((volatile struct ADC_REGS  *)target->ADC_DRV_OBJ_P_INS->address[num])
#define ADC_SOC_PTR  (*(volatile Uint32*)(target->ADC_DRV_OBJ_P_INS->socadd[num]))
#define CLA_DRV_PTR  (*(volatile Uint16*)(target->CLA_DRV_OBJ_P_INS->address[num]))
#define SCI_DRV_PTR  ((volatile struct SCI_REGS  *)(target->SCI_DRV_OBJ_P_INS->address))
#define PULSE_GEN_PTR ((volatile struct EPWM_REGS *)target->PCP_PULSEGEN_OBJ_P_INS->address)

extern uint32_t _Cla1ProgRunStart, _Cla1ProgLoadStart, _Cla1ProgLoadSize;
extern uint32_t _Cla1ConstRunStart, _Cla1ConstLoadStart, _Cla1ConstLoadSize;
extern Uint16 scitest;

typedef struct Cmpss_Obj
{
    Uint16 port[8];
    Uint16 group[8];
    Uint16 source[8];
    Uint16 filter_win[8];
    Uint16 filter_vote[8];
    Uint16 protect_value[8];
    Uint16 length;
    Uint32 address[8];
}Cmpss_OBJ;
typedef Cmpss_OBJ* Cmpss_Obj_p;

typedef struct Cmpss_Pcmc_Obj
{
    Uint16 port;
    Uint16 source;
    Uint16 filter_win;
    Uint16 filter_vote;
    Uint16 rampsource;
    float32 Itrip;
    float32 Icompramp;
    Uint32 address;
}Cmpss_Pcmc_OBJ;
typedef Cmpss_Pcmc_OBJ* Cmpss_Pcmc_Obj_p;

typedef struct Pwm_Dac_Obj
{
    Uint16  port1;
    Uint16  port2;
    float32 DACout1;
    float32 DACout2;
    float32 DACout3;
    float32 DACout4;
    Uint32  address1;
    Uint32  address2;
}Pwm_Dac_OBJ;
typedef Pwm_Dac_OBJ* Pwm_Dac_Obj_p;

typedef struct Pwm_Drv_Obj
{
    Uint16  mode;
    Uint16  port1;
    Uint16  port2;
    Uint16  port3;
    Uint16  socport;
    Uint16  fkhz;
    Uint16  dbtime;
    Uint16  blankwin;
    Uint32  address1;
    Uint32  address2;
    Uint32  address3;
}Pwm_Drv_OBJ;
typedef Pwm_Drv_OBJ* Pwm_Drv_Obj_p;

typedef struct Adc_Drv_Obj
{
    Uint16 port[8];
    Uint32 soc[8];
    Uint32 source[8];
    Uint32 acqps[8];
    Uint32 trig[8];
    Uint32 address[8];
    Uint32 socadd[8];
    Uint16 length;
}Adc_Drv_OBJ;
typedef Adc_Drv_OBJ* Adc_Drv_Obj_p;

typedef struct Cla_Drv_Obj
{
    Uint16 task[8];
    Uint16 source[8];
    Uint16 iack[8];
    Uint32 address[8];
    Uint16 length;
}Cla_Drv_OBJ;
typedef Cla_Drv_OBJ* Cla_Drv_Obj_p;

typedef struct Sci_Drv_Obj
{
    Uint16 port;
    Uint32 lspclk;
    Uint32 baudrate;
    Uint16 txbuf[12];
    Uint16 rxbuf[12];
    Uint16 txnum;
    Uint16 rxnum;
    Uint32 address;
}Sci_Drv_OBJ;
typedef Sci_Drv_OBJ* Sci_Drv_Obj_p;

typedef struct IO_str_Obj
{
    Uint16 portname;
    Uint16 portdir;
    Uint16 portdata;
}IO_str_OBJ;
typedef IO_str_OBJ* IO_str_Obj_p;

typedef struct IO_Pack_Obj
{
    //in
    IO_str_OBJ FLT1;
    IO_str_OBJ FLT2;
    IO_str_OBJ FLT3;
    IO_str_OBJ FLT4;
    IO_str_OBJ PSV;
    IO_str_OBJ PSI;
    IO_str_OBJ PPV;
    IO_str_OBJ OIN1;
    IO_str_OBJ OIN2;

    IO_str_Obj_p FLT1_p;
    IO_str_Obj_p FLT2_p;
    IO_str_Obj_p FLT3_p;
    IO_str_Obj_p FLT4_p;
    IO_str_Obj_p PSV_p;
    IO_str_Obj_p PSI_p;
    IO_str_Obj_p PPV_p;
    IO_str_Obj_p OIN1_p;
    IO_str_Obj_p OIN2_p;
    //out
    IO_str_OBJ PWM_rst1;
    IO_str_OBJ PWM_rst2;
    IO_str_OBJ PWM_en;
    IO_str_OBJ Relay_out;
    IO_str_OBJ Relay_in;
    IO_str_OBJ OOUT1;
    IO_str_OBJ OOUT2;
    IO_str_OBJ Twinkle;

    IO_str_Obj_p PWM_rst1_p;
    IO_str_Obj_p PWM_rst2_p;
    IO_str_Obj_p PWM_en_p;
    IO_str_Obj_p Relay_out_p;
    IO_str_Obj_p Relay_in_p;
    IO_str_Obj_p OOUT1_p;
    IO_str_Obj_p OOUT2_p;
    IO_str_Obj_p Twinkle_p;

}IO_Pack_OBJ;
typedef IO_Pack_OBJ* IO_Pack_Obj_p;

typedef struct PCP_DataExchange_Obj
{
    Uint16 Io_ref_data;
    Uint16 Vo_ref_data;
    Uint16 Duty_set_data;
    Uint16 Vin_Prt_data;
    Uint16 Vo_Prt_data;
    Uint16 Iin_Prt_data;
    Uint16 Io_Prt_data;
    Uint16 T_prt_data;
    Uint16 HW_Iin_Prt_data;
    Uint16 HW_Io_Prt_data;
    Uint16 CPa_data;
    Uint16 CPb_data;
    Uint16 CIa_data;
    Uint16 CIb_data;
    Uint16 Vin_dis_data;
    Uint16 Vo_dis_data;
    Uint16 Io_dis_data;
    Uint16 T_dis_data;
    Uint16 fre_set_data;
    unsigned char Prt_Status_H;
    unsigned char Prt_Status_L;
}PCP_DataExchange_OBJ;
typedef PCP_DataExchange_OBJ* PCP_DataExchange_Obj_p;

typedef struct PCP_PulseGen_Obj
{
    Uint16 ClkDiv;
    Uint16 HClkDiv;
    Uint16 Pulse_Freq;
    float32 Pulse_Duty;
    float32 Trigger_Time;
    Uint16 PWM_No;
    Uint16 address;
}PCP_PulseGen_OBJ;
typedef PCP_PulseGen_OBJ* PCP_PulseGen_Obj_p;

typedef struct PCP_Driver_Obj
{
    Cmpss_OBJ     CMPSS_OBJ_INS;
    Cmpss_Obj_p   CMPSS_OBJ_P_INS;
    Cmpss_Pcmc_OBJ     CMPSS_PCMC_OBJ_INS;
    Cmpss_Pcmc_Obj_p   CMPSS_PCMC_OBJ_P_INS;
    Pwm_Dac_OBJ   PWM_DAC_OBJ_INS;
    Pwm_Dac_Obj_p PWM_DAC_OBJ_P_INS;
    Pwm_Drv_OBJ   PWM_DRV_OBJ_INS;
    Pwm_Drv_Obj_p PWM_DRV_OBJ_P_INS;
    Adc_Drv_OBJ   ADC_DRV_OBJ_INS;
    Adc_Drv_Obj_p ADC_DRV_OBJ_P_INS;
    Cla_Drv_OBJ   CLA_DRV_OBJ_INS;
    Cla_Drv_Obj_p CLA_DRV_OBJ_P_INS;
    IO_Pack_OBJ   IO_PACK_OBJ_INS;
    IO_Pack_Obj_p IO_PACK_OBJ_P_INS;
    Sci_Drv_OBJ   SCI_DRV_OBJ_INS;
    Sci_Drv_Obj_p SCI_DRV_OBJ_P_INS;
    PCP_DataExchange_OBJ PCP_DATAEXCHANGE_OBJ_INS;
    PCP_DataExchange_Obj_p PCP_DATAEXCHANGE_OBJ_P_INS;
    PCP_PulseGen_OBJ   PCP_PULSEGEN_OBJ_INS;
    PCP_PulseGen_Obj_p PCP_PULSEGEN_OBJ_P_INS;
    void (*backgroundfun[3])(void);

}PCP_Driver_OBJ;
typedef PCP_Driver_OBJ* PCP_Driver_OBJ_p;


extern PCP_Driver_OBJ   PCP_handle;
extern PCP_Driver_OBJ_p PCP_P_handle;

extern int MEP_ScaleFactor;
extern Uint16 SFO_status;

extern Uint16 SM_StartupMSG_Flg;
extern Uint16 SM_ShutdownMSG_Flg;

extern Uint16 scitest;

extern Uint16 fa,fva,fvb,fvc;

extern uint32_t Cla1ProgRunStart,Cla1ProgLoadStart,Cla1ProgLoadSize;
extern uint32_t Cla1ConstRunStart,Cla1ConstLoadStart,Cla1ConstLoadSize;

extern float PulseDuty;

extern PCP_Driver_OBJ_p PCP_INIT(void *PCP_OBJ,Uint16 Memsize);
extern void PCP_Driver_Configure(PCP_Driver_OBJ_p NJC_OBJ);
extern void CMPSS_CFG(PCP_Driver_OBJ_p OBJ,Uint16* port,Uint16* group,Uint16* source,Uint16* filter_win,Uint16* filter_vote,Uint16* protect_value,Uint16 length);
extern void PWM_DAC_CFG(PCP_Driver_OBJ_p OBJ,Uint16 port1,Uint16 port2,float32 out1,float32 out2,float32 out3,float32 out4);
extern void PWM_DRV_CFG(PCP_Driver_OBJ_p OBJ,Uint16 mode,Uint16 port1, Uint16 port2, Uint16 port3,Uint16 socport,Uint16 fkhz,Uint16 dbtime,Uint16 blankwin);
extern void ADC_DRV_CFG(PCP_Driver_OBJ_p OBJ,Uint16 *port,Uint32* soc,Uint32* source,Uint32* acqps,Uint32* trig,Uint16 length);
extern void CLA_DRV_CFG(PCP_Driver_OBJ_p OBJ,Uint16* task,Uint16* source,Uint16* iack,Uint16 length);
extern void CMPSS_PCMC_CFG(PCP_Driver_OBJ_p OBJ,Uint16 port,Uint16 source,Uint16 rampsource,Uint16 filter_win,Uint16 filter_vote,float32 Itrip,float32 Icompramp);
extern void IO_CFG(PCP_Driver_OBJ_p OBJ);

extern void SCI_CFG(PCP_Driver_OBJ_p OBJ,Uint16 port,Uint32 lspclk,Uint32 baudrate);
extern void IO_HANDLE_FUN(PCP_Driver_OBJ_p OBJ);
extern void SCI_HANDLE_FUN(PCP_Driver_OBJ_p OBJ);
extern void SM_FUN(PCP_Driver_OBJ_p OBJ);
extern void PULSE_GEN_CFG(PCP_Driver_OBJ_p OBJ,Uint16 clkdiv,Uint16 hclkdiv,Uint16 pulseclk, float32 pulseduty, float32 triggertime,Uint16 pwmno);
extern void CLA_CL_ENA(PCP_Driver_OBJ_p OBJ);
extern void CLA_CL_DIS(PCP_Driver_OBJ_p OBJ);

extern void IO_PROTECT_SET(PCP_Driver_OBJ_p OBJ);
extern void PULSE_SET(PCP_Driver_OBJ_p OBJ);
extern void ADC_DRV_SET(PCP_Driver_OBJ_p OBJ);
extern void IO_SET();
extern void SCI_SET(PCP_Driver_OBJ_p OBJ);
extern void CLA_DRV_SET(PCP_Driver_OBJ_p);
extern void PWM_VCM_PS_SET(PCP_Driver_OBJ_p OBJ,Uint16 kHz,float phase_pu);
extern void PWM_DAC_SET(PCP_Driver_OBJ_p OBJ);
extern void CMPSS_PCMC_SET(PCP_Driver_OBJ_p OBJ);
extern void CMPSS_SET(PCP_Driver_OBJ_p OBJ);
extern void PI_Update(PCP_Driver_OBJ_p OBJ);
extern void Iref_Update(PCP_Driver_OBJ_p OBJ);
extern void Pulse_Update(PCP_Driver_OBJ_p OBJ);
extern void Soft_LSpd_Prt_Update(PCP_Driver_OBJ_p OBJ);
extern void Soft_Prt_Update(PCP_Driver_OBJ_p OBJ);

extern void InitEpwm1Gpio(void);
extern void InitEpwm2Gpio(void);
extern void InitEpwm3Gpio(void);
extern void InitEpwm4Gpio(void);
extern void InitEpwm5Gpio(void);
extern void InitEpwm6Gpio(void);
extern void InitEpwm7Gpio(void);
extern void InitEpwm8Gpio(void);


extern void PWM_PSFB_PCMC_FUN(PCP_Driver_OBJ_p OBJ);
extern void PWM_PSFB_VCM_FUN(PCP_Driver_OBJ_p OBJ);

Cmpss_Obj_p CMPSS_INIT(void *OBJ,Uint16 Memsize);
Pwm_Dac_Obj_p PWM_DAC_INIT(void *OBJ,Uint16 Memsize);
Pwm_Drv_Obj_p PWM_DRV_INIT(void *OBJ,Uint16 Memsize);
Adc_Drv_Obj_p ADC_DRV_INIT(void *OBJ,Uint16 Memsize);
Cla_Drv_Obj_p CLA_DRV_INIT(void *OBJ,Uint16 Memsize);
Cmpss_Pcmc_Obj_p CMPSS_PCMC_INIT(void *OBJ,Uint16 Memsize);
IO_Pack_Obj_p IO_INIT(void *OBJ,Uint16 Memsize);
Sci_Drv_Obj_p SCI_DRV_INIT(void *OBJ,Uint16 Memsize);
PCP_DataExchange_Obj_p PCP_DATA_EX_INIT(void *OBJ,Uint16 Memsize);
PCP_PulseGen_Obj_p PULSE_DRV_INIT(void *OBJ,Uint16 Memsize);

//Identify If SCI Transmitter is Ready to Transmit
inline bool Sci_tx_is_busy(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    return (SCI_DRV_PTR->SCICTL2.bit.TXRDY == 0);
}

//Identify If SCI Receiver is Ready to be Read.
inline bool Sci_rx_is_ready(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    return (SCI_DRV_PTR->SCIFFRX.bit.RXFFST != 0);
}

//So far Receiver Buffer is ready to be read, read characters cyclically and store them in rxbuf[n](8 digits).
inline bool Sci_in_char(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 num=0;
    bool flag;
    if(Sci_rx_is_ready(target))
        flag = 1;
    else
        flag = 0;
    while(Sci_rx_is_ready(target))
    {
        target->SCI_DRV_OBJ_P_INS->rxbuf[num] = SCI_DRV_PTR->SCIRXBUF.all;
        num++;
    }
    return flag;
}

//So far Transmitter Buffer is ready to transmit, store characters into the buffer.
inline void Sci_out_char(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 num;
    for(num=0; num<target->SCI_DRV_OBJ_P_INS->txnum;num++)
    {
        SCI_DRV_PTR->SCITXBUF.all = target->SCI_DRV_OBJ_P_INS->txbuf[num];
    }
}

#endif
