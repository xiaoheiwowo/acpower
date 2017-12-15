#include    "p24EP32MC202.h"


 int FICD __attribute__((space(prog), address(0x57F0))) = 0xFFCE ;
//_FICD(
//    ICS_PGD2 &           // ICD Communication Channel Select bits (Communicate on PGEC2 and PGED2)
//    JTAGEN_OFF           // JTAG Enable bit (JTAG is disabled)
//);
 int FPOR __attribute__((space(prog), address(0x57F2))) = 0xFFFF ;
//_FPOR(
//    ALTI2C1_OFF &        // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
//    ALTI2C2_OFF &        // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
//    WDTWIN_WIN25         // Watchdog Window Select bits (WDT Window is 25% of WDT period)
//);
 int FWDT __attribute__((space(prog), address(0x57F4))) = 0xFFFF ;
//_FWDT(
//    WDTPOST_PS32768 &    // Watchdog Timer Postscaler bits (1:32,768)
//    WDTPRE_PR128 &       // Watchdog Timer Prescaler bit (1:128)
//    PLLKEN_ON &          // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
//    WINDIS_OFF &         // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
//    FWDTEN_ON            // Watchdog Timer Enable bit (Watchdog timer always enabled)
//);
 int FOSC __attribute__((space(prog), address(0x57F6))) = 0xFF3B ;
//_FOSC(
//    POSCMD_NONE &        // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
//    OSCIOFNC_ON &        // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
//    IOL1WAY_ON &         // Peripheral pin select configuration (Allow only one reconfiguration)
//    FCKSM_CSECME         // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are enabled)
//);
 int FOSCSEL __attribute__((space(prog), address(0x57F8))) = 0xFF38 ;
//_FOSCSEL(
//    FNOSC_FRC &          // Oscillator Source Selection (Internal Fast RC (FRC))
//    PWMLOCK_OFF &        // PWM Lock Enable bit (PWM registers may be written without key sequence)
//    IESO_OFF             // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)
//);
 int FGS __attribute__((space(prog), address(0x57FA))) = 0xFFFF ;
//_FGS(
//    GWRP_OFF &           // General Segment Write-Protect bit (General Segment may be written)
//    GCP_OFF              // General Segment Code-Protect bit (General Segment Code protect is Disabled)
//);

#define uchar 	unsigned char
#define uint  	unsigned int
#define ulong	unsigned long		//������

#define True 		1
#define False		0
#define Equal       2
#define Clear       3

#define CLEAR       3
#define NOPARAM     4
#define OVER        5

#define TRUE 		1
#define FALSE		0
#define EQUAL       2                       //��ͬ

#define HIGH		1
#define LOW			0

#define High		1
#define Low			0

#define Level_H		1
#define Level_L		0

#define	FKON		1
#define	FKOFF		0

#define MHZ(x) 		x##000000				//��MHZ(x) ������x##000000
#define KHZ(x) 		x##000					//��KHZ(x) ������x##000

/***=======================================================
[1]==>>����������<<==
==========================================================**/

#define	WZAD_Channel	0					//λ���ź�ADת��ͨ�� AN0
/***=======================================================
	==>>���ܺ���<<==
==========================================================**/
#define ADC_Auto	1			//�Զ�ת��ģʽ
#define ADC_Manul	0			//�ֶ�ת��ģʽ
#define SCCR_Auto	7			//0B111 �ڲ������������������Զ�ת��
#define SCCR_Manul	0			//0B000 ����SAMP=0 ��������������ת�� ���ֶ�ģʽ��
/******************************************
 ASAM	=1 ��ʾ�Զ�����,���Զ���SAMP ��Ϊ1ʵ���Զ�ת���� 
 ASAM 	=0 ��SAMP �����Ʋ���
 ASAM=1 ʱ��Ҫ����SCCR = 111 �ڲ����������������������ת�����Զ�ת����
                       = 010 Timer3����
                       = 001 INT0����
                       = 000 ����SAMP=0 ��������������ת�� ���ֶ�ģʽ��
 �˺���������ʾ��ADC_Mode(x,y)
*******************************************/


int step=0;                                                 //���ת������
int OC1R_Before=469;

void Init_PWM()
{ 
    OC1CON1 = 0x00;
    OC1CON2 = 0x00;
   
    OC1CON1bits.OCTSEL = 0x00;                              //
    OC1CON1bits.OCM = 0x06;
    OC1CON2bits.SYNCSEL = 0x1F;
}
void ServoPosition(int Position)
{
    OC1RS = 18750;
    OC1R  = 469+Position;
}
void ServoMove(int step)
{
    OC1RS = 50000;                                          //����20ms   t=0.4us
    OC1R = OC1R_Before+2*step;                              //�ߵ�ƽ
    OC1R_Before = OC1R;
}

#define     WZAD           PORTAbits.RA0                    //λ��AD��ģ��ת��

uint 	V_Set;  					                            //��ǰλ�õ�ADֵ
/******************************************************** 
�������ƣ�void Init_ADC() 
��������: ADת���ĳ�ʼ��
ȫ�ֱ������� 
����˵������ 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Init_ADC() 
{ 
  /*1 = �˿����Ŵ�������ģʽ��ʹ�ܶ˿ڶ����룬 ADC �����·�������ӵ�AVSS
       0 = �˿����Ŵ���ģ��ģʽ����ֹ�˿ڶ����룬 ADC �������ŵ�ѹ                     */
    ANSELA=ANSELB=0;				//
    ANSELAbits.ANSA0=1;             //AN0
   
    AD1CSSLbits.CSS0=0;				//1 = ѡ���AN0��AN1 ��������ɨ�� 0 = ����ɨ��ʱ����ANx
    AD1CSSLbits.CSS1=0;

    AD1CON1bits.ADON=0;				//ֹͣAD
    AD1CON1bits.DONE=0;
    AD1CON1bits.ADSIDL=0;			//����ʱֹͣ������Ŀ����ʡ��
    AD1CON1bits.AD12B=1;			//12λģʽ	����Ҫ�޸�ϵͳ�Դ���.h�ļ���
    AD1CON1bits.FORM=0;				//00 �޷��ž���ʮ�����Ҷ���
    AD1CON1bits.SAMP=0;				//0 = SAMP λ��1 ʱ��ʼ����
    AD1CON1bits.ADDMABM=0;			//ADC DMA
    AD1CON1bits.SIMSAM=0;			//=һ�β��� ��=1 ͬʱ����
    AD1CON1bits.SSRCG=0;            //��0��ʽ��SSRC���б��
    
    AD1CON2bits.VCFG=0B000;			//00��ʾ�ڲ�AVDD   01��ʾ�ⲿVREF+
    AD1CON2bits.CSCNA=0;			//=1 ɨ������ ��ֻ��CHOSAͨ��ɨ��ɼ�  =0 ��ɨ������ɨ��
    AD1CON2bits.CHPS=0B00;          //12λģʽ�¶�Ϊ0
    AD1CON2bits.SMPI=0B0000;        //=0000 ÿ���1 �β���/ ת�����������DMA ��ַ������ж�
    AD1CON2bits.BUFM=0;				//=0:	��ADCBUF0 ��ʼ��仺���������������ж�ʱ��˳�����ÿ����ַ��FIFOģʽ��												//=1:	�ӵ�һ�β����ж�ʱ��ADCBUF0 ��ʼ��仺������������һ���ж�ʱ��AD1BUF(n/2����ʼ��䡾�ָ��ģʽ��
    AD1CON2bits.BUFS=0;				//���� BUFM=1��Ч��ʱ�����Ч��0 = ADC��ǰ����仺������0x0-0x7���û�Ӧ�ó���Ӧ����0x8-0xF �е�����

    AD1CON3bits.ADRC=0;				//1 = ADC �ڲ�RC ʱ�� 0 = ʱ����ϵͳʱ�Ӳ���
    AD1CON3bits.ADCS=8;				//AD��ʱ������ 1TAD=1Tcy ��֤TAD>75ns
    AD1CON3bits.SAMC=8;				//�Զ�����ʱ��λ
    
    AD1CON4bits.ADDMAEN=0;          //DMAʹ��=0 ��ֹDMA
    AD1CHS123bits.CH123NA=0;        //��ADC=12λģʽ���޷�ʵ�֣���Ϊ0
    AD1CHS123bits.CH123NB=0;        //��ADC=12λģʽ���޷�ʵ�֣���Ϊ0
    AD1CHS123bits.CH123SB=0;        //��ADC=12λģʽ���޷�ʵ�֣���Ϊ0
    AD1CHS123bits.CH123SA=0;        //��ADC=12λģʽ���޷�ʵ�֣���Ϊ0   

    AD1CHS0bits.CH0NB=0;			//����ͨ��������ѡ��Ϊ=0 AVSS
    AD1CHS0bits.CH0NA=0;			//����ͨ��������ѡ��Ϊ=0 AVSS

    IPC3bits.AD1IP=5;               //��A/D�����ȼ�5
    IFS0bits.AD1IF=0;               //��A/D�жϱ�־J
}
/******************************************************** 
�������ƣ�void Set_ADC_Para(uchar Mode,uchar x_TAD)
��������: ����ADC����
ȫ�ֱ������� 
����˵����x_TAD���Զ�ģʽ����Ч,�������ڵı��� 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����
=======================================================================
 ASAM	=1 ��ʾ�Զ�����,���Զ���SAMP ��Ϊ1ʵ���Զ�ת���� 
 ASAM 	=0 ��SAMP �����Ʋ���
 ASAM=1 ʱ��Ҫ����SCCR = 111 �ڲ����������������������ת�����Զ�ת����
 					   = 010 Timer3����
 					   = 001 INT0����
 					   = 000 ����SAMP=0 ��������������ת�� ���ֶ�ģʽ��
 �˺���������ʾ��ADC_Mode(x,y)					
=======================================================================
AD1CON3bits.ADCS=4;			//AD��ʱ������ 1TAD=1Tcy ��֤TAD>75ns
AD1CON3bits.SAMC=0b01000;	//�Զ�����ʱ��λ ����AD1CON1bits.SSRC=0B111 
							  ʱ,��Ч// 8*TAD ��֤>750ns 
TAD		=Tcy*(ADCS+1)/2			
ADCS	=(2*TAD/Tcy)-1
�����ϵͳʱ�ӣ� FOSC=32M   ��Tcy=2/32M =62.5ns	 ȡTAD=156ns ��
ADCS	=2*156/62.5-1 =3.922 ȡ4


ע�⣺ PIC24Fϵ�����һ�β���������Ҫ12��TADʱ��
ע��ADRC=1 ��ADC�ڲ�ʱ�� =����ԼΪ250nS ��������ADCS=1����
���ADRC=0 ����ϵͳʱ�ӣ���������ļ��㹫ʽ��ADCS����>=4���ɡ�
�������׵��²������ݲ�׼ȷ��
x_TAD*ADCS>=*750ns ==> x_TAD 
=======================================================================
********************************************************/ 
void Set_ADC_Para(uchar Mode,uchar x_TAD)
{
    if (Mode==ADC_Auto)
    {
        AD1CON1bits.ASAM=1;                                     //�Զ�����
        AD1CON1bits.SSRC=0B111;                                 //�����������Զ�ת��

        AD1CON3bits.ADRC=0;                                     //=0ϵͳʱ���ṩ =1ADC�ڲ�ʱ�� ԼΪ250ns
        AD1CON3bits.SAMC=x_TAD;                                 //�Զ�����ʱ��λ ����AD1CON1bits.SSRC=0B111ʱ,��Ч// 8*TAD ��֤>750ns
    }
    else
    {
        AD1CON1bits.SSRC=0;					                    //�������SAMP ������������ʼת��
        AD1CON1bits.ASAM=0;				                        //������ɺ�=1�Զ�������ʼ
    }
}	

/******************************************************** 
�������ƣ�uint Read_WZAD(uchar Times)
��������: ��ȡλ��AD
ȫ�ֱ������� 
����˵������ 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 	
void Set_ADC_Channel()
{
    AD1CON2bits.SMPI=0;					                     	//=�ɼ�[SMPI+1]�β��ж�һ�� ������WZAD ,KZAD
    AD1CON2bits.ALTS=0;						                    //=0:	ʼ��ʹ�ò���Aѡ���ͨ�����롣

    AD1CHS0bits.CH0SA=WZAD_Channel;                             //AN0 ѡ�� ��3210����λѡ��AD��ͨ��
} 


/******************************************************** 
�������ƣ�void ADC_Enable(uchar IsON)
��������: �Ƿ�ʹ��ADC
ȫ�ֱ������� 
����˵������ 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 	
void ADC_Enable(uchar IsON)
{
    AD1CON1bits.ADON=IsON;
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
/******************************************************** 
�������ƣ�void Read_Auto_WZ(uchar Times)
��������: ��ȡ�ⲿλ��AD
ȫ�ֱ������� 
����˵������ 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Read_Auto_WZ(uchar Times)
{
    uchar i;
    uint WZ[Times]; 				//ͳ��10�ε���ֵ��ƽ��ֵ
    for (i=0;i<Times;i++)
    {
        while(!AD1CON1bits.DONE) ClrWdt();
        AD1CON1bits.DONE=0;
        WZ[i]=ADC1BUF0;					//����ADת�����
    }
    V_Set=CountAvg_Advalue(WZ,Times);
}





/****************************************
1: ����ʱ��Ƶ�ʣ�
	Fosc=FIN*M/(N1*N2)
	M=PLLFBD;
	N1=
***************************************/
ulong 	Fosc;
ulong 	Fcy;
ulong	FIN;
uchar 	M;
uchar   N1,N2;             //Fin ��Ƶϵ��
uint    CTTimes;
/****************************************************** 
�������ƣ�Init_IO()
��������: ��ʼ��IO�˿�
ȫ�ֱ������� 
����˵����
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
2013-11-04 ����TC-02-BX-AC95-265V����ԭ��ͼ����
*******************************************************/ 
void Init_IO()
{
	TRISA=0b0000000000010011;       //RA2��ΪPDIN_OUT���
	TRISB=0b0000111000000111;       //K1MK2 K3û��ʹ��
	ODCA=0x0000;					//=0���������=1��©���
	ODCB=0x0000;					//=0���������=1��©���
	LATB=0xFFFF;
	LATA=0xFFFF;
}

/****************************************************** 
�������ƣ�void PRINRXX_Init()
��������: ��ʼ�����⸴�õ�IO�˿�
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
    //_U1RXR=20;                                          //U1RX ���ӵ�RP20
    //_RP38R=1;                                           //U1TX���ӵ�RP38
    
    //_U2RXR=34;                                          //U2RX ��������RPI34
    //_RP35R=3;                                           //U2TX���ӵ�RP35
    //_RP36R=16;                                          //OC1���ӵ�RP36
    __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to Lock Registers  
}
/**************************************************
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

/****************************************************** 
�������ƣ�void Init_SYS()
��������: ��ʼ��ϵͳ����
ȫ�ֱ������� 
����˵����
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
*******************************************************/ 
void Init_SYS()
{	
	OSCTUNbits.TUN=0B111111;					   //Ĭ��7.37 *(1-0.047%)
	CLKDIVbits.PLLPRE=0;							//Ĭ��0	����/2 =00001 = ����/3 ---11111 = ����/33
	CLKDIVbits.DOZEN=0;								//0 = ������ʱ��/ ����ʱ��Ƶ�ʱ�ǿ��Ϊ1:1
	CLKDIVbits.FRCDIV=0;							//0 = FRC 1 ��Ƶ��Ĭ�ϣ�001 = FRC 2 ��Ƶ010 = FRC 4 ��ƵXX
	CLKDIVbits.PLLPOST=0;							//000 = ���/2 01 = ���/4 ��Ĭ�ϣ�10 = ���� 11 = ���/8
	PLLFBDbits.PLLDIV=61;                           //M=67,����Fosc=7370000*65/4=119.7625MHz
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


/***=======================================================
[1]==>>����������<<==
==========================================================**/
#define TxCKPS_1		1				//��Ƶ��Ϊ1
#define TxCKPS_8		8				//��Ƶ��Ϊ8
#define TxCKPS_64		64				//��Ƶ��Ϊ64
#define TxCKPS_256		256				//��Ƶ��Ϊ256

#define T1_us			20000			//����T1 Ϊ20000uS
#define T2_us			500				//����T2 Ϊ500uS
#define T3_us			500				//����T3 Ϊ200uS  ���ֵ����̫С��̫С���޷������жϣ���Ϊ��������Ҫ��ȡAD	�������Ƕȣ�������Ҫʱ��ϳ�
#define T5_us			1000			//����T5 Ϊ2000uS

#define	Tcytimes	Fosc/MHZ(4)             //���Զ�ʱʱ��Ӧ��/2�ر�ע�� ���������ڣ�����Ӧ����FOSC/4


uint    T3Count;
uint	TxCKPS[5]={1,1,1,1,1};										//���涨ʱ����

/******************************************************** 
�������ƣ�void Init_Timer1(uint TxCKPS,ulong Xtal_Freq)
��������: ��ʼ��Timer1
ȫ�ֱ������� 
����˵������ 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Init_Timer1(uint Tx_CKPS)
{
	TxCKPS[0]=Tx_CKPS;										//���浱ǰ��Ƶ��
    
	T1CONbits.TON=0;										//��ͣ��ʱ��
	T1CONbits.TSIDL=0;										//����ģʽ�Ƿ�ʡ��
	T1CONbits.TCS=0B00;                                     //��չ��ʱ�ӣ�00 ����ʱ�� 01 T1CKʱ�� 10 LPRCʱ�� 11����
    T1CONbits.TGATE=1;
    

	if(Tx_CKPS==TxCKPS_1)			T1CONbits.TCKPS=0B00;	//ѡ���Ƶϵ�� ��00��==>1:1 
	else if(Tx_CKPS==TxCKPS_8)		T1CONbits.TCKPS=0B01;	//ѡ���Ƶϵ�� ��01��==>1:8
	else if(Tx_CKPS==TxCKPS_64)		T1CONbits.TCKPS=0B10;	//ѡ���Ƶϵ�� ��10��==>1:64
	else if(Tx_CKPS==TxCKPS_256)	T1CONbits.TCKPS=0B11;	//ѡ���Ƶϵ�� ��11��==>1:256
				
	T1CONbits.TCS=0;										//=��0��ѡ��ʱ��Դ��FOSC/2ʱ��	=��1�� ��T1ECS����
	IPC0bits.T1IP=3;										//�ж����ȼ�33
	TMR1=0;	
	//IFS0bits.T1IF=0;										//����жϱ�־λ
	//IEC0bits.T1IE=1;										//����T1�ж�
}	 

/******************************************************** 
�������ƣ�void Set_TMR1(uint X_us��uchar IsOpen)
��������: ����Time1��ʱֵ
ȫ�ֱ������� 
����˵����X_us: ��ʱʱ�䵥λuS�� IsOpen��=True��ʾ��Ҫ�� 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����ע�⣺�������1��Ƶ�����ֻ����ʱ10ms�������8��Ƶ�����ֻ�ܵ�80ms ��64 ����ܵ�640ms
********************************************************/ 
void Set_Timer1(uint X_us,uchar IsOpen)
{
	IFS0bits.T1IF=0;									//�����־λ
	ulong Tmp;
	Tmp=(ulong)(X_us)*58 ;
	PR1=Tmp/TxCKPS[0]-1;					
	T1CONbits.TON=IsOpen;
}	

/******************************************************** 
�������ƣ�void TMR1_Enable(uchar IsOpen)
��������: Timer1 ʹ�ܿ���
ȫ�ֱ������� 
����˵����IsOpen��=True��ʾ��Ҫ�� =False ��ʾ�ر�
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Timer1_Enable(uchar IsOpen)
{
	//IFS0bits.T1IF=0;									//�����־λ		
	T1CONbits.TON=IsOpen;
}

   /****&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 			��ʱ��2����====����ʼ��
 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&****/
 /******************************************************** 
�������ƣ�void Init_Timer2(uint TxCKPS,ulong Xtal_Freq)
��������: ��ʼ��Timer2
ȫ�ֱ������� 
����˵������ 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Init_Timer2(uint Tx_CKPS)
{
	TxCKPS[1]=Tx_CKPS;									//���浱ǰ��Ƶ��
	
	T2CONbits.TON=0;									//��ͣ��ʱ��
	T2CONbits.TSIDL=0;									//����ģʽ�Ƿ�ʡ��
	T2CONbits.T32=0;									//=0������Ϊ16λ��ʱ��
														//=1����TIMER3��ϳ�32λ��ʱ��
														
	if(Tx_CKPS==TxCKPS_1)			T2CONbits.TCKPS=0B00;	//ѡ���Ƶϵ�� ��00��==>1:1 
	else if(Tx_CKPS==TxCKPS_8)		T2CONbits.TCKPS=0B01;	//ѡ���Ƶϵ�� ��01��==>1:8
	else if(Tx_CKPS==TxCKPS_64)		T2CONbits.TCKPS=0B10;	//ѡ���Ƶϵ�� ��10��==>1:64
	else if(Tx_CKPS==TxCKPS_256)	T2CONbits.TCKPS=0B11;	//ѡ���Ƶϵ�� ��11��==>1:256
				
	T2CONbits.TCS=0;									//=��0��ѡ��ʱ��Դ��FOSC/2ʱ��	=��1�� ��T1ECS����
	IPC1bits.T2IP=4;									//�ж����ȼ�4 ���λ7��
	TMR2=0;	
	//IFS0bits.T2IF=0;									//����жϱ�־λ
	//IEC0bits.T2IE=True;									//����T2�ж�
}	 

/******************************************************** 
�������ƣ�void Setup_Timer2(uint X_us��uchar IsOpen)
��������: ����Time2��ʱֵ
ȫ�ֱ������� 
����˵����X_us: ��ʱʱ�䵥λuS�� IsOpen��=True��ʾ��Ҫ�� 
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
		��ʱʱ����㷽����
		ָ������Tcy ,Ԥ��Ƶϵ��N �����ڼĴ���PRX ����ֵΪX ����ʱʱ��Ϊ
		T��ʱ=Tcy*N(X+1) 
		Fcy=32MHZ/2M=16 ,T��ʱ=1ms=1000us
		X=(1000*Fcy/N)-1 	
			
		���Զ�ʱֵxms
********************************************************/ 
void Set_Timer2(uint X_us,uchar IsOpen)
{
	IFS0bits.T2IF=0;									//�����־λ
	ulong Tmp;
	Tmp=(X_us+40)*Fcy;
	Tmp=Tmp/TxCKPS[1]-1;
	PR2=(uint)(Tmp);						
	T2CONbits.TON=IsOpen;
}	

/******************************************************** 
�������ƣ�void TMR2_Enable(uchar IsOpen)
��������: Timer2 ʹ�ܿ���
ȫ�ֱ������� 
����˵����IsOpen��=True��ʾ��Ҫ�� =False ��ʾ�ر�
����˵������ 
�� �� �ˣ�FEIHONG_KLD
��    ����1.0 
˵    ����www.kld.cn
********************************************************/ 
void Timer2_Enable(uchar IsOpen)
{
	//IFS0bits.T2IF=0;									//�����־λ		
	T2CONbits.TON=IsOpen;
}

  /****&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 			��ʱ����2������====��������
 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&****/
int main()
{
    Init_SYS();
    Init_IO();
    PRINRXX_Init();

    Init_ADC();
    Set_ADC_Channel();
    Set_ADC_Para(ADC_Auto,50);
    ADC_Enable(1);
    
    Init_PWM();
    
    Init_Timer2(64);
    Timer2_Enable(1);    
    
    while(1)
    {
        Read_Auto_WZ(10);
        ServoPosition((uint)(V_Set/2));
        //ServoMove(step);
        ClrWdt();
    }
}
