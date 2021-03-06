/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.1.0 / 12-12-2016
 * @file joueur.c
 */


#include <stdio.h>
#include "headers/joueur.h"

/**
 * affiche le joueur actuel
 * @param joueur le joueur
 */
void afficherJoueurActuel(int joueur)
{
    if(joueur)
    {
        printf("C'est au joueur ");
        // rouge
        printf("\033[31m");
        printf("X");
        // noir
        printf("\033[30m");
        printf(" de jouer.\n");
    }
    else
    {
        printf("C'est au joueur ");
        // vert
        printf("\033[32m");
        printf("O");
        // noir
        printf("\033[30m");
        printf(" de jouer.\n");
    }
}

/**
 * change de joueur courant
 * @param joueur l'ancien joueur
 * @return nouveau joueur
 */
int changerJoueur(int joueur)
{
    if(joueur == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}