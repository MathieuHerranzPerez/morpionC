#ifndef joueur_H
#define joueur_H


/**
 * Affiche le joueur qui devra jouer le prochain coup (utile pour du Joueur vs Joueur)
 * @param joueur le joueur actuel
 */
void afficherJoueurActuel(int joueur);

/**
 * change de joueur courant
 * @param joueur l'ancien joueur
 * @return nouveau joueur
 */
int changerJoueur(int joueur);

#endif
