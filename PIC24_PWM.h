#include "Macro.h"

#ifndef  PIC24EP_PWM_H
#define  PIC24EP_PWM_H

extern void Init_PWM();
extern void ServoAbsPos(int wz);

extern int  _Position;
extern int  _Zero;
extern int  _PosMax;

#endif
