#include "p24EP32MC202.h"
#include "Macro.h"
#include "GapTest.h"
#include "PIC24_Function.h"
#include "Common.h"
#include "PIC24_TimerX.h"

/**************************************************
函数名称：void DelayUs(uint x)
函数功能：延时函数
入口参数：x
出口参数：无
备注：  
**************************************************/
void DelayUs(uint x)
{
    uint k;
    for (k = x * 4 * CTTimes; k > 0; k--)
        ;
}
/********************************************************
函数名称：DelayMs(uint x)
函数功能：延时函数
入口参数：x	
出口参数：无
备注：i<89 毫秒延时 精度0.5%	x=1时延时1ms	4M晶振
      i<89x4                                    16M 
**************************************************/
void DelayMs(uint x)
{
    ulong k;
    uint j;
    for (j = 20 * x; j > 0; j--)
    {
        for (k = KHZ(416.7); k > 0; k--)
            ClrWdt();
    }
}

void delaytime(int timerprod) //延时子程序，timerprod为需要延时的ms值
{
    int i = 0;
    while (i != timerprod)
    {
        if (_tf3 == 1)
        {
            _tf3 = 0;
            i++;
        }
    }
}
/******************************************************** 
函数名称：Flash_LED(uchar LED,uint Time,uchar FlashTimes)
功能描述：LED闪烁
全局变量：无 
参数说明：LED,TIME FLASHTIME
返回说明：
设 计 人：
版    本：1.0 
说    明：www.kld.cn
********************************************************/
void Flash_LED(uint Time, uchar FlashTimes)
{
    uchar i;
    for (i = 0; i < FlashTimes; i++)
    {
        LED2 = 0;
        delaytime(Time);
        LED2 = 1;
        delaytime(Time);
    }
}

void Flash_LED1(uint Time, uchar FlashTimes)
{
    uchar i;
    for (i = 0; i < FlashTimes; i++)
    {
        LED1 = 0;
        delaytime(Time);
        LED1 = 1;
        delaytime(Time);
    }
}
/**************************************************
函数名称：Abs_Subb(int a,int b)
函数功能：求绝对值差
入口参数：T,Time	
出口参数：无
备注：
**************************************************/
uint Abs_Subb(int a, int b)
{
    if (a >= b)
        return a - b;
    else
        return b - a;
}

/***************************************************
函数名称：uint CountAvg_Advalue(uint *ADV,uchar Times)
入口参数：无
出口参数：无
备注：去掉一个最大值，一个最小值，然后求平均值
**************************************************/
uint CountAvg_Advalue(uint *ADV, uchar Times)
{
    uchar i;
    uint Min, Max;
    Min = ADV[0];
    Max = Min;
    for (i = 0; i < Times; i++)
    {
        if (ADV[i] < Min)
            Min = ADV[i]; //统计最小值
        if (ADV[i] > Max)
            Max = ADV[i]; //统计最大值
    }
    uint CountAll = 0;
    uint Avg = 0;
    for (i = 0; i < Times; i++)
        CountAll = CountAll + ADV[i]; //统计总数
    CountAll = CountAll - Min - Max;  //去掉最大值，去掉最小值
    Avg = CountAll / (Times - 2);     //求平均值
    return Avg;
}

/***************************************************
函数名称：int Max(int *array, int arrayNum)
入口参数：无
出口参数：无
备注：最大值
**************************************************/
int Max(int *array, int arrayNum)
{
    int max;
    int min;
    if (arrayNum == 0)
    {
        return -1;
    }
    else
    {
        max = array[0];
        min = array[0];
        int i;
        for (i = 0; i < arrayNum; i++)
        {
            if (max < array[i])
            {
                max = array[i];
            }
            if (min > array[i])
            {
                min = array[i];
            }
        }
        return max;
    }
}

/***************************************************
函数名称：int Min(int *array, int arrayNum)
入口参数：无
出口参数：无
备注：最小值
**************************************************/
int Min(int *array, int arrayNum)
{
    int max;
    int min;
    if (arrayNum == 0)
    {
        return -1;
    }
    else
    {
        max = array[0];
        min = array[0];
        int i;
        for (i = 0; i < arrayNum; i++)
        {
            if (max < array[i])
            {
                max = array[i];
            }
            if (min > array[i])
            {
                min = array[i];
            }
        }
        return min;
    }
}