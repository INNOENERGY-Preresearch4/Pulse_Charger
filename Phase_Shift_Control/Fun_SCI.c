#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

Uint16 SM_StartupMSG_Flg = 0;
Uint16 SM_ShutdownMSG_Flg = 0;

void SCI_HANDLE_FUN(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    Uint16 checkdata,checknum;
    // if Receive FIFO is not empty, save data in rxbuf[]
    if(Sci_in_char(target))
    {
        //identify the type of a message: 82(StartUp), 80(SW protection Thrd), 70(HW protection Thrd), 67(PI parameter), 83(ShutDown), 68(transmit status)
        switch(target->SCI_DRV_OBJ_P_INS->rxbuf[0])
        {
        case 82:
            //checksum for validation
            if((target->SCI_DRV_OBJ_P_INS->rxbuf[7] * 256 + target->SCI_DRV_OBJ_P_INS->rxbuf[8])==(target->SCI_DRV_OBJ_P_INS->rxbuf[0] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[1] + target->SCI_DRV_OBJ_P_INS->rxbuf[2] + target->SCI_DRV_OBJ_P_INS->rxbuf[3] \
                    + target->SCI_DRV_OBJ_P_INS->rxbuf[4] + target->SCI_DRV_OBJ_P_INS->rxbuf[5] + target->SCI_DRV_OBJ_P_INS->rxbuf[6]))
            {
                SM_StartupMSG_Flg = 1;
                //Feedback Message to Screen
                target->SCI_DRV_OBJ_P_INS->txbuf[0]=123;
                target->SCI_DRV_OBJ_P_INS->txnum = 1;
                //since receive buffer could only store 8 digits, these data are transmitted separately
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
                SM_ShutdownMSG_Flg = 1;
                //Init reference value
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
        //send data in transmit buffer if it's not busy
        if(!Sci_tx_is_busy(target))
            {
                Sci_out_char(target);
            }
        //scitest++;
/*
        Uint16 clcnum;
        for(clcnum=0;clcnum<12;clcnum++)
        {
            target->SCI_DRV_OBJ_P_INS->rxbuf[clcnum] = 0;
            target->SCI_DRV_OBJ_P_INS->txbuf[clcnum] = 0;
        }
*/
    }

}
