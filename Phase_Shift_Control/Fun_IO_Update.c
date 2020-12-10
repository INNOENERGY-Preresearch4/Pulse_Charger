#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

//Run in Background
//save port data if port direction is IN(0)
//write port data if port direction is OUT(1)
//any GPIO change must also be applied in "Cfg_Basic.c" and other related files
void IO_HANDLE_FUN(PCP_Driver_OBJ_p OBJ)
{

    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    if(target->IO_PACK_OBJ_P_INS->FLT1_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->FLT1_p->portdata = GpioDataRegs.GPADAT.bit.GPIO11;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->FLT1_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO11   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->FLT2_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->FLT2_p->portdata = GpioDataRegs.GPADAT.bit.GPIO26;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->FLT2_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO26   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->FLT3_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->FLT3_p->portdata = GpioDataRegs.GPADAT.bit.GPIO23;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->FLT3_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO23   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->FLT4_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->FLT4_p->portdata = GpioDataRegs.GPADAT.bit.GPIO10;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->FLT4_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO10   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->PSV_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->PSV_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO40;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->PSV_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO40   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->PSI_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->PSI_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO57;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->PSI_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO57   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->PPV_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->PPV_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO34;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->PPV_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO34   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->OOUT1_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->OOUT1_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO59;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->OOUT1_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO59   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->OOUT2_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->OOUT2_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO56;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->OOUT2_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO56   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->Twinkle_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->Twinkle_p->portdata = GpioDataRegs.GPADAT.bit.GPIO7;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->Twinkle_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO7   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->OIN1_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->OIN1_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO33;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->OIN1_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO33   = 1;
    }
/*
    if(target->IO_PACK_OBJ_P_INS->OIN2_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->OIN2_p->portdata = GpioDataRegs.GPADAT.bit.GPIO26;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->OIN2_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO26   = 1;
    }
*/
    if(target->IO_PACK_OBJ_P_INS->PWM_en_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = GpioDataRegs.GPADAT.bit.GPIO24;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO24   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->PWM_rst1_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->PWM_rst1_p->portdata = GpioDataRegs.GPADAT.bit.GPIO25;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->PWM_rst1_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO25   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->PWM_rst2_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->PWM_rst2_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO39;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->PWM_rst2_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO39   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->Relay_in_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->Relay_in_p->portdata = GpioDataRegs.GPBDAT.bit.GPIO58;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->Relay_in_p->portdata == 0)
            GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1;
        else
            GpioDataRegs.GPBSET.bit.GPIO58   = 1;
    }

    if(target->IO_PACK_OBJ_P_INS->Relay_out_p->portdir == 0)
    {
        target->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = GpioDataRegs.GPADAT.bit.GPIO27;
    }
    else
    {
        if(target->IO_PACK_OBJ_P_INS->Relay_out_p->portdata == 0)
            GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;
        else
            GpioDataRegs.GPASET.bit.GPIO27   = 1;
    }
}
