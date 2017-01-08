/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.1.1 / 12-12-2016
 * @file morpion.c
 */
#include "headers/morpion.h"

int getTailleMorpion(tpm morpion)
{
    return morpion->taille;
}
tpm initialiserMorpion()
{
    int i, j, taille = 0;
    int testSaisie = 0;
    char chaine[2];
    tpm morpion = (tpm)malloc(sizeof(t_morpion));

    //On demande à l'utilisteur de saisir la taille du plateau
    printf("Saisir taille du morpion (5 < et < 15)\n");
    while(taille < 5 || taille > 15)
    {
        while(testSaisie != 1)
        {
            fgets(chaine, sizeof chaine, stdin);
            testSaisie = sscanf(chaine, "%d", &taille);
        }
        testSaisie = 0;
    }

    //On reserve de la place pour notre plateau
    morpion->morpion = (char**)malloc(taille * sizeof(char*));
    for(i = 0; i < taille; ++i)
    {
        morpion->morpion[i] = (char*)malloc(taille * sizeof(char));
    }

    //On initialise chaque case avec un espace
    for(i = 0; i < taille; ++i)
    {
        for(j = 0; j < taille; ++j)
        {
            morpion->morpion[i][j] = ' ';
        }
    }
    morpion->taille = taille;
    morpion->nbCoupsJoues = 1;
    return morpion;
}
int verifierCaseSaisieJouable(int i, int j, tpl liste)
{
    //si on trouve l'élément
    if(rechercherElmt(i, j, liste) != creerVide())
        return 1;
    else
        return 0;
}

tpl trouverCasesJouables(int i, int j, tpl liste, tpm morpion)
{
    int indI;
    int indJ;
    for(indI = i-1; indI <= i+1; ++indI)
    {
        for(indJ = j-1; indJ <= j+1; ++indJ)
        {
            if(indI >= 0 && indJ >= 0 && indI < getTailleMorpion(morpion) && indJ < getTailleMorpion(morpion)
               && morpion->morpion[indI][indJ] == ' ' && !rechercherElmt(indI, indJ, liste))
            {
                liste = ajoutListe(indI, indJ, liste);
            }
        }
    }
    return liste;
}
