/* 
 * File:   PIC24_QEI.h
 * Author: liupengyu
 *
 * Created on 2017年7月4日, 下午1:21
 */

#ifndef PIC24_QEI_H
#define	PIC24_QEI_H

#define MAXT 0.015
#define MINT 0.005
#define MAXTN 1
#define MINTN 28

extern int _abnumber;//舵机待用脉冲值


extern void InitT2();
extern void InitQEI();


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/*PIC24_QEI_H */

