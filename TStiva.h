#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


typedef struct celst
{
    struct celst *urm;
    void* info;
} TCelSt, *ACelSt;

typedef struct stiva
{
    size_t dime;
    ACelSt vf; 
} TStiva, *ASt;


void* InitS(size_t d);
int Push(void* a, void* ae);
int Pop(void* s, void* ae);
void afiStiva(TStiva a);
ASt InitPool(int N);