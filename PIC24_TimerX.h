
/* **********************************************
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 ************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC24_TIMERX_H
#define	PIC24_TIMERX_H

#include "Macro.h"

extern  char    AN1StableMK;//�����ѹ�ȶ���־λ��=0���ȶ���=1�ȶ�
extern  char    AN0StableMK;//�����ź��ȶ���־λ��=0���ȶ���=1�ȶ�
extern  int     ADC_AN1[AdcArNum];//����ֵ��������
extern  int     ADC_AN0[AdcArNum];//����ֵ��������
extern  int     px;//�����ֵ����λ�ñ��
extern  char    _tf2;
extern  char    _tf3;
extern  int     _UserTM1;//500ms����
extern  char    _UserTM1IF;//500ms����־λ

extern  char    _tn10;
extern  void    InitT1();
extern  void    InitT2();
extern  void    InitT3();
extern  void    ResetUserTM1(int pr1);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

