#ifndef morpion_H
#define morpion_H

#include <stdlib.h>
#include "liste.h"

typedef struct morp{
    int taille;
    char** morpion;
    int nbCoupsJoues;
} t_morpion, *tpm;

int getTailleMorpion(tpm morpion);
tpm initialiserMorpion();
int verifierCaseSaisieJouable(int i, int j, tpl liste);
tpl trouverCasesJouables(int i, int j, tpl liste, tpm morpion);

#endif
