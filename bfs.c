#include "bfs.h"


int* recherchepluscourtchemin(t_bfs* bfs,t_case*** kase)
{
    t_file* file;
    t_case* temp;
    int marque[NB_CASES_LIG][NB_CASES_COL];
    int ligne,colonne,compteur;
    int* longueurs; //On met un pointeurs pour pouvoir le changer même dans d'autre ssp

    longueurs=malloc(bfs->ordre*sizeof(int));

    for(compteur=0;compteur<bfs->ordre;compteur++)
    {
        longueurs[compteur]=0;
    }

    file=file_creer();
    for(ligne=0;ligne<NB_CASES_LIG;ligne++)
    {
        for(colonne=0;colonne<NB_CASES_COL;colonne++)
        {
            marque[ligne][colonne]=0;
        }
    }

    if(kase[bfs->Pos_Y][bfs->Pos_X]->type==CHATEAU)
    {
        for(ligne=bfs->Pos_Y;ligne<bfs->Pos_Y+CHATEAU_H;ligne++)
        {
            for(colonne=bfs->Pos_X;colonne<bfs->Pos_X+CHATEAU_W;colonne++)
            {
                tester_voisins(kase,ligne,colonne,longueurs,marque,file);

            }
        }
    }

    if(kase[bfs->Pos_Y][bfs->Pos_X]->type==CASERNE)
    {
        for(ligne=bfs->Pos_Y;ligne<bfs->Pos_Y+CASERNE_H;ligne++)
        {
            for(colonne=bfs->Pos_X;colonne<bfs->Pos_X+CASERNE_W;colonne++)
            {
                tester_voisins(kase,ligne,colonne,longueurs,marque,file);

            }
        }
    }

    if(kase[bfs->Pos_Y][bfs->Pos_X]->type==CENTRALE)
    {
        for(ligne=bfs->Pos_Y;ligne<bfs->Pos_Y+CENTRALE_H;ligne++)
        {
            for(colonne=bfs->Pos_X;colonne<bfs->Pos_X+CENTRALE_W;colonne++)
            {
                tester_voisins(kase,ligne,colonne,longueurs,marque,file);

            }
        }
    }

    while(!file_vide(file))
    {
        temp=file_defiler(file);
        ligne=((t_route*)temp->elem)->case_de_referenceY;
        colonne=((t_route*)temp->elem)->case_de_referenceX;
        tester_voisins(kase,ligne,colonne,longueurs,marque,file);
    }
    free(file); // free et non pas file_detruire car il ne faut pas détruire les éléments de la file, qui sont directement
    // des éléments (des cases) de notre terrain !
    return longueurs;//Longueur chemin le plus court
}

void tester_voisins(t_case*** kase,int ligne,int colonne,int* longueurs,int marque[NB_CASES_LIG][NB_CASES_COL],t_file* file)
{
    if(ligne>0)
    {
        if(marque[ligne-1][colonne]==0)
        {
            if(kase[ligne-1][colonne]->type==ROUTE)
            {
                file_enfiler(file,kase[ligne-1][colonne]);
                marque[ligne-1][colonne]=marque[ligne][colonne]+1;
            }
            if(kase[ligne-1][colonne]->type==HABITATION)
            {
                if((longueurs[((t_habitation*) kase[ligne-1][colonne]->elem)->indice]>marque[ligne][colonne])||(longueurs[((t_habitation*) kase[ligne-1][colonne]->elem)->indice]==0))
                {
                    longueurs[((t_habitation*) kase[ligne-1][colonne]->elem)->indice]=marque[ligne][colonne];
                }
            }
        }
    }
    if(ligne<NB_CASES_LIG-1)
    {
        if(marque[ligne+1][colonne]==0)
        {
            if(kase[ligne+1][colonne]->type==ROUTE)
            {
                file_enfiler(file,kase[ligne+1][colonne]);
                marque[ligne+1][colonne]=marque[ligne][colonne]+1;
            }
            if(kase[ligne+1][colonne]->type==HABITATION)
            {
                if((longueurs[((t_habitation*) kase[ligne+1][colonne]->elem)->indice]>marque[ligne][colonne])||(longueurs[((t_habitation*) kase[ligne+1][colonne]->elem)->indice]==0))
                {
                    longueurs[((t_habitation*) kase[ligne+1][colonne]->elem)->indice]=marque[ligne][colonne];
                }
            }
        }
    }
    if(colonne<NB_CASES_COL-1)
    {
        if(marque[ligne][colonne+1]==0)
        {
            if(kase[ligne][colonne+1]->type==ROUTE)
            {
                file_enfiler(file,kase[ligne][colonne+1]);
                marque[ligne][colonne+1]=marque[ligne][colonne]+1;
            }
            if(kase[ligne][colonne+1]->type==HABITATION)
            {
                if((longueurs[((t_habitation*) kase[ligne][colonne+1]->elem)->indice]>marque[ligne][colonne])||(longueurs[((t_habitation*) kase[ligne][colonne+1]->elem)->indice]==0))
                {
                    longueurs[((t_habitation*) kase[ligne][colonne+1]->elem)->indice]=marque[ligne][colonne];
                }
            }
        }
    }
    if(colonne>0)
    {
        if(marque[ligne][colonne-1]==0)
        {
            if(kase[ligne][colonne-1]->type==ROUTE)
            {
                file_enfiler(file,kase[ligne][colonne-1]);
                marque[ligne][colonne-1]=marque[ligne][colonne]+1;
            }
            if(kase[ligne][colonne-1]->type==HABITATION)
            {
                if((longueurs[((t_habitation*) kase[ligne][colonne-1]->elem)->indice]>marque[ligne][colonne])||(longueurs[((t_habitation*) kase[ligne][colonne-1]->elem)->indice]==0))
                {
                    longueurs[((t_habitation*) kase[ligne][colonne-1]->elem)->indice]=marque[ligne][colonne];
                }
            }
        }
    }
}
