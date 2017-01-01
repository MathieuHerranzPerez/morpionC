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

void rechercherSeriesSucc(int nbMaxOcc, int *series0, int *series1, tpm morpion);
void nbSeriesAlign(tpm morpion, int* seriesX, int* seriesO, int nbAlign, int nbNonBloque);

#endif //MORPIONC_PARCOURSMORPION_H_H
