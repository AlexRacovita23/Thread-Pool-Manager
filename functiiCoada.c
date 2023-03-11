#include "TCoada.h"

void* InitQ(size_t d)
{
    AQ q;
    q = (AQ)malloc(sizeof(TCoada));
    if(!q) return NULL;
    q->dime = d;
    q->ic = NULL;
    q->sc = NULL;
    return (void*)q;
}

int IntrQ(void* a, void* ae)
{ 
    ACelQ aux = (ACelQ)malloc(sizeof(TCelQ)); 
    if(!aux) return 0;
    aux->info = malloc(((TCoada*)a)->dime);
    if(!aux->info) return 0;
    memcpy(aux->info, ae, ((TCoada*)a)->dime);
    aux->urm=NULL;
    if(((TCoada*)a)->ic == NULL) {
        ((TCoada*)a)->ic=aux;
        ((TCoada*)a)->sc=aux;
    }
    else {
        ((TCoada*)a)->sc->urm = aux;
        ((TCoada*)a)->sc = aux;
    }
    //free (aux->info);
    //free (aux);
    return 1;
}

int ExtrQ(void* a, void *ae)
{
    ACelQ aux;

    aux = ((TCoada*)a)->ic;
    ((TCoada*)a)->ic = aux->urm;
    memcpy(ae, aux->info, ((TCoada*)a)->dime);
    free(aux->info);
    free(aux);

    if(((TCoada*)a)->ic == NULL) {
    ((TCoada*)a)->sc = NULL;
    }
    return 1;
}

void ConcatQ(void* destinatie, void*sursa)
{
    if(((TCoada*)destinatie)->ic == NULL && ((TCoada*)destinatie)->sc == NULL){
        ((TCoada*)destinatie)->ic = ((TCoada*)sursa)->ic;
        ((TCoada*)destinatie)->sc = ((TCoada*)sursa)->sc;
        ((TCoada*)sursa)->ic = ((TCoada*)sursa)->sc = NULL;
    }
    if(((TCoada*)sursa)->ic == NULL && ((TCoada*)sursa)->sc == NULL)
    return;
    ((TCoada*)destinatie)->sc->urm = ((TCoada*)sursa)->ic;
    ((TCoada*)destinatie)->sc = ((TCoada*)sursa)->sc;
    ((TCoada*)sursa)->ic = ((TCoada*)sursa)->sc = NULL;
}

int VidQ(void *a)
{
    if(((TCoada*)a)->ic == NULL && ((TCoada*)a)->sc == NULL){
        return 1;
    }
    return 0;

}