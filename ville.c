#include "ville.h"

t_ville* ville_allouer_initialiser(int mode_de_jeu)
{
    t_ville* nouv = NULL;
    nouv = (t_ville*)malloc(1*sizeof(t_ville));

    nouv->argent = ARGENT_INITIAL;
    nouv->nb_habitants = 0;
    nouv->qte_eau.capacite_disponible = 0;
    nouv->qte_eau.capacite_max = 0;
    nouv->qte_elec.capacite_disponible = 0;
    nouv->qte_elec.capacite_max = 0;
    nouv->timer = clock();
    nouv->pause = PAUSE_DESACTIVEE;
    nouv->niveau = NIVEAU_SOL;
    nouv->mode = mode_de_jeu;
    nouv->temps_de_jeu = date_allouer();

    nouv->terrain = terrain_allouer();

    nouv->collec_habitations = collection_habitation_creer();
    nouv->collec_centrales = collection_centrale_creer();
    nouv->collec_casernes = collection_casernes_creer();
    nouv->collec_chateaux = collection_chateau_creer();

    nouv->data_bfs.Pos_X =0;
    nouv->data_bfs.Pos_Y =0;
    nouv->data_bfs.ordre =0;

    return nouv;
}

void ville_actualiser_timer(t_ville* v)
{
    clock_t temps_actuel = clock();

    if(      (((double)(temps_actuel- v->timer))/CLOCKS_PER_SEC)    >=    PAS_DU_TIMER   )
    {
        v->timer = temps_actuel;
        if(v->pause == PAUSE_DESACTIVEE)
        {
            date_actualiser(v->temps_de_jeu);
            collection_habitation_actualiser_timer(v->collec_habitations);
        }
    }
}

void ville_afficher_niveau_sol(t_ville* v, int bouton_boite_a_outil)
{
    int l,c;

    int mx,my; // CHANGEMENT DE REFERENTIEL POUR CONNAITRE LES COORDONNEES DE LA SOURIS DANS LA SUB-BITMAP DE JEU
    // (voir les premiers "define" de "define.h" -> "GAME"

    int lig,col; // permet de savoir dans quelle case la souris est actuellement

    mx = mouse_x - GAME_POSX;
    my = mouse_y - GAME_POSY;

    lig = my / TAILLE_CASE;
    col = mx / TAILLE_CASE;

    draw_sprite(graphs->buffer_ville,graphs->fond_herbe,0,0);
    draw_sprite(graphs->buffer_ville,graphs->grille,0,0);

    // Affichage des routes. On est obligés de parcourir la matrice car on ne dispose pas de collection de routes
    // Pour optimiser:
    // - soit on fait une collection de routes
    // - soit on parcourt la matrice pour tout afficher (préférable pour l'éventuelle 3D iso)
    for(l=0; l<NB_CASES_LIG; l++)
    {
        for(c=0; c<NB_CASES_COL; c++)
        {
            if(v->terrain[l][c]->type == ROUTE)
            {
                route_afficher(v->terrain[l][c]->elem,v->niveau);
            }
        }
    }
    collection_habitation_afficher(v->collec_habitations,v->niveau);
    collection_centrale_afficher(v->collec_centrales,v->niveau);
    collection_chateau_afficher(v->collec_chateaux,v->niveau);
    collection_casernes_afficher(v->collec_casernes,v->niveau);

    // AFFICHAGE DE L'IMAGE TEMPORAIRE QUI SUIT LA SOURIS:
    if(bouton_boite_a_outil == BOUTON_AJOUTER_ROUTE)
    {
        if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
        {
            if(route_place_libre(col,lig,v->terrain))
            {
                //draw_lit_sprite(graphs->buffer_ville,graphs->terrain_vague,1+TAILLE_CASE*col,1+TAILLE_CASE*lig,makecol(0,250,0));
                draw_sprite(graphs->buffer_ville,graphs->route[NIVEAU_SOL][TYPE_ROUTE_HORIZONTALE],1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
            else
            {
                draw_trans_sprite(graphs->buffer_ville,graphs->route[NIVEAU_SOL][TYPE_ROUTE_HORIZONTALE],1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
        }
    }
    else if(bouton_boite_a_outil == BOUTON_AJOUTER_HABITATION)
    {
        if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
        {
            if(habitation_place_libre(col,lig,v->terrain))
            {
                draw_sprite(graphs->buffer_ville,graphs->terrain_vague,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
            else
            {
                draw_trans_sprite(graphs->buffer_ville,graphs->terrain_vague,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
        }
    }
    else if(bouton_boite_a_outil == BOUTON_AJOUTER_CHATEAU)
    {
        if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
        {
            if(chateau_place_libre(col,lig,v->terrain))
            {
                draw_sprite(graphs->buffer_ville,graphs->chateau,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
            else
            {
                draw_trans_sprite(graphs->buffer_ville,graphs->chateau,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
        }
    }
    else if(bouton_boite_a_outil == BOUTON_AJOUTER_CENTRALE)
    {
        if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
        {
            if(centrale_place_libre(col,lig,v->terrain))
            {
                draw_sprite(graphs->buffer_ville,graphs->centrale,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
            else
            {
                draw_trans_sprite(graphs->buffer_ville,graphs->centrale,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
        }
    }
    else if(bouton_boite_a_outil == BOUTON_AJOUTER_CASERNE)
    {
        if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
        {
            if(caserne_place_libre(col,lig,v->terrain))
            {
                draw_sprite(graphs->buffer_ville,graphs->caserne,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
            else
            {
                draw_trans_sprite(graphs->buffer_ville,graphs->caserne,1+TAILLE_CASE*col,1+TAILLE_CASE*lig);
            }
        }
    }
    // FIN DE L'AFFICHAGE TEMPORAIRE QUI SUIT LA SOURIS

    blit(graphs->buffer_ville,page, 0,0, GAME_POSX,GAME_POSY, GAME_W,GAME_H);
}

void ville_afficher_niveau_eau(t_ville* v)
{
    int l,c,compteur,eau_distribue=0;
    int mx,my; // CHANGEMENT DE REFERENTIEL POUR CONNAITRE LES COORDONNEES DE LA SOURIS DANS LA SUB-BITMAP DE JEU
    // (voir les premiers "define" de "define.h" -> "GAME"

    int lig,col; // permet de savoir dans quelle case la souris est actuellement
    t_habitation* tmp;
    t_chateau* cha;

    mx = mouse_x - GAME_POSX;
    my = mouse_y - GAME_POSY;

    lig = my / TAILLE_CASE;
    col = mx / TAILLE_CASE;

    draw_sprite(graphs->buffer_ville,graphs->fond_herbe,0,0);
    draw_sprite(graphs->buffer_ville,graphs->grille,0,0);

    for(l=0; l<NB_CASES_LIG; l++)
    {
        for(c=0; c<NB_CASES_COL; c++)
        {
            if(v->terrain[l][c]->type == ROUTE)
            {
                route_afficher(v->terrain[l][c]->elem,v->niveau);
            }
        }
    }

    collection_habitation_afficher(v->collec_habitations,v->niveau);
    collection_centrale_afficher(v->collec_centrales,v->niveau);
    collection_chateau_afficher(v->collec_chateaux,v->niveau);
    collection_casernes_afficher(v->collec_casernes,v->niveau);

    for(compteur=0; compteur<v->collec_chateaux->taille; compteur++)
    {
        cha=v->collec_chateaux->chateau[compteur];
        textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*cha->Pos_X,1+TAILLE_CASE*cha->Pos_Y+(CHATEAU_H*TAILLE_CASE/2),makecol(0,0,0),-1,"Chateau %d-%d :",cha->id_chateau.caseX,cha->id_chateau.caseY);
        textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*cha->Pos_X,1+TAILLE_CASE*cha->Pos_Y+(CHATEAU_H*TAILLE_CASE/2)+TAILLE_POLICE,makecol(0,0,0),-1,"%d/%d",cha->capacite.capacite_disponible,cha->capacite.capacite_max);
    }

    if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
    {
        if((v->terrain[lig][col]->type==HABITATION))
        {
            tmp=((t_habitation*) v->terrain[lig][col]->elem);
            for(compteur=0; compteur<FOURNISSEUR_MAX; compteur++)
            {
                eau_distribue+=tmp->chateaux_fournisseurs[compteur].qte_eau_distribuee;
            }
            textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*tmp->Pos_X,1+TAILLE_CASE*tmp->Pos_Y,makecol(0,100,255),-1,"%d/%d",eau_distribue,habitation_nbhabitants(tmp));
            compteur=0;
            while((tmp->chateaux_fournisseurs[compteur].qte_eau_distribuee != 0) && (compteur<FOURNISSEUR_MAX))
            {
                textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*tmp->Pos_X,1+TAILLE_CASE*tmp->Pos_Y+(compteur+1)*TAILLE_POLICE,makecol(255,255,255),-1,"Chateau %d-%d : %d",tmp->chateaux_fournisseurs[compteur].id_fournisseur.caseX,tmp->chateaux_fournisseurs[compteur].id_fournisseur.caseY,tmp->chateaux_fournisseurs[compteur].qte_eau_distribuee);
                compteur++;
            }
        }
    }

    blit(graphs->buffer_ville,page, 0,0, GAME_POSX,GAME_POSY, GAME_W,GAME_H);
}

void ville_afficher_niveau_elec(t_ville* v)
{
    int ligne,colonne,compteur,elec_distribue=0;
    int mx,my; // CHANGEMENT DE REFERENTIEL POUR CONNAITRE LES COORDONNEES DE LA SOURIS DANS LA SUB-BITMAP DE JEU
    // (voir les premiers "define" de "define.h" -> "GAME"

    int lig,col; // permet de savoir dans quelle case la souris est actuellement
    t_habitation* tmp;
    t_centrale* cent;

    mx = mouse_x - GAME_POSX;
    my = mouse_y - GAME_POSY;

    lig = my / TAILLE_CASE;
    col = mx / TAILLE_CASE;

    draw_sprite(graphs->buffer_ville,graphs->fond_herbe,0,0);
    draw_sprite(graphs->buffer_ville,graphs->grille,0,0);

    for(ligne=0; ligne<NB_CASES_LIG; ligne++)
    {
        for(colonne=0; colonne<NB_CASES_COL; colonne++)
        {
            if(v->terrain[ligne][colonne]->type == ROUTE)
            {
                route_afficher(v->terrain[ligne][colonne]->elem,v->niveau);
            }
        }
    }

    collection_habitation_afficher(v->collec_habitations,v->niveau);
    collection_centrale_afficher(v->collec_centrales,v->niveau);
    collection_chateau_afficher(v->collec_chateaux,v->niveau);
    collection_casernes_afficher(v->collec_casernes,v->niveau);

    for(compteur=0; compteur<v->collec_centrales->taille; compteur++)
    {
        cent=v->collec_centrales->centrale[compteur];
        textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*cent->Pos_X,1+TAILLE_CASE*cent->Pos_Y+(CENTRALE_H*TAILLE_CASE/2),makecol(0,0,0),-1,"Centrale %d-%d :",cent->id_centrale.caseX,cent->id_centrale.caseY);
        textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*cent->Pos_X,1+TAILLE_CASE*cent->Pos_Y+(CENTRALE_H*TAILLE_CASE/2)+TAILLE_POLICE,makecol(0,0,0),-1,"%d/%d",cent->capacite.capacite_disponible,cent->capacite.capacite_max);
    }

    if((mx>0)&&(mx<GAME_W)&&(my>0)&&(my<GAME_H))
    {
        if((v->terrain[lig][col]->type==HABITATION))
        {
            tmp=((t_habitation*) v->terrain[lig][col]->elem);
            if(tmp->electricite)
            {
                elec_distribue=habitation_nbhabitants(tmp);
            }
            else elec_distribue=0;
            textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*tmp->Pos_X,1+TAILLE_CASE*tmp->Pos_Y,makecol(255,255,255),-1,"%d/%d",elec_distribue,habitation_nbhabitants(tmp));
            textprintf_ex(graphs->buffer_ville,font,1+TAILLE_CASE*tmp->Pos_X,1+TAILLE_CASE*tmp->Pos_Y+TAILLE_POLICE,makecol(255,255,255),-1,"Centrale %d-%d : %d",tmp->id_centrale_fournisseuse.caseX,tmp->id_centrale_fournisseuse.caseY,elec_distribue);
        }
    }

    blit(graphs->buffer_ville,page, 0,0, GAME_POSX,GAME_POSY, GAME_W,GAME_H);
}

void ville_sauvegarder(const char* nom_fichier,t_ville* v)
{
    int i,j;
    FILE* fp;

    fp=fopen(nom_fichier,"w");
    if(fp==NULL)
    {
        printf("ERREUR FICHIER DE SAUVEGARDE");
        exit(1);
    }
    ///ON ECRIT LE MODE DE JEU
    fprintf(fp,"%d %d\n",v->mode,v->argent);
    fprintf(fp,"%d %d %lf\n",v->temps_de_jeu->heures,v->temps_de_jeu->minutes,v->temps_de_jeu->secondes);
    ///ON ECRIT LA MATRICE DU TERRAIN (pour pouvoir charger les routes)
    for(i=0; i<NB_CASES_LIG; i++)
    {
        for(j=0; j<NB_CASES_COL; j++)
        {
            fprintf(fp,"%d ",v->terrain[i][j]->type);
        }
        fprintf(fp,"\n");
    }
    ///ON ECRIT LES INFORMATIONS DE LA COLLECTION D'HABITATION
    fprintf(fp,"%d\n",v->collec_habitations->taille);
    for(i=0; i<v->collec_habitations->taille; i++)
    {
        habitation_sauvegarder(v->collec_habitations->habitation[i],fp);
    }
    fprintf(fp,"\n\n");
    ///ON ECRIT LES INFORMATIONS DE LA COLLECTION DE CHATEAUX
    fprintf(fp,"%d\n",v->collec_chateaux->taille);
    for(i=0; i<v->collec_chateaux->taille; i++)
    {
        fprintf(fp,"%d %d\n",v->collec_chateaux->chateau[i]->Pos_X,v->collec_chateaux->chateau[i]->Pos_Y);
    }
    fprintf(fp,"\n\n");
    ///ON ECRIT LES INFORMATIONS DE LA COLLECTION DE CENTRALE
    fprintf(fp,"%d\n",v->collec_centrales->taille);
    for(i=0; i<v->collec_centrales->taille; i++)
    {
        fprintf(fp,"%d %d\n",v->collec_centrales->centrale[i]->Pos_X,v->collec_centrales->centrale[i]->Pos_Y);
    }
    fprintf(fp,"\n\n");
    ///ON ECRIT LES INFORMATIONS DE LA COLLECTION DE CASERNE
    fprintf(fp,"%d\n",v->collec_casernes->taille);
    for(i=0; i<v->collec_casernes->taille; i++)
    {
        fprintf(fp,"%d %d\n",v->collec_casernes->caserne[i]->pos_X,v->collec_casernes->caserne[i]->pos_Y);
    }
    fclose(fp);
}

// sera appelé comme ceci: ville_charger("test.txt",ed->maville);
void ville_charger(const char* nom_fichier,t_ville* v)
{
    FILE* fp;
    int temp,i,j,x,y;

    fp=fopen(nom_fichier,"r");
    if(fp==NULL)
    {
        printf("ERREUR FICHIER DE SAUVEGARDE");
        exit(1);
    }

    fscanf(fp,"%d",&v->mode);
    fscanf(fp,"%d",&v->argent);
    fscanf(fp,"%d %d %lf",&v->temps_de_jeu->heures,&v->temps_de_jeu->minutes,&v->temps_de_jeu->secondes);
    printf("h:%d,m:%d,s:%lf\n",v->temps_de_jeu->heures,v->temps_de_jeu->minutes,v->temps_de_jeu->secondes);
    ///ON CHARGE LES ROUTES ET ON LES PLACES
    for(i=0; i<NB_CASES_LIG; i++)
    {
        for(j=0; j<NB_CASES_COL; j++)
        {
            fscanf(fp,"%d",&temp);
            if(temp==1)
            {
                route_placer(route_creer(),j,i,v->terrain);
            }
        }
    }

    ///ON CHARGE LA COLLECTION D'HABITATION
    fscanf(fp,"%d",&temp);
    for(i=0; i<temp; i++)
    {
        collection_habitation_ajouter_habitation(v->collec_habitations,habitation_creer());
        habitation_charger(v->collec_habitations->habitation[i],fp);
        habitation_placer(v->collec_habitations->habitation[i],v->collec_habitations->habitation[i]->Pos_X,v->collec_habitations->habitation[i]->Pos_Y,v->terrain);
    }

    ///ON CHARGE LA COLLECTION DE CHATEAUX
    fscanf(fp,"%d",&temp);
    for(i=0; i<temp; i++)
    {
        fscanf(fp,"%d %d",&x,&y);
        collection_chateau_ajouter_chateau(v->collec_chateaux,chateau_creer());
        v->collec_chateaux->chateau[i]->Pos_X=x;
        v->collec_chateaux->chateau[i]->Pos_Y=y;
        chateau_placer(v->collec_chateaux->chateau[i],x,y,v->terrain);
    }

    ///ON CHARGE LA COLLECTION DE CENTRALES
    fscanf(fp,"%d",&temp);
    for(i=0; i<temp; i++)
    {
        fscanf(fp,"%d %d",&x,&y);
        collection_centrale_ajouter_centrale(v->collec_centrales,centrale_creer());
        v->collec_centrales->centrale[i]->Pos_X=x;
        v->collec_centrales->centrale[i]->Pos_Y=y;
        centrale_placer(v->collec_centrales->centrale[i],x,y,v->terrain);
    }

    ///ON CHARGE LA COLLECTION DE CASERNES
    fscanf(fp,"%d",&temp);
    for(i=0; i<temp; i++)
    {
        fscanf(fp,"%d %d",&x,&y);
        collection_casernes_ajouter_caserne(v->collec_casernes,caserne_creer());
        v->collec_casernes->caserne[i]->pos_X=x;
        v->collec_casernes->caserne[i]->pos_Y=y;
        caserne_placer(v->collec_casernes->caserne[i],x,y,v->terrain);
    }
    fclose(fp);
}

void ville_afficher(t_ville* v, int bouton_boite_a_outils)
{
    switch(v->niveau)
    {
    case NIVEAU_SOL:
        ville_afficher_niveau_sol(v,bouton_boite_a_outils);
        break;
    case NIVEAU_EAU:
        ville_afficher_niveau_eau(v);
        break;
    case NIVEAU_ELEC:
        ville_afficher_niveau_elec(v);
        break;
    }
}

void ville_liberer(t_ville* v)
{
    int lig,col;
    for(lig=0; lig<NB_CASES_LIG; lig++)
    {
        for(col=0; col<NB_CASES_COL; col++)
        {
            if(v->terrain[lig][col]->type == ROUTE)
            {
                route_liberer(v->terrain[lig][col]->elem);
            }
        }
    }
    terrain_liberer(v->terrain);
    collection_habitation_liberer(v->collec_habitations);
    collection_casernes_liberer(v->collec_casernes);
    collection_chateau_liberer(v->collec_chateaux);
    collection_centrale_liberer(v->collec_centrales);
    date_liberer(v->temps_de_jeu);

    free(v);
}

void afficher_utilisations(t_ville* v)
{
    int i,j;
    if(key_press[KEY_R])
    {
        printf("\n\n\n");
        for(i=0; i<v->collec_chateaux->taille; i++)
        {
            printf("Chateau (%d;%d): %d/%d\n",v->collec_chateaux->chateau[i]->Pos_X,v->collec_chateaux->chateau[i]->Pos_Y,v->collec_chateaux->chateau[i]->capacite.capacite_disponible,v->collec_chateaux->chateau[i]->capacite.capacite_max);
        }
        for(i=0; i<v->collec_centrales->taille; i++)
        {
            printf("Centrale (%d;%d): %d/%d\n",v->collec_centrales->centrale[i]->Pos_X,v->collec_centrales->centrale[i]->Pos_Y,v->collec_centrales->centrale[i]->capacite.capacite_disponible,v->collec_centrales->centrale[i]->capacite.capacite_max);
        }
        for(i=0; i<v->collec_habitations->taille; i++)
        {
            printf("Habitation (%d;%d): Stade: %d, Timer:%.2f, Eau: %d, Elec: %d\n",v->collec_habitations->habitation[i]->Pos_X,v->collec_habitations->habitation[i]->Pos_Y,v->collec_habitations->habitation[i]->stade, v->collec_habitations->habitation[i]->chrono, v->collec_habitations->habitation[i]->eau,v->collec_habitations->habitation[i]->electricite);
            printf("Centrale fournisseuse: (%d;%d)\n",v->collec_habitations->habitation[i]->id_centrale_fournisseuse.caseX,v->collec_habitations->habitation[i]->id_centrale_fournisseuse.caseY);
            for(j=0; j<FOURNISSEUR_MAX; j++)
            {
                printf("Chateau fournisseur: (%d;%d): fourni %d eau\n",v->collec_habitations->habitation[i]->chateaux_fournisseurs[j].id_fournisseur.caseX,v->collec_habitations->habitation[i]->chateaux_fournisseurs[j].id_fournisseur.caseY,v->collec_habitations->habitation[i]->chateaux_fournisseurs[j].qte_eau_distribuee);
            }
        }
        printf("\n\n\n");
    }
}

void ville_gerer(t_ville* v, int bouton_boite_a_outil)
{
    int** longueurs_casernes;
    int** longueurs_chateaux;
    int** longueurs_centrales;
    int* capacite_centrale;
    int* capacite_chateaux;
    int i;

    if(v->pause == PAUSE_DESACTIVEE)
    {
        // Actions à effectuer à chaque tour de boucle
        ville_actualiser_timer(v);
        collection_habitation_debut_tour(v->collec_habitations,&(v->nb_habitants));
        collection_centrale_debut_tour(v->collec_centrales);
        collection_chateau_debut_tour(v->collec_chateaux);

        ///Calcul des longueurs entre les habitations et les différents batiments
        longueurs_chateaux=collection_chateau_tableau_longueurs(v->collec_chateaux,v->terrain,&(v->data_bfs),v->collec_habitations);
        longueurs_centrales=collection_centrale_tableau_longueurs(v->collec_centrales,v->terrain,&(v->data_bfs),v->collec_habitations);
        longueurs_casernes=collection_casernes_tableau_longueurs(v->collec_casernes,v->terrain,&(v->data_bfs),v->collec_habitations);

        ///distribution de l'eau et de l'électicité et protection des incendies
        collection_centrale_distribuer(v->collec_centrales,v->collec_habitations,longueurs_centrales);
        collection_chateau_distribuer(v->collec_chateaux,v->collec_habitations,longueurs_chateaux);
        collection_casernes_proteger(v->collec_casernes,v->collec_habitations,longueurs_casernes);

        /** On tient à jour la qté d'eau et d'elec disponible **/
        v->qte_eau.capacite_disponible = collection_chateau_eau_disponible(v->collec_chateaux);
        v->qte_eau.capacite_max = CAPACITE_CHATEAU * v->collec_chateaux->taille;

        v->qte_elec.capacite_disponible = collection_centrale_elec_disponible(v->collec_centrales);
        v->qte_elec.capacite_max = CAPACITE_CENTRALE * v->collec_centrales->taille;

        capacite_chateaux=collection_chateau_tableau_capacite(v->collec_chateaux);
        capacite_centrale=collection_centrale_tableau_capacite(v->collec_centrales);

        collection_habitation_evolution(v->collec_habitations,v->mode,&(v->argent),v->collec_chateaux->taille,v->collec_centrales->taille,longueurs_chateaux,longueurs_centrales,capacite_chateaux,capacite_centrale);

        ///LIBERATION MEMOIRE DES TABLEAUX DYNAMIQUES ALLOUES
        for(i=0; i<v->collec_centrales->taille; i++)
        {
            free(longueurs_centrales[i]);
        }
        free(longueurs_centrales);
        free(capacite_centrale);

        for(i=0; i<v->collec_chateaux->taille; i++)
        {
            free(longueurs_chateaux[i]);
        }
        free(longueurs_chateaux);
        free(capacite_chateaux);

        for(i=0; i<v->collec_casernes->taille; i++)
        {
            free(longueurs_casernes[i]);
        }
        free(longueurs_casernes);

    }


    // Actions utilisateur
    ville_gerer_actions_utilisateur(v,bouton_boite_a_outil);
}

void ville_gerer_actions_utilisateur(t_ville* v, int bouton_boite_a_outil)
{
    int mx,my; // CHANGEMENT DE REFERENTIEL POUR CONNAITRE LES COORDONNEES DE LA SOURIS DANS LA SUB-BITMAP DE JEU
    // (voir les premiers "define" de "define.h" -> "GAME"

    int lig,col; // permet de savoir dans quelle case la souris est actuellement

    char chemin[TAILLE_CHAINE];

    t_habitation* maison;
    t_chateau* chateau;
    t_centrale* centrale;
    t_caserne* pompier;

    mx = mouse_x - GAME_POSX;
    my = mouse_y - GAME_POSY;

    lig = my / TAILLE_CASE;
    col = mx / TAILLE_CASE;

    if((bouton_boite_a_outil == BOUTON_AJOUTER_ROUTE) && (v->pause != PAUSE_ACTIVEE) && (v->niveau == NIVEAU_SOL))
    {
        if(mouse_b&1 && v->argent >= COUT_ROUTE)
        {
            if(route_place_libre(col,lig,v->terrain))
            {
                route_placer(route_creer(),col,lig,v->terrain);
                v->argent-= COUT_ROUTE;
            }
        }
    }
    else if((bouton_boite_a_outil == BOUTON_AJOUTER_HABITATION) && (v->pause != PAUSE_ACTIVEE) && (v->niveau == NIVEAU_SOL))
    {
        if(bouton == 'g' && v->argent >= COUT_HABITATION)
        {
            if(habitation_routeadj(v->terrain,col,lig))/*&& habitation_place_libre(col,lig,v->terrain)*/
            {
            maison=habitation_creer();
            habitation_placer(maison,col,lig,v->terrain);
            collection_habitation_ajouter_habitation(v->collec_habitations,maison);
            v->argent -= COUT_HABITATION;
            }
        }
    }
    else if((bouton_boite_a_outil == BOUTON_AJOUTER_CHATEAU) && (v->pause != PAUSE_ACTIVEE) && (v->niveau == NIVEAU_SOL))
    {
        if(bouton == 'g' && v->argent >= COUT_CHATEAU)
        {
            if(chateau_place_libre(col,lig,v->terrain))
            {
                chateau=chateau_creer();
                chateau_placer(chateau,col,lig,v->terrain);
                collection_chateau_ajouter_chateau(v->collec_chateaux,chateau);
                v->argent -= COUT_CHATEAU;
            }
        }
    }
    else if((bouton_boite_a_outil == BOUTON_AJOUTER_CENTRALE) && (v->pause != PAUSE_ACTIVEE) && (v->niveau == NIVEAU_SOL))
    {
        if(bouton == 'g' && v->argent >= COUT_CENTRALE)
        {
            if(centrale_place_libre(col,lig,v->terrain))
            {
                centrale=centrale_creer();
                centrale_placer(centrale,col,lig,v->terrain);
                collection_centrale_ajouter_centrale(v->collec_centrales,centrale);
                v->argent -= COUT_CENTRALE;
            }
        }
    }
    else if((bouton_boite_a_outil == BOUTON_AJOUTER_CASERNE) && (v->pause != PAUSE_ACTIVEE) && (v->niveau == NIVEAU_SOL))
    {
        if(bouton == 'g' && v->argent >= COUT_CASERNE)
        {
            if(caserne_place_libre(col,lig,v->terrain))
            {
                pompier=caserne_creer();
                caserne_placer(pompier,col,lig,v->terrain);
                collection_casernes_ajouter_caserne(v->collec_casernes,pompier);
                v->argent -= COUT_CENTRALE;
            }
        }
    }
    else if(bouton_boite_a_outil == BOUTON_PAUSE) // || (touche == 'p') ne fonctionne pas toujours, souci compalleg
    {
        v->pause = !(v->pause);
    }
    else if(bouton_boite_a_outil == BOUTON_SAUVEGARDER)
    {
        if(remplit_chemin_sauvegarde(chemin))
            ville_sauvegarder(chemin, v);
    }
    else if(bouton_boite_a_outil == BOUTON_NIVEAU_SOL)
    {
        v->pause = PAUSE_DESACTIVEE;
        v->niveau = NIVEAU_SOL;
    }
    else if(bouton_boite_a_outil == BOUTON_NIVEAU_EAU)
    {
        v->pause = PAUSE_ACTIVEE;
        v->niveau = NIVEAU_EAU;
    }
    else if(bouton_boite_a_outil == BOUTON_NIVEAU_ELEC)
    {
        v->pause = PAUSE_ACTIVEE;
        v->niveau = NIVEAU_ELEC;
    }
}

