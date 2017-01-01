
#include <stdio.h>
#include "headers/joueur.h"

void afficherJoueurActuel(int joueur)
{
    if(joueur)
    {
        printf("C'est au joueur ");
        printf("\033[31m");
        printf("X");
        printf("\033[30m");
        printf(" de jouer.\n");
    }
    else
    {
        printf("C'est au joueur ");
        printf("\033[32m");
        printf("O");
        printf("\033[30m");
        printf(" de jouer.\n");
    }
}

int changerJoueur(int joueur)
{
    if(joueur == 1)
        return 0;
    return 1;
}
