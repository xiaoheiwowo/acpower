#include "p24EP32MC202.h"
#include "libpic30.h"
#include "PIC24_Flash.h"
//����һ��rom����512��ָ���֣���ַ��*2=1024����ַ�����룬�������
int __attribute__((space(prog),aligned(512*2))) dat[512]={0,1688,3867,3788,36,1875};

_prog_addressT p;
int source[128]={0};//�ṩ���û��Ŀɴ洢����
unsigned int offset;
unsigned int k;
void _copy_rom_source()    //��rom��Ԥ���128���֣�int����������source[128]������
{
    TBLPAG=__builtin_tblpage(dat);            //���rom��ҳ��ַ
    offset=__builtin_tbloffset(dat);          //���rom����ƫ�Ƶ�ַ
    for (0;k<128;k++)
    {
         source[k]=__builtin_tblrdl(offset+2*k);
    }
}
void _wr_source_rom()       //��source[128]�������rom��
{
    NVMCON=0X4001;//д˫���趨
    TBLPAG=0xfa;//д������ҳ��ַ
    offset=0;   //д������ƫ��0����2ֻ������ָ����
    unsigned int offset00;
    unsigned int page00;
    page00=__builtin_tblpage(dat);    //��дrom��ʼ��ַ����ҳ
    offset00=__builtin_tbloffset(dat);//��дrom��ʼ��ַ����ҳ��ƫ��
    
    k=0;
    while(k<128/2)                //д128/2=64��ѭ����ÿ��д����sourceԪ��
    {
       
        __builtin_tblwtl(offset,source[2*k]);     //���һ����������д����
        __builtin_tblwtl(offset+2,source[2*k+1]); //��ڶ�����������д����
        
        NVMADRU=page00;                //��д��romҳ������
        NVMADR=offset00+4*k;           //дһ��ѭ������Ԫ�أ��õ�4����ַ��ÿ��Ԫ��ռ��������ַ��ֻ��ռ�õĵ͵�ַ
        _IPL=7;
        __builtin_write_NVM();         //����д����ʹ��
        _IPL=0;
        k++;
    }
}

void _eraseflash()                     //����ҳ����
{
    _init_prog_address(p,dat);  //c30�⺯�������dat����ָ�룬������ͨcָ�룬ע��
    _IPL=7;
    _erase_flash(p);                    //��������ҳ512��ָ����
    _IPL=0;
}

