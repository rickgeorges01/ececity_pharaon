#include "caserne.h"

t_caserne* caserne_creer()
{
    t_caserne* nouv;
    nouv=malloc(sizeof(t_caserne));

    ///Mettre les informations du fichier texte
    nouv->temps_de_rechargement=0;
    nouv->Travaille=0;
    nouv->pos_X = 0;
    nouv->pos_Y = 0;

    return nouv;
}

void caserne_liberer(t_caserne* c)
{
    free(c);
}

int caserne_place_libre(int x,int y,t_case*** kase)
/*
Fonction qui retourne s'il est possible de créer une caserne à un emplacement donné(retourne 1 si libre et 0 pas possible)
*/
{
    int i,j;
    int libre=1;
    // On regarde si on sort de la grille
    if(!caserne_depassement_matrice(x,y))// Si condition vrai (libre) on verifie, en parcourant case par case, si il n'y a deja pas un objet construit.
    {
        for(i=x;i<x+CASERNE_W;i++)
        {
            for(j=y;j<y+CASERNE_H;j++)
            {
                if(kase[j][i]->type!=VIDE)
                {
                    libre=0; // Pas libre
                }
            }
        }
    }
    else libre=0; //Sinon pas libre
    return libre;
}

int caserne_depassement_matrice(int colonne,int ligne)
/*
Fonction qui vérifie si on dépasse la grille(sord du bord)
*/
{
    int depasse=1;
    if((colonne>=0)&&(colonne<NB_CASES_COL-(CASERNE_W-1))&&(ligne>=0)&&(ligne<NB_CASES_LIG-(CASERNE_H-1)))
    {
        depasse=0;
    }
    return depasse;
}

void caserne_placer(t_caserne* caserne,int x,int y,t_case*** kase)
/*
Fonction qui place une case(si toutes les conditions sont remplis)
*/
{
    int i,j;
    caserne->pos_X=x;
    caserne->pos_Y=y;
    for(i=x;i<x+CASERNE_W;i++)
    {
        for(j=y;j<y+CASERNE_H;j++)
        {
            case_remplir(kase[j][i],CASERNE,caserne); // On met toutes les cases de type casernes
        }
    }
}

void caserne_afficher(t_caserne* caserne,int niveau)
//Affichage
{
    if(niveau==NIVEAU_SOL)
    {
        draw_sprite(graphs->buffer_ville,graphs->caserne,1+TAILLE_CASE*caserne->pos_X,1+TAILLE_CASE*caserne->pos_Y);
    }
    draw_trans_sprite(graphs->buffer_ville,graphs->caserne,1+TAILLE_CASE*caserne->pos_X,1+TAILLE_CASE*caserne->pos_Y);
}

