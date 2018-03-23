#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "structs/types.h"

int semAdd();
int semSub();
int semMul();
int semDiv();
int semMod();
int semWrite();
int semNot();
int semInf();
int semInfeq();
int semSup();
int semSupeq();
int semEq();
int semDif();
int semFadd();
int semFsub();
int semFmul();
int semFdiv();
int semFcos();
int semFsin();
int semFtan();
int semFinf();
int semFinfeq();
int semFsup();
int semFsupeq();
int semFeq();
int semFdif();
int semPadd();
int semConcat();
int semAlloc();
int semAllocn();
int semFree();
int semEqual();
int semAtoi();
int semAtof();
int semItof();
int semFtoi();
int semStri();
int semStrf();
int semPushi(Value);
int semPushn();
int semPushf(Value);
int semPushs();
int semPushg();
int semPushl();
int semPushsp();
int semPushfp();
int semPushgp();
int semLoad();
int semLoadn();
int semDup();
int semDupn();
int semPop();
int semPopn();
int semStorel();
int semStoreg();
int semStore();
int semStoren();
int semCheck();
int semSwap();
int semRead();
int semReadi();
int semReadf();
int semReads();
int semJump();
int semJz();
int semPusha();
int semCall();
int semReturn();
int semStart();
int semNop();
int semErr();
int semStop();
int semLabel();


#endif