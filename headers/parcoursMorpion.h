#ifndef MORPIONC_PARCOURSMORPION_H_H
#define MORPIONC_PARCOURSMORPION_H_H

#include "morpion.h"

/**
 * regarde à partir d'une case données par i et j si la ligne est bloquée (pas 5 cases libres)
 * @param i
 * @param j
 * @param morpion
 * @param placeDispo
 * @return
 */
int estBloqueH(int i, int j, tpm morpion, int placeDispo);
/**
 * regarde à partir d'une case données par i et j si la colonne est bloquée (pas 5 cases libres)
 * @param i
 * @param j
 * @param morpion
 * @param placeDispo
 * @return
 */
int estBloqueV(int i, int j, tpm morpion, int placeDispo);
/**
 * regarde à partir d'une case données par i et j si la diag asc est bloquée (pas 5 cases libres)
 * @param i
 * @param j
 * @param morpion
 * @param placeDispo
 * @return
 */
int estBloqueDA(int i, int j, tpm morpion, int placeDispo);
/**
 * regarde à partir d'une case données par i et j si la diag des est bloquée (pas 5 cases libres)
 * @param i
 * @param j
 * @param morpion
 * @param placeDispo
 * @return
 */
int estBloqueDD(int i, int j, tpm morpion, int placeDispo);

/**
 * recherche les series de nbMaxOcc symboles (O ou X) successifs dans tous les sens
 * @param nbMaxOcc nombre d'occurence successives a chercher
 * @param series0 premier symbole
 * @param series1 deuxieme symbole
 * @param morpion le morpion
 */
void rechercherSeriesSucc(int nbMaxOcc, int *series0, int *series1, tpm morpion);

/**
 * Compte le nombre de series alignees de nbAlign symboles en prenant en compte la place disponible (nbNonBloque)
 * @param morpion le morpion
 * @param seriesX nb series de X
 * @param seriesO nb series de O
 * @param nbAlign nombre de symboles alignes a compter
 * @param nbNonBloque place disponible autour de la serie de symboles
 */
void nbSeriesAlign(tpm morpion, int* seriesX, int* seriesO, int nbAlign, int nbNonBloque);

#endif //MORPIONC_PARCOURSMORPION_H_H
