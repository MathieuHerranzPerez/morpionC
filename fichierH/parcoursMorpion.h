#ifndef MORPIONC_PARCOURSMORPION_H_H
#define MORPIONC_PARCOURSMORPION_H_H

#include "morpion.h"

int estBloqueH(int i, int j, tpm morpion, int placeDispo);
int estBloqueV(int i, int j, tpm morpion, int placeDispo);
int estBloqueDA(int i, int j, tpm morpion, int placeDispo);
int estBloqueDD(int i, int j, tpm morpion, int placeDispo);
void rechercherSeriesSucc(int nbMaxOcc, int *series0, int *series1, tpm morpion);
void nbSeriesAlign(tpm morpion, int* seriesX, int* seriesO, int nbAlign, int nbNonBloque);

#endif //MORPIONC_PARCOURSMORPION_H_H
