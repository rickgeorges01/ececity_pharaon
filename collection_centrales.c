#include "collection_centrales.h"

t_collection_centrale* collection_centrale_creer() // création
{
    t_collection_centrale* nouv;
    nouv=malloc(sizeof(t_collection_centrale));
    nouv->taille=0;  //taille initiale
    nouv->taille_max=TAILLE_INITIALE_COLLECTION;
    nouv->centrale=malloc(nouv->taille_max*sizeof(t_centrale*));  // Liste des centrales
    return nouv;
}

void collection_centrale_ajouter_centrale(t_collection_centrale* collection_centrale,t_centrale* new_centrale)///Ajout d'une centrale
{
    // Réallocation si la taille maximale est ateinte
    if(collection_centrale->taille == collection_centrale->taille_max) // Si la taille maximale est atteinte
    {
        collection_centrale_reallouer(collection_centrale); // Augmenter la taille en faisant une suppression , réallocation et recopie
    }
    collection_centrale->centrale[collection_centrale->taille]=new_centrale; // Aujouter à la derniére place la nouvelle centrale
    collection_centrale->taille++;  //mise à jour de la taille
}

void collection_centrale_liberer(t_collection_centrale* collection_centrale)
{
    int i;

    for(i=0; i<collection_centrale->taille; i++)
    {
        centrale_liberer(collection_centrale->centrale[i]);
    }
    free(collection_centrale->centrale);
    free(collection_centrale);
}

void collection_centrale_reallouer(t_collection_centrale* collection_centrale)
{
    collection_centrale->taille_max=3*collection_centrale->taille_max/2; // augmente la taille d'un facteur 3/2
    collection_centrale->centrale=realloc(collection_centrale->centrale,collection_centrale->taille_max*sizeof(t_centrale*)); //réallocaton
}

void collection_centrale_afficher(t_collection_centrale* collection_centrale,int niveau)
{
    int i;
    for(i=0; i<collection_centrale->taille; i++)
    {
        centrale_afficher(collection_centrale->centrale[i],niveau);
    }
}

void collection_centrale_distribuer(t_collection_centrale* collection_centrale,t_collection_habitation* collection_habitation,int** longueurs)
{
    int i=0,j=0;
    int quantitee;

    for(i=0; i<collection_centrale->taille; i++)  // parcours les centrales
    {
        for(j=0; j<collection_habitation->taille; j++) // Parcours les habitations
        {
            if((collection_habitation->habitation[j]->electricite==0)&&(longueurs[i][j]!=0))
            {
                quantitee=habitation_nbhabitants(collection_habitation->habitation[j]); //retourne le nombre d'habitant en fonction du stade de développement
                if(collection_centrale->centrale[i]->capacite.capacite_disponible>=quantitee)
                {
                    centrale_distribuer(collection_centrale->centrale[i],collection_habitation->habitation[j],quantitee);
                }
            }
        }
    }
}

void collection_centrale_debut_tour(t_collection_centrale* collection_centrale)
/*
Fonction qui permet d'initialiser la liste des centrale au début d'une partie(utile lors du chragement d'une partie sauvegardée)
*/
{
    int i;
    for(i=0; i<collection_centrale->taille; i++)
    {
        centrale_debut_tour(collection_centrale->centrale[i]);
    }
}

int collection_centrale_elec_disponible(t_collection_centrale* collection_centrale)
// retourne le nombre de centrale avec de la capacité disponible
{
    int elec_totale_dispo = 0;
    int i;
    for(i=0;i<collection_centrale->taille;i++)
    {
        elec_totale_dispo += centrale_elec_disponible(collection_centrale->centrale[i]);
    }
    return elec_totale_dispo;
}

int** collection_centrale_tableau_longueurs(t_collection_centrale* collection_centrale,t_case*** kase,t_bfs* bfs,t_collection_habitation* collection_habitation)
// calcul les plus cours chemins entre les centrales et les habitations
{
    int i;
    int** longueurs_centrales_maisons;

    longueurs_centrales_maisons=malloc(collection_centrale->taille*sizeof(int*));

    bfs->ordre=collection_habitation->taille;
    for(i=0; i<collection_centrale->taille; i++)
    {
        bfs->Pos_X=collection_centrale->centrale[i]->Pos_X;
        bfs->Pos_Y=collection_centrale->centrale[i]->Pos_Y;
        longueurs_centrales_maisons[i]=recherchepluscourtchemin(bfs,kase);
    }
    return longueurs_centrales_maisons;
}

int* collection_centrale_tableau_capacite(t_collection_centrale* collection_centrale)
//tableau des capacités des centrales
{
    int* capacite_centrale;
    int i;
    capacite_centrale=malloc(collection_centrale->taille*sizeof(int));
    for(i=0;i<collection_centrale->taille;i++)
    {
        capacite_centrale[i]=collection_centrale->centrale[i]->capacite.capacite_disponible;
    }
    return capacite_centrale;
}
