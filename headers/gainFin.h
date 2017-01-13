#ifndef gainFin_H
#define gainFin_H

#include "morpion.h"

/**
 * Si il y a égalité, la grille du morpion passé en paramettre sera pleine. En fonction de la grille,
 * renvoie si la partie est terminee par egalite
 * @param morpion
 * @return vrai ou faux
 */
int estFin(tpm morpion);

/**
 * Verifie si le coup joué aux coordonnées i j est gagnant
 * @param i ligne
 * @param j colonne
 * @param morpion
 * @return vrai ou faux
 */
int estGain(int i, int j, tpm morpion);

#endif
