#include "Global.h"
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "Background.h"
#include "Cla_Interrupts.h"

//Set Modules for Corresponding Functions

void (*PWM_DRV_FUN[])(PCP_Driver_OBJ_p OBJ)={PWM_PSFB_VCM_FUN,PWM_PSFB_PCMC_FUN};

void PCP_Driver_Configure(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ*)OBJ;
    //Set GPIO
    IO_SET();
    //Set SCI Message Transmission
    SCI_SET(target);
    //Set Current Pulse
    PULSE_SET(target);
    //Set HW Protection Thrd (via PWM Duty Cycle)
    PWM_DAC_SET(target);
    //Set ADC
    ADC_DRV_SET(target);
    //Set SW Protection Thrd (via CMPSS)
    CMPSS_SET(target);
    //Set Peak Current Control
    CMPSS_PCMC_SET(target);
    //Set EPWM TZ for SW Protection
    IO_PROTECT_SET(target);
    //Set CLA
    CLA_DRV_SET(target);
    //Choose to operate in Phase-shift Mode or Peak-current-control Mode
    PWM_DRV_FUN[target->PWM_DRV_OBJ_P_INS->mode](target);
}
