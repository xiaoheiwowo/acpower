#include	"Macro.h"


#ifndef COMMON_H
#define COMMON_H

/****====================================================
��2�����ܺ�����������
====================================================****/
extern  void DelayUs(uint x);
extern	void DelayMs(uint x);
extern  void delaytime(int timerprod);//��ʱ�ӳ���timerprodΪ��Ҫ��ʱ��msֵ
extern	uint Abs_Subb(int a,int b);
extern	uint CountAvg_Advalue(uint *ADV,uchar Times);

extern  void Flash_LED(uint Time,uchar FlashTimes);
extern  void Flash_LED1(uint Time,uchar FlashTimes);
extern  int Max(int *array, int arrayNum);
extern  int Min(int *array, int arrayNum);

#endif

