#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct celq
{
    struct celq *urm;
    void* info;
} TCelQ, *ACelQ;

typedef struct coada
{
    size_t dime;
    ACelQ ic, sc;
} TCoada, *AQ;

void* InitQ(size_t d);
int IntrQ(void* a, void* ae);
int ExtrQ(void* a, void *ae);
int VidQ(void *a);
void ConcatQ(void* destinatie, void*sursa);