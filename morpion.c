
#include "fichierH/morpion.h"
#include "fichierH/liste.h"

int getTailleMorpion(tpm morpion)
{
    return morpion->taille;
}
tpm initialiserMorpion()
{
    int i, j, taille = 0;
    tpm morpion = (tpm)malloc(sizeof(t_morpion));

    //On demande à l'utilisteur de saisir la taille du plateau
    while(taille < 2 || taille > 15)
    {
        printf("saisir la taille du morpion (>5 et <15)\n>");
        scanf("%d", &taille);
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
