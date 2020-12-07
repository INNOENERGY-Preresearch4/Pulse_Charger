#include <Global.h>
#include "F28x_Project.h"
#include "SFO_V8.h"
#include "background.h"
#include "Cla_Interrupts.h"

void (*CLATASKptr[])(void)={claTask1,claTask2,claTask3,claTask4,claTask5,claTask6,claTask7,claTask8};

void CLA_DRV_SET(PCP_Driver_OBJ_p OBJ)
{
    PCP_Driver_OBJ* target = (PCP_Driver_OBJ*)OBJ;
    #if (FLASH)
        memcpy((uint32_t *)&Cla1ProgRunStart, (uint32_t *)&Cla1ProgLoadStart,
               (uint32_t)&Cla1ProgLoadSize );
        memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart,
               (uint32_t)&Cla1ConstLoadSize );
    #endif

    EALLOW;

    CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
    Cla1Regs.MIER.all = 0x00;
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU!=1){};

    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1!=1){};

    MemCfgRegs.LSxMSEL.bit.MSEL_LS0        = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0    = 1;//LS0 as Program memory

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1        = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1    = 0;//LS1 as Data memory
    MemCfgRegs.LSxMSEL.bit.MSEL_LS2        = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS2    = 0;//LS2 as Data memory

    #if (FLASH)
        MemCfgRegs.LSxMSEL.bit.MSEL_LS3        = 1;
        MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS3    = 0;//LS3 as Data memory
    #endif //defined(_FLASH)

    Uint16 num;
    Cla1Regs.MCTL.bit.IACKE = 1;
    for(num=0;num<target->CLA_DRV_OBJ_P_INS->length;num++)
    {
        CLA_DRV_PTR = (Uint16)CLATASKptr[(target->CLA_DRV_OBJ_P_INS->task[num]-1)];
        if((target->CLA_DRV_OBJ_P_INS->task[num]/4)==0)
            DmaClaSrcSelRegs.CLA1TASKSRCSEL1.all |= (target->CLA_DRV_OBJ_P_INS->source[num] << (8*(target->CLA_DRV_OBJ_P_INS->task[num]-1)));
        else
            DmaClaSrcSelRegs.CLA1TASKSRCSEL2.all |= (target->CLA_DRV_OBJ_P_INS->source[num] << (8*(target->CLA_DRV_OBJ_P_INS->task[num]-5)));
    }
    for(num=0;num<target->CLA_DRV_OBJ_P_INS->length;num++)
    {
        if(target->CLA_DRV_OBJ_P_INS->task[num] != 2)
        Cla1Regs.MIER.all |= 1<<(target->CLA_DRV_OBJ_P_INS->task[num]-1);
        Cla1Regs.MIFRC.all |= target->CLA_DRV_OBJ_P_INS->iack[num] <<(target->CLA_DRV_OBJ_P_INS->task[num]-1);
    }
    EDIS;
}
