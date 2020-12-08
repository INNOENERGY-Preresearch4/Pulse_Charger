#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

Uint16 SM_First_Startup_Flg = 1;//default : first startup;
Uint16 SM_Startup_Cnt ;
Uint16 SM_Shutdown_Flg = 0;
Uint16 SM_Shutdown_Cnt ;
Uint16 SM_Startup_Flg =0;
Uint16 Wait_time1;
Uint16 Wait_time2;

void SM_FUN(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    //initialization
    if( SM_Startup_Flg == 1 )
        SM_StartupMSG_Flg = 0;//prevent next startup
    else if( SM_StartupMSG_Flg == 1 )//start up
    {
        SM_Startup_Flg = 1;
        SM_Startup_Cnt = 0;
        SM_Shutdown_Flg =0;
    }
    if( SM_Shutdown_Flg == 1 )
        SM_ShutdownMSG_Flg =0;//prevent next shutdown
    else if( SM_ShutdownMSG_Flg == 1)//shutdown
        {
        SM_Shutdown_Flg = 1;
        SM_Shutdown_Cnt = 0;
        SM_Startup_Flg = 0;
        }

    //PWM enable Delay
    if( SM_Startup_Flg == 1 )
    {
        if ( SM_First_Startup_Flg ==1 )
        {
            target->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = 1;
            SM_First_Startup_Flg =0;//first
        }
        if(SM_Startup_Cnt == Startup_Wait_time1)//2
        {
            EALLOW;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm1Regs.TZCLR.bit.OST = 1;
            EDIS;
         }
        else if(SM_Startup_Cnt == Startup_Wait_time2)//10
        {
            EALLOW;
            Cla1Regs.MIER.bit.INT1 = 1;
            Cla1Regs.MIER.bit.INT2 = 1;
            EDIS;
            target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 1;//GPIO24 set
            GpioDataRegs.GPASET.bit.GPIO24   = 1;
            SM_Startup_Flg = 0;//startup done
          }
        if(SM_Startup_Cnt < 10 )//10*10ms
            SM_Startup_Cnt += 1;
    }

        //PWM disable Delay
    if (SM_Shutdown_Flg == 1)
    {
        if(SM_Shutdown_Cnt == Shutdown_Wait_time1)//10*10ms
        {
            EALLOW;
            Cla1Regs.MIER.bit.INT1 = 0;
            Cla1Regs.MIER.bit.INT2 = 0;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EDIS;
        }
        else if(SM_Shutdown_Cnt == Shutdown_Wait_time2 )
        {
            target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 0;//GPIO24 clear
            EALLOW;
            Cla1Regs.MIFRC.bit.INT7 = 1;
            EDIS;
            SM_Shutdown_Flg = 0;
        }
        if(SM_Shutdown_Cnt < 10)//10*10ms
            SM_Shutdown_Cnt += 1;
    }
}

