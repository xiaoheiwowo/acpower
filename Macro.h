#ifndef __Macro_H__
#define __Macro_H__

/* ---------------------------------------------------------- */
/* KLD�����궨��===��ʼ */
/* ---------------------------------------------------------- */
#define uchar 	unsigned char
#define uint  	unsigned int
#define ulong	unsigned long		//������

#define MHZ(x) 		x##000000				//��MHZ(x) ������x##000000
#define KHZ(x) 		x##000					//��KHZ(x) ������x##000

#define	AN0	   0					//�����ź�ADת��ͨ�� AN0
#define AN1    1                   //�����ź�ADת��ͨ�� AN1

#define SET   PORTBbits.RB15
#define K1    PORTBbits.RB14
#define K2    PORTBbits.RB13
#define K3    PORTBbits.RB12
#define GAP   PORTBbits.RB11
#define EXT   PORTBbits.RB2
#define LED1  LATBbits.LATB8
#define LED2  LATBbits.LATB9
#define AdcArNum 10
#define DT0      13
#define DT1      13
#define DT2      3//��ѹ�������
#endif										//�궨�岢�з�������
