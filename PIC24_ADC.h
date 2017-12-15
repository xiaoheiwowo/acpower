#include "Macro.h"

#ifndef PIC24EP_ADC_H
#define PIC24EP_ADC_H

extern	void Init_ADC();									//AD初始化
extern	void ADC_Enable(uchar IsON);						//使能ADC
extern  int  Read_AN0();						//读取位置AD
extern  int  Read_AN1();						//读取位置AD

#endif
