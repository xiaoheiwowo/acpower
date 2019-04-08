#include "Macro.h"

#ifndef PIC24EP_Function_H
#define PIC24EP_Function_H

#define T1_us			20000			//定义T1 为20000uS
#define	Tcytimes	Fosc/MHZ(4)             //所以定时时间应该/2特别注意 而不是周期，所以应该是FOSC/4

extern  ulong 	Fosc;
extern  ulong 	Fcy;
extern  ulong	FIN;
extern  uchar 	M;
extern  uchar   N1,N2;             //Fin 分频系数
extern  uint    CTTimes;

/***=======================================================
	==>>功能函数<<==
========================================================**/
extern	void Init_IO();                                 //端口设置
extern	void Init_SYS();                                //系统设置
extern  void PRINRXX_Init();                            //外部引脚映射
extern  void InitT1();
extern  void InitT3();//timer3初始化函数，设定1ms定时器_tf3是1ms到标志
extern  void Init();

#endif
