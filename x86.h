#ifndef __X86_HEADER__
#define __X86_HEADER__


#include "stdint.h"


typedef enum{
	// Encoding:
	// XY AA BB CC DD EE
	// X  : If nonzero, the opcode has no parameters or prefixes
	//  Y : Number of opcode bytes (+1)
	// AA : Required Prefix byte (00 if none. 66 and 48 are encoded by other means)
	// BB : Primary   Opcode
	// CC : Secondary Opcode (00 if none)
	// DD : Tertiary  Opcode (00 if none)
	// EE : 0X, encoding additional R/M bits

	// Last 4 bits encode:
	// XYYY :
	//   X  = does this opcode use R/M field?
	//   Ys = contents of R/M field
	
	// Arithmetic
	X86_ADD       = 0x000000000000,
	X86_SUB       = 0x000028000000,
	X86_INC       = 0x000040000000,
	X86_DEC       = 0x000048000000,
	X86_NEG       = 0x0000F600000B,
	X86_MUL       = 0x0000F600000C,
	X86_DIV       = 0x0000F600000E,
	X86_IDIV      = 0x0000F600000F,
	X86_IMUL      = 0x01000FAF0000,
	X86_LEA       = 0x00008D000000,
	
	// Carry Arithmetic
	X86_CMC       = 0x0000F5000000,
	X86_CLC       = 0x0000F8000000,
	X86_ADC       = 0x000010000000,
	X86_SBB       = 0x000018000000,
	
	// Memory
	X86_PUSH      = 0x000006000000,
	X86_POP       = 0x000007000000,
	X86_MOV       = 0x000089000000,
	X86_XCHG      = 0x000086000000,
	
	// Bitwise
	X86_OR        = 0x000008000000,
	X86_AND       = 0x000020000000,
	X86_XOR       = 0x000030000000,
	X86_NOT       = 0x0000F608000A,
	X86_ROL       = 0x0000D0000008,
	X86_ROR       = 0x0000D0000009,
	X86_RCL       = 0x0000D000000A,
	X86_RCR       = 0x0000D000000B,
	X86_SAL       = 0x0000D000000C,
	X86_SAR       = 0x0000D000000D,
	X86_SHL       = 0x0000D000000E,
	X86_SHR       = 0x0000D000000F,
	
	// Comparison
	X86_CMP       = 0x000038000000,
	X86_TEST      = 0x000084000000,
	
	// Flow
	X86_CALL      = 0x00009A000000,
	X86_JCC       = 0x000070000000,
	X86_RETN      = 0x1000C3000000,
	X86_INT       = 0x0000CC000000,
	X86_JMP       = 0x0000E9000000,
	
	// Other
	X86_NOP       = 0x000090000000

}X86Opcode;

typedef enum{

	// Arithmetic
	BX86_ADD,
	BX86_SUB,
	BX86_MUL,
	BX86_DIV,
	BX86_IMUL,
	BX86_IDIV,
	BX86_NEG,
	BX86_INC,
	BX86_DEC
	
}X86Bytecode;


typedef enum{
	RAX   =  0,
	RCX   =  1,
	RDX   =  2,
	RBX   =  3,
	RSP   =  4,
	RBP   =  5,
	RSI   =  6,
	RDI   =  7,
	R8    =  8,
	R9    =  9,
	R10   = 10,
	R11   = 11,
	R12   = 12,
	R13   = 13,
	R14   = 14,
	R15   = 15,
	XMM0  = 16,
	XMM1  = 17,
	XMM2  = 18,
	XMM3  = 19,
	XMM4  = 20,
	XMM5  = 21,
	XMM6  = 22,
	XMM7  = 23,
	XMM8  = 24,
	XMM9  = 25,
	XMM10 = 26,
	XMM11 = 27,
	XMM12 = 28,
	XMM13 = 29,
	XMM14 = 30,
	XMM15 = 31,
	NOREG = -1
}X86Register;

typedef enum{
	SC_8,
	SC_16,
	SC_32,
	SC_64,
	
	VC_16x8,
	VC_32x8,
	VC_64x8,
	VC_8x16,
	VC_16x16,
	VC_32x16,
	VC_4x32,
	VC_8x32,
	VC_16x32,
	VC_2x64,
	VC_4x64,
	VC_8x64
}ValSize;

typedef enum{
	AM_RR,	
	AM_RM
}X86AddrMode;

typedef struct{
	// opc a, [b*scale + c + disp]
	int8_t       scale;
	X86Register  a, b, c;
	int32_t      disp;
}X86Addr;

typedef struct{
	X86Opcode 	opc;
	ValSize	  	bitsize;
	X86Register a, b;
	X86AddrMode mode;
	X86Addr     addr;
	uint64_t    imm; 
	uint8_t		lock;
}X86Op;


typedef struct{
	X86Op*		ops;
	int opct, opcap;
}X86Block;

typedef struct{
	uint8_t*	bytes;
	uint64_t	offset;
	int			codesize, codecap;
	
	uint64_t*	addresses;
	int*		addroffsets;
	
	int			addrsize, addrcap;
}MachineBlock;


typedef struct{
	X86Opcode   opc;
	ValSize     bitsize;
	int         a, b, c, q, r;
	uint64_t    imm;
	uint8_t     lock;
}X86BCOp;

typedef struct{
	X86BCOp*    ops;
	int opct, opcap;
}X86BCBlock;



typedef struct{
	X86Register reg;
	int         offset;
}X86Value;

typedef struct{
	X86Value*      pars;
	X86Value*      rets;
	X86Value*      vars;
	
	int parct, retct, varct, stackSize, blockSize;

	X86BCBlock*    bcblocks;
	MachineBlock*  mcblocks;
	
	int bct, mct;
}X86Function;



MachineBlock writeMachineBlock(X86Block);

X86Block allocRegs       (X86BCBlock);
int      functionRegAlloc(X86Function*);
int      x86OpRegMasks   (X86Opcode, uint64_t*, uint64_t*, uint64_t*, uint64_t*, uint64_t*, uint8_t*);

int      functionRegAlloc(X86Function*);

#endif
