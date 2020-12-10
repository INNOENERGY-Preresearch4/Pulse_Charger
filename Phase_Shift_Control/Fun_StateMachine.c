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
    //Startup Process
    if( SM_Startup_Flg == 1 )
    {
        SM_StartupMSG_Flg = 0;//Startup Interlock
    }
    else if( SM_StartupMSG_Flg == 1 )//First Startup
    {
        //Lock Interlock
        SM_Startup_Flg = 1;
        SM_Startup_Cnt = 0;
        SM_Shutdown_Flg =0;
    }
    //Shutdown Process
    if( SM_Shutdown_Flg == 1 )
        SM_ShutdownMSG_Flg = 0;//Shutdown Interlock
    else if( SM_ShutdownMSG_Flg == 1)
        {
        SM_Shutdown_Flg = 1;
        SM_Shutdown_Cnt = 0;
        SM_Startup_Flg = 0;
        }

    //PWM Enable Delay
    if( SM_Startup_Flg == 1 )
    {
        if ( SM_First_Startup_Flg ==1 )
        {
            //Output Relay Signal Enable (with Delay)
            target->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = 1;
            SM_First_Startup_Flg =0;
        }
        if(SM_Startup_Cnt == Startup_Wait_time1)
        {
            //Clear PWM Trip Zone
            EALLOW;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm1Regs.TZCLR.bit.OST = 1;
            EDIS;
         }
        else if(SM_Startup_Cnt == Startup_Wait_time2)
        {
            //Enable ADC/CLA
            EALLOW;
            Cla1Regs.MIER.bit.INT1 = 1;
            Cla1Regs.MIER.bit.INT2 = 1;
            EDIS;
            //Enable PWM
            target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 1;
            GpioDataRegs.GPASET.bit.GPIO24   = 1;
            //Release Interlock
            SM_Startup_Flg = 0;
          }
        if(SM_Startup_Cnt < 10 )
            SM_Startup_Cnt += 1;
    }

        //PWM Disable Delay
    if (SM_Shutdown_Flg == 1)
    {
        if(SM_Shutdown_Cnt == Shutdown_Wait_time1)
        {
            //Disable ADC/CLA, Set PWM Trip Zone
            EALLOW;
            Cla1Regs.MIER.bit.INT1 = 0;
            Cla1Regs.MIER.bit.INT2 = 0;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EDIS;
        }
        else if(SM_Shutdown_Cnt == Shutdown_Wait_time2 )
        {
            //Disable PWM
            target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 0;//GPIO24 clear
            //CLA Task7 Software Force to Initialize Variables in CLA
            EALLOW;
            Cla1Regs.MIFRC.bit.INT7 = 1;
            EDIS;
            //Release Interlock
            SM_Shutdown_Flg = 0;
        }
        if(SM_Shutdown_Cnt < 10)//40*10ms
            SM_Shutdown_Cnt += 1;
    }
}

