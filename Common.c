#include "p24EP32MC202.h"
#include "Macro.h"
#include "GapTest.h"
#include "PIC24_Function.h"
#include "Common.h"
#include "PIC24_TimerX.h"



/**************************************************
�������ƣ�void DelayUs(uint x)
�������ܣ���ʱ����
��ڲ�����x
���ڲ�������
��ע��  
**************************************************/
void DelayUs(uint x)	
 {
	uint k;
	for (k=x*4*CTTimes;k>0;k--);
 }
/********************************************************
�������ƣ�DelayMs(uint x)
�������ܣ���ʱ����
��ڲ�����x	
���ڲ�������
��ע��i<89 ������ʱ ����0.5%	x=1ʱ��ʱ1ms	4M����
      i<89x4                                    16M 
**************************************************/
void DelayMs(uint x)
{
	ulong k;
    uint j;
	for (j=20*x;j>0;j--)
	{
		for (k=KHZ(416.7);k>0;k--)ClrWdt();
	}
}

void delaytime(int timerprod)//��ʱ�ӳ���timerprodΪ��Ҫ��ʱ��msֵ
{
    int i=0;
    while(i!=timerprod)
    {
        if(_tf3==1)
        {
            _tf3=0;
            i++;
        }
    }
}
/******************************************************** 
�������ƣ�Flash_LED(uchar LED,uint Time,uchar FlashTimes)
����������LED��˸
ȫ�ֱ������� 
����˵����LED,TIME FLASHTIME
����˵����
�� �� �ˣ�
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Flash_LED(uint Time,uchar FlashTimes)
{
    uchar i;
    for(i=0;i<FlashTimes;i++)
    {
        LED2=0;
        delaytime(Time);
        LED2=1;
        delaytime(Time);
    }
}

void Flash_LED1(uint Time,uchar FlashTimes)	
{
    uchar i;
    for(i=0;i<FlashTimes;i++)
    {
        LED1=0;
        delaytime(Time);
        LED1=1;
        delaytime(Time);
    }
}
/**************************************************
�������ƣ�Abs_Subb(int a,int b)
�������ܣ������ֵ��
��ڲ�����T,Time	
���ڲ�������
��ע��
**************************************************/
uint Abs_Subb(int a ,int b)
{
    if (a>=b) return a-b;
    else return b-a;
}

/***************************************************
�������ƣ�uint CountAvg_Advalue(uint *ADV,uchar Times)
��ڲ�������
���ڲ�������
��ע��ȥ��һ�����ֵ��һ����Сֵ��Ȼ����ƽ��ֵ
**************************************************/
uint CountAvg_Advalue(uint *ADV,uchar Times)
{
	uchar i;
	uint Min,Max;
	Min=ADV[0];
	Max=Min;
	for(i=0;i<Times;i++) 
	{
		 if(ADV[i]<Min) Min=ADV[i];						//ͳ����Сֵ
		 if(ADV[i]>Max) Max=ADV[i];						//ͳ�����ֵ
	}
	uint CountAll=0;
	uint Avg=0;
	for(i=0;i<Times;i++) CountAll=CountAll+ADV[i]; 		//ͳ������
	CountAll=CountAll-Min-Max;							//ȥ�����ֵ��ȥ����Сֵ
	Avg=CountAll/(Times-2);								//��ƽ��ֵ
	return Avg;
}

/***************************************************
�������ƣ�int Max(int *array, int arrayNum)
��ڲ�������
���ڲ�������
��ע�����ֵ
**************************************************/
int Max(int *array, int arrayNum)
{    
    int max;    
    int min;
    if(arrayNum==0)
    {
        return -1;
    }
    else
    {
        max=array[0];
        min=array[0];
        int i;
        for(i=0; i<arrayNum; i++)
        { 
           if(max<array[i])
            {
                max=array[i];
            }
            if(min>array[i])
            { 
               min=array[i];
            } 
       }
       return max;
    }
}

/***************************************************
�������ƣ�int Min(int *array, int arrayNum)
��ڲ�������
���ڲ�������
��ע����Сֵ
**************************************************/
int Min(int *array, int arrayNum)
{    
    int max;    
    int min;
    if(arrayNum==0)
    {
        return -1;
    }
    else
    {
        max=array[0];
        min=array[0];
        int i;
        for(i=0; i<arrayNum; i++)
        { 
           if(max<array[i])
            {
                max=array[i];
            }
            if(min>array[i])
            { 
               min=array[i];
            }
       }
       return min;
    }
}