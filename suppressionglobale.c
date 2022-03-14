#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void main()
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