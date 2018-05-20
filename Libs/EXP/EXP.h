
#ifndef EXP_H
#define EXP_H

//**********************************Macros**********************************************************
#define HI(x) (x & 0xFF00)>>8
#define LO(x) (x & 0x00FF)
#define HI16(x) (x & 0xFFFF0000)>>16
#define LO16(x) (x & 0x0000FFFF)
#define ADD(x,y) (x<<8)|(y)
#define ADD16(x,y) (x<<16)|(y)
#define max(a,b) ((a) > (b) ? (a) : (b))
#define HIGH_LEVEL 1
#define LOW_LEVEL  0

#endif