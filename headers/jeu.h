#ifndef jeu_H_
#define jeu_H_


#include <stdio.h>
#include "morpion.h"
#include "liste.h"

/**
 * affiche la grille du morpion
 * @param morpion
 */
void afficherMorpion(tpm morpion);

/**
 * Place dans la grille le coup joué aux coordonnées i j, selon le joueur
 * @param i ligne
 * @param j colonnes
 * @param joueur (0 ou 1) pour savoir si il faut placer O ou X
 * @param morpion
 */
void jouerJoueur(int i, int j, int joueur, tpm morpion);

/**
 * affiche les regles du morpion
 */
void afficherRegles();

#endif
