#include "collection_habitations.h"

t_collection_habitation* collection_habitation_creer() // création
{
    t_collection_habitation* nouv;
    nouv=malloc(sizeof(t_collection_habitation));
    nouv->taille=0;
    nouv->taille_max=TAILLE_INITIALE_COLLECTION;
    nouv->habitation=malloc(nouv->taille_max*sizeof(t_habitation*)); // Liste des habitations
    return nouv;
}

void collection_habitation_trier(t_collection_habitation* collection_habitation)
{
    int i;
    qsort(collection_habitation->habitation, collection_habitation->taille, sizeof(t_habitation*),habitation_comparer);
    for(i=0;i<collection_habitation->taille;i++)
    {
        collection_habitation->habitation[i]->indice=i;
    }
}

void collection_habitation_ajouter_habitation(t_collection_habitation* collection_habitation,t_habitation* new_habitation)
{
    // Réallocation si la taille maximale est ateinte
    if(collection_habitation->taille==collection_habitation->taille_max) // Si la taille maximale est atteinte
    {
        collection_habitation_reallouer(collection_habitation); // Augmenter la taille en faisant une suppression , réallocation et recopie
    }
    collection_habitation->habitation[collection_habitation->taille]=new_habitation; // Aujouter à la derniére place la nouvelle habitation
    collection_habitation->taille++; //mise à jour de la taille
}

void collection_habitation_liberer(t_collection_habitation* collection_habitation)
{
    int i;

    for(i=0;i<collection_habitation->taille;i++)
    {
        habitation_liberer(collection_habitation->habitation[i]);
    }
    free(collection_habitation->habitation);
    free(collection_habitation);
}

void collection_habitation_actualiser_timer(t_collection_habitation* collection_habitation)
{
    int i;

    for(i=0;i<collection_habitation->taille;i++)
    {
        habitation_actualiser_timer(collection_habitation->habitation[i]);
    }
}

void collection_habitation_reallouer(t_collection_habitation* collection_habitation)
{
    collection_habitation->taille_max=3*collection_habitation->taille_max/2; // augmente la taille d'un facteur 3/2
    collection_habitation->habitation=realloc(collection_habitation->habitation,collection_habitation->taille_max*sizeof(t_habitation*)); //réallocaton
}

void collection_habitation_afficher(t_collection_habitation* collection_habitation,int niveau)
{
    int i;
    for(i=0;i<collection_habitation->taille;i++)
    {
        habitation_afficher(collection_habitation->habitation[i],niveau);
    }
}

void collection_habitation_evolution(t_collection_habitation* collection_habitation,int mode, int* argent,int nb_chateaux,int nb_centrales,int** longueurs_chateaux,int** longueurs_centrales,int* capacite_chateaux,int* capacite_centrale)
{
    int i;
    if(collection_habitation->taille!=0)
    {
        for(i=0;i<collection_habitation->taille;i++)
        {
            habitation_evoluer(collection_habitation->habitation[i],mode,argent,nb_chateaux,nb_centrales,longueurs_chateaux,longueurs_centrales,capacite_chateaux,capacite_centrale);
        }
    }
}

void collection_habitation_debut_tour(t_collection_habitation* collection_habitation, int* nb_habitants)
{
    int i;
    for(i=0;i<collection_habitation->taille;i++)
    {
        habitation_debut_tour(collection_habitation->habitation[i]);
    }
    collection_habitation_trier(collection_habitation);
    *nb_habitants = collection_habitation_nombre_habitants(collection_habitation);
}

int collection_habitation_nombre_habitants(t_collection_habitation* collection_habitation)
{
    int nb_hab_total = 0;
    int i;

    for(i=0;i<collection_habitation->taille;i++)
    {
        nb_hab_total = nb_hab_total + habitation_nbhabitants(collection_habitation->habitation[i]);
    }

    return nb_hab_total;
}
