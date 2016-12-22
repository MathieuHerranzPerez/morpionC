#ifndef ia_H_
#define ia_H_

#include "liste.h"
#include "morpion.h"
#include "parcoursMorpion.h"

/**
 *
 * Permet à l'IA de jouer en fonction de l'état du morpion
 * @param morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 * @return
 */
tpl jouerIA(tpm morpion, int joueur);
void nbSeriesAlign(tpm morpion, int* seriesX, int* seriesO, int nbAlign);
void choisirDifficulte();

#endif
