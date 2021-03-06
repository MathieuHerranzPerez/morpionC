#ifndef ia_H_
#define ia_H_


#include "liste.h"
#include "morpion.h"
#include "parcoursMorpion.h"

/**
 * Renvoie la profondeur de l'arbre alphaBeta / minMax
 * @return la profondeur
 */
int getProfondeur();

/**
 * Renvoie la fonction eval utilisee
 * @return le numero de la fonction eval
 */
int getFonctionEval1();

/**
 * Initialise fonctionEval1 a fctEval
 * @param fctEval
 */
void setFonctionEval1(int fctEval);

/**
 * Initialise profondeur a prof
 * @param prof
 */
void setProfondeur(int prof);

/**
 * Permet a l'IA de jouer en fonction de l'etat du morpion, suivant le joueur actuel,
 * grace a un minMAx si estAlphaBeta est faux, avec un alphaBeta sinon
 * @param morpion le morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 * @return la liste apres que l'IA ai joue
 */
tpl jouerIA(tpm morpion, int joueur, int estAlphaBeta);

/**
 * Permet à l'utilisateur de selectionner une difficulte d'IA
 */
void choisirDifficulte();

/**
 * Permet à l'utilisateur de selectionner une complexite d'IA
 */
void choisirIAenJvIA();

/**
 * Permet à l'utilisateur de selectionner une complexite d'IA
 */
void choisirIAenIAvIA();

/**
 * Indique une case pas trop mal à jouer en l'affichant
 * @param morpion le morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 * @param estAlphaBeta (0 ou 1) si on veux utiliser un algo AlphaBeta ou minMax
 */
void aiderJoueur(tpm morpion, int joueur, int estAlphaBeta);

#endif
