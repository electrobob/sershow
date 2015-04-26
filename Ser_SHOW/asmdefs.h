//****************************************************************************
//
//	asmdefs.h - little helpers for asm
//

#ifndef _ASMDEFS_H
#define _ASMDEFS_H

/*
 *	These macros provide a way of defining structures in ASM
 *
 *	ie.:
 *	DSTART mystruct
 *	DBYTE somebyte
 *	DWORT someword
 *	DLONG blub
 *
 *	Will result in the followings symbols being set:
 *	mystruct_somebyte = 0
 *	mystruct_someword = 1
 *	mystruct_blub = 3
 *	mystruct_sizeof = 7
 */

#ifdef __ASSEMBLER__

.macro STRUCT NAME OFFSET=0
.set STRUCT_OFFSET, \OFFSET
.set STRUCT_NAME, \NAME
.endm

.macro LABEL NAME
.set \NAME, STRUCT_OFFSET
.endm

.macro BYTE NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+1
.endm

.macro UBYTE NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+1
.endm

.macro WORD NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+2
.endm

.macro UWORD NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+2
.endm

.macro LONG NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+4
.endm

.macro ULONG NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+4
.endm

.macro SPACE NAME SIZE
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+\SIZE
.endm

.macro PTR NAME
.equiv \NAME, STRUCT_OFFSET
.set STRUCT_OFFSET, STRUCT_OFFSET+2
.endm

//  STORE - put byte into I/O space using either "out"
//  or "sts"

.macro  STORE addr,reg
.if     \addr < 0x60
out    \addr - 0x20,\reg
.else
sts    \addr,\reg
.endif
.endm

//  LOAD - load byte from I/O space using either "in"
//  or "lds"

.macro  LOAD reg,addr
.if     \addr < 0x60
in     \reg,\addr - 0x20
.else
lds    \reg,\addr
.endif
.endm

//  LDA16 - load 16 bit address into x/y/z register

.macro  LDA16 reg,addr
.ifeqs  "\reg", "X"
ldi     XL, lo8(\addr)
ldi     XH, hi8(\addr)
.else
    .ifeqs  "\reg", "Y"
    ldi     YL, lo8(\addr)
    ldi     YH, hi8(\addr)
	.else
        .ifeqs  "\reg", "Z"
        ldi     ZL, lo8(\addr)
        ldi     ZH, hi8(\addr)
	    .else
		.error "LDA16 macro - unknown register \reg"
		.endif
    .endif
.endif
.endm

//	Muliple push / poop

.macro	MPUSH reg, morereg:vararg
		push	\reg
		.ifnb	\morereg
			MPUSH	\morereg
		.endif
.endm

.macro	MPOP reg, morereg:vararg
		pop		\reg
		.ifnb	\morereg
			MPOP	\morereg
		.endif
.endm

#endif
#endif
