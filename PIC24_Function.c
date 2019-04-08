#include "p24EP32MC202.h"
#include "Macro.h"
#include "PIC24_Function.h"
ulong 	Fosc;
ulong 	Fcy;
ulong	FIN;
uchar 	M;
uchar   N1,N2;             //Fin 分频系数
uint    CTTimes;

/****************************************************** 
函数名称：Init_IO()
功能描述：初始化IO端口
全局变量：无 
参数说明：
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
*******************************************************/ 
void Init_IO()
{
	TRISA=0b0000000000000011;       //RA2作为PDIN_OUT输出
	TRISB=0b1111100011000100;       //
	ODCA=0x0000;					//=0正常输出，=1开漏输出
	ODCB=0x0000;					//=0正常输出，=1开漏输出
	LATB=0xFFFF;
	LATA=0xFFFF;
    ANSELA=0x03;
    ANSELB=0x00;
}
/****************************************************** 
函数名称：void PRINRXX_Init()
功能描述：初始化特殊复用的IO端口
全局变量：无 
参数说明：
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：
*******************************************************/ 
void PRINRXX_Init()
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); //clear the bit 6 of OSCCONL ==> Unlock Registers
    RPOR4bits.RP42R = 16;  
    _QEA1R=39;             //RB7为A波
    _QEB1R=38;             //RB6为B波
    __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to Lock Registers  
}

/****************************************************** 
函数名称：void Init_SYS()
功能描述：初始化系统参数
全局变量：无 
参数说明：
返回说明：无 
设 计 人：FEIHONG_KLD
版    本： 1.0 
说    明： www.kld.cn
*******************************************************/ 
void Init_SYS()
{	
	OSCTUNbits.TUN=0B000111;					   //默认7.37 *(1-0.047%)
	CLKDIVbits.PLLPRE=0;							//默认0	输入/2 =00001 = 输入/3 ---11111 = 输入/33
	CLKDIVbits.DOZEN=0;								//0 = 处理器时钟/ 外设时钟频率比强制为1:1
	CLKDIVbits.FRCDIV=0;							//0 = FRC 1 分频（默认）001 = FRC 2 分频010 = FRC 4 分频XX
	CLKDIVbits.PLLPOST=0;							//000 = 输出/2 01 = 输出/4 （默认）10 = 保留 11 = 输出/8
	PLLFBDbits.PLLDIV=63;                           //M=65,这样Fosc=7370000*65/4=119.7625MHz
    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001)
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1) {};
    N1=CLKDIVbits.PLLPRE+2;                                           //Fin分频系数
    N2=2*(CLKDIVbits.PLLPOST+1);                                      //Fin分频系数
	M=PLLFBDbits.PLLDIV+2;
	FIN=7366536;                                  //输入频率为7.37M
    Fosc=FIN*M/(N1*N2);                                 //VCO输出频率
    Fcy=Fosc/2;                                         //时钟频率	59.88MHz   TP=16.7ns
    CTTimes=(uint)(Fcy/MHZ(4));
	DelayMs(50);
}

/******************************************************** 
函数名称：void Init()
功能描述：初始化
全局变量：无 
参数说明：
返回说明：
设 计 人：
版    本：1.0 
说    明：
********************************************************/
void Init()
{
    Init_SYS();                                     //初始化系统参数
    Init_IO();                                      //初始化IO口
    PRINRXX_Init();                                 //端口映射
    Init_ADC();                                     //初始化ADC
    ADC_Enable(1);                                  //使能AD
    Init_PWM();                                     //初始化PWM输出
    InitQEI();                                      //初始化QEI
    InitT2();                                       //初始化Timer2
    InitT1();                                       //初始化Timer1
    InitT3();

    Flash_LED1(100,2);//初始化完成
}
