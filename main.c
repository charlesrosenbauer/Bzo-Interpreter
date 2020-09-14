#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "parser.h"
#include "memory.h"
#include "util.h"
#include "types.h"
//#include "x86-reader.h"
#include "hashtable.h"
//#include "elf.h"

#include "bytecode.h"





int main(int argc, char** argv){

  
  FILE*  pFile;
  uint8_t*  buffer;
  int       size;
  size_t    result;

  if(argc < 2){
    printf("No input program.\n");
    return -1;
  }

  loadFile(argv[1], &buffer, &size);  

  PARSERSTATE p;
  p.text = (char*)buffer;
  p.head = 0;
  p.size = size;

  PROGRAM* prog;
  prog = parseProgram(&p, 64, 64);

  int err = buildTypes(prog);
  if(err != 0){
    printf("Error building types.\n");
    return -1;
  }

  printProgram(prog);

  /*
  CodeBlock blk = makeCodeBlock(BKT_FUNCTION_HEAD, 64, 64, 3, 1);
  appendOpcode(&blk, (IR_Instruction){IR_ADD, IRP_I64, (IR_Pars){1, 2, 0, 6, 0}});
  appendOpcode(&blk, (IR_Instruction){IR_NOP, IRP_I64, (IR_Pars){3, 6, 0, 5, 0}});
  appendOpcode(&blk, (IR_Instruction){IR_ADD, IRP_I64, (IR_Pars){3, 6, 0, 4, 0}});
  printCodeBlock(blk);
  printf("#: %lu\n", hashBlock(&blk));

  blk = denopCodeBlock(blk);
  printCodeBlock(blk);
  printf("#: %lu\n", hashBlock(&blk));*/

  //HASHTABLE x86tab = loadOpcodeTable("x86ops");
  //printf("%i\n", x86tab.stacktop);

  //uint8_t* elfbuffer;
  //int elfsize = makeELF(&elfbuffer, NULL, 0, NULL, 0);
  //writeELF("program", elfbuffer, elfsize);
}
