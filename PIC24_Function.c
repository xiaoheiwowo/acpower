#include "p24EP32MC202.h"
#include "Macro.h"
#include "PIC24_Function.h"
ulong 	Fosc;
ulong 	Fcy;
ulong	FIN;
uchar 	M;
uchar   N1,N2;             //Fin ��Ƶϵ��
uint    CTTimes;

/****************************************************** 
�������ƣ�Init_IO()
������������ʼ��IO�˿�
ȫ�ֱ������� 
����˵����
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
*******************************************************/ 
void Init_IO()
{
	TRISA=0b0000000000000011;       //RA2��ΪPDIN_OUT���
	TRISB=0b1111100011000100;       //
	ODCA=0x0000;					//=0���������=1��©���
	ODCB=0x0000;					//=0���������=1��©���
	LATB=0xFFFF;
	LATA=0xFFFF;
    ANSELA=0x03;
    ANSELB=0x00;
}
/****************************************************** 
�������ƣ�void PRINRXX_Init()
������������ʼ�����⸴�õ�IO�˿�
ȫ�ֱ������� 
����˵����
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����
*******************************************************/ 
void PRINRXX_Init()
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); //clear the bit 6 of OSCCONL ==> Unlock Registers
    RPOR4bits.RP42R = 16;  
    _QEA1R=39;             //RB7ΪA��
    _QEB1R=38;             //RB6ΪB��
    __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to Lock Registers  
}

/****************************************************** 
�������ƣ�void Init_SYS()
������������ʼ��ϵͳ����
ȫ�ֱ������� 
����˵����
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ���� 1.0 
˵    ���� www.kld.cn
*******************************************************/ 
void Init_SYS()
{	
	OSCTUNbits.TUN=0B000111;					   //Ĭ��7.37 *(1-0.047%)
	CLKDIVbits.PLLPRE=0;							//Ĭ��0	����/2 =00001 = ����/3 ---11111 = ����/33
	CLKDIVbits.DOZEN=0;								//0 = ������ʱ��/ ����ʱ��Ƶ�ʱ�ǿ��Ϊ1:1
	CLKDIVbits.FRCDIV=0;							//0 = FRC 1 ��Ƶ��Ĭ�ϣ�001 = FRC 2 ��Ƶ010 = FRC 4 ��ƵXX
	CLKDIVbits.PLLPOST=0;							//000 = ���/2 01 = ���/4 ��Ĭ�ϣ�10 = ���� 11 = ���/8
	PLLFBDbits.PLLDIV=63;                           //M=65,����Fosc=7370000*65/4=119.7625MHz
    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001)
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1) {};
    N1=CLKDIVbits.PLLPRE+2;                                           //Fin��Ƶϵ��
    N2=2*(CLKDIVbits.PLLPOST+1);                                      //Fin��Ƶϵ��
	M=PLLFBDbits.PLLDIV+2;
	FIN=7366536;                                  //����Ƶ��Ϊ7.37M
    Fosc=FIN*M/(N1*N2);                                 //VCO���Ƶ��
    Fcy=Fosc/2;                                         //ʱ��Ƶ��	59.88MHz   TP=16.7ns
    CTTimes=(uint)(Fcy/MHZ(4));
	DelayMs(50);
}

/******************************************************** 
�������ƣ�void Init()
������������ʼ��
ȫ�ֱ������� 
����˵����
����˵����
�� �� �ˣ�
��    ����1.0 
˵    ����
********************************************************/
void Init()
{
    Init_SYS();                                     //��ʼ��ϵͳ����
    Init_IO();                                      //��ʼ��IO��
    PRINRXX_Init();                                 //�˿�ӳ��
    Init_ADC();                                     //��ʼ��ADC
    ADC_Enable(1);                                  //ʹ��AD
    Init_PWM();                                     //��ʼ��PWM���
    InitQEI();                                      //��ʼ��QEI
    InitT2();                                       //��ʼ��Timer2
    InitT1();                                       //��ʼ��Timer1
    InitT3();

    Flash_LED1(100,2);//��ʼ�����
}
