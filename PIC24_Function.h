#include "Macro.h"

#ifndef PIC24EP_Function_H
#define PIC24EP_Function_H

#define T1_us			20000			//����T1 Ϊ20000uS
#define	Tcytimes	Fosc/MHZ(4)             //���Զ�ʱʱ��Ӧ��/2�ر�ע�� ���������ڣ�����Ӧ����FOSC/4

extern  ulong 	Fosc;
extern  ulong 	Fcy;
extern  ulong	FIN;
extern  uchar 	M;
extern  uchar   N1,N2;             //Fin ��Ƶϵ��
extern  uint    CTTimes;

/***=======================================================
	==>>���ܺ���<<==
========================================================**/
extern	void Init_IO();                                 //�˿�����
extern	void Init_SYS();                                //ϵͳ����
extern  void PRINRXX_Init();                            //�ⲿ����ӳ��
extern  void InitT1();
extern  void InitT3();//timer3��ʼ���������趨1ms��ʱ��_tf3��1ms����־
extern  void Init();

#endif
