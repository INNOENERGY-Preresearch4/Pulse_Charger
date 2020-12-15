		.cdecls   C,LIST,"Cla_Interrupts.h"
		.cdecls   C,LIST,"CLAShared.h"
		.cdecls   C,LIST,"user_def.h"

		.if __TI_EABI__
		.asg	Cla_Avg_Run, _Cla_Avg_Run
		.asg	Cla_Pulse_Run, _Cla_Pulse_Run
		.asg	Cla_PCMC_Control_Run, _Cla_PCMC_Control_Run
		.asg	Cla_PS_Control_Run, _Cla_PS_Control_Run
		.endif

		.global _Cla_Avg_Run
		.global _Cla_PCMC_Control_Run
		.global _Cla_PS_Control_Run
		.global _Cla_Pulse_Run
   		.def	__cla_Cla_Fun_sp

SIZEOF_LFRAME		.set	2
LFRAME_MR3			.set	0

		.align 	2
__cla_Cla_Fun_sp	.usect ".scratchpad:Cla1Prog:_DCL_Cla_Fun_LSECT", SIZEOF_LFRAME, 0, 1
		.asg	 __cla_Cla_Fun_sp, LFRAME
		.sect 	"Cla1Prog:_Cla_Fun_LSECT"
;Avearge Function
_Cla_Avg_Run:

		MSETFLG 	RNDF32=1
		MMOV32		@LFRAME + LFRAME_MR3, MR3
		MNOP

		MMOV32      MR1,@0x436c ;EPWM4 CMPB Register
		MMOVZ16     MR0,@0x4304 ;EPWM4 TBCTR Register
		MMOVI32		MR2,#10000
		MADD32		MR1, MR1, MR2
		MLSR32      MR1, 16     ;take CMPB part: CMPB Register consists of CMPB(H16) and CMPBHR(L16)
		MCMP32      MR0, MR1    ;compare TBCTR and  CMPB, and set state
		MNOP
		MNOP
		MNOP
		MBCNDD      Skip1, GEQ  ;if state = 1, skip
		MNOP
		MNOP
		MNOP
;		MMOVI32     MR0,#0x01000000
;		MMOVZ16     MR1,@0x7f04
;		MOR32       MR0,MR0,MR1
;		MMOV32      @0x7f04,MR0
		MNOP
		MNOP
		MMOVI32     MR3,#0x1
		MMOV16      @0x7404,MR3 ;ADC Interrupt Flag Clear Register, Enabling Further Interrupts
		MRCNDD		UNC
		MNOP
		MNOP
		MNOP
		MMOV32		MR3, @LFRAME + LFRAME_MR3
Skip1:

		MUI16TOF32  MR1,@_AdcaResultRegs.ADCRESULT3 ;Assign ADC result of Io[n] to MR1
		MMPYF32     MR1, MR1, #42.9268 ;calibration, MR1 = ADCResult*42.9268
	    MMOV32      MR2,*MAR0[4]++ ;MR2 =  out, after: MAR0 = mult2
	    MMOV32      MR0,*MAR0[-2]++ ;MR0 =  mult2, after: MAR0 = mult
	    MMPYF32		MR0, MR2, MR0   ;MR0 = out*mult2
		MSUBF32     MR1,MR1,MR2 ; MR1 =  (ADCResult*42.9268 - out)
		MMOV32      MR3,*MAR0[-2]++ ;MR3 =  mult, after: MAR0 = out
		MMPYF32     MR1,MR1,MR3 ; MR1 =  mult * (ADCResult*42.9268 - out)
		MADDF32     MR1,MR1,MR0 ; MR1 =  mult * (ADCResult*42.9268 - out) +  out*mult2
		MMOVI32     MR3,#0x1
		MMOV32      *MAR0,MR1	; out =  mult * (ADCResult*42.9268 - out) +  out*mult2
		MMOV16      @0x7404,MR3 ;ADC Interrupt Flag Clear Register, Enabling Further Interrupts

		MRCNDD		UNC
		MNOP
		MNOP
		MNOP
		MMOV32		MR3, @LFRAME + LFRAME_MR3
		;.unasg      LFRAME

;Peak Current Control Function (Not used)
_Cla_PCMC_Control_Run:
		MSETFLG 	RNDF32=1
		MMOV32		@LFRAME + LFRAME_MR3, MR3
		MNOP

		MMOV32      MR1,*MAR0
		MMOV32      @_io,MR1
		MMPYF32     MR1,MR1,#0.00045777;I_scaling_Ctr/RX_SC_General calibration = 30/65535 = 0.00045777
		MSUBF32     MR0,MR0,MR1   ;K0 = (Iref-Io*0.00045777)
		MMOV32      MR1,*MAR1[2]++ ;before: MAR1 = b0, after: MAR1 = b1
		MMPYF32     MR1,MR0,MR1   ;K1 = b0*(Iref-Io*0.00045777)
		MMOV32      MR2,*MAR1[4]++ ;before: MAR1 = b1, after: MAR1 = ek1
		MMOV32      MR3,*MAR1
		MMPYF32     MR2,MR2,MR3   ;K2 = b1*ek1
		MMOV32      *MAR1[-2]++,MR0 ;before: MAR1 = ek1, after: MAR1 = a1, ek1_new = (Iref-Io)*0.00045777
		MADDF32     MR1,MR1,MR2   ;K3 = K1 + K2 = b0*(Iref-Io)*0.00045777 + b1*ek1
		MMOV32      MR2,*MAR1[4]++ ;before: MAR1 = a1, after: MAR1 = uk1
		MMOV32      MR3,*MAR1[2]++ ;before: MAR1 = uk1, after: MAR1 = umax
		MMPYF32     MR2,MR2,MR3   ;K4 = a1*uk1
		MADDF32     MR1,MR1,MR2 ;K5 = K3 + K4 = b0*(Iref-Io*0.00045777) + b1*ek1 + a1*uk1
		MMOV32      MR2,*MAR1[2]++ ;before: MAR1 = umax, after: MAR1 = umin
		MMOV32      MR3,*MAR1[-4]++ ;before: MAR1 = umin, after: MAR1 = uk1
		MMINF32     MR1,MR2 ; K6 = min(K5, umax)
		MMAXF32     MR1,MR3 ; K7 = min(K6, umin)
		MMOV32      *MAR1,MR1 ;uk1 = K7
		MMPYF32     MR1,MR1,#65520.0
		MF32TOUI16  MR2,MR1
		MMOV32      @0x5D4A,MR2 ;Ramp Shadow Resgister
		MMOVI32     MR3,#0x1
		MMOV32      @0x43AA,MR3 ;EPWM4 Event Trigger Clear Register
		MRCNDD		UNC
		MNOP
		MNOP
		MNOP
		MMOV32		MR3, @LFRAME + LFRAME_MR3

;		.unasg      LFRAME

;Phase Shift Control Function (Currently used)
_Cla_PS_Control_Run:
		MSETFLG 	RNDF32=1
		MMOV32		@LFRAME + LFRAME_MR3, MR3
		MNOP

		MMOV32      MR1,*MAR0
		MMOV32      @_io,MR1
		MMPYF32     MR1,MR1,#0.00045777;30/65535 calibration
		MSUBF32     MR0,MR0,MR1   ;K0 = (Iref-Io*0.00045777)
		MMOV32      MR1,*MAR1[2]++ ;before: MAR1 = b0, after: MAR1 = b1
		MMPYF32     MR1,MR0,MR1   ;K1 = b0*(Iref-Io*0.00045777)
		MMOV32      MR2,*MAR1[4]++ ;before: MAR1 = b1, after: MAR1 = ek1
		MMOV32      MR3,*MAR1
		MMPYF32     MR2,MR2,MR3   ;K2 = b1*ek1
		MMOV32      *MAR1[-2]++,MR0 ;before: MAR1 = ek1, after: MAR1 = a1, ek1_new = (Iref-Io)*0.00045777
		MADDF32     MR1,MR1,MR2   ;K3 = K1 + K2 = b0*(Iref-Io*0.00045777) + b1*ek1
		MMOV32      MR2,*MAR1[4]++ ;before: MAR1 = a1, after: MAR1 = uk1
		MMOV32      MR3,*MAR1[2]++ ;before: MAR1 = uk1, after: MAR1 = umax
		MMPYF32     MR2,MR2,MR3   ;K4 = a1*uk1
		MADDF32     MR1,MR1,MR2 ;K5 = K3 + K4 = b0*(Iref-Io*0.00045777) + b1*ek1 + a1*uk1
		MMOV32      MR2,*MAR1[2]++ ;before: MAR1 = umax, after: MAR1 = umin
		MMOV32      MR3,*MAR1[-4]++ ;before: MAR1 = umin, after: MAR1 = uk1
		MMINF32     MR1,MR2 ; K6 = min(K5, umax)
		MMAXF32     MR1,MR3 ; K7 = min(K6, umin)
		MMOV32      *MAR1,MR1 ;uk1 = K7
		MMOVI32     MR2,#0x01F3 ;PRD
		MI16TOF32   MR2,MR2
		MMPYF32     MR2,MR2,MR1
		MF32TOUI16  MR2,MR2
;		MLSL32      MR2,#16
;		MMOV32      @0x4160,MR2;EPWM2 TBPHS Register
		MLSL32      MR2,#8
		MMOVI32     MR0,#0x00FF0000
		MAND32		MR1, MR2, MR0
		MMOV32      @0x4160,MR1;EPWM2 TBPHS Register
		MLSR32      MR2,#8
		MMOVI32     MR0,#0x000000FF
		MAND32		MR1, MR2, MR0
		MMOV32      @0x412E,MR1;EPWM2 TRREM Register
		MMOVI32     MR3,#0x1
		MMOV32      @0x43AA,MR3;EPWM4 Event Trigger Clear Register
		MRCNDD		UNC
		MNOP
		MNOP
		MNOP
		MMOV32		MR3, @LFRAME + LFRAME_MR3

;		.unasg      LFRAME

;Pulse Generator (Not used)
_Cla_Pulse_Run:
		MSETFLG 	RNDF32=1
		MMOV32		@LFRAME + LFRAME_MR3, MR3
		MNOP

		MMOVI32     MR1,#0x4
		MNOP
		MNOP
		MNOP
		MNOP
		MNOP
		MEALLOW
		MNOP
		MNOP
		MMOV32      @0x409B, MR1
;		MMOVI32     MR1,#0x4
		MMOV32      @0x419B, MR1
		MNOP
		MNOP
		MEDIS

		MRCNDD		UNC
		MNOP
		MNOP
		MNOP
		MMOV32		MR3, @LFRAME + LFRAME_MR3

		.unasg      LFRAME
