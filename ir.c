#include "util.h"
#include "ir.h"
#include "bytecode.h"

#include "stdlib.h"
#include "stdio.h"




typedef struct{
	Lisp*      expr;
	CodeBlock* scope;
}IRT_Expr;

typedef struct{
	Lisp*      expr;
	CodeBlock* scope;
}IRT_Let;

typedef struct{
	Type*      partypes;
	int        parct;
	Lisp*      expr;
	
	CodeBlock* scope;
}IRT_Lambda;


int modelExpr  (IRT_Expr* expr, BCProgram* bc){

	Lisp* input  = expr->expr->here.val.PVAL;
	Lisp* xforms = expr->expr->next;
	
	while(xforms != NULL){
		Lisp* xform = xforms->here.val.PVAL;
		
		// Build expression
		
		xforms = xforms->next;
	}

	return 0;
}

int modelLet   (IRT_Let* let, BCProgram* bc){

	return 0;
}

int modelLambda(IRT_Lambda* lm, BCProgram* bc){
	
	if((lm->expr->here.typ == OPRTYP) && (lm->expr->here.val.UVAL == LET)){
		IRT_Let lt;
		lt.scope = lm->scope;
		if(modelLet(&lt, bc)) return 1;
	}
	
	return 0;
}
















int countCodeVars(Lisp* code){
	int count = 0;
	while(code != NULL){
		int here = 0;
		if     (code->here.typ == LSPTYP) here = countCodeVars(code->here.val.PVAL);
		else if(code->here.typ == VARTYP) here = code->here.val.UVAL+1;
		count = (here > count)? here : count;
		code  = code->next;
	}
	return count;
}


int buildFunction(Program* prog, FnDef* fn, BCProgram* bc){
	int varct = countCodeVars(fn->codeSource);
	CodeBlock fnheader = makeCodeBlock(BKT_FUNCTION_HEAD, varct*2, varct, fn->prct, fn->rtct);
	
	Lisp* code = fn->codeSource;
	code = (code->here.typ == LSPTYP)? code->here.val.PVAL : code;
	printf("TOP : %i %lu\n", code->here.typ, code->here.val.UVAL);
	
	if((code->here.typ == OPRTYP) && (code->here.val.UVAL == LAMBDA)){
		IRT_Lambda lm;
		lm.scope = &fnheader;
		
	}else{
		printf("Unable to build function\n");
		return -1;
	}
	
	printCodeBlock (fnheader);
	addProgramBlock(bc, fnheader);
	
	return 0;
}



int buildBytecode(Program* prog, BCProgram* bc){
	
	*bc = makeBCProgram(prog->fnct * 16, prog->fnct);
	for(int i = 0; i < prog->fnct; i++){
		
		// For each function:
		// 1. Map variables within scope
		// 2. 
		if(prog->funcs[i].fnid == i){
			buildFunction(prog, &prog->funcs[i], bc);
		}
	}
	
	return 0;
}
