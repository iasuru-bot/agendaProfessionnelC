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



void completer_rdv(rdv *rdv_a_completer) {
    int i;
    unsigned long int annee,mois,jour,heure; 
    printf("Quel est le nom du rendez-vous ?\n");
    scanf("%s",&rdv_a_completer->nom_rdv);
    printf("Quel est le lieu du rendez-vous ?\n");
    scanf("%s",&rdv_a_completer->lieu_rdv);
    printf("Combien de personnes sont présentes lors de ce rendez-vous? (max:20)\n");
    scanf("%d",&rdv_a_completer->nb_personne);
    for ( i = 0; i <rdv_a_completer->nb_personne; i++)
    {
        printf("Qui est présent au rendez-vous ?\n");
        scanf("%s",&rdv_a_completer->personne_rdv[i]);
    }
    printf("Quelle est l annee du rendez-vous ? (AA)\n");
    scanf("%lu",&annee);
    printf("Quel est le mois du rendez-vous ? (MM)\n");
    scanf("%lu",&mois);
    printf("Quel est le jour du rendez-vous ? (JJ)\n");
    scanf("%lu",&jour);
    printf("Quelle est l heure du rendez-vous ? (HHMM)\n");
    scanf("%lu",&heure);
    rdv_a_completer->debut_rdv=(heure+(jour*10000)+(mois*1000000)+(annee*100000000));
    printf("Quelle est la durée du rendez-vous ? __h\n");
    scanf("%d",&rdv_a_completer->duree_rdv);
}



//Procédure permettant d'ajouter un rendez-vous complété dans la liste chainée on rentre en parametre juste le début de la liste nommé listo
void ajout(agenda *listo){
    int nbr_place=0;
    rdv *new_rdv = malloc(sizeof(rdv));
    rdv *test_rdv=listo->pointeur_1;
    rdv *rdv_prec=NULL;
    completer_rdv(new_rdv);
    listo->nb_rdv=listo->nb_rdv+1;
    if (test_rdv==NULL|| listo->nb_rdv==0)
    {
        listo->pointeur_1=new_rdv;
        new_rdv->rdv_suiv=NULL;
    }
    else
    {
        while (new_rdv->debut_rdv>test_rdv->debut_rdv)
        {
            
            nbr_place++;
            rdv_prec=test_rdv;
            test_rdv=test_rdv->rdv_suiv;
            if(test_rdv==NULL)
            {
                rdv_prec->rdv_suiv=new_rdv;
                new_rdv->rdv_suiv=NULL;
                break;
            }
        }
        if (nbr_place==0)
        {
            listo->pointeur_1=new_rdv;
            new_rdv->rdv_suiv=test_rdv;
        }
        else
        {
            
            new_rdv->rdv_suiv=test_rdv;
            rdv_prec->rdv_suiv=new_rdv;
        }
    }
}

void main(){
    FILE *fichier_tete;
    FILE *fichier_rdv;
    FILE *sauvegarde_temporaire;
    int ajout_rdv,vide=0,i,j,nb_rdv_sauvegarde;

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
            //Initialisation car pas de rendez-vous
            
            liste->nb_rdv=0;
            liste->pointeur_1 =NULL;
        }
        //Reconstruction de la liste chainée du fichier
        else
        {
            //Reconstruction de la chaine 
            //Initialisation du début de la chaine 

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
            //"Reconstruction des rendez-vous 
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
        //fermeture des fichiers utilisées pour la reconstruction
        fclose(fichier_rdv);
        fclose(fichier_tete);
        //Ajout de nouveau rendez-vous
        do
        {
            ajout(liste);
            printf("Voulez vous rajouter un autre rendez-vous? oui=1/non=0\n");
            scanf("%d",&ajout_rdv);
            system("cls");
        } while (ajout_rdv==1);
        system("cls");
        printf("Les rendez-vous ont bien été entrés\n");
        //affichage de la liste chainée de rdv avec les nouveaux rendez-vous
        rdv *affiche_rdv=liste->pointeur_1;
        j=1;
        while (affiche_rdv!=NULL)
        {
            printf("\n______________________________________________________________________________________________\n");
            printf("Le rendez-vous %d : \n date: %lu nom: %s durée: %dh lieu: %s personne(s):",j,affiche_rdv->debut_rdv,affiche_rdv->nom_rdv,affiche_rdv->duree_rdv,affiche_rdv->lieu_rdv);
            for ( i = 0; i <affiche_rdv->nb_personne ; i++)
            {
                printf("%s ",affiche_rdv->personne_rdv[i]);
            }       
            affiche_rdv=affiche_rdv->rdv_suiv;
            j++;
        }
        printf("\n______________________________________________________________________________________________\n");

        //Sauvegarde de la chaine dans le fichier de manière triée.
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
        printf("Les rendez-vous ont bien été enregistrés\n");
        
    }
}