#ifndef USER_DEF_H_
#define USER_DEF_H_

#define McuClk  100000000.0f
#define Kp_Init 0.003f
#define Ki_Init 0.5f

#define FLASH             1

#define PWM_PSFB_VCM      0
#define PWM_PSFB_PCMC     1

#define SFO_Finished      1
#define SFO_Running       0
#define SFO_Error         2

#define CMPSS_MUX_ADCAB   3
#define CMPSS_MUX_PGA_OF  0
#define CMPSS_MUX_ADCC    1
#define CMPSS_MUX_PGA_IN  2
#define CMPSS_MUX_PGA_OUT 4

#define Timer_0_1ms       10000     // 100MHz/10KHz = 10^4
#define Timer_1ms         100000    // 100MHz/1KHz = 10^5
#define Timer_10ms        1000000   // 100MHz/100Hz = 10^6
#define Timer_100ms       10000000  // 100MHz/10Hz = 10^7
#define Timer_1000ms      100000000 // 100MHz/1Hz = 10^8

//Protection Setting
#define CMPSS1            1
#define CMPSS2            2
#define CMPSS3            3
#define CMPSS4            4
#define CMPSS5            5
#define CMPSS6            6
#define CMPSS7            7

#define CMPSSH            1
#define CMPSSL            0

#define VoPrtThrdInit    3600 //3600 = 55V
#define IoPrtThrdInit    3177 //3177 = 30A
#define IinPrtThrdInit   1500 //1500 = 26A

#define WinWidth         10
#define VoteNum           8

//CLA Setting
#define CLATASK1            1
#define CLATASK2            2
#define CLATASK3            3
#define CLATASK4            4
#define CLATASK5            5
#define CLATASK6            6
#define CLATASK7            7
#define CLATASK8            8

#define SoftwareFC          0
#define ADCA1_INT           1
#define ADCA2_INT           2
#define ADCA3_INT           3
#define ADCA4_INT           4
#define ADCA_EVT_INT        5
#define ADCB1_INT           6
#define ADCB2_INT           7
#define ADCB3_INT           8
#define ADCB4_INT           9
#define ADCB_EVT_INT        10
#define ADCC1_INT           11
#define ADCC2_INT           12
#define ADCC3_INT           13
#define ADCC4_INT           14
#define ADCC_EVT_INT        15
#define XINT1               29
#define XINT2               30
#define XINT3               31
#define XINT4               32
#define XINT5               33
#define EPWM1INT            36
#define EPWM2INT            37
#define EPWM3INT            38
#define EPWM4INT            39
#define EPWM5INT            40
#define EPWM6INT            41
#define EPWM7INT            40
#define EPWM8INT            41
#define TINT0               68//timer0
#define TINT1               69//timer1
#define TINT2               70//timer2

#define Force               1
#define Null                0

//ADC Setting
#define ADCA               0
#define ADCB               1
#define ADCC               2

#define SOC0               0
#define SOC1               1
#define SOC2               2
#define SOC3               3
#define SOC4               4
#define SOC5               5
#define SOC6               6
#define SOC7               7
#define SOC8               8
#define SOC9               9
#define SOC10              10
#define SOC11              11
#define SOC12              12
#define SOC13              13
#define SOC14              14
#define SOC15              15

#define ADC_IN0               0
#define ADC_IN1               1
#define ADC_IN2               2
#define ADC_IN3               3
#define ADC_IN4               4
#define ADC_IN5               5
#define ADC_IN6               6
#define ADC_IN7               7
#define ADC_IN8               8
#define ADC_IN9               9
#define ADC_IN10              10
#define ADC_IN11              11
#define ADC_IN12              12
#define ADC_IN13              13
#define ADC_IN14              14
#define ADC_IN15              15

#define ACQPSWidth           7

#define SoftOnly                0
#define TINT_0                  1
#define TINT_1                  2
#define TINT_2                  3
//
#define SOCA_ePWM1              5
#define SOCB_ePWM1              6
#define SOCA_ePWM2              7
#define SOCB_ePWM2              8
#define SOCA_ePWM3              9
#define SOCB_ePWM3              10
#define SOCA_ePWM4              11
#define SOCB_ePWM4              12
#define SOCA_ePWM5              13
#define SOCB_ePWM5              14
#define SOCA_ePWM6              15
#define SOCB_ePWM6              16
#define SOCA_ePWM7              17
#define SOCB_ePWM7              18
#define SOCA_ePWM8              19
#define SOCB_ePWM8              20


#define Enable              1
#define Disable             0

#define EndConv             1
#define StartConv           0

#define Div1                0
#define Div2                2
#define Div3                4
#define Div4                6
#define Div5                8
#define Div6                10
#define Div7                12
#define Div8                14

//PWM for HW Protection
#define HwPrtThrdVin         85.0f
#define HwPrtThrdVo          60.0f
#define HwPrtThrdIin         30.0f
#define HwPrtThrdIo          30.0f

//EPWM for Drv
#define ePWM1               1
#define ePWM2               2
#define ePWM3               3
#define ePWM4               4
#define ePWM5               5
#define ePWM6               6
#define ePWM7               7
#define ePWM8               8

#define ePWM1SOCA           1
#define ePWM2SOCA           2

#define Fs                  200 //kHz
#define DdTime              200 //ns
#define BlankWin            20  //ns(only used for PCMC control)

//Pulse Setting
//TBCLK = EPWMCLK/(HSPCLKDIV * CLKDIV)
//CLKDIV: 000: /1, 001: /2, 010: /4, 011: /8, 100: /16, 101: /32, 110: /64, 111: /128
//HSPCLKDIV: 000: /1, 001: /2, 010: /4, 011: /6, 100: /8, 101: /10, 110: /12, 111: /14
#define ClkDiv1              0
#define ClkDiv2              1
#define ClkDiv4              2
#define ClkDiv8              3
#define ClkDiv16             4
#define ClkDiv32             5
#define ClkDiv64             6
#define ClkDiv128            7

#define HSPClkDiv1           0
#define HSPClkDiv2           1
#define HSPClkDiv4           2
#define HSPClkDiv6           3
#define HSPClkDiv8           4
#define HSPClkDiv10          5
#define HSPClkDiv12          6
#define HSPClkDiv14          7

#define PulseFreq_Init       1000 //Hz
#define PulseDuty_Init       0.8f
#define PI_Update_Time       0.99f

//Cmpss for PCMC Setting

#define FilterWin           1
#define FilterVote          1
#define RampRef_Init        0.5f
#define RampDec             0.5f
//SCI Setting
#define Port1               1

#define LspClk              25000000//LSPCLK uses a /4 divider by default (100M/4 = 25MHz)
#define BaudRate            57600
//
#define PulseFreqMin        0.001f   //1Hz

#define T_Launch_Prt        5
#define T_Launch            30
#define T_Prt_Min           80
#define T_Prt_Max           120

#define Startup_Wait_time1    2
#define Startup_Wait_time2    10
#define Shutdown_Wait_time1    0
#define Shutdown_Wait_time2    3

#define Kp_Scaling 0.01f
#define Ki_Scaling 10.0f
#define Iref_Scaling 50.0f
#define PulseFreq_Scaling 10000.0f
#define PulseDuty_Max 1.0f

#define In_Relay_Thrd 40.0f

#define V_scaling 85.0f
#define I_scaling 41.0f
#define I_scaling_Ctr 30.0f
#define Tmt_scaling 60.0f

#define RX_SC_Freq 50000.0f
#define RX_SC_General 65535.0f

#define ADC_Cnt_Max 4096.0f

#define I_pcmc_max 36.66f

#endif
