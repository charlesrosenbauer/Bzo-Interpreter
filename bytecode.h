#ifndef __BYTECODE_HEADER__
#define __BYTECODE_HEADER__

#include "stdint.h"


typedef enum{
	IR_ADD		= 0x01,
	IR_SUB		= 0x02,
	IR_AND		= 0x03,
	IR_OR 		= 0x04,
	IR_XOR		= 0x05
}IR_Opcode;

typedef enum{
	IRP_I8,
	IRP_I16,
	IRP_I32,
	IRP_I64,
	IRP_U8,
	IRP_U16,
	IRP_U32,
	IRP_U64
}IR_PType;

typedef struct{
	IR_PType ptyp;		// For now, just this. Will add more complexity later.
}IR_Type;

typedef struct{
	IR_Opcode opc;
	IR_Type	  type;
	uint16_t  a, b, c, d;
}IR_Instruction;

typedef struct{
	// Table for instructions
	IR_Instruction* ops;
	int opSize, opCap;

	// Type table for local variables
	IR_Type* varTyps;
	int vtSize, vtCap;


}CodeBlock;




#endif