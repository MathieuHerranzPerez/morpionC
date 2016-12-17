
#include "fichierH/jeu.h"

void afficherMorpion(tpm morpion)
{
    int i, j;
    //clear le terminal
    //printf("\033[H\033[J  ");
    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        printf("%d ", i);
    }
    printf("\n");

    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        printf("%d|", i);
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[i][j] == 'X')
            {
              //printf("\033[31m");
              printf("X");
             // printf("\033[30m");
              printf("|");
            }
            else if(morpion->morpion[i][j] == 'O')
            {
             // printf("\033[32m");
              printf("O");
              //printf("\033[30m");
              printf("|");
            }
            else
              printf("%c|", morpion->morpion[i][j]);
        }
        printf("\n");
    }
}

void jouerJoueur(int i, int j, int joueur, tpm morpion)
{
    if(joueur == 1)
        morpion->morpion[i][j] = 'X';
    else
        morpion->morpion[i][j] = 'O';
}
