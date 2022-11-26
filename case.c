#include <stdio.h>
#include "case.h"

void case_vider(t_case* k)
{
    /*
    Fonction qui permet de libérer(vider une case) ie: qui met à vide ses atributs
    */
    k->elem=NULL;
    k->type=VIDE;
}

void case_remplir(t_case* k,int type,void* elem)
{
    /*
    Fonction qui permet de remplir une case c'est à dire d'attribuer un type donné à une case
    */
    k->type=type;
    k->elem=elem;
}

t_case* case_allouer()
    /*
    Fonction qui permet d'allouer une case de type vide
    */
{
    t_case* nouv;
    nouv=malloc(sizeof(t_case));

    case_vider(nouv);

    return nouv;
}

int case_libre(t_case* kase)
    /*
    Fonction qui teste si une case est libre: 0 si vide et 1 sinon
    */
{
    int libre=1;

    if(kase->type!=VIDE)
    {
        libre=0;
    }
    return libre;
}

void case_free(t_case* k)
    /*
    Fonction qui permet de supprimer une case
    */
{
    free(k);
}
