#ifndef ia_H_
#define ia_H_

#include "liste.h"
#include "morpion.h"

/**
 * Permet à l'IA de jouer en fonction de l'état du morpion
 * @param morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 */
void IA_jouer(tpm morpion, int joueur);

#endif
