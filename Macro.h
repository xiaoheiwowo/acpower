#ifndef __Macro_H__
#define __Macro_H__

/* ---------------------------------------------------------- */
/* KLD公共宏定义===开始 */
/* ---------------------------------------------------------- */
#define uchar 	unsigned char
#define uint  	unsigned int
#define ulong	unsigned long		//长整型

#define MHZ(x) 		x##000000				//用MHZ(x) 来代替x##000000
#define KHZ(x) 		x##000					//用KHZ(x) 来代替x##000

#define	AN0	   0					//控制信号AD转换通道 AN0
#define AN1    1                   //反馈信号AD转换通道 AN1

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
#define DT2      2//电压允许误差
#endif										//宏定义并列放在这里
