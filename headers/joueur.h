#ifndef joueur_H
#define joueur_H

/**
 * Affiche le joueur qui devra jouer le prochain coup (utile pour du Joueur vs Joueur)
 * @param joueur
 */
void afficherJoueurActuel(int joueur);

/**
 * Change de joueur en changeant la variable joueur
 * @param joueur
 * @return 0 ou 1
 */
int changerJoueur(int joueur);

#endif
