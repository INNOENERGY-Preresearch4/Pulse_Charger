#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void SCI_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    EALLOW;
    switch(target->SCI_DRV_OBJ_P_INS->port)
    {
        case Port1:
            GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;//3 = Asynchronous

            GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;
            GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;
            GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0b01;
            GpioCtrlRegs.GPAMUX2.bit.GPIO16  = 0b10;//SCIA_TX
            GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0b01;
            GpioCtrlRegs.GPAMUX2.bit.GPIO17  = 0b10;//SCIA_RX
            break;
        default:break;
    }
    SCI_DRV_PTR->SCICTL1.all        = 0;//reset
    SCI_DRV_PTR->SCICCR.bit.SCICHAR = 0x7;//8bit - 255 max.
    SCI_DRV_PTR->SCICTL1.bit.RXENA  = 1;//enable RX
    SCI_DRV_PTR->SCICTL1.bit.TXENA  = 1;//enable TX
    //BRR = (SCIHBAUD << 8) + (SCILBAUD)
    //BRR = LSPCLK / (SCI Asynchronous Baud * 8) - 1
    SCI_DRV_PTR->SCIHBAUD.bit.BAUD  = 0xff & ( (target->SCI_DRV_OBJ_P_INS->lspclk/(target->SCI_DRV_OBJ_P_INS->baudrate * 8) - 1) >> 8 );
    SCI_DRV_PTR->SCILBAUD.bit.BAUD  = 0xff & (target->SCI_DRV_OBJ_P_INS->lspclk/(target->SCI_DRV_OBJ_P_INS->baudrate * 8) - 1);
    SCI_DRV_PTR->SCIFFTX.all        = 0xE020;
    SCI_DRV_PTR->SCIFFRX.all        = 0x2000;
    SCI_DRV_PTR->SCIFFCT.all        = 0;
    SCI_DRV_PTR->SCICTL1.all        = 0x23;
    SCI_DRV_PTR->SCIPRI.bit.FREESOFT= 3;
    EDIS;
}

