#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void IO_SET()
{
    EALLOW;
    //GPAQSEL2, GPIO Qualification Type (GPIO16 to GPIO31)
    (*(volatile Uint32*)(0x7C00+0x4))  = 0x8000;
    //GPAQSEL1, GPIO Qualification Type (GPIO0 to GPIO15)
    (*(volatile Uint32*)(0x7C00+0x2))  = 0xA20000;
    //GPBQSEL1, GPIO Qualification Type (GPIO32 to GPIO47)
    (*(volatile Uint32*)(0x7C00+0x42)) = 0x20028;
    //GPBQSEL2, GPIO Qualification Type (GPIO48 to GPIO59)
    (*(volatile Uint32*)(0x7C00+0x44)) = 0x80000;
    //GPAPUD, GPIO A Pull Up
    (*(volatile Uint32*)(0x7C00+0xC))  = 0xF4FFFFFF;
    //GPBPUD, GPIO B Pull Up
    (*(volatile Uint32*)(0x7C00+0x4C)) = 0xF2FFFF7F;
    //GPADIR, GPIO A Direction
    (*(volatile Uint32*)(0x7C00+0xA))  = 0xB000080;
    //GPBDIR, GPIO B Direction
    (*(volatile Uint32*)(0x7C00+0x4A)) = 0xD000080;
    //GPBINV, 0,1 inversion
    (*(volatile Uint32*)(0x7C00+0x50)) = 0b10;
    GpioCtrlRegs.GPAAMSEL.all          = 0;//GPIO A Analog Mode Select
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26   = 2;//6-sample qualification
    GpioCtrlRegs.GPAQSEL1.bit.GPIO11   = 2;//6-sample qualification
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1  = 5;//Qualification sampling period for GPIO8 to GPIO15, 5 = 10 SYSCLK cycles = 0.1us
    GpioCtrlRegs.GPACTRL.bit.QUALPRD2  = 5;//Qualification sampling period for GPIO16 to GPIO23, 5 = 10 SYSCLK cycles = 0.1us
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3  = 5;//Qualification sampling period for GPIO24 to GPIO31, 5 = 10 SYSCLK cycles = 0.1us
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0  = 5;//Qualification sampling period for GPIO32 to GPIO39, 5 = 10 SYSCLK cycles = 0.1us
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD1  = 5;//Qualification sampling period for GPIO40 to GPIO47, 5 = 10 SYSCLK cycles = 0.1us
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD2  = 5;//Qualification sampling period for GPIO48 to GPIO55, 5 = 10 SYSCLK cycles = 0.1us
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD3  = 5;//Qualification sampling period for GPIO56 to GPIO63, 5 = 10 SYSCLK cycles = 0.1us
    EDIS;
}
