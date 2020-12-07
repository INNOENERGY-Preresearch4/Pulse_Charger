#include "F28x_Project.h"
#include"emavg.h"
#ifndef BACKGROUND_H_
#define BACKGROUND_H_



extern void backgroundfa(void);
extern void backgroundfb(void);
extern void backgroundfc(void);

extern void backgroundfa1(void);
extern void backgroundfa2(void);
extern void backgroundfa3(void);
extern void backgroundfa4(void);


extern void backgroundfb1(void);
extern void backgroundfb2(void);
extern void backgroundfb3(void);
extern void backgroundfb4(void);


extern void backgroundfc1(void);
extern void backgroundfc2(void);
extern void backgroundfc3(void);
extern void backgroundfc4(void);

extern float Vin;
extern Uint16 In_Relay_flag;


extern EMAVG emavgTemp;
extern EMAVG emavgVin;
extern EMAVG emavgVo;
extern EMAVG emavgIo;
extern Uint16 otflag;
#endif /* BACKGROUND_H_ */
