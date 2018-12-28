#include    "p24EP32MC202.h"
#include    "PIC24_QEI.h"
#include    "PIC24_ADC.h"
#include    "PIC24_Function.h"
#include    "PIC24_PWM.h"
#include    "Common.h"
#include    "PIC24_Flash.h"
#include    "Macro.h"
#include    "GapTest.h"
#include    "PIC24_TimerX.h"


// PIC24EP32MC202 Configuration Bit Settings

// 'C' source line config statements

// FICD
#pragma config ICS = PGD3               // ICD Communication Channel Select bits (Communicate on PGEC3 and PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECME           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are enabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config PWMLOCK = ON             // PWM Lock Enable bit (Certain PWM registers may only be written after key sequence)
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


int    sourcebak[6] = {0,1609,3867,3788,36,1875};//������������
long   _Uset = 0;
long   _Uout = 0;
float  _RatioUset_AN0 = 0;
float  _RatioUout_AN1 = 0;
float  _RatioUout_Servo = 0;
char   _FirstFinish = 0;

int main()
{
    Init();//��ʼ��
    _copy_rom_source();//��ȡrom�е�source
/**********************************���Դ��뿪ʼ**************************************/

/**********************************���Դ������**************************************/
    char A = 0;//������־λ

    int Ts = 0;
    AN0StableMK = 0;
    AN1StableMK = 0;
    while(1)
    {
        _Zero = source[0];//�¼���λ��
        _Gap = source[4];//����ת��϶
        _RatioUset_AN0 = source[3]/300.0;//����5v�źż���õ�
        _RatioUout_AN1 = source[2]/305.0;//�����źŲ�������õ�
        _RatioUout_Servo = source[1]/305.0;
        _PosMax = source[5];//�ϼ���λ��
        if(SET == 1)                                   //���set��ť
        {
            LED1 = 1;
            LED2 = 1;
            if(A == 0)
            {
                _eraseflash();//����ҳ
                _wr_source_rom();//��source�������ROM
                A=1;
            }
            else
            {
                if(AN0StableMK == 0)
                {
                    _Uset = Read_AN0()/_RatioUset_AN0;
                    Ts = _Uset*_RatioUout_Servo;
                    ServoAbsPos(Ts);//�״ε���
                    ResetUserTM1(100);//��ʱ100ms
                }
                else
                {
                    if(_UserTM1IF == 1)
                    {
                        if(AN1StableMK == 1)
                        {
                            _Uset = Read_AN0()/_RatioUset_AN0;
                            _Uout = Read_AN1()/_RatioUout_AN1;
                            if(_Uset<5)
                            {
                                ServoAbsPos(5);
                            }
                            else if(Abs_Subb(_Uset,_Uout)>DT2)
                            {
                                _Uout = Read_AN1()/_RatioUout_AN1;
                                Ts = _Position*_Uset/_Uout;
                                ServoAbsPos(Ts);//��������
                                ResetUserTM1(1000);//1s����һ��
                                Flash_LED(100,2);
                            }
                        }
                    }
                }
            }
        }
        else if(SET==0)
        {
            LED1 = 0;
            LED2 = 1;
            SET_CPU_IPL(7);
            if(_abnumber != 0)
            {
                if(OC1R<2350)
                {
                    OC1R = OC1R + _abnumber;
                }
            }
//            ServoAbsPos(_Position+_abnumber);                               //������λ��
            _abnumber = 0;
            SET_CPU_IPL(3);
            if(K1 == 0 && K2 != 0 && K3 != 0 && GAP != 0 && EXT != 0)//����0��ѹ���λ��
            {
                if(A == 0)
                {
                    source[0] = OC1R-469;//_Zero����
                    A = 1;
                    Flash_LED(100,2);
                }
            }
            else if(K2 == 0 && K1 != 0 && K3 != 0 && GAP != 0 && EXT != 0)//����305V�Ĳ���
            {
                if(A == 0)
                {
                    source[1] = OC1R-469-source[0];//305vλ��
                    source[2] = Read_AN1();//305v ad����ֵ
                    source[5] = source[2];
                    A = 1;
                    Flash_LED(100,2);
                    //��150V����
                }
            }
            else if(K3 == 0 && K2 != 0 && K1 != 0 && GAP != 0 && EXT != 0)//��������źŵ�5vʱ�Ĳ���ֵ
            {
                if(A == 0)
                {
                    source[3] = Read_AN0();
                    A = 1;
                    Flash_LED(100,2);
                    //��300V����
                }
            }
            else if(GAP == 0 && K2 != 0 && K3 != 0 && K1 != 0 && EXT != 0)//����϶
            {
                if(A == 0)
                {
                    source[4] = _GapTest();
                    Flash_LED(100,2);
                    A = 1;
                }
            }
            else if(EXT == 0 && GAP != 0 && K2 != 0 && K3 != 0 && K1 != 0)//�ָ�Ĭ������
            {
                if(A == 0)
                {
                    source[0] = sourcebak[0];
                    source[1] = sourcebak[1];
                    source[2] = sourcebak[2];
                    source[3] = sourcebak[3];
                    source[4] = sourcebak[4];
                    source[5] = sourcebak[5];
                    Flash_LED(100,2);
                    A = 1;
                }
            }
            else A = 0;
        }
    }
}
