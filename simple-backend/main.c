#include "stdio.h"

#include "type.h"
#include "func.h"
#include "util.h"


int main(){
	Union str2 = makeUnion(5);
	((TypeUnion*)str2.pars)[0].prim = P_I32;
	str2.kinds[0]                   = TK_PRIMITIVE;
	((TypeUnion*)str2.pars)[1].prim = P_U8;
	str2.kinds[1]                   = TK_PRIMITIVE;
	((TypeUnion*)str2.pars)[2].prim = P_F16;
	str2.kinds[2]                   = TK_PRIMITIVE;
	((TypeUnion*)str2.pars)[3].prim = P_I8;
	str2.kinds[3]                   = TK_PRIMITIVE;
	((TypeUnion*)str2.pars)[4].prim = P_I32;
	str2.kinds[4]                   = TK_PRIMITIVE;
	
	Type   vec2 = (Type){(TypeUnion)str2, TK_UNION, 0, 0};
	
	if(!calcTypeSize(&vec2)) printf("Size fail\n");
	printType(vec2);
}
