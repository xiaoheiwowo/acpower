#include "Macro.h"

#ifndef PIC24EP_ADC_H
#define PIC24EP_ADC_H

extern	void Init_ADC();									//AD��ʼ��
extern	void ADC_Enable(uchar IsON);						//ʹ��ADC
extern  int  Read_AN0();						//��ȡλ��AD
extern  int  Read_AN1();						//��ȡλ��AD

#endif
