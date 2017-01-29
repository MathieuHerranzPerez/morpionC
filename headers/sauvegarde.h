#include "morpion.h"

#ifndef MORPIONC_SAUVEGARDE_H
#define MORPIONC_SAUVEGARDE_H

/**
 * SAuvegarde l'etat du morpion dans le fichier sauvegarde.txt
 * @param morpion
 * @param mode
 * @param difficulte
 * @param numIA
 */
void sauvegarder(tpm morpion, int mode/*1 ou 2*/, int difficulte, int numIA);

/**
 * Restaure le morpion ainsi que le contexte
 * @param mode
 * @param difficulte
 * @param numIA
 * @param joueur
 * @return le morpion
 */
tpm restaurerSauvegarde(int * mode, int * difficulte, int * numIA, int * joueur);

/**
 * Verifie si le fichier sauvegarde.txt est vide
 * @return vrai ou faux
 */
int estFichierSauvegardeVide();

/**
 * Supprime le fichier sauvegarde.txt
 */
void supprimerFichierSauvegarde();

#endif //MORPIONC_SAUVEGARDE_H
