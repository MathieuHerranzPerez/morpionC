#include "fichierH/liste.h"
#include "fichierH/ia.h"
#include "fichierH/joueur.h"
#include "fichierH/morpion.h"
#include "fichierH/jeu.h"
#include "fichierH/gainFin.h"

/**
 * Fonction plus lente que trouverCasesJouables(), mais n'oblige pas à une copie de liste
 * @param morpion
 * @return
 */

int profondeur = 3;

static tpl trouverJouables(tpm morpion)
{
    tpl liste = creerVide();
    int i, j, k, l;
    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[i][j] != ' ')
            {
                for(k = i-1; k <= i+1; ++k)
                {
                    for(l = j-1; l <= j+1; ++l)
                    {
                        if(k >= 0 && k < getTailleMorpion(morpion) && l >= 0 && l < getTailleMorpion(morpion))
                        {
                            if(morpion->morpion[l][k] == ' ' && !rechercherElmt(l, k, liste))
                            {
                                liste = ajoutListe(l, k, liste);
                            }
                        }
                    }
                }
            }
        }
    }
    return liste;
}



/**
 * Enleve de la grille le dernier coups joué aux coordonnées i j
 * @param i colonne
 * @param j ligne
 * @param morpion
 */
static void dejouer(int i, int j, tpm morpion)
{
    morpion->morpion[i][j] = ' ';
}

/**
 * Compte le nombre de mêmes symboles succéssifs à partir des coordonnées i j
 * @param i colonne
 * @param j ligne
 * @param morpion
 * @return le nombre de symboles succéssifs
 */
int compterSuccHorizontale(int i, int j, tpm morpion)
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
    return cptNbAlignes;
}

int compterSuccVerticale(int i, int j, tpm morpion)
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
    return cptNbAlignes;
}

int compterSuccDiagDesc(int i, int j, tpm morpion)
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
    return cptNbAlignes;
}

int compterSuccDiagAsc(int i, int j, tpm morpion)
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
    return cptNbAlignes;
}

static int estBloqueH(int i, int j, tpm morpion)
{
    int indJ;
    int cptPlaceDispo;

    cptPlaceDispo = 1;
    indJ = j;

    while(indJ < getTailleMorpion(morpion)-1 && (morpion->morpion[i][j] == morpion->morpion[i][indJ+1]
            || morpion->morpion[i][indJ+1] == ' '))
    {
        ++cptPlaceDispo;
        indJ = indJ +1;
    }
    indJ = j;
    while(indJ > 0 && (morpion->morpion[i][indJ] == morpion->morpion[i][indJ-1]
            || morpion->morpion[i][indJ-1] == ' '))
    {
        ++cptPlaceDispo;
        indJ = indJ -1;
    }

    if(cptPlaceDispo >= 5)
        return 0;
    return 1;
}

static int estBloqueV(int i, int j, tpm morpion)
{
    int indI;
    int cptPlaceDispo;

    cptPlaceDispo = 1;
    indI = i;

    while(indI < getTailleMorpion(morpion)-1 && (morpion->morpion[indI][j] == morpion->morpion[indI+1][j]
            || morpion->morpion[indI+1][j] == ' '))
    {
        ++cptPlaceDispo;
        indI = indI +1;
    }
    indI = i;
    while(indI > 0 && morpion->morpion[indI][j] == morpion->morpion[indI-1][j])
    {
        ++cptPlaceDispo;
        indI = indI -1;
    }
    if(cptPlaceDispo >= 5)
        return 0;
    return 1;
}

static int estBloqueDA(int i, int j, tpm morpion)
{
    int indI;
    int indJ;
    int cptPlaceDispo;

    cptPlaceDispo=1;
    indI = i;
    indJ=j;

    while(indI > 0 &&  indJ < getTailleMorpion(morpion)-1
          && (morpion->morpion[indI][indJ] == morpion->morpion[indI-1][indJ+1]
              || morpion->morpion[indI-1][indJ+1] == ' '))
    {
        ++cptPlaceDispo;
        indI = indI-1;
        indJ = indJ+1;
    }
    indI = i;
    indJ = j;
    while(indI < getTailleMorpion(morpion)-1 && indJ > 0
          && (morpion->morpion[indI][indJ] == morpion->morpion[indI+1][indJ-1]
              || morpion->morpion[indI-1][indJ+1] == ' '))
    {
        ++cptPlaceDispo;
        indI = indI+1;
        indJ = indJ-1;
    }
    if(cptPlaceDispo >= 5)
        return 0;
    return 1;
}

static int estBloqueDD(int i, int j, tpm morpion)
{
    int indI;
    int indJ;
    int cptPlaceDispo;

    cptPlaceDispo = 1;
    indI = i;
    indJ=j;

    while(indI > 0 && indJ > 0
          && (morpion->morpion[indI][indJ] == morpion->morpion[indI-1][indJ-1]
              || morpion->morpion[indI-1][indJ-1] == ' '))
    {
        ++cptPlaceDispo;
        indI = indI-1;
        indJ = indJ-1;
    }
    indI = i;
    indJ = j;
    while(indI < getTailleMorpion(morpion)-1 && indJ < getTailleMorpion(morpion)-1
          && (morpion->morpion[indI][indJ] == morpion->morpion[indI+1][indJ+1]
              || morpion->morpion[indI+1][indJ+1] == ' '))
    {
        ++cptPlaceDispo;
        indI = indI+1;
        indJ = indJ+1;
    }
    if(cptPlaceDispo >= 5)
        return 0;
    return 1;
}

static int eval(int indI, int indJ, tpm morpion, int estMax)
{
    int cptHorizontale, cptVerticale, cptDiagDes, cptDiagAsc;
    int poidsH = 0,
        poidsV = 0,
        poidsDA = 0,
        poidsDD = 0;
    int meilleurPoids;

    // compter nb pions successifs et si ils sont bloqués

    //horizontale
    cptHorizontale = compterSuccHorizontale(indI, indJ, morpion);
    if(cptHorizontale >= 3 && cptHorizontale < 5 && !estBloqueH(indI, indJ, morpion))
    {
        poidsH = 50;
    }
    else if(cptHorizontale == 5)
    {
        if(estMax)
            return 200;
        else
            return -200;
    }

    //verticale
    cptVerticale = compterSuccVerticale(indI, indJ, morpion);
    if(cptVerticale >= 3 && cptVerticale < 5 && !estBloqueV(indI, indJ, morpion))
    {
        poidsV = 50;
    }
    else if(cptVerticale == 5)
    {
        if(estMax)
            return 200;
        else
            return -200;
    }

    //diag asc
    cptDiagAsc = compterSuccDiagAsc(indI, indJ, morpion);
    if(cptDiagAsc >= 3 && cptDiagAsc < 5 && !estBloqueDA(indI, indJ, morpion))
    {
        poidsDA = 50;
    }
    else if (cptDiagAsc == 5)
    {
        if(estMax)
            return 200;
        else
            return -200;
    }

    //diag desc
    cptDiagDes = compterSuccDiagDesc(indI, indJ, morpion);
    if(cptDiagDes >= 3 && cptDiagAsc < 5 && !estBloqueDD(indI, indJ, morpion))
    {
        poidsDD = 50;
    }
    else if (cptDiagDes == 5)
    {
        if(estMax)
            return 200;
        else
            return -200;
    }

    if(poidsH > poidsV)
        meilleurPoids = poidsH;
    else
        meilleurPoids = poidsV;

    if(poidsDA > meilleurPoids)
        meilleurPoids = poidsDA;
    if(poidsDD > meilleurPoids)
        meilleurPoids = poidsDD;

    if(estMax)
        return meilleurPoids;
    else
        return meilleurPoids * (-1);
}

static int minMax(tpm morpion ,int profondeur, int estMax, tpl listeCoord, int joueur)
{
    int poidsM;
    int i, j, tmp;
    tpl listeTmp;
    tpl liste = creerVide();

    if(estMax)
        poidsM = -1000;
    else
        poidsM = 1000;

    if(profondeur == 0 || estGain(teteListeI(listeCoord), teteListeJ(listeCoord), morpion) || estFin(morpion))
    {
        return eval(teteListeI(listeCoord), teteListeJ(listeCoord), morpion, estMax);
    }

    liste = trouverJouables(morpion);
    listeTmp = liste;

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        //if
        joueur = changerJoueur(joueur);

        if(estMax)
            estMax = 0;
        else
            estMax = 1;

        tmp = minMax(morpion, profondeur-1, estMax, liste, joueur);
        if(estMax)
            if(tmp > poidsM)
                poidsM = tmp;
        else
            if(tmp < poidsM)
                poidsM = tmp;

        // on remet la case testée à defaut
        dejouer(i, j, morpion);

        liste = queueListe(liste);
    }
    listeTmp = supprimerListe(listeTmp);
    free(listeTmp);
    free(liste);
    return poidsM;
}

tpl jouerIA(tpm morpion, int joueur)
{
    int maxi = -10000;
    int tmp, indI, indJ;
    int i, j;
    tpl liste = creerVide();

    liste = trouverJouables(morpion);
    tpl listeTmp = liste;

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        joueur = changerJoueur(joueur);

        tmp = minMax(morpion, profondeur-1, 0, liste, joueur);
        if(tmp > maxi)
        {
             maxi = tmp;
             indI = i;
             indJ = j;
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);

        liste = queueListe(liste);
    }
    listeTmp = supprimerListe(listeTmp);
    free(liste);
    free(listeTmp);
    jouerJoueur(indI, indJ, joueur, morpion);

    liste = trouverJouables(morpion);
    return liste;
}

////int gagnant(tpm morpion)
//// meilleurCoups(eval())


