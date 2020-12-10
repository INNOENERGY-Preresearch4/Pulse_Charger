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
            GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;//GPIO Pull Up
            GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;//GPIO Pull Up
            GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0b01;
            GpioCtrlRegs.GPAMUX2.bit.GPIO16  = 0b10;//Select GPIO16 Function: 0110 = 6 = SCIA_TX, page888
            GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0b01;
            GpioCtrlRegs.GPAMUX2.bit.GPIO17  = 0b10;//Select GPIO17 Function: 0110 = 6 = SCIA_RX, page888
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
    SCI_DRV_PTR->SCIFFTX.all        = 0xE020;//Transmitter Setting
    SCI_DRV_PTR->SCIFFRX.all        = 0x2000;//Receiver Setting
    SCI_DRV_PTR->SCIFFCT.all        = 0;     //disable Auto-Baud
    SCI_DRV_PTR->SCICTL1.all        = 0x23; //TX/RX Enable and Reset
    SCI_DRV_PTR->SCIPRI.bit.FREESOFT= 3;    //Freesoft Emulation Mode
    EDIS;
}

