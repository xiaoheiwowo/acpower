#include "p24EP32MC202.h"
#include "Macro.h"
#include "PIC24_PWM.h"
#include "Common.h"
#include "PIC24_Flash.h"
#include "GapTest.h"

int _Position = 0; //0点修正后的舵机位置
int _PosMax = 1875;
int _Zero = 0; //0点补偿
/******************************************************** 
函数名称：Init_PWM()
功能描述：初始化比较输出PWM
全局变量：无 
参数说明：
返回说明： 
设 计 人：
版    本：1.0 
说    明：使用定时器T1, 64分频 OC1RS=18750时，PWM周期为20ms，
********************************************************/
void Init_PWM()
{
    OC1CON1 = 0x00;
    OC1CON2 = 0x00;
    OC1CON1bits.OCTSEL = 0x04;
    OC1CON1bits.OCM = 0x06;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1RS = 18750;
    //    OC1R=469;
}

/******************************************************** 
函数名称：void ServoAbsPOS(int wz)
功能描述：舵机运动到绝对位置
全局变量：无 
参数说明：Position
返回说明：y
设 计 人：
版    本：1.0 
说    明：与上次运动方向相反加补偿，电压调节精度约1V.
********************************************************/
void ServoAbsPos(int wz)
{
    if (wz < _PosMax - _Zero)
    {
        if (wz - _Position >= 0) //wz变化4以下不动作
        {
            OC1R = 469 + _Zero + wz + _Gap;
        }
        else if (wz - _Position < 0)
        {
            OC1R = 469 + _Zero + wz;
        }
        _Position = wz;
    }
}
