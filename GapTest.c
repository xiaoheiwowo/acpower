#include "GapTest.h"
#include <p24EP32MC202.h>
#include <stdlib.h>
#include "PIC24_ADC.h"
#include "PIC24_PWM.h"
#include "PIC24_Function.h"
#include "Common.h"
#include "PIC24_TimerX.h"

#define PWM_ONE_R 1875//һ��360�ȶ�Ӧpwm��������
#define TM 250//150ms //ad�������
#define DV 0.8//1.0v  //��ѹ�ȶ��������Сֵ
int _Gap;//����ת��϶
char _ad_change(int ad)//����˶����ѹad2���˶�ǰad�Ƿ�仯�������仯����1���䷵��0
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
char _ad_change_tar()//����˶����ѹad��TMʱ���ad2�Ƿ�仯���ȶ�����1���ȶ�����0
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
�������ƣ�ServoMove(int step)             
����������������λ��
ȫ�ֱ������� 
����˵����step
����˵����
�� �� �ˣ�
��    ����1.0 
˵    ����
********************************************************/ 
void ServoMove(int step)                                   
{
    OC1RS = 18750;                                          //����20ms   t=0.4us
    OC1R  = OC1R+step;                                      //�ߵ�ƽ
}



void gapsize(char *M,char *N)//�������¼�϶M�����ϼ�϶N�ı���ָ�롣
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
    gapsize(&m,&n);//�����Լ�
    _Gap=(m+n)/2;
    return _Gap;
}