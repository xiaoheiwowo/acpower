#include "p24EP32MC202.h"
#include "Macro.h"
#include "PIC24_ADC.h"
#include "Common.h"

/******************************************************** 
函数名称：void Init_ADC() 
功能描述：AD转换的初始化
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Init_ADC() 
{ 
    // 1 = 端口引脚处于数字模式，使能端口读输入， ADC 输入多路开关连接到AVSS
    // 0 = 端口引脚处于模拟模式，禁止端口读输入， ADC 采样引脚电压
    ANSELA=ANSELB=0;
    ANSELAbits.ANSA0=1;             //AN0
    ANSELAbits.ANSA1=1;
    
    AD1CSSLbits.CSS0=1;				//1 = 选择对AN0，AN1 进行输入扫描 0 = 输入扫描时跳过ANx
    AD1CSSLbits.CSS1=1;

    AD1CON1bits.ADON=0;				//停止AD
    AD1CON1bits.DONE=0;
    AD1CON1bits.ADSIDL=0;			//空闲时停止工作，目的是省电
    AD1CON1bits.AD12B=1;			//12位模式	【需要修改系统自带的.h文件】
    AD1CON1bits.FORM=0;				//00 无符号绝对十进制右对齐
    AD1CON1bits.SAMP=0;				//0 = SAMP 位置1 时开始采样
    AD1CON1bits.ADDMABM=0;			//ADC DMA
    AD1CON1bits.SIMSAM=0;			//=一次采样 ，=1 同时采样
    AD1CON1bits.SSRCG=0;            //以0方式对SSRC进行编程
    AD1CON1bits.ASAM=1;                                     //自动采样
    AD1CON1bits.SSRC=0B111;                                 //采样结束后自动转换
    
    AD1CON2bits.VCFG=0B000;			//00表示内部AVDD   01表示外部VREF+
    AD1CON2bits.CSCNA=1;			//=1 扫描输入 则只对CHOSA通道扫描采集  =0 不扫描输入扫描
    AD1CON2bits.CHPS=0B00;          //12位模式下读为0
    AD1CON2bits.SMPI=0B0001;        //=0000 每完成1 次采样/ 转换操作后递增DMA 地址或产生中断
    AD1CON2bits.BUFM=0;				//=0:	从ADCBUF0 开始填充缓冲区，且在连续中断时按顺序填充每个地址【FIFO模式】												//=1:	从第一次产生中断时从ADCBUF0 开始填充缓冲区，产生下一次中断时从AD1BUF(n/2）开始填充【分割缓冲模式】
    AD1CON2bits.BUFS=0;				//仅当 BUFM=1有效的时候才有效，0 = ADC当前在填充缓冲区的0x0-0x7，用户应用程序应访问0x8-0xF 中的数据

    AD1CON3bits.ADRC=0;				//1 = ADC 内部RC 时钟 0 = 时钟由系统时钟产生
    AD1CON3bits.ADCS=11;				//AD的时钟周期 1TAD=1Tcy 保证TAD>75ns       TAD=0.2us
    AD1CON3bits.SAMC=15;				//自动采样时间位  15*TAD
    
    AD1CON4bits.ADDMAEN=0;          //DMA使能=0 禁止DMA
    AD1CHS123bits.CH123NA=0;        //在ADC=12位模式下无法实现，读为0
    AD1CHS123bits.CH123NB=0;        //在ADC=12位模式下无法实现，读为0
    AD1CHS123bits.CH123SB=0;        //在ADC=12位模式下无法实现，读为0
    AD1CHS123bits.CH123SA=0;        //在ADC=12位模式下无法实现，读为0   

    AD1CHS0bits.CH0NB=0;			//采样通道负输入选择为=0 AVSS
    AD1CHS0bits.CH0NA=0;			//采样通道负输入选择为=0 AVSS

    IPC3bits.AD1IP=5;               //置A/D的优先级5
    IFS0bits.AD1IF=0;               //清A/D中断标志J
}

/******************************************************** 
函数名称：void ADC_Enable(uchar IsON)
功能描述：是否使能ADC
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 	
void ADC_Enable(uchar IsON)
{
    AD1CON1bits.ADON=IsON;
} 


/******************************************************** 
函数名称：int Read_AN0(uchar Times)
功能描述：读取AN0
全局变量：无 
参数说明：AN0,采样次数
返回说明：采样值
设 计 人：
版    本：1.0 
说    明：
********************************************************/ 

int Read_AN0()
{
    uint  Sampling;  		
    uchar i;
    uint AN0BUF[10]={0}; 				//统计10次的数值的平均值
    for (i=0;i<10;i++)
    {
        while(!_AD1IF) ClrWdt();
        _AD1IF=0;
        DelayUs(20);
        AN0BUF[i]=ADC1BUF0;					//保存AD转换结果
    }
    Sampling=CountAvg_Advalue(AN0BUF,10);                           //当前位置的AD值
    return Sampling;
}
/******************************************************** 
函数名称：int Read_AN1(uchar Times)
功能描述：读取AN1
全局变量：无 
参数说明：AN1,采样次数
返回说明：采样值
设 计 人：
版    本：1.0 
说    明：
********************************************************/ 

int Read_AN1()
{
    uint  Sampling;  		
    uchar i;
    uint AN1BUF[10]={0}; 				//统计10次的数值的平均值
    for (i=0;i<10;i++)
    {
        while(!_AD1IF) ClrWdt();
        _AD1IF=0;
        DelayUs(20);
        AN1BUF[i]=ADC1BUF1;					//保存AD转换结果
    }
    Sampling=CountAvg_Advalue(AN1BUF,10);                           //当前位置的AD值
    return Sampling;
}
