//280049C CPU_ClK = 100MHz
#include "Global.h"
#include "Cla_Interrupts.h"
#include "F28x_Project.h"
#include "user_def.h"
#include "Background.h"
#include "SFO_V8.h"

Uint16 fa=0;

Uint16 SFO_status;
int MEP_ScaleFactor;
extern volatile struct EPWM_REGS *ePWM[] = {&EPwm1Regs, &EPwm1Regs,&EPwm2Regs, &EPwm3Regs};

PCP_Driver_OBJ   PCP_handle;
PCP_Driver_OBJ_p PCP_P_handle;

//Initialize ADC and SOC
//ADCAIn3:Vo, ADCAIn2:Io, ADCBIn2:T1, ADCBIn8:Vin, ADCBIn3:T2
Uint16 ADCPORT[]={ADCA,ADCA,ADCB,ADCB,ADCB};//Select ADC (A/B/C)
Uint32 ADCSOC[]={SOC2,SOC3,SOC3,SOC4,SOC5}; //Select SOC (Control Start-of Conversion of ADC:Input/Trigger/Acquisition Window)
//Uint32 ADCSOURCE[]={ADC_IN2,ADC_IN3,ADC_IN2,ADC_IN8};           //Select Input
Uint32 ADCSOURCE[]={ADC_IN3,ADC_IN2,ADC_IN2,ADC_IN8,ADC_IN3};           //Select Input
Uint32 ADCACQPS[]={ACQPSWidth,ACQPSWidth,ACQPSWidth,ACQPSWidth,ACQPSWidth};//Select Acquisition Window
Uint32 ADCTRIG[]={SOCA_ePWM1,SOCA_ePWM1,SOCA_ePWM1,SOCA_ePWM1,SOCA_ePWM1}; //Select Trigger Source

//Initialize CLA Tasks and Their Trigger Source
Uint16 CLATASK[]={CLATASK1, CLATASK2, CLATASK7};
Uint16 CLASOURCE[]={ADCA1_INT, EPWM4INT, SoftwareFC};//Task Trigger Source
Uint16 CLAIACK[]={Null,Null,Force};//One-shot Software Triggering

//Initialize Comparators and Reference Voltages for Protection
//bypass PGA via manual switches on ControlCard (Reference: "controlCARD Information Guide")
//Pg.1454 Source Table
//A2(PGA1_IN/Output Current), A3(ADCA1/Output Voltage), A5(ADCA2/Input Current)
Uint16 CMPSSPORT[]={CMPSS1,CMPSS1,CMPSS2};
Uint16 CMPSSGROUP[]={CMPSSL,CMPSSH,CMPSSH};//
Uint16 CMPSSSOURCE[]={CMPSS_MUX_PGA_OF,CMPSS_MUX_ADCAB,CMPSS_MUX_ADCAB};
Uint16 CMPSSFWIN[]={WinWidth,WinWidth,WinWidth};
Uint16 CMPSSFVOTE[]={VoteNum,VoteNum,VoteNum};
Uint16 CMPSSPVALUE[]={IoPrtThrdInit,VoPrtThrdInit,IinPrtThrdInit};

//Initialize CLA DataRAM
//Cla to CPU Message, real output current
#pragma DATA_SECTION(io,"Cla1ToCpuMsgRAM");
float io;
//CPU to Cla Message, referenced output current
#pragma DATA_SECTION(iref,"CpuToCla1MsgRAM");
float iref;
//CPU to Cla Message, Kp of PI controller
#pragma DATA_SECTION(Kp,"CpuToCla1MsgRAM");
float Kp;
//CPU to Cla Message, Ki of PI controller
#pragma DATA_SECTION(Ki,"CpuToCla1MsgRAM");
float Ki;
//CPU to Cla Message, period of current pulse
#pragma DATA_SECTION(Tpulse,"CpuToCla1MsgRAM");
float Tpulse;
#pragma DATA_SECTION(Prt_flag,"CpuToCla1MsgRAM");
bool Prt_flag;

void main(void)
{
    InitSysCtrl();
//Copy Program from Flash to RAM
#if (FLASH)
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitFlash();
#endif
    
//Initialize Interrupts / PIE Vector
    DINT;//Disable Interrupts
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
//End Initialization

/*
//DAC Test
    EALLOW;
    DELAY_US(20);
        DacaRegs.DACCTL.bit.DACREFSEL  = 1;GPIO27
        DacaRegs.DACCTL.bit.LOADMODE   = 0;
        DacaRegs.DACCTL.bit.MODE       = 1;
        DacaRegs.DACOUTEN.bit.DACOUTEN = 1;
        DELAY_US(20);
    EDIS;
    DacaRegs.DACVALS.bit.DACVALS   = 1800;
//DAC Test end
*/

//Set Data for Functions
    PCP_P_handle = PCP_INIT(&PCP_handle,sizeof(PCP_handle));
    IO_CFG(PCP_P_handle);
    SCI_CFG(PCP_P_handle,Port1,LspClk,BaudRate);
    CMPSS_CFG(PCP_P_handle,CMPSSPORT,CMPSSGROUP,CMPSSSOURCE,CMPSSFWIN,CMPSSFVOTE,CMPSSPVALUE,sizeof(CMPSSPORT)/sizeof(CMPSSPORT[0]));
    PWM_DAC_CFG(PCP_P_handle,ePWM7,ePWM8,HwPrtThrdVo,HwPrtThrdIo,HwPrtThrdIin,HwPrtThrdVin);
    PWM_DRV_CFG(PCP_P_handle,PWM_PSFB_PCMC,ePWM1,ePWM2,ePWM3,ePWM1SOCA,Fs,DdTime,BlankWin);
    PULSE_GEN_CFG(PCP_P_handle,ClkDiv16,HSPClkDiv10,PulseFreq_Init,PulseDuty_Init,PI_Update_Time,ePWM4);
    ADC_DRV_CFG(PCP_P_handle,ADCPORT,ADCSOC,ADCSOURCE,ADCACQPS,ADCTRIG,sizeof(ADCPORT)/sizeof(ADCPORT[0]));
    CLA_DRV_CFG(PCP_P_handle,CLATASK,CLASOURCE,CLAIACK,sizeof(CLATASK)/sizeof(CLATASK[0]));
    CMPSS_PCMC_CFG(PCP_P_handle,CMPSS7,CMPSS_MUX_PGA_OF,ePWM3,FilterWin,FilterVote,RampRef_Init,RampDec);

//Set Filter Factor for Display
    EMAVG_reset(&emavgTemp);
    EMAVG_config(&emavgTemp, 0.031416);
    EMAVG_reset(&emavgVin);
    EMAVG_config(&emavgVin, 0.031416);
    EMAVG_reset(&emavgVo);
    EMAVG_config(&emavgVo, 0.031416);
    EMAVG_reset(&emavgIo);
    EMAVG_config(&emavgIo, 0.031416);

//Set Timer for Background Functions
    CpuTimer0Regs.PRD.all = Timer_1ms;
    CpuTimer1Regs.PRD.all = Timer_10ms;
    CpuTimer2Regs.PRD.all = Timer_100ms;
//Set Registers
    PCP_Driver_Configure(PCP_P_handle);



//SFO(Scale Factor Optimizer Function) is used for driving the micro-edge positioner (MEP) calibration module to run SFO diagnostics and determine the appropriate MEP scale factor
//Pg.1927
    SFO_status = SFO_Running;
    while(SFO_status == SFO_Running)
    {
        SFO_status = SFO();
        if(SFO_status == SFO_Error)
        {
            ESTOP0;
        }
    }

//Background Function
    for(;;)
    {
        PCP_P_handle->backgroundfun[fa++]();
        if(fa>=3)fa=0;
    }


}

