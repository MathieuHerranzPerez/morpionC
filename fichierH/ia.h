#ifndef ia_H_
#define ia_H_

#include "liste.h"
#include "morpion.h"

void IA_jouer(t_morpion morpion, int profondeur);
int Max(t_morpion morpion ,int profondeur, t_liste liste);
int Min(t_morpion morpion ,int profondeur, t_liste liste);
int eval(t_morpion morpion);
int gagnant(t_morpion morpion);

#endif
