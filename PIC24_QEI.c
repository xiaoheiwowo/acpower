#include     "p24EP32MC202.h"
#include     "PIC24_QEI.h"
#include     "PIC24_TimerX.h"

//**************************QEI�趨************************************��
void InitQEI()//�ж�ΪĬ��4��
{
    _QEIEN=1;                // ʹ��QEIģ��_QEIEN=1;
    POS1HLD=0;//λ�ü�������ֵ
    POS1CNTL=1000;//λ�ü�������ֵ
    _FLTREN=1;//ʹ�����������˲�����Ҫ�����ϵ����˲�
    _QFDIV=0b111;//Fcy128��Ƶ�˲�
    _GATEN=0;
    _INTDIV=0b111;//Fcy128��Ƶ�������
    
    _PCHEQIEN=1;//λ�ü��������ڵ��ڱȽ��ж�����
    QEI1GECL=1000;//����λ�ñȽ�ֵ�Ĵ���
    QEI1GECH=0;//����λ�ñȽ�ֵ�Ĵ���
    
    _PCLEQIEN=1;//λ�ü�����С�ڵ��ڱȽ��ж�����
    QEI1LECL=1000;//С��λ�ñȽ�ֵ�Ĵ���
    QEI1LECH=0;//С��λ�ñȽ�ֵ�Ĵ���
    
    INT1TMRL=0;//�����������ֵ
    INT1TMRH=0;//�����������ֵ
    INT1HLDL=0;//������ּĴ�����ֵ
    INT1HLDH=0;//������ּĴ�����ֵ
//  _QEI1IP=5;//�Ƚ��жϼ�5
    _QEI1IF=0;//�жϱ�־λ����
    _QEI1IE=1;//���QEI�Ƚ��ж�
}

//int ac=0;������
int  _abnumber=0;             //ȫ�ֱ����������������ֵ����������ʱ�����������жϣ������д����

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


//    ac=ac+1;������
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
                        intls3=-28000000;//�൱��1���Ӽ���������ļ���ֵ��Ӧ�������MAXT
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
     POS1CNTL=1000;//��λ�üĴ���
     _QEI1IF=0;//�жϱ�־λ����
}
