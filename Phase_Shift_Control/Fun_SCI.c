#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

Uint16 PWM_Startup_Flg = 0;
Uint16 PWM_Startup_Cnt = 0;
Uint16 PWM_Startup_End_Flg = 0;
Uint16 PWM_End_Flg = 0;
Uint16 PWM_End_Cnt = 0;

void SCI_HANDLE_FUN(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 checkdata,checknum;
    if(Sci_in_char(target))
    {
        switch(target->SCI_DRV_OBJ_P_INS->rxbuf[0])
        {
        case 82:
            if((target->SCI_DRV_OBJ_P_INS->rxbuf[7] * 256 + target->SCI_DRV_OBJ_P_INS->rxbuf[8])==(target->SCI_DRV_OBJ_P_INS->rxbuf[0] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[1] + target->SCI_DRV_OBJ_P_INS->rxbuf[2] + target->SCI_DRV_OBJ_P_INS->rxbuf[3] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[4] + target->SCI_DRV_OBJ_P_INS->rxbuf[5] + target->SCI_DRV_OBJ_P_INS->rxbuf[6]))
            {
                //target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 1;
                if (PWM_Startup_Flg == 0)
                {
                    PWM_End_Flg = 0;
                    PWM_Startup_Cnt = 0;
                    PWM_Startup_Flg = 1;
                    PWM_Startup_End_Flg = 0;
                    PWM_End_Cnt = 0;
                }

                target->IO_PACK_OBJ_P_INS->Relay_out_p->portdata = 1;
                target->SCI_DRV_OBJ_P_INS->txbuf[0]=123;
                target->SCI_DRV_OBJ_P_INS->txnum = 1;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_ref_data = target->SCI_DRV_OBJ_P_INS->rxbuf[1]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[2];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Duty_set_data = target->SCI_DRV_OBJ_P_INS->rxbuf[3]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[4];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->fre_set_data= target->SCI_DRV_OBJ_P_INS->rxbuf[5]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[6];
            }
            break;

        case 80:
            if((target->SCI_DRV_OBJ_P_INS->rxbuf[8] * 256 + target->SCI_DRV_OBJ_P_INS->rxbuf[9])==(target->SCI_DRV_OBJ_P_INS->rxbuf[0] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[1] + target->SCI_DRV_OBJ_P_INS->rxbuf[2] + target->SCI_DRV_OBJ_P_INS->rxbuf[3] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[4] + target->SCI_DRV_OBJ_P_INS->rxbuf[5] + target->SCI_DRV_OBJ_P_INS->rxbuf[6]+ target->SCI_DRV_OBJ_P_INS->rxbuf[7]))
            {
                target->SCI_DRV_OBJ_P_INS->txbuf[0]=123;
                target->SCI_DRV_OBJ_P_INS->txnum = 1;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Iin_Prt_data = target->SCI_DRV_OBJ_P_INS->rxbuf[1]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[2];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_Prt_data  = target->SCI_DRV_OBJ_P_INS->rxbuf[3]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[4];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_Prt_data  = target->SCI_DRV_OBJ_P_INS->rxbuf[5]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[6];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->T_prt_data   = target->SCI_DRV_OBJ_P_INS->rxbuf[7];
            }
            break;

        case 70:
            if((target->SCI_DRV_OBJ_P_INS->rxbuf[5] * 256 + target->SCI_DRV_OBJ_P_INS->rxbuf[6])==(target->SCI_DRV_OBJ_P_INS->rxbuf[0] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[1] + target->SCI_DRV_OBJ_P_INS->rxbuf[2] + target->SCI_DRV_OBJ_P_INS->rxbuf[3] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[4]))
            {
                target->SCI_DRV_OBJ_P_INS->txbuf[0]=123;
                target->SCI_DRV_OBJ_P_INS->txnum = 1;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->HW_Iin_Prt_data = target->SCI_DRV_OBJ_P_INS->rxbuf[1]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[2];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->HW_Io_Prt_data  = target->SCI_DRV_OBJ_P_INS->rxbuf[3]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[4];
            }
            break;

        case 67:
            if((target->SCI_DRV_OBJ_P_INS->rxbuf[5] * 256 + target->SCI_DRV_OBJ_P_INS->rxbuf[6])==(target->SCI_DRV_OBJ_P_INS->rxbuf[0] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[1] + target->SCI_DRV_OBJ_P_INS->rxbuf[2] + target->SCI_DRV_OBJ_P_INS->rxbuf[3] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[4]))
            {
                target->SCI_DRV_OBJ_P_INS->txbuf[0]=123;
                target->SCI_DRV_OBJ_P_INS->txnum = 1;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->CPa_data     = target->SCI_DRV_OBJ_P_INS->rxbuf[1]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[2];
                target->PCP_DATAEXCHANGE_OBJ_P_INS->CIa_data     = target->SCI_DRV_OBJ_P_INS->rxbuf[3]*256 + target->SCI_DRV_OBJ_P_INS->rxbuf[4];
            }
            break;

        case 83:
            if((target->SCI_DRV_OBJ_P_INS->rxbuf[1] * 256 + target->SCI_DRV_OBJ_P_INS->rxbuf[2])==(target->SCI_DRV_OBJ_P_INS->rxbuf[0]))
            {
                PWM_Startup_Cnt = 0;
                PWM_Startup_Flg = 0;
                PWM_Startup_End_Flg = 0;
                PWM_End_Flg = 1;

                EALLOW;
//                    GpioCtrlRegs.GPACSEL4.bit.GPIO24   = 0x0;
//                    EPwm2Regs.TZFRC.bit.OST = 1;
//                    EPwm1Regs.TZFRC.bit.OST = 1;
//                    target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata    = 0;
                EDIS;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_ref_data  = 0;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_ref_data  = 0;
                target->PCP_DATAEXCHANGE_OBJ_P_INS->fre_set_data = 0;
                target->SCI_DRV_OBJ_P_INS->txbuf[0]=123;
                target->SCI_DRV_OBJ_P_INS->txnum = 1;
            }
            break;

            case 68:
                target->SCI_DRV_OBJ_P_INS->txbuf[0] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Vin_dis_data/256;
                target->SCI_DRV_OBJ_P_INS->txbuf[1] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Vin_dis_data%256;
                target->SCI_DRV_OBJ_P_INS->txbuf[2] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_dis_data/256;
                target->SCI_DRV_OBJ_P_INS->txbuf[3] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Vo_dis_data%256;
                target->SCI_DRV_OBJ_P_INS->txbuf[4] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_dis_data/256;
                target->SCI_DRV_OBJ_P_INS->txbuf[5] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Io_dis_data%256;
                //target->SCI_DRV_OBJ_P_INS->txbuf[6] = (unsigned char)target->PCP_DATAEXCHANGE_OBJ_P_INS->T_dis_data;
                target->SCI_DRV_OBJ_P_INS->txbuf[6] = target->PCP_DATAEXCHANGE_OBJ_P_INS->T_dis_data;
                target->SCI_DRV_OBJ_P_INS->txbuf[7] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Prt_Status_H;
                target->SCI_DRV_OBJ_P_INS->txbuf[8] = target->PCP_DATAEXCHANGE_OBJ_P_INS->Prt_Status_L;
                checkdata = 0;
                for(checknum=0;checknum<9;checknum++)
                    checkdata+=target->SCI_DRV_OBJ_P_INS->txbuf[checknum];
                target->SCI_DRV_OBJ_P_INS->txbuf[9]=checkdata/256;
                target->SCI_DRV_OBJ_P_INS->txbuf[10]=checkdata%256;
                target->SCI_DRV_OBJ_P_INS->txnum = 11;
                break;

            default:
                break;
        }
        if(!Sci_tx_is_busy(target))
            {
                Sci_out_char(target);
            }
        //scitest++;
        Uint16 clcnum;
        for(clcnum=0;clcnum<12;clcnum++)
        {
            target->SCI_DRV_OBJ_P_INS->rxbuf[clcnum] = 0;
            target->SCI_DRV_OBJ_P_INS->txbuf[clcnum] = 0;
        }
    }
    //PWM enable Delay
    if (PWM_Startup_Flg == 1)
    {
        if(PWM_Startup_Cnt == 2)
        {
            EALLOW;
            EPwm2Regs.TZCLR.bit.OST = 1;
            EPwm1Regs.TZCLR.bit.OST = 1;
            EDIS;
            PWM_Startup_Cnt += 1;
        }
        else if(PWM_Startup_Cnt == 10)
        {
            EALLOW;
            Cla1Regs.MIER.bit.INT1 = 1;
            Cla1Regs.MIER.bit.INT2 = 1;
            EDIS;
            target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 1;
            GpioDataRegs.GPASET.bit.GPIO24 = 1;
            PWM_Startup_End_Flg = 1;
            PWM_Startup_Cnt += 1;
        }
        else if(PWM_Startup_Cnt < 10)//10*10ms
        {
            PWM_Startup_Cnt += 1;
        }
        else
        {
            ;
        }
    }
        //PWM disable Delay
    if (PWM_End_Flg == 1)
    {
        if(PWM_End_Cnt == 0)//10*10ms
        {
            EALLOW;
            Cla1Regs.MIER.bit.INT1 = 0;
            Cla1Regs.MIER.bit.INT2 = 0;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm1Regs.TZFRC.bit.OST = 1;
            EDIS;
            PWM_End_Cnt += 1;
        }
        else if(PWM_End_Cnt == 3)
        {
            target->IO_PACK_OBJ_P_INS->PWM_en_p->portdata = 0;
            EALLOW;
            Cla1Regs.MIFRC.bit.INT7 = 1;
            EDIS;
            PWM_End_Cnt += 1;
        }
        else if(PWM_End_Cnt < 10)//10*10ms
        {
            PWM_End_Cnt += 1;
        }
    }
}
