/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.2.0 / 20-12-2016
 * @file jeu.c
 */
#include "headers/jeu.h"

void afficherMorpion(tpm morpion)
{
    int i, j;
    printf("   ");
    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        printf("%d ", i % 10);
    }
    printf("\n");

    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        if(i < 10)
            printf(" ");
        printf("%d|", i);
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[i][j] == 'X')
            {
                printf("\033[31m");
                printf("X");
                printf("\033[30m");
                printf("|");
            }
            else if(morpion->morpion[i][j] == 'O')
            {
                printf("\033[32m");
                printf("O");
                printf("\033[30m");
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

void afficherRegles()
{
    printf("===== Regles du jeu =====\n\n"
                   "Chacun leurs tours les joueurs devront placer leurs symboles dans la grille du morpion.\n "
                   "Le premier symbole doit etre place au milieu de la grille. Les suivants\n "
                   "devront etre places a cote des symboles deja places. Le jeu prend fin si la grille\n "
                   "est remplie ou si un des deux joueurs reussit a aligner cinq de ses symboles,\n "
                   "en diagonale, verticale ou en horizontale.\n\n========================\n\n");
}
