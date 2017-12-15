#include "GapTest.h"
#include <p24EP32MC202.h>
#include <stdlib.h>
#include "PIC24_ADC.h"
#include "PIC24_PWM.h"
#include "PIC24_Function.h"
#include "Common.h"
#include "PIC24_TimerX.h"

#define PWM_ONE_R 1875//一周360度对应pwm波脉冲数
#define TM 250//150ms //ad采样间隔
#define DV 0.8//1.0v  //电压稳定后许可最小值
int _Gap;//正反转间隙
char _ad_change(int ad)//舵机运动后电压ad2与运动前ad是否变化函数，变化返回1不变返回0
{
    int ad2;
    int dad,dadvn;
    delaytime(TM);
    ad2=Read_AN1();
    dad=abs(ad2-ad);
    dadvn=(int)(12.4*DV);
    if(dad>=dadvn)
    {
       return 1;
    }
    else
    {
       return 0;
    }
}
char _ad_change_tar()//舵机运动后电压ad与TM时间后ad2是否变化，稳定返回1不稳定返回0
{
    int ad2,ad;
    int dad,dadvn;
    ad=Read_AN1();
    delaytime(TM);
    ad2=Read_AN1();
    dad=abs(ad2-ad);
    dadvn=(int)(12.4*DV);
    if(dad>=dadvn)
    {
       return 0;
    }
    else
    {
       return 1;
    }    
}
/******************************************************** 
函数名称：ServoMove(int step)             
功能描述：舵机相对位置
全局变量：无 
参数说明：step
返回说明：
设 计 人：
版    本：1.0 
说    明：
********************************************************/ 
void ServoMove(int step)                                   
{
    OC1RS = 18750;                                          //周期20ms   t=0.4us
    OC1R  = OC1R+step;                                      //高电平
}



void gapsize(char *M,char *N)//返回向下间隙M和向上间隙N的变量指针。
{
    int ad1;
    
    ServoAbsPos(PWM_ONE_R/2);
    
    while(!_ad_change_tar())
    {
        delaytime(TM);
    }
    ServoMove(152);
    delaytime(300);
    while(!_ad_change_tar())
    {
        delaytime(TM);
    }
    *M=0;

    ad1=Read_AN1();
    while(1)
    {
        
        ServoMove(-1);
        if(!_ad_change(ad1))
        {
            *M=*M+1;
        }
        else
        {
            break;
        }
    }
    Flash_LED(100,1);
    
    ServoMove(-152);
    delaytime(300);
    while(!_ad_change_tar())
    {
        delaytime(TM);
    }
    ad1=Read_AN1();
    *N=0;
    while(1)
    {
        
        ServoMove(1); 
        if(!_ad_change(ad1))
        {
            *N=*N+1;
        }
        else
        {
            break;
        }
    }
}

int _GapTest()
{
    char m,n;
    gapsize(&m,&n);//运行自检
    _Gap=(m+n)/2;
    return _Gap;
}