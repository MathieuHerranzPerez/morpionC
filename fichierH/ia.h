#ifndef ia_H_
#define ia_H_

#include "liste.h"
#include "morpion.h"
#include "parcoursMorpion.h"

/**
 * Permet à l'IA de jouer en fonction de l'état du morpion
 * @param morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 * @return la liste après que l'IA ai joué
 */
tpl jouerIA(tpm morpion, int joueur);

/**
 * Permet à l'utilisateur de selectionner une difficulté d'IA
 */
void choisirDifficulte();

/**
 * Permet à l'utilisateur de selectionner une complexité d'IA
 */
void choisirIAenJvIA();

/**
 * Permet à l'utilisateur de selectionner une complexité d'IA
 */
void choisirIAenIAvIA();

#endif
