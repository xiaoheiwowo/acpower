/*
 * File:   PIC24_TimerX.c
 * Author: liupengyu
 *
 * Created on 2017年8月4日, 下午1:26
 */


#include "p24EP32MC202.h"
#include "Macro.h"

char AN1StableMK = 0;//输出电压稳定标志位，=0不稳定，=1稳定
char AN0StableMK = 0;//给定信号稳定标志位，=0不稳定，=1稳定
int ADC_AN1[AdcArNum] = {0};//采样值保存数组
int ADC_AN0[AdcArNum] = {0};//采样值保存数组
int px = 0;//数组旧值保存位置标号
char _tn10 = 0;//100ms10次计数值
char _tf2 = 0; //1s到标志
char _tf3 = 0; //
int  _UserTM1 = 0;
int  _UserPR1 = 0;
char _UserTM1IF = 0;

/**********************************************************************************************
 *                                                                                            *
 *                                       定时器初始化                                         *
 *                                                                                            *
 *********************************************************************************************/

/******************************************************** 
函数名称：void InitT1()
功能描述：初始化Timer1
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：Fp=60MHz, TCKPS=64  T1CLK=0.9375MHz 周期为1.0666us PR1=46875 可以50ms中断一次。
********************************************************/ 
void InitT1()
{
//	TxCKPS[0]=Tx_CKPS;									   	        //保存当前分频比
	T1CONbits.TON=1;										//启停定时器
    T1CONbits.TCKPS=0B10;	                                //选择分频系数 【10】==>1:64
	T1CONbits.TCS=0;										//=【0】选择时钟源：FOSC/2时钟	=【1】 有T1ECS决定
//	IPC0bits.T1IP=3;										        //中断优先级3
	TMR1=0;	
    PR1=23585;                                              //25ms中断
	IFS0bits.T1IF=0;										//清除中断标志位
	IEC0bits.T1IE=1;										//允许T1中断
}

/******************************************************** 
函数名称：void InitT2()
功能描述：初始化Timer2
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：
********************************************************/ 
void InitT2()//中断为默认4级
{
    T2CONbits.TCS=0;         //内部时钟
    T2CONbits.TCKPS=0b11;    //Fcy256分频
    TMR2=0;                 //初值0      
    PR2=22157;             //1s匹配值
    _T2IF=0;                //T2中断标志位
    //  _T2IP=3;                              //优先级
    _T2IE=1;                //T2中断许可
    T2CONbits.TON=1;        //T2启动
}
/******************************************************** 
函数名称：void InitT3()
功能描述：初始化Timer3
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：
********************************************************/ 

void InitT3()//中断为默认4级
{
    T3CONbits.TCS=0;         //内部时钟
    T3CONbits.TCKPS=0b11;    //Fcy256分频
    
    TMR3=0;                 //初值0      
    PR3=234;             //1ms匹配值
    _T3IF=0;                //T3中断标志位
    _T3IE=1;                //T3中断许可
    T3CONbits.TON=1;        //T3启动
}

/**********************************************************************************************
 *                                                                                                                                *
 *                                                     定时器中断程序                                                  *
 *                                                                                                                                *
 * *******************************************************************************************/

/******************************************************** 
函数名称：void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
功能描述：判断电压值是否稳定,监控采样电压稳定子程序
全局变量：无 
参数说明：
返回说明：
设 计 人：
版    本：1.0 
说    明：定时器1中断程序
********************************************************/ 
void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
{
    int AN0Max,AN0Min,AN1Max,AN1Min;
    px++;
    if(px==AdcArNum)
    {
        px=0;
    }
    ADC_AN1[px]=Read_AN1();
    ADC_AN0[px]=Read_AN0();

    AN0Max=Max(ADC_AN0,AdcArNum);
    AN0Min=Min(ADC_AN0,AdcArNum);
    AN1Max=Max(ADC_AN1,AdcArNum);
    AN1Min=Min(ADC_AN1,AdcArNum);

    if(AN0Max-AN0Min<=DT0)
    {
        AN0StableMK=1;
    }
    else
    {
        AN0StableMK=0;//稳定标志位置0
    }
    if(AN1Max-AN1Min<=DT1)
    {
        AN1StableMK=1;
    }
    else
    {
        AN1StableMK=0;//稳定标志位置0s
    }

    TMR1=0;
    _T1IF=0;//清中断标志位
}


/******************************************************** 
函数名称：void __attribute__((__interrupt__,no_auto_psv)) _T2Interrupt(void)
功能描述：QEI模块使用
全局变量：无 
参数说明：
返回说明：
设 计 人：
版    本：1.0 
说    明：定时器1中断程序
********************************************************/ 
void __attribute__((__interrupt__,no_auto_psv)) _T2Interrupt(void)
{
    _tn10++;
    if (_tn10==10)
    {
        _tn10=0;
        T2CONbits.TON=0;     //T2模块停止运行，但中断没有禁止
        _tf2=1;             //置1s到标志位
    }
//  TMR2=0;//中断自动清零，不必手动清零
    _T2IF=0;                   //清中断标志位
}



/******************************************************** 
函数名称：void __attribute__((__interrupt__,no_auto_psv)) _T3Interrupt(void)
功能描述：判断电压值是否稳定,监控采样电压稳定子程序
全局变量：无 
参数说明：
返回说明：
设 计 人：
版    本：1.0 
说    明：定时器1中断程序
********************************************************/ 
void __attribute__((__interrupt__,no_auto_psv)) _T3Interrupt(void)
{   
    _tf3 = 1;             //置1ms到标志位
    if(_UserTM1 < _UserPR1)
    {
        _UserTM1++;//定时
    }
    else _UserTM1IF = 1;
    _T3IF = 0;                   //清中断标志位
}
/******************************************************** 
函数名称：void ResetUserTM1(int pr1)
功能描述：用户定时器
全局变量：无 
参数说明：
返回说明：
设 计 人：
版    本：1.0 
说    明：用户定时器1
********************************************************/ 
void ResetUserTM1(int pr1)
{
    //关中断？
    _UserTM1 = 0;
    _UserPR1 = pr1;
    _UserTM1IF = 0;
}