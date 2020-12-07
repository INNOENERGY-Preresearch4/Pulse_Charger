#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"


void CMPSS_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ *target = (PCP_Driver_OBJ *)OBJ;
    Uint16 num;
    EALLOW;
    for(num=0;num<target->CMPSS_OBJ_P_INS->length;num++)
    {
        //Bits to select one of the many sources on CMPHP/CMPLP inputs
        //Pg.1454 Source Table
        if(target->CMPSS_OBJ_P_INS->group[num] == 1)
            AnalogSubsysRegs.CMPHPMXSEL.all |= ((Uint32)target->CMPSS_OBJ_P_INS->source[num])<<(3*target->CMPSS_OBJ_P_INS->port[num]);
        if(target->CMPSS_OBJ_P_INS->group[num] == 0)
            AnalogSubsysRegs.CMPLPMXSEL.all |= ((Uint32)target->CMPSS_OBJ_P_INS->source[num])<<(3*target->CMPSS_OBJ_P_INS->port[num]);
        CMPSS_PTR->COMPCTL.bit.COMPDACE     = 1;//Comparator/DAC enable.
        CMPSS_PTR->COMPCTL.all             |= (3<<(2+(8*(1 - target->CMPSS_OBJ_P_INS->group[num]))));//COMPCTL 3-2 CTRIPHSEL(High comparator CTRIPH(to EPWM Xbar) source select.), 3 = Latched output of digital filter drives CTRIPH
        CMPSS_PTR->COMPCTL.all             |= (3<<(4+(8*(1 - target->CMPSS_OBJ_P_INS->group[num]))));//COMPCTL 3-2 CTRIPOUTHSEL(High comparator CTRIPOUTH(to Output Xbar) source select.), 3 = Latched output of digital filter drives CTRIPOUTH
        CMPSS_PTR->COMPHYSCTL.bit.COMPHYS   = 1;//typical hysteresis
        //0x12h DACLVALS (Low DAC Value Shadow Register)
        //0x06h DACHVALS (High DAC Value Shadow Register)(0xC may be wrong)
        (*(volatile Uint32*)(target->CMPSS_OBJ_P_INS->address[num]+0x12-(0xC*target->CMPSS_OBJ_P_INS->group[num]))) = target->CMPSS_OBJ_P_INS->protect_value[num];
        //0x16h = CTRIPLFILCTL(low) Register, 3-0 RESERVED, 8-4 SAMPWIN, 13-9 THRESH, 14 RESERVED, 15 FILINIT
        //0x18h = CTRIPHFILCTL(high) Register, 3-0 RESERVED, 8-4 SAMPWIN, 13-9 THRESH, 14 RESERVED, 15 FILINIT
        (*(volatile Uint32*)(target->CMPSS_OBJ_P_INS->address[num]+0x16+(0x2*target->CMPSS_OBJ_P_INS->group[num]))) |= ((target->CMPSS_OBJ_P_INS->filter_win[num]<<4)+(target->CMPSS_OBJ_P_INS->filter_vote[num]<<9)+((Uint16)1<<15));

        CMPSS_PTR->COMPSTSCLR.bit.HLATCHCLR = 1;//reset comp sts
        CMPSS_PTR->COMPSTSCLR.bit.LLATCHCLR = 1;//reset comp sts
    }
    EDIS;
}
