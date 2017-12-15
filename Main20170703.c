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
#define ulong	unsigned long		//长整型

#define True 		1
#define False		0
#define Equal       2
#define Clear       3

#define CLEAR       3
#define NOPARAM     4
#define OVER        5

#define TRUE 		1
#define FALSE		0
#define EQUAL       2                       //相同

#define HIGH		1
#define LOW			0

#define High		1
#define Low			0

#define Level_H		1
#define Level_L		0

#define	FKON		1
#define	FKOFF		0

#define MHZ(x) 		x##000000				//用MHZ(x) 来代替x##000000
#define KHZ(x) 		x##000					//用KHZ(x) 来代替x##000

/***=======================================================
[1]==>>变量定义区<<==
==========================================================**/

#define	WZAD_Channel	0					//位置信号AD转换通道 AN0
/***=======================================================
	==>>功能函数<<==
==========================================================**/
#define ADC_Auto	1			//自动转换模式
#define ADC_Manul	0			//手动转换模式
#define SCCR_Auto	7			//0B111 内部计数器结束采样后自动转换
#define SCCR_Manul	0			//0B000 清零SAMP=0 结束采样并启动转换 【手动模式】
/******************************************
 ASAM	=1 表示自动采样,并自动将SAMP 置为1实现自动转换。 
 ASAM 	=0 由SAMP 来控制采样
 ASAM=1 时需要设置SCCR = 111 内部计数器结束后采样并启动转换【自动转换】
                       = 010 Timer3启动
                       = 001 INT0启动
                       = 000 清零SAMP=0 结束采样并启动转换 【手动模式】
 此函数可以显示：ADC_Mode(x,y)
*******************************************/


int step=0;                                                 //舵机转动步数
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
    OC1RS = 50000;                                          //周期20ms   t=0.4us
    OC1R = OC1R_Before+2*step;                              //高电平
    OC1R_Before = OC1R;
}

#define     WZAD           PORTAbits.RA0                    //位置AD的模拟转换

uint 	V_Set;  					                            //当前位置的AD值
/******************************************************** 
函数名称：void Init_ADC() 
功能描述: AD转换的初始化
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Init_ADC() 
{ 
  /*1 = 端口引脚处于数字模式，使能端口读输入， ADC 输入多路开关连接到AVSS
       0 = 端口引脚处于模拟模式，禁止端口读输入， ADC 采样引脚电压                     */
    ANSELA=ANSELB=0;				//
    ANSELAbits.ANSA0=1;             //AN0
   
    AD1CSSLbits.CSS0=0;				//1 = 选择对AN0，AN1 进行输入扫描 0 = 输入扫描时跳过ANx
    AD1CSSLbits.CSS1=0;

    AD1CON1bits.ADON=0;				//停止AD
    AD1CON1bits.DONE=0;
    AD1CON1bits.ADSIDL=0;			//空闲时停止工作，目的是省电
    AD1CON1bits.AD12B=1;			//12位模式	【需要修改系统自带的.h文件】
    AD1CON1bits.FORM=0;				//00 无符号绝对十进制右对齐
    AD1CON1bits.SAMP=0;				//0 = SAMP 位置1 时开始采样
    AD1CON1bits.ADDMABM=0;			//ADC DMA
    AD1CON1bits.SIMSAM=0;			//=一次采样 ，=1 同时采样
    AD1CON1bits.SSRCG=0;            //以0方式对SSRC进行编程
    
    AD1CON2bits.VCFG=0B000;			//00表示内部AVDD   01表示外部VREF+
    AD1CON2bits.CSCNA=0;			//=1 扫描输入 则只对CHOSA通道扫描采集  =0 不扫描输入扫描
    AD1CON2bits.CHPS=0B00;          //12位模式下读为0
    AD1CON2bits.SMPI=0B0000;        //=0000 每完成1 次采样/ 转换操作后递增DMA 地址或产生中断
    AD1CON2bits.BUFM=0;				//=0:	从ADCBUF0 开始填充缓冲区，且在连续中断时按顺序填充每个地址【FIFO模式】												//=1:	从第一次产生中断时从ADCBUF0 开始填充缓冲区，产生下一次中断时从AD1BUF(n/2）开始填充【分割缓冲模式】
    AD1CON2bits.BUFS=0;				//仅当 BUFM=1有效的时候才有效，0 = ADC当前在填充缓冲区的0x0-0x7，用户应用程序应访问0x8-0xF 中的数据

    AD1CON3bits.ADRC=0;				//1 = ADC 内部RC 时钟 0 = 时钟由系统时钟产生
    AD1CON3bits.ADCS=8;				//AD的时钟周期 1TAD=1Tcy 保证TAD>75ns
    AD1CON3bits.SAMC=8;				//自动采样时间位
    
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
函数名称：void Set_ADC_Para(uchar Mode,uchar x_TAD)
功能描述: 设置ADC参数
全局变量：无 
参数说明：x_TAD：自动模式下有效,采样周期的倍数 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：
=======================================================================
 ASAM	=1 表示自动采样,并自动将SAMP 置为1实现自动转换。 
 ASAM 	=0 由SAMP 来控制采样
 ASAM=1 时需要设置SCCR = 111 内部计数器结束后采样并启动转换【自动转换】
 					   = 010 Timer3启动
 					   = 001 INT0启动
 					   = 000 清零SAMP=0 结束采样并启动转换 【手动模式】
 此函数可以显示：ADC_Mode(x,y)					
=======================================================================
AD1CON3bits.ADCS=4;			//AD的时钟周期 1TAD=1Tcy 保证TAD>75ns
AD1CON3bits.SAMC=0b01000;	//自动采样时间位 仅当AD1CON1bits.SSRC=0B111 
							  时,有效// 8*TAD 保证>750ns 
TAD		=Tcy*(ADCS+1)/2			
ADCS	=(2*TAD/Tcy)-1
如果是系统时钟： FOSC=32M   则Tcy=2/32M =62.5ns	 取TAD=156ns 则
ADCS	=2*156/62.5-1 =3.922 取4


注意： PIC24F系列完成一次采样至少需要12个TAD时钟
注意ADRC=1 是ADC内部时钟 =周期约为250nS 所以设置ADCS=1即可
如果ADRC=0 采用系统时钟，根据上面的计算公式则ADCS至少>=4即可。
否则容易导致采用数据不准确。
x_TAD*ADCS>=*750ns ==> x_TAD 
=======================================================================
********************************************************/ 
void Set_ADC_Para(uchar Mode,uchar x_TAD)
{
    if (Mode==ADC_Auto)
    {
        AD1CON1bits.ASAM=1;                                     //自动采样
        AD1CON1bits.SSRC=0B111;                                 //采样结束后自动转换

        AD1CON3bits.ADRC=0;                                     //=0系统时钟提供 =1ADC内部时钟 约为250ns
        AD1CON3bits.SAMC=x_TAD;                                 //自动采样时间位 仅当AD1CON1bits.SSRC=0B111时,有效// 8*TAD 保证>750ns
    }
    else
    {
        AD1CON1bits.SSRC=0;					                    //软件清零SAMP 结束采样并开始转换
        AD1CON1bits.ASAM=0;				                        //采样完成后=1自动立即开始
    }
}	

/******************************************************** 
函数名称：uint Read_WZAD(uchar Times)
功能描述: 读取位置AD
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 	
void Set_ADC_Channel()
{
    AD1CON2bits.SMPI=0;					                     	//=采集[SMPI+1]次才中断一次 读两次WZAD ,KZAD
    AD1CON2bits.ALTS=0;						                    //=0:	始终使用采样A选择的通道输入。

    AD1CHS0bits.CH0SA=WZAD_Channel;                             //AN0 选择 【3210】四位选择AD的通道
} 


/******************************************************** 
函数名称：void ADC_Enable(uchar IsON)
功能描述: 是否使能ADC
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

/***************************************************
函数名称：uint CountAvg_Advalue(uint *ADV,uchar Times)
入口参数：无
出口参数：无
备注：去掉一个最大值，一个最小值，然后求平均值
**************************************************/
uint CountAvg_Advalue(uint *ADV,uchar Times)
{
	uchar i;
	uint Min,Max;
	Min=ADV[0];
	Max=Min;
	for(i=0;i<Times;i++) 
	{
		 if(ADV[i]<Min) Min=ADV[i];						//统计最小值
		 if(ADV[i]>Max) Max=ADV[i];						//统计最大值
	}
	uint CountAll=0;
	uint Avg=0;
	for(i=0;i<Times;i++) CountAll=CountAll+ADV[i]; 		//统计总数
	CountAll=CountAll-Min-Max;							//去掉最大值，去掉最小值
	Avg=CountAll/(Times-2);								//求平均值
	return Avg;
}
/******************************************************** 
函数名称：void Read_Auto_WZ(uchar Times)
功能描述: 读取外部位置AD
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Read_Auto_WZ(uchar Times)
{
    uchar i;
    uint WZ[Times]; 				//统计10次的数值的平均值
    for (i=0;i<Times;i++)
    {
        while(!AD1CON1bits.DONE) ClrWdt();
        AD1CON1bits.DONE=0;
        WZ[i]=ADC1BUF0;					//保存AD转换结果
    }
    V_Set=CountAvg_Advalue(WZ,Times);
}





/****************************************
1: 定义时钟频率：
	Fosc=FIN*M/(N1*N2)
	M=PLLFBD;
	N1=
***************************************/
ulong 	Fosc;
ulong 	Fcy;
ulong	FIN;
uchar 	M;
uchar   N1,N2;             //Fin 分频系数
uint    CTTimes;
/****************************************************** 
函数名称：Init_IO()
功能描述: 初始化IO端口
全局变量：无 
参数说明：
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
2013-11-04 按照TC-02-BX-AC95-265V控制原理图更改
*******************************************************/ 
void Init_IO()
{
	TRISA=0b0000000000010011;       //RA2作为PDIN_OUT输出
	TRISB=0b0000111000000111;       //K1MK2 K3没有使用
	ODCA=0x0000;					//=0正常输出，=1开漏输出
	ODCB=0x0000;					//=0正常输出，=1开漏输出
	LATB=0xFFFF;
	LATA=0xFFFF;
}

/****************************************************** 
函数名称：void PRINRXX_Init()
功能描述: 初始化特殊复用的IO端口
全局变量：无 
参数说明：
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：
*******************************************************/ 
void PRINRXX_Init()
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); //clear the bit 6 of OSCCONL ==> Unlock Registers
     RPOR4bits.RP42R = 16;  
    //_U1RXR=20;                                          //U1RX 链接到RP20
    //_RP38R=1;                                           //U1TX连接到RP38
    
    //_U2RXR=34;                                          //U2RX 输入引脚RPI34
    //_RP35R=3;                                           //U2TX连接到RP35
    //_RP36R=16;                                          //OC1连接到RP36
    __builtin_write_OSCCONL(OSCCON | 0x40); //set the bit 6 of OSCCONL to Lock Registers  
}
/**************************************************
函数名称：DelayMs(uint x)
函数功能：延时函数
入口参数：x	
出口参数：无
备注：i<89 毫秒延时 精度0.5%	x=1时延时1ms	4M晶振
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
函数名称：void Init_SYS()
功能描述: 初始化系统参数
全局变量：无 
参数说明：
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
*******************************************************/ 
void Init_SYS()
{	
	OSCTUNbits.TUN=0B111111;					   //默认7.37 *(1-0.047%)
	CLKDIVbits.PLLPRE=0;							//默认0	输入/2 =00001 = 输入/3 ---11111 = 输入/33
	CLKDIVbits.DOZEN=0;								//0 = 处理器时钟/ 外设时钟频率比强制为1:1
	CLKDIVbits.FRCDIV=0;							//0 = FRC 1 分频（默认）001 = FRC 2 分频010 = FRC 4 分频XX
	CLKDIVbits.PLLPOST=0;							//000 = 输出/2 01 = 输出/4 （默认）10 = 保留 11 = 输出/8
	PLLFBDbits.PLLDIV=61;                           //M=67,这样Fosc=7370000*65/4=119.7625MHz
    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001)
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1) {};
    
    N1=CLKDIVbits.PLLPRE+2;                                           //Fin分频系数
    N2=2*(CLKDIVbits.PLLPOST+1);                                      //Fin分频系数
	M=PLLFBDbits.PLLDIV+2;
	FIN=7366536;                                  //输入频率为7.37M
    Fosc=FIN*M/(N1*N2);                                 //VCO输出频率
    Fcy=Fosc/2;                                         //时钟频率	59.88MHz   TP=16.7ns
    CTTimes=(uint)(Fcy/MHZ(4));
	DelayMs(50);
}


/***=======================================================
[1]==>>变量定义区<<==
==========================================================**/
#define TxCKPS_1		1				//分频比为1
#define TxCKPS_8		8				//分频比为8
#define TxCKPS_64		64				//分频比为64
#define TxCKPS_256		256				//分频比为256

#define T1_us			20000			//定义T1 为20000uS
#define T2_us			500				//定义T2 为500uS
#define T3_us			500				//定义T3 为200uS  这个值不能太小，太小将无法跳出中断，因为这里面需要读取AD	及其计算角度，所以需要时间较长
#define T5_us			1000			//定义T5 为2000uS

#define	Tcytimes	Fosc/MHZ(4)             //所以定时时间应该/2特别注意 而不是周期，所以应该是FOSC/4


uint    T3Count;
uint	TxCKPS[5]={1,1,1,1,1};										//保存定时器的

/******************************************************** 
函数名称：void Init_Timer1(uint TxCKPS,ulong Xtal_Freq)
功能描述: 初始化Timer1
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Init_Timer1(uint Tx_CKPS)
{
	TxCKPS[0]=Tx_CKPS;										//保存当前分频比
    
	T1CONbits.TON=0;										//启停定时器
	T1CONbits.TSIDL=0;										//空闲模式是否省电
	T1CONbits.TCS=0B00;                                     //扩展是时钟：00 辅助时钟 01 T1CK时钟 10 LPRC时钟 11保留
    T1CONbits.TGATE=1;
    

	if(Tx_CKPS==TxCKPS_1)			T1CONbits.TCKPS=0B00;	//选择分频系数 【00】==>1:1 
	else if(Tx_CKPS==TxCKPS_8)		T1CONbits.TCKPS=0B01;	//选择分频系数 【01】==>1:8
	else if(Tx_CKPS==TxCKPS_64)		T1CONbits.TCKPS=0B10;	//选择分频系数 【10】==>1:64
	else if(Tx_CKPS==TxCKPS_256)	T1CONbits.TCKPS=0B11;	//选择分频系数 【11】==>1:256
				
	T1CONbits.TCS=0;										//=【0】选择时钟源：FOSC/2时钟	=【1】 有T1ECS决定
	IPC0bits.T1IP=3;										//中断优先级33
	TMR1=0;	
	//IFS0bits.T1IF=0;										//清除中断标志位
	//IEC0bits.T1IE=1;										//允许T1中断
}	 

/******************************************************** 
函数名称：void Set_TMR1(uint X_us，uchar IsOpen)
功能描述: 设置Time1延时值
全局变量：无 
参数说明：X_us: 定时时间单位uS， IsOpen：=True表示需要打开 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：注意：如果才用1分频，最大只能延时10ms，如果是8分频，最大只能到80ms ，64 最大能到640ms
********************************************************/ 
void Set_Timer1(uint X_us,uchar IsOpen)
{
	IFS0bits.T1IF=0;									//清除标志位
	ulong Tmp;
	Tmp=(ulong)(X_us)*58 ;
	PR1=Tmp/TxCKPS[0]-1;					
	T1CONbits.TON=IsOpen;
}	

/******************************************************** 
函数名称：void TMR1_Enable(uchar IsOpen)
功能描述: Timer1 使能控制
全局变量：无 
参数说明：IsOpen：=True表示需要打开 =False 表示关闭
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Timer1_Enable(uchar IsOpen)
{
	//IFS0bits.T1IF=0;									//清除标志位		
	T1CONbits.TON=IsOpen;
}

   /****&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 			定时器2设置====【开始】
 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&****/
 /******************************************************** 
函数名称：void Init_Timer2(uint TxCKPS,ulong Xtal_Freq)
功能描述: 初始化Timer2
全局变量：无 
参数说明：无 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Init_Timer2(uint Tx_CKPS)
{
	TxCKPS[1]=Tx_CKPS;									//保存当前分频比
	
	T2CONbits.TON=0;									//启停定时器
	T2CONbits.TSIDL=0;									//空闲模式是否省电
	T2CONbits.T32=0;									//=0单独作为16位定时器
														//=1则与TIMER3组合成32位定时器
														
	if(Tx_CKPS==TxCKPS_1)			T2CONbits.TCKPS=0B00;	//选择分频系数 【00】==>1:1 
	else if(Tx_CKPS==TxCKPS_8)		T2CONbits.TCKPS=0B01;	//选择分频系数 【01】==>1:8
	else if(Tx_CKPS==TxCKPS_64)		T2CONbits.TCKPS=0B10;	//选择分频系数 【10】==>1:64
	else if(Tx_CKPS==TxCKPS_256)	T2CONbits.TCKPS=0B11;	//选择分频系数 【11】==>1:256
				
	T2CONbits.TCS=0;									//=【0】选择时钟源：FOSC/2时钟	=【1】 有T1ECS决定
	IPC1bits.T2IP=4;									//中断优先级4 最高位7级
	TMR2=0;	
	//IFS0bits.T2IF=0;									//清除中断标志位
	//IEC0bits.T2IE=True;									//允许T2中断
}	 

/******************************************************** 
函数名称：void Setup_Timer2(uint X_us，uchar IsOpen)
功能描述: 设置Time2延时值
全局变量：无 
参数说明：X_us: 定时时间单位uS， IsOpen：=True表示需要打开 
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
		定时时间计算方法：
		指令周期Tcy ,预分频系数N ，周期寄存器PRX 所赋值为X ，则定时时间为
		T定时=Tcy*N(X+1) 
		Fcy=32MHZ/2M=16 ,T定时=1ms=1000us
		X=(1000*Fcy/N)-1 	
			
		所以定时值xms
********************************************************/ 
void Set_Timer2(uint X_us,uchar IsOpen)
{
	IFS0bits.T2IF=0;									//清除标志位
	ulong Tmp;
	Tmp=(X_us+40)*Fcy;
	Tmp=Tmp/TxCKPS[1]-1;
	PR2=(uint)(Tmp);						
	T2CONbits.TON=IsOpen;
}	

/******************************************************** 
函数名称：void TMR2_Enable(uchar IsOpen)
功能描述: Timer2 使能控制
全局变量：无 
参数说明：IsOpen：=True表示需要打开 =False 表示关闭
返回说明：无 
设 计 人：FEIHONG_KLD
版    本：1.0 
说    明：www.kld.cn
********************************************************/ 
void Timer2_Enable(uchar IsOpen)
{
	//IFS0bits.T2IF=0;									//清除标志位		
	T2CONbits.TON=IsOpen;
}

  /****&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 			定时器【2】设置====【结束】
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
