#include     "p24EP32MC202.h"
#include     "PIC24_QEI.h"
#include     "PIC24_TimerX.h"

//**************************QEI设定************************************、
void InitQEI()//中断为默认4级
{
    _QEIEN=1;                // 使能QEI模块_QEIEN=1;
    POS1HLD=0;//位置计数器初值
    POS1CNTL=1000;//位置计数器初值
    _FLTREN=1;//使能输入数字滤波，主要靠板上电容滤波
    _QFDIV=0b111;//Fcy128分频滤波
    _GATEN=0;
    _INTDIV=0b111;//Fcy128分频间隔计数
    
    _PCHEQIEN=1;//位置计数器大于等于比较中断允许
    QEI1GECL=1000;//大于位置比较值寄存器
    QEI1GECH=0;//大于位置比较值寄存器
    
    _PCLEQIEN=1;//位置计数器小于等于比较中断允许
    QEI1LECL=1000;//小于位置比较值寄存器
    QEI1LECH=0;//小于位置比较值寄存器
    
    INT1TMRL=0;//间隔计数器初值
    INT1TMRH=0;//间隔计数器初值
    INT1HLDL=0;//间隔保持寄存器初值
    INT1HLDH=0;//间隔保持寄存器初值
//  _QEI1IP=5;//比较中断级5
    _QEI1IF=0;//中断标志位清零
    _QEI1IE=1;//许可QEI比较中断
}

//int ac=0;调试用
int  _abnumber=0;             //全局变量，舵机待用脉冲值，调用清零时请屏蔽所以中断，避免读写竞争

void __attribute__((__interrupt__,no_auto_psv)) _QEI1Interrupt(void)
{
    int ab;
    static long  intls1=0;
    static long  intls2=0;
    static long  intls3=0;
    static float intls4=0;
    typedef union
    {
        long   intls;
        struct
        {
            int intlsL;
            int intlsH;
        };
    } INTLS;
      INTLS _intls;


//    ac=ac+1;调试用
    ab=POS1CNTL;
    _intls.intlsL= INT1HLDL;
    _intls.intlsH= INT1HLDH;
//    ac=INT1HLDL;
    if(ab==1000)
    {
        _QEI1IF=0;
    }
    else
    {
        if(PORTBbits.RB7==PORTBbits.RB6)
        {
            TMR2=0;
            _tn10=0;
            T2CONbits.TON=1;
            if(ab<=1000)
            {
               intls2=-_intls.intls;  
            }
            else
            {
               intls2=_intls.intls;
            }
            if((intls1>=0 && intls2>=0) || (intls1<=0 && intls2<=0))
            {
                if(_tf2==1)
                {
                    _tf2=0;
                    if(ab<=1000)
                    {
                        intls3=-28000000;//相当于1分钟间隔计数器的计数值，应该足大于MAXT
                    }
                    else
                    {
                        intls3=28000000;
                    }   
                }
                else
                {
                   intls3=intls1+intls2;
                } 
            }
            else
            {
                intls3=0;
            }
            intls1=0;
            intls2=0;
            if(intls3!=0)  
            {
                intls4=intls3;
                intls4=intls4*2.1333/1000000.0;                
                intls3=0;
                if(intls4>=MAXT || intls4<=-MAXT)
                {
                    if(ab>=1000)
                    {
                         _abnumber=_abnumber+MAXTN; 
                    }
                    else
                    {
                         _abnumber=_abnumber-MAXTN; 
                    }

                }
                else if(intls4<=MINT || intls4>=-MINT)
                {
                    if(ab>=1000)
                    {
                        _abnumber=_abnumber+MINTN; 
                    }
                    else
                    {
                        _abnumber=_abnumber-MINTN;
                    }

                }
                else
                {
                    if(ab>=1000)
                    {
                         _abnumber=_abnumber+(MINTN-MAXTN)/(MINT-MAXT)*(intls4-MINT)+MINTN;
                    }
                    else
                    {
                         _abnumber=_abnumber+(MINTN-MAXTN)/(MINT-MAXT)*(intls4+MINT)-MINTN;
                    }
                }
            }    
        }
        else
        {
            if(ab<=1000)
            {
                intls1=-_intls.intls; 
            }
            else
            {
                intls1=_intls.intls; 
            }
        }
    }
     POS1HLD=0;
     POS1CNTL=1000;//清位置寄存器
     _QEI1IF=0;//中断标志位清零
}
