#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

PCP_Driver_OBJ_p PCP_INIT(void *PCP_OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(PCP_Driver_OBJ))
        return (PCP_Driver_OBJ_p)NULL;
    PCP_Driver_OBJ_p handle   = (PCP_Driver_OBJ_p)PCP_OBJ;
    handle->CMPSS_OBJ_P_INS=CMPSS_INIT(&handle->CMPSS_OBJ_INS,sizeof(handle->CMPSS_OBJ_INS));
    handle->CMPSS_PCMC_OBJ_P_INS=CMPSS_PCMC_INIT(&handle->CMPSS_PCMC_OBJ_INS,sizeof(handle->CMPSS_PCMC_OBJ_INS));
    handle->PWM_DAC_OBJ_P_INS=PWM_DAC_INIT(&handle->PWM_DAC_OBJ_INS,sizeof(handle->PWM_DAC_OBJ_INS));
    handle->PWM_DRV_OBJ_P_INS=PWM_DRV_INIT(&handle->PWM_DRV_OBJ_INS,sizeof(handle->PWM_DRV_OBJ_INS));
    handle->ADC_DRV_OBJ_P_INS=ADC_DRV_INIT(&handle->ADC_DRV_OBJ_INS,sizeof(handle->ADC_DRV_OBJ_INS));
    handle->CLA_DRV_OBJ_P_INS=CLA_DRV_INIT(&handle->CLA_DRV_OBJ_INS,sizeof(handle->CLA_DRV_OBJ_INS));
    handle->IO_PACK_OBJ_P_INS=IO_INIT(&handle->IO_PACK_OBJ_INS,sizeof(handle->IO_PACK_OBJ_INS));
    handle->SCI_DRV_OBJ_P_INS=SCI_DRV_INIT(&handle->SCI_DRV_OBJ_INS,sizeof(handle->SCI_DRV_OBJ_INS));
    handle->PCP_PULSEGEN_OBJ_P_INS=PULSE_DRV_INIT(&handle->PCP_PULSEGEN_OBJ_INS,sizeof(handle->PCP_PULSEGEN_OBJ_INS));
    handle->PCP_DATAEXCHANGE_OBJ_P_INS=PCP_DATA_EX_INIT(&handle->PCP_DATAEXCHANGE_OBJ_INS,sizeof(handle->PCP_DATAEXCHANGE_OBJ_INS));
    handle->backgroundfun[0] = backgroundfa;
    handle->backgroundfun[1] = backgroundfb;
    handle->backgroundfun[2] = backgroundfc;
    return handle;
}

Cmpss_Obj_p CMPSS_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Cmpss_OBJ))
        return (Cmpss_Obj_p)NULL;
    Cmpss_Obj_p handle   = (Cmpss_Obj_p)OBJ;
    return handle;
}

Cmpss_Pcmc_Obj_p CMPSS_PCMC_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Cmpss_OBJ))
        return (Cmpss_Pcmc_Obj_p)NULL;
    Cmpss_Pcmc_Obj_p handle   = (Cmpss_Pcmc_Obj_p)OBJ;
    return handle;
}

Pwm_Dac_Obj_p PWM_DAC_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Pwm_Dac_OBJ))
        return (Pwm_Dac_Obj_p)NULL;
    Pwm_Dac_Obj_p handle   = (Pwm_Dac_Obj_p)OBJ;
    return handle;
}

Pwm_Drv_Obj_p PWM_DRV_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Pwm_Drv_OBJ))
        return (Pwm_Drv_Obj_p)NULL;
    Pwm_Drv_Obj_p handle   = (Pwm_Drv_Obj_p)OBJ;
    return handle;
}

Adc_Drv_Obj_p ADC_DRV_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Adc_Drv_OBJ))
        return (Adc_Drv_Obj_p)NULL;
    Adc_Drv_Obj_p handle   = (Adc_Drv_Obj_p)OBJ;
    return handle;
}

Cla_Drv_Obj_p CLA_DRV_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Cla_Drv_OBJ))
        return (Cla_Drv_Obj_p)NULL;
    Cla_Drv_Obj_p handle   = (Cla_Drv_Obj_p)OBJ;
    return handle;
}

Sci_Drv_Obj_p SCI_DRV_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(Sci_Drv_OBJ))
        return (Sci_Drv_Obj_p)NULL;
    Sci_Drv_Obj_p handle   = (Sci_Drv_Obj_p)OBJ;
    return handle;
}

PCP_PulseGen_Obj_p PULSE_DRV_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(PCP_PulseGen_OBJ))
        return (PCP_PulseGen_Obj_p)NULL;
    PCP_PulseGen_Obj_p handle   = (PCP_PulseGen_Obj_p)OBJ;
    return handle;
}

IO_Pack_Obj_p IO_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(IO_Pack_OBJ))
        return (IO_Pack_Obj_p)NULL;
    IO_Pack_Obj_p handle   = (IO_Pack_Obj_p)OBJ;
    handle->FLT1_p      = &handle->FLT1;
    handle->FLT2_p      = &handle->FLT2;
    handle->FLT3_p      = &handle->FLT3;
    handle->FLT4_p      = &handle->FLT4;
    handle->OIN1_p      = &handle->OIN1;
    handle->OIN2_p      = &handle->OIN2;
    handle->OOUT1_p     = &handle->OOUT1;
    handle->OOUT2_p     = &handle->OOUT2;
    handle->Twinkle_p   = &handle->Twinkle;
    handle->PSI_p       = &handle->PSI;
    handle->PSV_p       = &handle->PSV;
    handle->PPV_p       = &handle->PPV;
    handle->PWM_en_p    = &handle->PWM_en;
    handle->PWM_rst1_p  = &handle->PWM_rst1;
    handle->PWM_rst2_p  = &handle->PWM_rst2;
    handle->Relay_in_p  = &handle->Relay_in;
    handle->Relay_out_p = &handle->Relay_out;
    return handle;
}

PCP_DataExchange_Obj_p PCP_DATA_EX_INIT(void *OBJ,Uint16 Memsize)
{
    if(Memsize < sizeof(PCP_DataExchange_OBJ))
        return (PCP_DataExchange_Obj_p)NULL;
    PCP_DataExchange_Obj_p handle   = (PCP_DataExchange_Obj_p)OBJ;
    return handle;
}
