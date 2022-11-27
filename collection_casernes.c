#include "collection_casernes.h"

t_collection_casernes* collection_casernes_creer() // création
{
    t_collection_casernes* nouv;
    nouv=malloc(sizeof(t_collection_casernes));
    nouv->taille = 0; //taille initiale
    nouv->taille_max=TAILLE_INITIALE_COLLECTION;
    nouv->caserne=malloc(nouv->taille_max*sizeof(t_caserne*)); // Liste des casernes
    return nouv;
}

void collection_casernes_ajouter_caserne(t_collection_casernes* collection_casernes,t_caserne* new_caserne)//Ajout d'une caserne
{
    // Réallocation si la taille maximale est ateinte
    if(collection_casernes->taille==collection_casernes->taille_max) // Si la taille maximale est atteinte
    {
        collection_casernes_reallouer(collection_casernes); // Augmenter la taille en faisant une suppression , réallocation et recopie
    }
    collection_casernes->caserne[collection_casernes->taille]=new_caserne; // Aujouter à la derniére place la nouvelle caserne
    collection_casernes->taille++; //mise à jour de la taille
}

void collection_casernes_liberer(t_collection_casernes* collection_casernes)
{
    int i;

    for(i=0;i<collection_casernes->taille;i++)
    {
        caserne_liberer(collection_casernes->caserne[i]);
    }
    free(collection_casernes->caserne);
    free(collection_casernes);
}

void collection_casernes_reallouer(t_collection_casernes* collection_casernes)
{
    collection_casernes->taille_max=3*collection_casernes->taille_max/2; // augmente la taille d'un facteur 3/2
    collection_casernes->caserne=realloc(collection_casernes->caserne,collection_casernes->taille_max*sizeof(t_caserne*)); //réallocaton
}

int** collection_casernes_tableau_longueurs(t_collection_casernes* collection_casernes,t_case*** kase,t_bfs* bfs,t_collection_habitation* collection_habitation)
// calcul les plus cours chemins entre les casernes et les habitations
{
    int i;
    int** longueurs_casernes_maisons;

    longueurs_casernes_maisons=malloc(collection_casernes->taille*sizeof(int*));

    bfs->ordre=collection_habitation->taille_max;
    for(i=0; i<collection_casernes->taille; i++)
    {
        bfs->Pos_X=collection_casernes->caserne[i]->pos_X;
        bfs->Pos_Y=collection_casernes->caserne[i]->pos_Y;
        longueurs_casernes_maisons[i]=recherchepluscourtchemin(bfs,kase);
    }
    return longueurs_casernes_maisons;
}

void collection_casernes_proteger(t_collection_casernes* collection_casernes,t_collection_habitation* collection_habitation,int** longueurs)
{
    int i,j;
    for(i=0;i<collection_habitation->taille;i++) // parcours les caserenes
    {
        for(j=0;j<collection_casernes->taille;j++)
        {
            if((longueurs[j][i]<RAYON_INFLUENCE_CASERNE)&&(longueurs[j][i]))
            {
                collection_habitation->habitation[i]->protegee=PROTEGEE;
            }
        }
    }
}

void collection_casernes_afficher(t_collection_casernes* collection_casernes,int niveau)
{
    int i;
    for(i=0; i<collection_casernes->taille; i++)
    {
        caserne_afficher(collection_casernes->caserne[i],niveau);
    }
}
