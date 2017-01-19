#ifndef liste_H
#define liste_H

#include <stdio.h>
#include <stdlib.h>

typedef struct structListe
{
    int i;
    int j;
    struct structListe * suiv;
}
t_liste, * tpl;

/**
 * creer une liste vide
 * @return return nullptr
 */
tpl creerVide(void);

/**
 * verifie si la liste donnée en param est nulle ou non
 * @param liste
 * @return vrai ou faux
 */
int estVide(tpl liste);

/**
 * ajoute les valeurs i et j dans la liste liste
 * @param i
 * @param j
 * @param liste
 * @return la nouvelle liste
 */
tpl ajoutListe(int i, int j, tpl liste);

/**
 * return la valeur de la donnée i de la liste liste
 * @param liste
 * @return i
 */
int teteListeI(tpl liste);

/**
 * return la valeur de la donnée j de la liste liste
 * @param liste
 * @return j
 */
int teteListeJ(tpl liste);

/**
 * return un ptr sur le suivant de la liste liste
 * @param liste
 * @return une liste
 */
tpl queueListe(tpl liste);

/**
 * demande à l'utilisateur de choisir des valeurs pour creer une liste
 * @return return une liste
 */
tpl creeListe();

/**
 * compte la taille de la liste liste
 * @param liste
 * @return return une longueur
 */
int compterLongueur(tpl liste);

/**
 * creer une liste en recopiant la liste passée en param
 * @param liste
 * @return return la nouvelle liste
 */
tpl copierListe(tpl liste);

/**,
 * recherche l'element de la liste liste contenant i et j et renvoie la sous-liste à partir de cet element ou null
 * @param i
 * @param j
 * @param liste
 * @return retourne une liste (qui peut être vide)
 */
tpl rechercherElmt(int i, int j, tpl liste);

/**
 * supprime les elements de la liste liste contenant les valeurs i et j
 * @param i
 * @param j
 * @param liste
 * @return return la liste privée de ces éléments
 */
tpl supprimerElmt(int i, int j, tpl liste);

/**
 * recopie la liste liste inversée
 * @param liste
 * @return return la nouvelle liste obtenue
 */
tpl recopierInverse(tpl liste);

/**
 * afficher la liste liste
 * @param liste
 */
void afficherListe(tpl liste);

/**
 * supprime la liste liste
 * @param liste
 * @return return nullptr
 */
tpl supprimerListe(tpl liste);

#endif
