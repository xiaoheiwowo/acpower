#include "p24EP32MC202.h"
#include "Macro.h"
#include "PIC24_PWM.h"
#include "Common.h"
#include "PIC24_Flash.h"
#include "GapTest.h"


int   _Position = 0;//0��������Ķ��λ��
int   _PosMax = 1875;
int   _Zero = 0;//0�㲹��
/******************************************************** 
�������ƣ�Init_PWM()
������������ʼ���Ƚ����PWM
ȫ�ֱ������� 
����˵����
����˵���� 
�� �� �ˣ�
��    ����1.0 
˵    ����ʹ�ö�ʱ��T1, 64��Ƶ OC1RS=18750ʱ��PWM����Ϊ20ms��
********************************************************/ 
void Init_PWM()
{
    OC1CON1 = 0x00;
    OC1CON2 = 0x00;
    OC1CON1bits.OCTSEL = 0x04;                              
    OC1CON1bits.OCM = 0x06;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1RS=18750;
//    OC1R=469;
}

/******************************************************** 
�������ƣ�void ServoAbsPOS(int wz)
��������������˶�������λ��
ȫ�ֱ������� 
����˵����Position
����˵����y
�� �� �ˣ�
��    ����1.0 
˵    �������ϴ��˶������෴�Ӳ�������ѹ���ھ���Լ0.5V.
********************************************************/ 
void ServoAbsPos(int wz)
{
    if(wz<_PosMax-_Zero)
    {
        if(wz-_Position>=0)//wz�仯4���²�����
        {
            OC1R = 469+_Zero+wz+_Gap;
        }
        else if(wz-_Position<0)
        {
            OC1R = 469+_Zero+wz;
        }
        _Position = wz;
    }
}
