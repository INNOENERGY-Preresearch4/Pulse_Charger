#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"


void ADC_DRV_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 num;
    //Set Reference Voltage of ADC to 3.3V
    SetVREF(ADCA,ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADCB,ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADCC,ADC_INTERNAL, ADC_VREF3P3);

    EALLOW;
    //ADC_A/ADC_B Clock Enable
    CpuSysRegs.PCLKCR13.bit.ADC_A = Enable;
    CpuSysRegs.PCLKCR13.bit.ADC_B = Enable;

    for(num=0;num<target->ADC_DRV_OBJ_P_INS->length;num++)
    {
        ADC_DRV_PTR->ADCCTL1.bit.ADCPWDNZ     = Enable;//ADC powered up
        ADC_DRV_PTR->ADCCTL1.bit.INTPULSEPOS  = EndConv;//ADC Interrupt Pulse Position :
        ADC_DRV_PTR->ADCCTL2.bit.PRESCALE     = Div3;   //ADC Clock Prescale : SYSCLK/N
        ADC_DRV_PTR->ADCINTSEL1N2.bit.INT1CONT= 0;//ADCINT1 Interrupt Mode: 0 = only flg clr, 1= irrespective of flg
        ADC_DRV_PTR->ADCINTSEL1N2.bit.INT1E   = Enable;//ADC Interrupt Enable
        ADC_DRV_PTR->ADCINTSEL1N2.bit.INT1SEL = target->ADC_DRV_OBJ_P_INS->soc[num];//ADCINT1 EOC Source Select
        ADC_DRV_PTR->ADCINTFLGCLR.bit.ADCINT1 = 1;//ADC Interrupt  flg clr
        //ADCSOCxCTL: 8-0 SOCx Acquisition Prescale, 18-15 SOCx Channel Select, 24-20 SOCx Trigger Source Select
        ADC_SOC_PTR = (target->ADC_DRV_OBJ_P_INS->acqps[num] + (target->ADC_DRV_OBJ_P_INS->source[num] <<15) + ((target->ADC_DRV_OBJ_P_INS->trig[num]) <<20));
        ADC_DRV_PTR->ADCINTFLGCLR.bit.ADCINT1 = 1;
    }
    Device_cal();
    DELAY_US(1000);
    EDIS;
}
