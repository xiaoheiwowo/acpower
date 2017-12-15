/* 
 * File:   flashmudol.h
 * Author: dengheping
 *本子模块在调用的时候内部会对中断局部置7屏蔽中断，用后置0恢复cpu中断默认级别0
 * 本模块有三个函数和一个128个元素的数组可供用户使用，source[128]是读写缓存数组，调用
 * _wr_source_rom(）函数将把source[]数组存入rom中，调用_copy_rom_source()函数则把
 * rom中数据存入source[],每次从rom中读写都为一个128元素的整数组，在写rom前一定要调用函数
 *  _eraseflash()，对相应rom区进行擦除，否则无法写入rom。
 * Created on 2017年7月5日, 下午1:50
 */

#ifndef PIC24_FLASH_H
#define	PIC24_FLASH_H
void _eraseflash();
void _wr_source_rom();   //把rom中预存的128个字（int）的数存入source[128]数组中
void _copy_rom_source(); //把source[128]数组存入rom中
extern int source[128];    //提供给用户的可存储数组

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PIC24_FLASH_H */

