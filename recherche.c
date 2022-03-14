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
    int ajout_rdv,vide=0,i,j,nb_rdv_sauvegarde,choix,comparaison;
    char nom_recherche[50];
    unsigned long int date_recherche;
    unsigned long int annee,mois,jour,heure,minute; 
    //Récupération des fichiers
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
                    precedent_rdv->rdv_suiv=rdv_a_remplir;
                    precedent_rdv=rdv_a_remplir;
                }
                //si c'est le dernier rendez-vous on doit mettre le rendez-vous suivant sur NULL
                if (i==liste->nb_rdv-1)
                {
                    rdv_a_remplir->rdv_suiv==NULL;
                }
                 
            }
        }
        rdv *affiche_rdv=liste->pointeur_1;
        printf("Voulez vous rechercher votre rendez-vous par nom(1) ou par date(2)? \n");
        scanf("%d",&choix);
        if (choix==1)
        {
            j=1;
            printf("Quel est le nom de votre rendez-vous? \n");
            scanf("%s",&nom_recherche);
            while (affiche_rdv!=NULL)
            {
                //comparaison des noms
                comparaison=strcmp(affiche_rdv->nom_rdv,nom_recherche);

                if (comparaison==0)
                {
                    annee=affiche_rdv->debut_rdv/100000000;
                    mois=(affiche_rdv->debut_rdv-(annee*100000000))/1000000;
                    jour=(affiche_rdv->debut_rdv-(annee*100000000)-(mois*1000000))/10000;
                    heure=(affiche_rdv->debut_rdv-(annee*100000000)-(mois*1000000)-(jour*10000))/100;
                    minute=affiche_rdv->debut_rdv-(annee*100000000)-(mois*1000000)-(jour*10000)-(heure*100);
                    printf("\n______________________________________________________________________________________________\n");
                    printf("Le rendez-vous %d : \n annee: %lu mois: %lu jour: %lu heure:%lu:%lu nom: %s durée: %dh lieu: %s personne(s):",j,annee,mois,jour,heure,minute,affiche_rdv->nom_rdv,affiche_rdv->duree_rdv,affiche_rdv->lieu_rdv);
                    for ( i = 0; i <affiche_rdv->nb_personne ; i++)
                    {
                        printf("%s ",affiche_rdv->personne_rdv[i]);
                    }       
                    printf("\n______________________________________________________________________________________________\n");
                }
                //passage a la prochaine étape
                affiche_rdv=affiche_rdv->rdv_suiv;
                j++;
            }
            
        }
        else if (choix==2)
        {
            j=1;
            printf("Quelle est l annee du rendez-vous ? (AA)\n");
            scanf("%lu",&annee);
            printf("Quel est le mois du rendez-vous ? (MM)\n");
            scanf("%lu",&mois);
            printf("Quel est le jour du rendez-vous ? (JJ)\n");
            scanf("%lu",&jour);
            printf("Quelle est l heure du rendez-vous ? (HHMM)\n");
            scanf("%lu",&heure);
            date_recherche=(heure+(jour*10000)+(mois*1000000)+(annee*100000000));
            while (affiche_rdv!=NULL)
            {
                
                if (date_recherche==affiche_rdv->debut_rdv)
                {
                    annee=affiche_rdv->debut_rdv/100000000;
                    mois=(affiche_rdv->debut_rdv-(annee*100000000))/1000000;
                    jour=(affiche_rdv->debut_rdv-(annee*100000000)-(mois*1000000))/10000;
                    heure=(affiche_rdv->debut_rdv-(annee*100000000)-(mois*1000000)-(jour*10000))/100;
                    minute=affiche_rdv->debut_rdv-(annee*100000000)-(mois*1000000)-(jour*10000)-(heure*100);
                    printf("\n______________________________________________________________________________________________\n");
                    printf("Le rendez-vous %d : \n annee: %lu mois: %lu jour: %lu heure:%lu:%lu nom: %s durée: %dh lieu: %s personne(s):",j,annee,mois,jour,heure,minute,affiche_rdv->nom_rdv,affiche_rdv->duree_rdv,affiche_rdv->lieu_rdv);
                    for ( i = 0; i <affiche_rdv->nb_personne ; i++)
                    {
                        printf("%s ",affiche_rdv->personne_rdv[i]);
                    }
                    printf("\n______________________________________________________________________________________________\n");       
                }
                affiche_rdv=affiche_rdv->rdv_suiv;
                j++;
            }
        }
        printf("La recherche et l'affichage de votre rendez vous est terminée\n");
        //fermeture des fichiers utilisées pour la reconstruction
        fclose(fichier_rdv);
        fclose(fichier_tete);
    }
}