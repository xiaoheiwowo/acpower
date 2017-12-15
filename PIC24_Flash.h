/* 
 * File:   flashmudol.h
 * Author: dengheping
 *����ģ���ڵ��õ�ʱ���ڲ�����жϾֲ���7�����жϣ��ú���0�ָ�cpu�ж�Ĭ�ϼ���0
 * ��ģ��������������һ��128��Ԫ�ص�����ɹ��û�ʹ�ã�source[128]�Ƕ�д�������飬����
 * _wr_source_rom(����������source[]�������rom�У�����_copy_rom_source()�������
 * rom�����ݴ���source[],ÿ�δ�rom�ж�д��Ϊһ��128Ԫ�ص������飬��дromǰһ��Ҫ���ú���
 *  _eraseflash()������Ӧrom�����в����������޷�д��rom��
 * Created on 2017��7��5��, ����1:50
 */

#ifndef PIC24_FLASH_H
#define	PIC24_FLASH_H
void _eraseflash();
void _wr_source_rom();   //��rom��Ԥ���128���֣�int����������source[128]������
void _copy_rom_source(); //��source[128]�������rom��
extern int source[128];    //�ṩ���û��Ŀɴ洢����

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PIC24_FLASH_H */

