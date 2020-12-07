#include "Global.h"
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "Background.h"
#include "Cla_Interrupts.h"

void (*PWM_DRV_FUN[])(PCP_Driver_OBJ_p OBJ)={PWM_PSFB_VCM_FUN,PWM_PSFB_PCMC_FUN};

void PCP_Driver_Configure(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ*)OBJ;

    IO_SET();
    SCI_SET(target);
    PULSE_SET(target);
    PWM_DAC_SET(target);
    ADC_DRV_SET(target);
    CMPSS_SET(target);
    CMPSS_PCMC_SET(target);
    IO_PROTECT_SET(target);
    CLA_DRV_SET(target);
    PWM_DRV_FUN[target->PWM_DRV_OBJ_P_INS->mode](target);
}
