#include "parser.h"
#include "interpreter.h"
#include "stdlib.h"
#include "stdio.h"





int parseHex(PARSERSTATE* state, int skip, uint64_t* ret){

  int index = state->head + skip;
  int revert= index;
  uint64_t val = 0;
  while((index < state->size)){
    char c = state->text[index];
    if((c >= '0') && (c <= '9')){
      val *= 16;
      val |= (c - '0');
    }else if((c >= 'A') && (c <= 'F')){
      val *= 16;
      val |= (c - 'A') + 10;
    }else if((c >= 'a') && (c <= 'f')){
      val *= 16;
      val |= (c - 'a') + 10;
    }else{
      *ret = val;
      state->head = index-1;
      return 1;
    }
    index++;
  }
  state->head = index-1;
  *ret = val;
  return 1;
}


int parseIdentifier(PARSERSTATE* state, char x, uint64_t* ret){

  char c = state->text[state->head];
  if(c == x){
    return parseHex(state, 1, ret);
  }
  *ret = 0;
  return 0;
}


int parseVar(PARSERSTATE* state, uint64_t* ret){
  return parseIdentifier(state, 'v', ret);
}

int parseTyp(PARSERSTATE* state, uint64_t* ret){
  return parseIdentifier(state, 't', ret);
}

int parseFnc(PARSERSTATE* state, uint64_t* ret){
  return parseIdentifier(state, 'f', ret);
}

int parseInt(PARSERSTATE* state, int64_t*  ret){
  uint64_t x;
  int r = parseIdentifier(state, 'i', &x);
  *ret = *(int64_t*)(&x);
  return r;
}

int parseUnt(PARSERSTATE* state, uint64_t* ret){
  return parseIdentifier(state, 'u', ret);
}

int parseFlt(PARSERSTATE* state, double* ret){
  uint64_t x;
  int r = parseIdentifier(state, 'r', &x);
  *ret = *(double*)(&x);
  return r;
}

int parseOpc(PARSERSTATE* state, uint64_t* ret){
  return parseIdentifier(state, 'x', ret);
}


int parseString(PARSERSTATE* state, STRING* ret){

  char c = state->text[state->head];
  if(c != '\"'){
    ret->size = 0;
    ret->text = NULL;
    return 0;
  }

  //Figure out how long the string is.
  int len = 0;
  int ix = state->head+1;
  int cont = 1;
  while(cont){
    if(state->text[ix] == '\\'){
      ix++;
    }else if(state->text[ix] == '\"'){
      len--;
      cont = 0;
    }
    len++;
    ix++;
    cont = cont && (ix < state->size);
  }

  ret->text = malloc(sizeof(char) * len);
  ix      = state->head+1;
  int tix = 0;
  cont = 1;
  while(cont){
    c = state->text[ix];
    if(c == '\\'){
      switch(state->text[ix+1]){
        case 'n' : ret->text[tix] = '\n'; break;
        case 'v' : ret->text[tix] = '\v'; break;
        case 't' : ret->text[tix] = '\t'; break;
        case 'r' : ret->text[tix] = '\r'; break;
        case '\\': ret->text[tix] = '\\'; break;
        case '\'': ret->text[tix] = '\''; break;
        case '\"': ret->text[tix] = '\"'; break;
        case '0' : ret->text[tix] = '\0'; break;
        case 'a' : ret->text[tix] = '\a'; break;
        default  : return 0;
      }
      ix++;
    }
    ret->text[tix] = state->text[ix];
    ix ++;
    tix++;
    cont = (tix < len);
    cont = cont && (state->text[ix+1] != '\"');
  }

  state->head = ix+1;
  ret->size = len;



  return 1;
}

void skipWhitespace(PARSERSTATE* state){
  while(1){
    char c = state->text[state->head];
    if((c == ' ') || (c == '\t') || (c == '\v') || (c == '\n')){
      state->head++;
    }else{
      return;
    }
    if(state->head == state->size){
      return;
    }
  }
}



LISP* parseLispAlt(PARSERSTATE* state){

  skipWhitespace(state);

  int nodes = 0;
  VAL vals[128];
  int typs[128];

  int cont = 1;
  while((state->head < state->size) && cont){
    if(nodes == 128){
      printf("Excessively long list error.");
      exit(1);
    }

    skipWhitespace(state);
    char c = state->text[state->head];
    switch(c){
      case '(':{
        printf("(\n");
        state->head++;
        vals[nodes].PVAL = parseLispAlt(state);
        typs[nodes]      = LSPTYP;
        nodes++;
      } break;

      case 'v':{
        uint64_t v;
        if(!parseVar(state, &v)){
          printf("Var error at %i.\n", state->head);
        }else{
          printf("v%lu\n", v);
          vals[nodes].UVAL = v;
          typs[nodes]      = VARTYP;
          nodes++;
        }
      } break;

      case 't':{
        uint64_t v;
        if(!parseTyp(state, &v)){
          printf("Typ error at %i.\n", state->head);
        }else{
          printf("t%lu\n", v);
          vals[nodes].UVAL = v;
          typs[nodes]      = TYPTYP;
          nodes++;
        }
      } break;

      case 'f':{
        uint64_t v;
        if(!parseFnc(state, &v)){
          printf("Fnc error at %i.\n", state->head);
        }else{
          printf("f%lu\n", v);
          vals[nodes].UVAL = v;
          typs[nodes]      = FNCTYP;
          nodes++;
        }
      } break;

      case 'i':{
        int64_t v;
        if(!parseInt(state, &v)){
          printf("Int error at %i.\n", state->head);
        }else{
          printf("i%li\n", v);
          vals[nodes].IVAL = v;
          typs[nodes]      = INTTYP;
          nodes++;
        }
      } break;

      case 'u':{
        uint64_t v;
        if(!parseUnt(state, &v)){
          printf("Unt error at %i.\n", state->head);
        }else{
          printf("u%lu\n", v);
          vals[nodes].UVAL = v;
          typs[nodes]      = VARTYP;
          nodes++;
        }
      } break;

      case 'r':{
        double v;
        if(!parseFlt(state, &v)){
          printf("Flt error at %i.\n", state->head);
        }else{
          printf("r%f\n", v);
          vals[nodes].FVAL = v;
          typs[nodes]      = FLTTYP;
          nodes++;
        }
      } break;

      case 'x':{
        uint64_t v;
        if(!parseOpc(state, &v)){
          printf("Opc error at %i.\n", state->head);
        }else{
          printf("x%lu\n", v);
          vals[nodes].UVAL = v;
          typs[nodes]      = OPRTYP;
          nodes++;
        }
      } break;

      case '\"':{
        STRING v;
        if(!parseString(state, &v)){
          printf("Str error at %i.\n", state->head);
        }else{
          printf("s%i\n", v.size);
          vals[nodes].SVAL = v;
          typs[nodes]      = STRTYP;
          nodes++;
        }
      } break;

      case ')':{
        printf(")\n");
        cont = 0;
      } break;

      case '\0':{
        cont = 0;
      } break;

      default: printf("@%i, unexpected: %c (%i)\n", state->head, state->text[state->head], state->text[state->head]);
    }
    state->head++;
  }

  if(nodes == 0){
    return NULL;
  }

  LISP  head;
  LISP* lasthead =  NULL;
  LISP* tapehead = &head;
  for(int i = 0; i < nodes; i++){
    lasthead = tapehead;
    tapehead = malloc(sizeof(LISP));
    lasthead->next = tapehead;
    tapehead->refc = 1;
    tapehead->here = vals[i];
    tapehead->type = typs[i];
    tapehead->next = NULL;
  }

  return head.next;
}



void printLisp(LISP* l){
  LISP* here = l;
  if(here == NULL){
    printf("nil ");
    return;
  }

  printf("(");
  while(here != NULL){
    switch(here->type){
      case FNCTYP : printf("f%lu ", here->here.UVAL     ); break;
      case INTTYP : printf("i%li ", here->here.IVAL     ); break;
      case UNTTYP : printf("u%lu ", here->here.UVAL     ); break;
      case FLTTYP : printf("r%f ",  here->here.FVAL     ); break;
      case STRTYP : printf("s%i ",  here->here.SVAL.size); break;
      case LSPTYP : printLisp(here->here.PVAL           ); break;
      case VARTYP : printf("v%lu ", here->here.UVAL     ); break;
      case OPRTYP : printf("x%lu ", here->here.UVAL     ); break;
      case TYPTYP : printf("t%lu ", here->here.UVAL     ); break;
      default:      printf("%i "  , here->type          ); break;
    }
    here = here->next;
  }
  printf(") ");
}
