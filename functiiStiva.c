#include "TStiva.h"

void* InitS(size_t d)
{
    ASt s;
    s = (ASt)malloc(sizeof(TStiva));
    if(!s) return NULL;
    s->dime = d;
    s->vf = NULL;
    return (void*)s;

}

int Push(void* a, void* ae)
{
    ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));
    if(!aux) return 0;
    aux->info = malloc(((TStiva*)a)->dime);
    memcpy(aux->info, ae, ((TStiva*)a)->dime);
    aux->urm = ((TStiva*)a)->vf;
    ((TStiva*)a)->vf = aux;
    //free(aux->info);
    //free(aux);
    return 1;
}

int Pop(void* s, void* ae)
{
    if (((TStiva*)s)->vf == NULL) return 0;
    memcpy(ae, ((TStiva*)s)->vf->info, ((TStiva*)s)->dime);
    ACelSt aux = ((TStiva*)s)->vf;
    ((TStiva*)s)->vf = aux->urm;
    //free(aux->info);
    //free(aux);
    return 1;
}

int VidS(void *a)
{
    if(((TStiva*)a)->vf == NULL){
        return 1;
    }
    return 0;

}
