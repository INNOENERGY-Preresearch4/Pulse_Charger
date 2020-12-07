#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void IO_SET()
{
    EALLOW;
    //GPAQSEL2
    (*(volatile Uint32*)(0x7C00+0x4))  = 0x8000;
    //GPAQSEL1
    (*(volatile Uint32*)(0x7C00+0x2))  = 0xA20000;
    //GPBQSEL1
    (*(volatile Uint32*)(0x7C00+0x42)) = 0x20028;
    //GPBQSEL2
    (*(volatile Uint32*)(0x7C00+0x44)) = 0x80000;
    //GPAPUD
    (*(volatile Uint32*)(0x7C00+0xC))  = 0xF4FFFFFF;
    //GPBPUD
    (*(volatile Uint32*)(0x7C00+0x4C)) = 0xF2FFFF7F;
    //GPADIR
    (*(volatile Uint32*)(0x7C00+0xA))  = 0xB000080;
    //GPBDIR
    (*(volatile Uint32*)(0x7C00+0x4A)) = 0xD000080;
    //GPBINV
    (*(volatile Uint32*)(0x7C00+0x50)) = 0b10;
    GpioCtrlRegs.GPAAMSEL.all          = 0;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1  = 5;//Period 0.1us (5*2 = 10 times) for GPIO8 - GPIO15
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26   = 2;//6-sample qualification
    GpioCtrlRegs.GPAQSEL1.bit.GPIO11   = 2;//6-sample qualification
    GpioCtrlRegs.GPACTRL.bit.QUALPRD2  = 5;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3  = 5;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0  = 5;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD1  = 5;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD2  = 5;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD3  = 5;
    EDIS;
}
