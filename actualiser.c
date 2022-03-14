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
    int ajout_rdv,vide=0,i,j,nb_rdv_sauvegarde;
    unsigned long int annee,mois,jour,heure,minute,date_aujourdhui;
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
            printf("Il n'y a pas de rendez vous à afficher");
        }
        //Reconstruction de la liste chainée du fichier
        else
        {
            //Recupere le nb de rdv
            fscanf(fichier_tete,"%d",&nb_rdv_sauvegarde);
            //stocke dans la tete de liste
            liste->nb_rdv=nb_rdv_sauvegarde;
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
                    rdv_a_remplir->rdv_suiv==NULL;
                    precedent_rdv->rdv_suiv=rdv_a_remplir;
                    precedent_rdv=rdv_a_remplir;
                }
                
                    
                
                 
            }
            //fermeture des fichiers utilisées pour la reconstruction
            fclose(fichier_rdv);
            fclose(fichier_tete);
            printf("Attention ce programme va supprimer tous les rendez-vous passées en fonction de la date que vous rentrez\n Si vous souhaitez annuler, entrez 0 a chaque fois\n");
            printf("En quelle annee sommes-nous? (AA)\n");
            scanf("%lu",&annee);
            printf("Quel est le mois d aujourd hui ? (MM)\n");
            scanf("%lu",&mois);
            printf("Quel jour sommes-nous aujourd hui ? (JJ)\n");
            scanf("%lu",&jour);
            printf("Quelle heure est-il ? (HHMM)\n");
            scanf("%lu",&heure);
            date_aujourdhui=(heure+(jour*10000)+(mois*1000000)+(annee*100000000));
            rdv *test_rdv=liste->pointeur_1;
            i=0;
            while (test_rdv->debut_rdv<date_aujourdhui)
            {
                if (test_rdv->rdv_suiv==NULL)
                {
                   
                    liste->pointeur_1=NULL;
                    liste->nb_rdv=0;
                    break;
                }
                test_rdv=test_rdv->rdv_suiv;
                i++;
            }
            if(test_rdv!=NULL)
            {
                liste->pointeur_1=test_rdv;
                liste->nb_rdv=liste->nb_rdv-i;
            }
            printf("allo\n");
            //Si on a effacé tout nos rendez-vous alors on remet les fichiers a zéro
            if (liste->nb_rdv<=0)
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
}
        