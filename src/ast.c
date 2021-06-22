#include "stdint.h"
#include "stdio.h"

#include "program.h"
#include "util.h"
#include "ast.h"





void printASTExpr(ASTExpr* expr){
	switch(expr->type){
		case XT_INT : printf(" I%li " , expr->tk.data.i64     ); break;
		case XT_FLT : printf(" F%f "  , expr->tk.data.f64     ); break;
		case XT_STR : printf(" S%s "  , expr->tk.data.str.text); break;
		case XT_TAG : printf(" T%s "  , expr->tk.data.str.text); break;
		case XT_ID  : printf(" ID%li ", expr->tk.data.i64     ); break;
		case XT_MID : printf(" MI%li ", expr->tk.data.i64     ); break;
		default: printf(" (?) "); break;
	}
}
