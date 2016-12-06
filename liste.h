#ifndef liste_H
#define liste_H

#include <stdio.h>
#include <stdlib.h>

typedef struct structListe
{
    int i;
    int j;
    struct structListe * suiv;
} t_liste, * tpl;


tpl creerVide(void);		// créé une liste vide
int estVide(tpl liste);				// verifie si la liste est vide
tpl ajoutListe(int i, int j, tpl liste);
int teteListeI(tpl liste);
int teteListeJ(tpl liste);
tpl queueListe(tpl liste);
tpl creeListe();
int compterLongueur(tpl liste);
tpl copierListe(tpl liste);
tpl rechercherElmt(int i, int j, tpl liste);
tpl supprimerElmt(int i, int j, tpl liste);
tpl recopierInverse(tpl liste);
void afficherListe(tpl liste);

#endif
