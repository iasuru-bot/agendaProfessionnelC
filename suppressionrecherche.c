#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct rdv {
    char nom_rdv[50];
    unsigned long int debut_rdv;
    int duree_rdv;
    int nb_personne;
    char lieu_rdv[50];
    char personne_rdv[20][50];
    struct rdv *rdv_suiv;
}rdv;

typedef struct agenda {
    int nb_rdv;
    struct rdv *pointeur_1;
}agenda;

void main(){

    FILE *fichier_tete;
    FILE *fichier_rdv;
    FILE *sauvegarde_temporaire;
    int ajout_rdv,vide=0,i,j,nb_rdv_sauvegarde,choix,comparaison;char nom_recherche[50];
    unsigned long int date_recherche,annee,mois,jour,heure,minute;
    
    //Récupération du fichier
    fichier_tete=fopen("liste_tete.txt","r");
    fichier_rdv=fopen("liste_rdv.txt","r");
    if (fichier_rdv==NULL || fichier_tete==NULL)
    {
        printf("Impossible de charger la liste de rendez-vous\n");
    }
    else
    {
        //Initilisation de la liste si le fichier est vide
        //si le fichier est vide donc condition à changer
        vide=fgetc(fichier_tete);
        //cree la tete de liste
        agenda *liste=malloc(sizeof(agenda));
        if(vide==EOF)
        {
            printf("Il n'y a pas de rendez vous à supprimer");
        }
        //Reconstruction de la liste chainée du fichier
        else
        {
            //Recupere le nb de rdv
            fscanf(fichier_tete,"%d",&nb_rdv_sauvegarde);
            //stocke dans la tete de liste
            liste->nb_rdv=nb_rdv_sauvegarde;
            //printf("%d", liste->nb_rdv);
            //cree le premier rdv
            rdv *premier_rdv = malloc(sizeof(rdv));
            //je lis la tete de liste avec le rendez-vous
            liste->pointeur_1=premier_rdv;
            //creation de deux pointeurs permettants de remplir chaque rdv et de les lier ensemble
            rdv *rdv_a_remplir =premier_rdv;
            rdv *precedent_rdv = NULL;
            for (i = 0; i < liste->nb_rdv; i++)
            {
                //permet de remplir le 1er rdv en fonction de ce qu'il ya dans le fichier texte sauvegardé avant
                //cas particulier car comme on la fais précédemment: relier la tete de liste avec de rdv
                if(i==0)
                {
                    fscanf(fichier_rdv, "%s %s %d %lu %d", &rdv_a_remplir->nom_rdv, &rdv_a_remplir->lieu_rdv, &rdv_a_remplir->duree_rdv, &rdv_a_remplir->debut_rdv, &rdv_a_remplir->nb_personne);
                    for ( j = 0; j < rdv_a_remplir->nb_personne; j++)
                    {
                        fscanf(fichier_rdv,"%s",&rdv_a_remplir->personne_rdv[j]);
                    }
                    rdv_a_remplir->rdv_suiv=NULL;
                    precedent_rdv=rdv_a_remplir;
                }    
                //cas plus globale.
                else
                {
                    //crée de la place pour le rdv
                    rdv *nouveau_rdv = malloc(sizeof(rdv));
                    rdv_a_remplir=nouveau_rdv;
                    //je le remplis
                    fscanf(fichier_rdv, "%s %s %d %lu %d", &rdv_a_remplir->nom_rdv, &rdv_a_remplir->lieu_rdv, &rdv_a_remplir->duree_rdv, &rdv_a_remplir->debut_rdv, &rdv_a_remplir->nb_personne);
                    for ( j = 0; j < rdv_a_remplir->nb_personne; j++)
                    {
                        fscanf(fichier_rdv,"%s",&rdv_a_remplir->personne_rdv[j]);
                    }
                    //j'établis le lien avec le rdv précédent
                    precedent_rdv->rdv_suiv=rdv_a_remplir;
                    precedent_rdv=rdv_a_remplir;
                }
                //si c'est le dernier rendez-vous on doit mettre le rendez-vous suivant sur NULL
                if (i==liste->nb_rdv-1)
                {
                    rdv_a_remplir->rdv_suiv==NULL;
                }
            }
            fclose(fichier_rdv);
            //Recherche du rendez-vous(le premier portant le caractere recherché)
            rdv *reliaison_rdv=NULL;
            rdv *recherche_rdv=liste->pointeur_1;
            printf("Voulez vous supprimer votre rendez-vous par nom(1) ou par date(2)? \n");
            scanf("%d",&choix);
            if (choix==1)
            {
                printf("Quel est le nom de votre rendez-vous? \n");
                scanf("%s",&nom_recherche);
                //Dans le cas ou c'est le premier rdv de ma liste je dois relier ma tete avec le deuxième 
                comparaison=strcmp(recherche_rdv->nom_rdv,nom_recherche);
                if (comparaison==0)
                {
                    liste->pointeur_1=recherche_rdv->rdv_suiv;
                    liste->nb_rdv--;
                }
                else
                {
                    recherche_rdv=recherche_rdv->rdv_suiv;
                    reliaison_rdv=liste->pointeur_1;
                    while (recherche_rdv!=NULL)
                    {
                        //comparaison des noms
                        comparaison=strcmp(recherche_rdv->nom_rdv,nom_recherche);

                        if (comparaison==0)
                        {
                            /*cas du dernier élément de la liste est compris dans le cas globale car comme le dernier élément pointe sur null 
                            alors lors de la suppression le précdént rdv va pointer sur null donc c'est bon
                            cas globale:
                            Refaire les liens de la chaine (cas différent avec le premier et le dernier)*/
                            reliaison_rdv->rdv_suiv=recherche_rdv->rdv_suiv;
                            liste->nb_rdv--;
                            break;
                            //Supprimer le rendez-vous
                            //free()
                            
                        }
                        //passage a la prochaine étape
                        reliaison_rdv=reliaison_rdv->rdv_suiv;
                        recherche_rdv=recherche_rdv->rdv_suiv;
                    }
                }
                printf("Tous les rendez-vous contenant le nom %s ont été supprimés \n",nom_recherche);
            }
            else if (choix==2)
            {
                
                printf("Quelle est l annee du rendez-vous ? (AA)\n");
                scanf("%lu",&annee);
                printf("Quel est le mois du rendez-vous ? (MM)\n");
                scanf("%lu",&mois);
                printf("Quel est le jour du rendez-vous ? (JJ)\n");
                scanf("%lu",&jour);
                printf("Quelle est l heure du rendez-vous ? (HHMM)\n");
                scanf("%lu",&heure);
                date_recherche=(heure+(jour*10000)+(mois*1000000)+(annee*100000000));
                    //Dans le cas ou c'est le premier rdv de ma liste je dois relier ma tete avec le deuxième 
                if (recherche_rdv->debut_rdv==date_recherche)
                {
                    liste->pointeur_1=recherche_rdv->rdv_suiv;
                    liste->nb_rdv--;
                    
                }
                else
                {
                    recherche_rdv=recherche_rdv->rdv_suiv;
                    reliaison_rdv=liste->pointeur_1;
                    while (recherche_rdv!=NULL)
                    {
                        if (recherche_rdv->debut_rdv==date_recherche)
                        {
                            reliaison_rdv->rdv_suiv=recherche_rdv->rdv_suiv;
                            liste->nb_rdv--;
                            break;
                            //Supprimer le rendez-vous
                            //free()
                        }
                        //passage a la prochaine étape
                        reliaison_rdv=reliaison_rdv->rdv_suiv;
                        recherche_rdv=recherche_rdv->rdv_suiv;
                    }
                   
                }
                 printf("Tous les rendez-vous contenant la date %lu ont été supprimés \n",date_recherche);
            }
            //Sauvegarder la chaine
            
            //Si on a effacé tout nos rendez-vous alors on remet les fichiers a zéro
            if (liste->nb_rdv==0)
            {
                FILE * rdv;
                FILE * tete;
                remove("liste_rdv.txt");
                remove("liste_tete.txt");    
                tete=fopen("liste_tete.txt","w");
                rdv=fopen("liste_rdv.txt","w");
                fclose(rdv);
                fclose(tete);
            }
            //sinon on sauvegarde normalement
            else
            {
                //Supprime l'ancienne sauvegarde
                remove("liste_tete.txt");
                //Ouverture d'un fichier pour écrire dedans
                sauvegarde_temporaire=fopen("nouvelle_liste_rdv.txt","w");
                fichier_tete=fopen("liste_tete.txt","w");  
                //curseur permettant de sauvegardé chaque rdv
                rdv *sauvegarde_rdv=liste->pointeur_1;
                //écriture dans un fichier temporaire
                fprintf(fichier_tete,"O\n");
                fprintf(fichier_tete,"%d",liste->nb_rdv);
                //boucle pour lire chaque données de chaque rdv
                for ( i = 0; i < liste->nb_rdv ; i++)
                {
                    fprintf(sauvegarde_temporaire, "%s %s %d %lu %d \n", sauvegarde_rdv->nom_rdv, sauvegarde_rdv->lieu_rdv, sauvegarde_rdv->duree_rdv, sauvegarde_rdv->debut_rdv, sauvegarde_rdv->nb_personne);
                    for ( j = 0; j < sauvegarde_rdv->nb_personne; j++)
                    {
                        fprintf(sauvegarde_temporaire,"%s \n",sauvegarde_rdv->personne_rdv[j]);
                    }
                    sauvegarde_rdv=sauvegarde_rdv->rdv_suiv;
                    //libérer l'espace mémoire
                    //free(&sauvegarde_rdv);
                }
                //free(liste);
                //fermeture des fichiers sauvegardés
                fclose(sauvegarde_temporaire);
                fclose(fichier_tete);
                //supprimer l'ancien
                remove("liste_rdv.txt");
                //renommer le fichier temporaire
                rename("nouvelle_liste_rdv.txt","liste_rdv.txt");
                
            }
        }
    }




    /*FILE *fichier_tete;
    FILE *fichier_rdv;
    FILE *sauvegarde_temporaire;
    int ajout_rdv,vide=0,i,j,nb_rdv_sauvegarde,choix,comparaison;char nom_recherche[50];
     unsigned long int date_recherche;
    //Récupération du fichier
    fichier_tete=fopen("liste_tete.txt","r");
    fichier_rdv=fopen("liste_rdv.txt","r");
    if (fichier_rdv==NULL || fichier_tete==NULL)
    {
        printf("Impossible de charger la liste de rendez-vous\n");
    }
    else
    {
        //Initilisation de la liste si le fichier est vide
        //si le fichier est vide donc condition à changer
        vide=fgetc(fichier_tete);
        //cree la tete de liste
        agenda *liste=malloc(sizeof(agenda));
        if(vide==EOF)
        {
            printf("Il n'y a pas de rendez vous à supprimer");
        }
        //Reconstruction de la liste chainée du fichier
        else
        {
            printf("Reconstruction de la chaine \n");
            printf("Initialisation du début de la chaine \n");
            //Recupere le nb de rdv
            fscanf(fichier_tete,"%d",&nb_rdv_sauvegarde);
            //stocke dans la tete de liste
            liste->nb_rdv=nb_rdv_sauvegarde;
            printf("%d", liste->nb_rdv);
            //cree le premier rdv
            rdv *premier_rdv = malloc(sizeof(rdv));
            //je lis la tete de liste avec le rendez-vous
            liste->pointeur_1=premier_rdv;
            //creation de deux pointeurs permettants de remplir chaque rdv et de les lier ensemble
            rdv *rdv_a_remplir =premier_rdv;
            rdv *precedent_rdv = NULL;
            printf("Reconstruction des rendez-vous \n");
            for (i = 0; i < liste->nb_rdv; i++)
            {
                //permet de remplir le 1er rdv en fonction de ce qu'il ya dans le fichier texte sauvegardé avant
                //cas particulier car comme on la fais précédemment: relier la tete de liste avec de rdv
                if(i==0)
                {
                    fscanf(fichier_rdv, "%s %s %d %lu %d", &rdv_a_remplir->nom_rdv, &rdv_a_remplir->lieu_rdv, &rdv_a_remplir->duree_rdv, &rdv_a_remplir->debut_rdv, &rdv_a_remplir->nb_personne);
                    for ( j = 0; j < rdv_a_remplir->nb_personne; j++)
                    {
                        fscanf(fichier_rdv,"%s",&rdv_a_remplir->personne_rdv[j]);
                    }
                    precedent_rdv=rdv_a_remplir;
                }    
                //cas plus globale.
                else
                {
                    //crée de la place pour le rdv
                    rdv *nouveau_rdv = malloc(sizeof(rdv));
                    rdv_a_remplir=nouveau_rdv;
                    //je le remplis
                    fscanf(fichier_rdv, "%s %s %d %lu %d", &rdv_a_remplir->nom_rdv, &rdv_a_remplir->lieu_rdv, &rdv_a_remplir->duree_rdv, &rdv_a_remplir->debut_rdv, &rdv_a_remplir->nb_personne);
                    for ( j = 0; j < rdv_a_remplir->nb_personne; j++)
                    {
                        fscanf(fichier_rdv,"%s",&rdv_a_remplir->personne_rdv[j]);
                    }
                    //j'établis le lien avec le rdv précédent
                    precedent_rdv->rdv_suiv=rdv_a_remplir;
                    precedent_rdv=rdv_a_remplir;
                }
                //si c'est le dernier rendez-vous on doit mettre le rendez-vous suivant sur NULL
                if (i==liste->nb_rdv-1)
                {
                    rdv_a_remplir->rdv_suiv==NULL;
                }
            }
            fclose(fichier_rdv);
            //Recherche du rendez-vous(le premier portant le caractere recherché)
            rdv *reliaison_rdv=NULL;
            rdv *recherche_rdv=liste->pointeur_1;
            printf("Voulez vous supprimer votre rendez-vous par nom(1) ou par date(2)? \n");
            scanf("%d",&choix);
            if (choix==1)
            {
                printf("Quel est le nom de votre rendez-vous? \n");
                scanf("%s",&nom_recherche);
                //Dans le cas ou c'est le premier rdv de ma liste je dois relier ma tete avec le deuxième 
                comparaison=strcmp(recherche_rdv->nom_rdv,nom_recherche);
                if (comparaison==0)
                {
                    liste->pointeur_1=recherche_rdv->rdv_suiv;
                    liste->nb_rdv--;
                }
                else
                {
                    recherche_rdv=recherche_rdv->rdv_suiv;
                    reliaison_rdv=liste->pointeur_1;
                    while (recherche_rdv!=NULL)
                    {
                        //comparaison des noms
                        comparaison=strcmp(recherche_rdv->nom_rdv,nom_recherche);

                        if (comparaison==0)
                        {
                            /*cas du dernier élément de la liste est compris dans le cas globale car comme le dernier élément pointe sur null 
                            alors lors de la suppression le précdént rdv va pointer sur null donc c'est bon
                            cas globale:
                            Refaire les liens de la chaine (cas différent avec le premier et le dernier)*/
                            /*reliaison_rdv->rdv_suiv=recherche_rdv->rdv_suiv;
                            liste->nb_rdv--;
                            //Supprimer le rendez-vous
                            //free()
                            
                        }
                        //passage a la prochaine étape
                        reliaison_rdv=reliaison_rdv->rdv_suiv;
                        recherche_rdv=recherche_rdv->rdv_suiv;
                    }
                }
                printf("Tous les rendez-vous contenant le nom %s ont été supprimés \n",nom_recherche);
            }
            else if (choix==2)
            {
                
                printf("Quelle est la date de votre rendez-vous? \n");
                scanf("%lu",&date_recherche);
                //Dans le cas ou c'est le premier rdv de ma liste je dois relier ma tete avec le deuxième 
                if (recherche_rdv->debut_rdv==date_recherche)
                {
                    liste->pointeur_1=recherche_rdv->rdv_suiv;
                    liste->nb_rdv--;
                    
                }
                else
                {
                    recherche_rdv=recherche_rdv->rdv_suiv;
                    reliaison_rdv=liste->pointeur_1;
                    while (recherche_rdv!=NULL)
                    {
                        if (recherche_rdv->debut_rdv==date_recherche)
                        {
                            reliaison_rdv->rdv_suiv=recherche_rdv->rdv_suiv;
                            liste->nb_rdv--;
                           
                            //Supprimer le rendez-vous
                            //free()
                        }
                        //passage a la prochaine étape
                        reliaison_rdv=reliaison_rdv->rdv_suiv;
                        recherche_rdv=recherche_rdv->rdv_suiv;
                    }
                   
                }
                 printf("Tous les rendez-vous contenant la date %lu ont été supprimés \n",date_recherche);
            }
            //Sauvegarder la chaine
            
            //Si on a effacé tout nos rendez-vous alors on remet les fichiers a zéro
            if (liste->nb_rdv==0)
            {
                FILE * rdv;
                FILE * tete;
                remove("liste_rdv.txt");
                remove("liste_tete.txt");    
                tete=fopen("liste_tete.txt","w");
                rdv=fopen("liste_rdv.txt","w");
                fclose(rdv);
                fclose(tete);
            }
            //sinon on sauvegarde normalement
            else
            {
                //Supprime l'ancienne sauvegarde
                remove("liste_tete.txt");
                //Ouverture d'un fichier pour écrire dedans
                sauvegarde_temporaire=fopen("nouvelle_liste_rdv.txt","w");
                fichier_tete=fopen("liste_tete.txt","w");  
                //curseur permettant de sauvegardé chaque rdv
                rdv *sauvegarde_rdv=liste->pointeur_1;
                //écriture dans un fichier temporaire
                fprintf(fichier_tete,"O\n");
                fprintf(fichier_tete,"%d",liste->nb_rdv);
                //boucle pour lire chaque données de chaque rdv
                for ( i = 0; i < liste->nb_rdv ; i++)
                {
                    fprintf(sauvegarde_temporaire, "%s %s %d %lu %d \n", sauvegarde_rdv->nom_rdv, sauvegarde_rdv->lieu_rdv, sauvegarde_rdv->duree_rdv, sauvegarde_rdv->debut_rdv, sauvegarde_rdv->nb_personne);
                    for ( j = 0; j < sauvegarde_rdv->nb_personne; j++)
                    {
                        fprintf(sauvegarde_temporaire,"%s \n",sauvegarde_rdv->personne_rdv[j]);
                    }
                    sauvegarde_rdv=sauvegarde_rdv->rdv_suiv;
                    //libérer l'espace mémoire
                    //free(&sauvegarde_rdv);
                }
                //free(liste);
                //fermeture des fichiers sauvegardés
                fclose(sauvegarde_temporaire);
                fclose(fichier_tete);
                //supprimer l'ancien
                remove("liste_rdv.txt");
                //renommer le fichier temporaire
                rename("nouvelle_liste_rdv.txt","liste_rdv.txt");
                
            }
            
           
            
        }
    }*/
}