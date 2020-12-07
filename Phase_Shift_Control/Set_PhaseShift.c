#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void PWM_VCM_PS_SET(PCP_Driver_OBJ_p OBJ,Uint16 kHz,float phase_pu)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint32 phase_cal;
    Uint32 PRD_cal;
    if(phase_pu < 0.0)
    {
        phase_pu = 0.0;
    }
    if(phase_pu > 0.45)
    {
        phase_pu = 0.45;
    }
    PRD_cal = 25600000/kHz;
    phase_cal = (Uint32)(PRD_cal * (0.5-phase_pu));

    PWM_DRV_PTR2->TBPHS.bit.TBPHS = phase_cal >> 8;
    PWM_DRV_PTR2->TRREM.bit.TRREM = (Uint16)(0xFF - ((Uint16)(phase_cal & 0xff)));
}
