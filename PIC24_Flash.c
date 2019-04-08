#include "p24EP32MC202.h"
#include "libpic30.h"
#include "PIC24_Flash.h"
//定义一个rom区域512个指令字，地址则*2=1024个地址，对齐，方便擦除
int __attribute__((space(prog),aligned(512*2))) dat[512]={0,1688,3867,3788,36,1875};

_prog_addressT p;
int source[128]={0};//提供给用户的可存储数组
unsigned int offset;
unsigned int k;
void _copy_rom_source()    //把rom中预存的128个字（int）的数存入source[128]数组中
{
    TBLPAG=__builtin_tblpage(dat);            //获得rom表页地址
    offset=__builtin_tbloffset(dat);          //获得rom表内偏移地址
    for (0;k<128;k++)
    {
         source[k]=__builtin_tblrdl(offset+2*k);
    }
}
void _wr_source_rom()       //把source[128]数组存入rom中
{
    NVMCON=0X4001;//写双字设定
    TBLPAG=0xfa;//写锁存器页地址
    offset=0;   //写锁存器偏移0或者2只有两个指令字
    unsigned int offset00;
    unsigned int page00;
    page00=__builtin_tblpage(dat);    //待写rom起始地址所在页
    offset00=__builtin_tbloffset(dat);//待写rom起始地址所在页内偏移
    
    k=0;
    while(k<128/2)                //写128/2=64次循环，每次写两个source元素
    {
       
        __builtin_tblwtl(offset,source[2*k]);     //向第一个锁存器中写低字
        __builtin_tblwtl(offset+2,source[2*k+1]); //向第二个锁存器中写低字
        
        NVMADRU=page00;                //待写入rom页，不变
        NVMADR=offset00+4*k;           //写一个循环两个元素，用掉4个地址，每个元素占用两个地址，只用占用的低地址
        _IPL=7;
        __builtin_write_NVM();         //启动写序列使能
        _IPL=0;
        k++;
    }
}

void _eraseflash()                     //擦除页函数
{
    _init_prog_address(p,dat);  //c30库函数，获得dat特殊指针，不是普通c指针，注意
    _IPL=7;
    _erase_flash(p);                    //启动擦除页512个指令字
    _IPL=0;
}

