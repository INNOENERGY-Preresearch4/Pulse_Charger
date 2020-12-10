#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

//Software Protection Setting (through Xbar and Trip Zone)
void IO_PROTECT_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    EALLOW;

    InputXbarRegs.INPUT2SELECT = (target->IO_PACK_OBJ_P_INS->FLT1_p->portname);//Bridge1 OC
    InputXbarRegs.INPUT3SELECT = (target->IO_PACK_OBJ_P_INS->FLT2_p->portname);//Bridge2 OC
    EPwmXbarRegs.TRIP7MUX0TO15CFG.bit.MUX0 = 1;//Output OC & OV :CMPSS1.CTRIPH_OR_CTRIPL
    EPwmXbarRegs.TRIP8MUX0TO15CFG.bit.MUX2 = 0;//Input OC :CMPSS2.CTRIPH
    XbarRegs.XBARCLR1.all      = 0xffffffff;
    XbarRegs.XBARCLR2.all      = 0xffffffff;
    EPwmXbarRegs.TRIP7MUXENABLE.bit.MUX0 = 1;//Enable Mux0 to drive Trip Input 7
    EPwmXbarRegs.TRIP8MUXENABLE.bit.MUX2 = 1;//Enable Mux2 to drive Trip Input 8

    //Trip Input 2,3,7,8 Selected as Combined Input (2:GateDriver1, 3:GateDriver2, 7:Output OC & OV, 8:Input OC)
    //EPWM1
    PWM_DRV_PTR1->DCALTRIPSEL.bit.TRIPINPUT2 = 1;
    PWM_DRV_PTR1->DCALTRIPSEL.bit.TRIPINPUT3 = 1;
    PWM_DRV_PTR1->DCALTRIPSEL.bit.TRIPINPUT7 = 1;
    PWM_DRV_PTR1->DCALTRIPSEL.bit.TRIPINPUT8 = 1;

    PWM_DRV_PTR1->DCBLTRIPSEL.bit.TRIPINPUT2 = 1;
    PWM_DRV_PTR1->DCBLTRIPSEL.bit.TRIPINPUT3 = 1;
    PWM_DRV_PTR1->DCBLTRIPSEL.bit.TRIPINPUT7 = 1;
    PWM_DRV_PTR1->DCBLTRIPSEL.bit.TRIPINPUT8 = 1;

    PWM_DRV_PTR1->DCTRIPSEL.bit.DCALCOMPSEL = 0b1111;//Combined Signals trip EPWM1 DCAL
    PWM_DRV_PTR1->TZDCSEL.bit.DCAEVT1       = 0b100;//trigger DCAEVT1  event when DCAL = high
    PWM_DRV_PTR1->TZSEL.bit.DCAEVT1         = 1;//DCAEVT1 One-Shot Trip Zone Enable
    PWM_DRV_PTR1->TZCTL.bit.TZA             = 0b10;//DCAEVT1 event force EPWM1A to low

    PWM_DRV_PTR1->DCTRIPSEL.bit.DCBLCOMPSEL = 0b1111;//Combined Signals trip EPWM1 DCBL
    PWM_DRV_PTR1->TZDCSEL.bit.DCBEVT1       = 0b100;//trigger DCAEVT1  event when DCBL = high
    PWM_DRV_PTR1->TZSEL.bit.DCBEVT1         = 1;//DCBEVT1 One-Shot Trip Zone Enable
    PWM_DRV_PTR1->TZCTL.bit.TZB             = 0b10;//DCAEVT1 event force EPWM1B to low
    //EPWM2
    PWM_DRV_PTR2->DCALTRIPSEL.bit.TRIPINPUT2 = 1;
    PWM_DRV_PTR2->DCALTRIPSEL.bit.TRIPINPUT3 = 1;
    PWM_DRV_PTR2->DCALTRIPSEL.bit.TRIPINPUT7 = 1;
    PWM_DRV_PTR2->DCALTRIPSEL.bit.TRIPINPUT8 = 1;

    PWM_DRV_PTR2->DCBLTRIPSEL.bit.TRIPINPUT2 = 1;
    PWM_DRV_PTR2->DCBLTRIPSEL.bit.TRIPINPUT3 = 1;
    PWM_DRV_PTR2->DCBLTRIPSEL.bit.TRIPINPUT7 = 1;
    PWM_DRV_PTR2->DCBLTRIPSEL.bit.TRIPINPUT8 = 1;

    PWM_DRV_PTR2->DCTRIPSEL.bit.DCALCOMPSEL = 0b1111;//Combined Signals trip EPWM2 DCAL
    PWM_DRV_PTR2->TZDCSEL.bit.DCAEVT1       = 0b100;//trigger DCAEVT1  event when DCAL = high
    PWM_DRV_PTR2->TZSEL.bit.DCAEVT1         = 1;//DCAEVT1 One-Shot Trip Zone Enable
    PWM_DRV_PTR2->TZCTL.bit.TZA             = 0b10;//DCAEVT1 event force EPWM1A to low

    PWM_DRV_PTR2->DCTRIPSEL.bit.DCBLCOMPSEL = 0b1111;//Combined Signals trip EPWM2 DCBL
    PWM_DRV_PTR2->TZDCSEL.bit.DCBEVT1       = 0b100;//trigger DCAEVT1  event when DCBL = high
    PWM_DRV_PTR2->TZSEL.bit.DCBEVT1         = 1;//DCAEVT1 One-Shot Trip Zone Enable
    PWM_DRV_PTR2->TZCTL.bit.TZB             = 0b10;//DCAEVT1 event force EPWM1B to low

    EDIS;
}
