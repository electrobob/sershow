//****************************************************************************
//
//  gammacorrection.h - PWM gamma correction module
//

#ifndef _GAMMA_CORRECTION_H
#define _GAMMA_CORRECTION_H

#include <avr/common.h>

//  Functions are implemented as MACROS

#ifdef __ASSEMBLER__

//  GCN_LOOKUP7TO8 - lookups 7 bit brightness to 8 bit pwm, Z reg destroyed

.macro  GCN_LOOKUP7TO8  addr,reg
    ldi     r30, lo8(\addr)
    ldi     r31, hi8(\addr)
    andi    \reg, 0x7f
    add     r30, \reg
    clr     \reg
    add     r31, \reg
    lpm     \reg, Z
.endm

.macro  GCN_TABLE7TO8
    .byte   0,0,0,0,0,0,0,0
    .byte   1,1,1,1,1,2,2,2
    .byte   3,3,3,4,4,5,5,6
    .byte   7,7,8,8,9,10,11,11
    .byte   12,13,14,15,16,17,18,19
    .byte   20,21,22,24,25,26,27,29
    .byte   30,31,33,34,36,37,39,40
    .byte   42,44,45,47,49,51,53,55
    .byte   56,58,60,62,65,67,69,71
    .byte   73,75,78,80,82,85,87,90
    .byte   92,95,97,100,103,105,108,111
    .byte   114,117,120,122,125,128,132,135
    .byte   138,141,144,147,151,154,157,161
    .byte   164,168,171,175,179,182,186,190
    .byte   193,197,201,205,209,213,217,221
    .byte   225,229,233,238,242,246,251,255
.endm

#else

#define GCN_LOOKUP7TO8(TBL, BRIGHT) (*(TBL+((BRIGHT)&0x7f)))

#define GCN_TABLE7TO8 {\
    0,0,0,0,0,0,0,0,\
    1,1,1,1,1,2,2,2,\
    3,3,3,4,4,5,5,6,\
    7,7,8,8,9,10,11,11,\
    12,13,14,15,16,17,18,19,\
    20,21,22,24,25,26,27,29,\
    30,31,33,34,36,37,39,40,\
    42,44,45,47,49,51,53,55,\
    56,58,60,62,65,67,69,71,\
    73,75,78,80,82,85,87,90,\
    92,95,97,100,103,105,108,111,\
    114,117,120,122,125,128,132,135,\
    138,141,144,147,151,154,157,161,\
    164,168,171,175,179,182,186,190,\
    193,197,201,205,209,213,217,221,\
    225,229,233,238,242,246,251,255 }
    
void GCN_copy7to8(uint8_t *dst, uint8_t *src, uint8_t *tbl);

#endif
#endif /* GAMMACORRECTION_H_ */