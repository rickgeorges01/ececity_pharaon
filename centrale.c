#include "centrale.h"

t_centrale* centrale_creer()
{
    t_centrale* nouv;
    nouv=malloc(sizeof(t_centrale));

    FILE * pf = fopen("Info_CT.txt","r");
    if(!pf)
    {
        printf("erreur fichier, sortie");
        exit(0);
    }

    ///Mettre infortmation dans un fichier texte

    fscanf(pf, "%d", &nouv->capacite.capacite_disponible);

    ///Au final plus simple d'utili une variable globale
    //nouv->capacite.capacite_disponible = CAPACITE_CENTRALE;
    //nouv->capacite.capacite_max = CAPACITE_CENTRALE;
    nouv->capacite.capacite_max = nouv->capacite.capacite_disponible ;

    fscanf(pf, "%d", &nouv->heigth);
    fscanf(pf, "%d", &nouv->width);

    nouv->Pos_X = 0;
    nouv->Pos_Y = 0;

    nouv->id_centrale.caseX = -1;
    nouv->id_centrale.caseX = -1;

    free(pf);

    return nouv;
}

void centrale_liberer(t_centrale* c)
{
    free(c);
}

void centrale_distribuer(t_centrale* centrale,t_habitation* habitation,int quantitee)
/*
Fonction qui sert à distribuer de l'electricité à une habitation
*/
{
    habitation->id_centrale_fournisseuse.caseX = centrale->id_centrale.caseX; // on recopie l'ID de la centrale
    habitation->id_centrale_fournisseuse.caseY = centrale->id_centrale.caseY; // on recopie l'ID de la centrale
    centrale->capacite.capacite_disponible-=quantitee;
    habitation->electricite=1;
}

int centrale_place_libre(int col,int lig,t_case*** kase)
{
    int i,j;
    int libre=1;
    // On regarde si on sort de la grille
    if(!centrale_depassement_matrice(col,lig)) // Si condition vrai (libre) on regarde, en parcourant case par case, si il n'y a pas deja un objet construit 
    {
        for(i=lig;i<lig+CENTRALE_H;i++)
        {
            for(j=col;j<col+CENTRALE_W;j++)
            {
                if(kase[i][j]->type!=VIDE)
                {
                    libre=0;
                }
            }
        }
    }
    else libre=0; //Sinon pas libre
    return libre;
}

int centrale_depassement_matrice(int colonne,int ligne)
/*
Fonction qui vérifie si on dépasse la grille(sord du bord)
*/
{
    int depasse=1;
    if((colonne>=0)&&(colonne<=NB_CASES_COL-CENTRALE_W)&&(ligne>=0)&&(ligne<=NB_CASES_LIG-CENTRALE_H))
    {
        depasse=0;
    }
    return depasse;
}

void centrale_placer(t_centrale* centrale,int col,int lig,t_case*** kase)
/*
Fonction qui place une centrale(si toutes les conditions sont remplis)
*/

{
    int i,j;
    centrale->Pos_X=col;
    centrale->Pos_Y=lig;
    centrale->id_centrale.caseX = col;
    centrale->id_centrale.caseY = lig;
    for(i=lig;i<lig+CENTRALE_H;i++)
    {
        for(j=col;j<col+CENTRALE_W;j++)
        {
            case_remplir(kase[i][j],CENTRALE,centrale); //on met toutes les cases de type centrale
        }
    }
}

void centrale_afficher(t_centrale* centrale,int niveau)
//Affichage
{
    if(niveau==NIVEAU_EAU)
    {
        draw_trans_sprite(graphs->buffer_ville,graphs->centrale,1+TAILLE_CASE*centrale->Pos_X,1+TAILLE_CASE*centrale->Pos_Y);
    }
    else
    {
        draw_sprite(graphs->buffer_ville,graphs->centrale,1+TAILLE_CASE*centrale->Pos_X,1+TAILLE_CASE*centrale->Pos_Y);
    }
}

void centrale_debut_tour(t_centrale* centrale)
{
    centrale->capacite.capacite_disponible = CAPACITE_CENTRALE;
}

int centrale_elec_disponible(t_centrale* centrale)
{
    return centrale->capacite.capacite_disponible;
}
