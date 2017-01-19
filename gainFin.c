/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.2.0 / 12-12-2016
 * @file gainFin.c
 */

#include "headers/gainFin.h"

/**
 * Verifie si la grille est pleine (donc egalite)
 * @param morpion le morpion
 * @return vrai ou faux
 */
int estFin(tpm morpion)
{
    return morpion->nbCoupsJoues == getTailleMorpion(morpion) * getTailleMorpion(morpion);
}

/**
 * Verifie si cinq symboles sont alignes sur la diagonale ascendante
 * @param i ligne
 * @param j colonne
 * @param morpion le morpion
 * @return vrai ou faux
 */
static int estGainDiagAsc(int i, int j, tpm morpion)
{
    int indI;
    int indJ;
    int cptNbAlignes;

    cptNbAlignes=1;
    indI = i;
    indJ=j;

    while(indI > 0 &&  indJ < getTailleMorpion(morpion)-1
          && morpion->morpion[indI][indJ] == morpion->morpion[indI-1][indJ+1])
    {
        cptNbAlignes = cptNbAlignes+1;
        indI = indI-1;
        indJ = indJ+1;
    }
    indI = i;
    indJ = j;
    while(indI < getTailleMorpion(morpion)-1 && indJ > 0
          && morpion->morpion[indI][indJ] == morpion->morpion[indI+1][indJ-1])
    {
        cptNbAlignes = cptNbAlignes+1;
        indI = indI+1;
        indJ = indJ-1;
    }
    return (cptNbAlignes == 5);
}

/**
 * Verifie si cinq symboles sont alignes sur la diagonale descendante
 * @param i ligne
 * @param j colonne
 * @param morpion le morpion
 * @return vrai ou faux
 */
static int estGainDiagDes(int i, int j, tpm morpion)
{
    int indI;
    int indJ;
    int cptNbAlignes;

    cptNbAlignes=1;
    indI = i;
    indJ=j;

    while(indI > 0 && indJ > 0
          && morpion->morpion[indI][indJ] == morpion->morpion[indI-1][indJ-1])
    {
        cptNbAlignes = cptNbAlignes+1;
        indI = indI-1;
        indJ = indJ-1;
    }
    indI = i;
    indJ = j;
    while(indI < getTailleMorpion(morpion)-1 && indJ < getTailleMorpion(morpion)-1
          && morpion->morpion[indI][indJ] == morpion->morpion[indI+1][indJ+1])
    {
        cptNbAlignes = cptNbAlignes+1;
        indI = indI+1;
        indJ = indJ+1;
    }
    return (cptNbAlignes == 5);
}

/**
 * Verifie si cinq symboles sont alignes sur la verticale
 * @param i ligne
 * @param j colonne
 * @param morpion le morpion
 * @return vrai ou faux
 */
static int estGainVertical(int i, int j, tpm morpion)
{
    int indI;
    int cptNbAlignes;

    cptNbAlignes=1;
    indI = i;

    while(indI < getTailleMorpion(morpion)-1 && morpion->morpion[indI][j] == morpion->morpion[indI+1][j])
    {
        cptNbAlignes = cptNbAlignes+1;
        indI = indI +1;
    }
    indI = i;
    while(indI > 0 && morpion->morpion[indI][j] == morpion->morpion[indI-1][j])
    {
        cptNbAlignes = cptNbAlignes+1;
        indI = indI -1;
    }
    return (cptNbAlignes == 5);
}

/**
 * Verifie si cinq symboles sont alignes sur l'horizontale
 * @param i ligne
 * @param j colonne
 * @param morpion le morpion
 * @return vrai ou faux
 */
static int estGainHorizontal(int i, int j, tpm morpion)
{
    int indJ;
    int cptNbAlignes;

    cptNbAlignes = 1;
    indJ = j;

    while(indJ < getTailleMorpion(morpion)-1 && morpion->morpion[i][indJ] == morpion->morpion[i][indJ+1])
    {
        cptNbAlignes = cptNbAlignes+1;
        indJ = indJ +1;
    }
    indJ = j;
    while(indJ > 0 && morpion->morpion[i][indJ] == morpion->morpion[i][indJ-1])
    {
        cptNbAlignes = cptNbAlignes+1;
        indJ = indJ -1;
    }
    return (cptNbAlignes == 5);
}

/**
 * Verifie si un joueur a gagne
 * @param i ligne
 * @param j colonne
 * @param morpion le morpion
 * @return vrai ou faux
 */
int estGain(int i, int j, tpm morpion)
{
    return (estGainDiagAsc(i, j, morpion) || estGainDiagDes(i, j, morpion) ||
            estGainHorizontal(i, j, morpion) || estGainVertical(i, j, morpion));
}
