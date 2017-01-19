#ifndef morpion_H
#define morpion_H

#include <stdlib.h>
#include "liste.h"


typedef struct morp
{
    int taille;
    char** morpion;
    int nbCoupsJoues;
} t_morpion, *tpm;

/**
 * recupere la taille du morpion
 * @param morpion le morpion
 * @return la taille des colonnes (ie taille des lignes)
 */
int getTailleMorpion(tpm morpion);

/**
 * Demande à l'utilisateur de saisir une taille de plateau et l'initialise
 * @return une struct correspondant à ce que souhaite l'utilisateur
 */
tpm initialiserMorpion();

/**
 * Verifie que la case aux coordonnees i j est bien jouable ; qu'elle ne soit pas en dehors du plateau,
 * et non plus sur une case deja jouee, grace a la liste des cases jouables
 * @param i colonne
 * @param j ligne
 * @param liste la liste contenant les coups possibles
 * @return vrai ou faux
 */
int estCaseSaisieJouable(int i, int j, tpl liste);

/**
 * Mise a jour de la liste des coups jouables, grace aux coordonnees de la derniere case jouee
 * @param i colonne
 * @param j ligne
 * @param liste liste des coups jouables
 * @param morpion
 * @return la liste mise à jour
 */
tpl trouverCasesJouables(int i, int j, tpl liste, tpm morpion);

#endif
