%{
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include "structs/code.h"
#include "structs/types.h"

extern int yylineno;
extern Code code;
extern GHashTable* labels;

void yyerror (const char*);
void add(CodeElem);
int yylex();
%}

%union{
    int i;
    float f;
    GString* s;
}


%token _ADD _SUB _MUL _DIV _MOD _NOT _INF _INFEQ _SUP _SUPEQ _FADD _FSUB _FMUL _FDIV _FINF _FINFEQ _FSUP _FSUPEQ _PADD _CONCAT _ALLOC _ALLOCN _FREE _EQUAL _ATOI _ATOF _ITOF _FTOI _STRI _STRF _PUSHI _PUSHN _PUSHF _PUSHS _PUSHG _PUSHL _PUSHSP _PUSHFP _PUSHGP _LOAD _LOADN _DUP _DUPN _POP _POPN _STOREL _STOREG _STORE _STOREN _CHECK _SWAP _WRITE _READ _READI _READF _READS _JUMP _JZ _PUSHA _CALL _RETURN _START _NOP _ERR _STOP
%token<i> _INT
%token<f> _FLOAT
%token<s> _STRING _LABEL


%%
Program : Program Elem
        |
        ;

Elem    : Instr
        | _LABEL':'         { g_hash_table_insert(labels, $1->str, newHashData(code.array.len)); }
        ;

Instr   : _PUSHI   _INT    { Code_add( newCodeElem( PUSHI , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHF   _FLOAT  { Code_add( newCodeElem( PUSHF , newValue((Uvalue) $2, T_float ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHN   _FLOAT  { Code_add( newCodeElem( PUSHN , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _START           { Code_add( newCodeElem( START , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _ADD             { Code_add( newCodeElem( ADD   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _WRITE           { Code_add( newCodeElem( WRITE , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _NOT             { Code_add( newCodeElem( NOT   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _SUB             { Code_add( newCodeElem( SUB   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _MUL             { Code_add( newCodeElem( MUL   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _DIV             { Code_add( newCodeElem( DIV   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _MOD             { Code_add( newCodeElem( MOD   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _INF             { Code_add( newCodeElem( INF   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _INFEQ           { Code_add( newCodeElem( INFEQ , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _SUP             { Code_add( newCodeElem( SUP   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _SUPEQ           { Code_add( newCodeElem( SUPEQ , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FADD            { Code_add( newCodeElem( FADD  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FSUB            { Code_add( newCodeElem( FSUB  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FMUL            { Code_add( newCodeElem( FMUL  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FDIV            { Code_add( newCodeElem( FDIV  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FINF            { Code_add( newCodeElem( FINF  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FINFEQ          { Code_add( newCodeElem( FINFEQ, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FSUP            { Code_add( newCodeElem( FSUP  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FSUPEQ          { Code_add( newCodeElem( FSUPEQ, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _EQUAL           { Code_add( newCodeElem( EQUAL , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PADD            { Code_add( newCodeElem( PADD  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _ALLOC   _INT    { Code_add( newCodeElem( ALLOC , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _ALLOCN          { Code_add( newCodeElem( ALLOCN, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _DUP	   _INT    { Code_add( newCodeElem( DUP	  , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _DUPN            { Code_add( newCodeElem( DUPN	, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHG   _INT    { Code_add( newCodeElem( PUSHG , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHL	 _INT    { Code_add( newCodeElem( PUSHL , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHSP          { Code_add( newCodeElem( PUSHSP, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHFP          { Code_add( newCodeElem( PUSHFP, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHGP          { Code_add( newCodeElem( PUSHGP, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _POP	   _INT    { Code_add( newCodeElem( POP	  , newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _POPN            { Code_add( newCodeElem( POPN	, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _JUMP    _LABEL  { Code_add( newCodeElem( JUMP  , newValue((Uvalue) $2, T_string), newValue((Uvalue) -1, NOTHING) ) ); }
        | _JZ      _LABEL  { Code_add( newCodeElem( JZ    , newValue((Uvalue) $2, T_string), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHA   _LABEL  { Code_add( newCodeElem( PUSHA , newValue((Uvalue) $2, T_string), newValue((Uvalue) -1, NOTHING) ) ); }
        | _CALL            { Code_add( newCodeElem( CALL  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _RETURN          { Code_add( newCodeElem( RETURN, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _STOREL  _INT    { Code_add( newCodeElem( STOREL, newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _STOREG  _INT    { Code_add( newCodeElem( STOREG, newValue((Uvalue) $2, T_int   ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _NOP             { Code_add( newCodeElem( NOP   , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _STOP            { Code_add( newCodeElem( STOP  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _SWAP            { Code_add( newCodeElem( SWAP  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _READ            { Code_add( newCodeElem( READ  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _ATOI            { Code_add( newCodeElem( ATOI  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _ATOF            { Code_add( newCodeElem( ATOF  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _ITOF            { Code_add( newCodeElem( ITOF  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FTOI            { Code_add( newCodeElem( FTOI  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _STRI            { Code_add( newCodeElem( STRI  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _STRF            { Code_add( newCodeElem( STRF  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _CONCAT          { Code_add( newCodeElem( CONCAT, newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _FREE            { Code_add( newCodeElem( FREE  , newValue((Uvalue) -1, NOTHING ), newValue((Uvalue) -1, NOTHING) ) ); }
        | _PUSHS   _STRING { Code_add( newCodeElem( PUSHS , newValue((Uvalue) $2, T_string), newValue((Uvalue) -1, NOTHING) ) ); }
        ;

//_CAST
//_CASTN
// _
//|_LOAD
//|_LOADN
// _
//|_STORE
//|_STOREN
// _
//|_CHECK
//|_ERR


%%

void yyerror(const char *s){
    fprintf(stdout, "\e[1mline:%d: \e[31mERRO\e[0m\e[1m: %s\e[0m \n" , yylineno, s);
    _exit(0);
}
