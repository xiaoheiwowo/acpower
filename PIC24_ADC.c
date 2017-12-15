#include "p24EP32MC202.h"
#include "Macro.h"
#include "PIC24_ADC.h"
#include "Common.h"

/******************************************************** 
�������ƣ�void Init_ADC() 
����������ADת���ĳ�ʼ��
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
    ANSELAbits.ANSA1=1;
    
    AD1CSSLbits.CSS0=1;				//1 = ѡ���AN0��AN1 ��������ɨ�� 0 = ����ɨ��ʱ����ANx
    AD1CSSLbits.CSS1=1;

    AD1CON1bits.ADON=0;				//ֹͣAD
    AD1CON1bits.DONE=0;
    AD1CON1bits.ADSIDL=0;			//����ʱֹͣ������Ŀ����ʡ��
    AD1CON1bits.AD12B=1;			//12λģʽ	����Ҫ�޸�ϵͳ�Դ���.h�ļ���
    AD1CON1bits.FORM=0;				//00 �޷��ž���ʮ�����Ҷ���
    AD1CON1bits.SAMP=0;				//0 = SAMP λ��1 ʱ��ʼ����
    AD1CON1bits.ADDMABM=0;			//ADC DMA
    AD1CON1bits.SIMSAM=0;			//=һ�β��� ��=1 ͬʱ����
    AD1CON1bits.SSRCG=0;            //��0��ʽ��SSRC���б��
    AD1CON1bits.ASAM=1;                                     //�Զ�����
    AD1CON1bits.SSRC=0B111;                                 //�����������Զ�ת��
    
    AD1CON2bits.VCFG=0B000;			//00��ʾ�ڲ�AVDD   01��ʾ�ⲿVREF+
    AD1CON2bits.CSCNA=1;			//=1 ɨ������ ��ֻ��CHOSAͨ��ɨ��ɼ�  =0 ��ɨ������ɨ��
    AD1CON2bits.CHPS=0B00;          //12λģʽ�¶�Ϊ0
    AD1CON2bits.SMPI=0B0001;        //=0000 ÿ���1 �β���/ ת�����������DMA ��ַ������ж�
    AD1CON2bits.BUFM=0;				//=0:	��ADCBUF0 ��ʼ��仺���������������ж�ʱ��˳�����ÿ����ַ��FIFOģʽ��												//=1:	�ӵ�һ�β����ж�ʱ��ADCBUF0 ��ʼ��仺������������һ���ж�ʱ��AD1BUF(n/2����ʼ��䡾�ָ��ģʽ��
    AD1CON2bits.BUFS=0;				//���� BUFM=1��Ч��ʱ�����Ч��0 = ADC��ǰ����仺������0x0-0x7���û�Ӧ�ó���Ӧ����0x8-0xF �е�����

    AD1CON3bits.ADRC=0;				//1 = ADC �ڲ�RC ʱ�� 0 = ʱ����ϵͳʱ�Ӳ���
    AD1CON3bits.ADCS=11;				//AD��ʱ������ 1TAD=1Tcy ��֤TAD>75ns       TAD=0.2us
    AD1CON3bits.SAMC=15;				//�Զ�����ʱ��λ  15*TAD
    
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
�������ƣ�void ADC_Enable(uchar IsON)
�����������Ƿ�ʹ��ADC
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


/******************************************************** 
�������ƣ�int Read_AN0(uchar Times)
������������ȡAN0
ȫ�ֱ������� 
����˵����AN0,��������
����˵��������ֵ
�� �� �ˣ�
��    ����1.0 
˵    ����
********************************************************/ 

int Read_AN0()
{
    uint  Sampling;  		
    uchar i;
    uint AN0BUF[10]={0}; 				//ͳ��10�ε���ֵ��ƽ��ֵ
    for (i=0;i<10;i++)
    {
        while(!_AD1IF) ClrWdt();
        _AD1IF=0;
        DelayUs(20);
        AN0BUF[i]=ADC1BUF0;					//����ADת�����
    }
    Sampling=CountAvg_Advalue(AN0BUF,10);                           //��ǰλ�õ�ADֵ
    return Sampling;
}
/******************************************************** 
�������ƣ�int Read_AN1(uchar Times)
������������ȡAN1
ȫ�ֱ������� 
����˵����AN1,��������
����˵��������ֵ
�� �� �ˣ�
��    ����1.0 
˵    ����
********************************************************/ 

int Read_AN1()
{
    uint  Sampling;  		
    uchar i;
    uint AN1BUF[10]={0}; 				//ͳ��10�ε���ֵ��ƽ��ֵ
    for (i=0;i<10;i++)
    {
        while(!_AD1IF) ClrWdt();
        _AD1IF=0;
        DelayUs(20);
        AN1BUF[i]=ADC1BUF1;					//����ADת�����
    }
    Sampling=CountAvg_Advalue(AN1BUF,10);                           //��ǰλ�õ�ADֵ
    return Sampling;
}
