#include "headers/liste.h"
#include "headers/ia.h"
#include "headers/joueur.h"
#include "headers/morpion.h"
#include "headers/jeu.h"
#include "headers/gainFin.h"
#include "headers/parcoursMorpion.h"



static int profondeur = 0;
static int joueurCourant = -1;
static int fonctionEval1;   // la fonction d'évaluation pour JvIA et IAvIA
static int fonctionEval2;   // l'autre fonction d'évaluation pour IAvIA

void choisirDifficulte()
{
    int prof = 0;
    while(prof < 1 || prof > 4) {
        printf("Veuillez choisir une difficulte : \n"
                       "- (1) bac a sable\n"
                       "- (2) facile\n"
                       "- (3) normal (long)\n"
                       "- (4) difficile (deconseille, vraiment tres long)\n"
                       ">");
        scanf("%d", & prof);
    }
    profondeur = prof;
}


/**
 * Vérifie grâce à nbSeriesAlign si il y a un gagnant
 * @param morpion
 * @return vrai ou faux
 */
static int estGainIA(tpm morpion)
{
    int seriesJ1 = 0;
    int seriesJ2 = 0;

    nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 5, 5);

    if(joueurCourant == 1 && seriesJ1)
        return 1;
    else if(joueurCourant == 1 && seriesJ2)
        return -1;
    else if(joueurCourant == 0 && seriesJ1)
        return -1;
    else if(joueurCourant == 0 && seriesJ2)
        return 1;
    else
        return 0;
}

/**
 * Trouve dans le morpion les cases jouables et renvois la liste des coups
 * plus lente que trouverCasesJouables(), mais n'oblige pas à une copie de liste
 * @param morpion
 * @return
 */
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
                            if(morpion->morpion[k][l] == ' ' && !rechercherElmt(k, l, liste))
                            {
                                liste = ajoutListe(k, l, liste);
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

///**
// * Compte le nombre de mêmes symboles succéssifs horizontalement à partir des coordonnées i j
// * @param i colonne
// * @param j ligne
// * @param morpion
// * @return le nombre de symboles succéssifs
// */
//int compterSuccHorizontale(int i, int j, tpm morpion)
//{
//    int indJ;
//    int cptNbAlignes;
//
//    cptNbAlignes = 1;
//    indJ = j;
//
//    while(indJ < getTailleMorpion(morpion)-1 && morpion->morpion[i][indJ] == morpion->morpion[i][indJ+1])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indJ = indJ +1;
//    }
//    indJ = j;
//    while(indJ > 0 && morpion->morpion[i][indJ] == morpion->morpion[i][indJ-1])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indJ = indJ -1;
//    }
//    return cptNbAlignes;
//}
//
//int compterSuccVerticale(int i, int j, tpm morpion)
//{
//    int indI;
//    int cptNbAlignes;
//
//    cptNbAlignes=1;
//    indI = i;
//
//    while(indI < getTailleMorpion(morpion)-1 && morpion->morpion[indI][j] == morpion->morpion[indI+1][j])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indI = indI +1;
//    }
//    indI = i;
//    while(indI > 0 && morpion->morpion[indI][j] == morpion->morpion[indI-1][j])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indI = indI -1;
//    }
//    return cptNbAlignes;
//}
//
//int compterSuccDiagDesc(int i, int j, tpm morpion)
//{
//    int indI;
//    int indJ;
//    int cptNbAlignes;
//
//    cptNbAlignes=1;
//    indI = i;
//    indJ=j;
//
//    while(indI > 0 && indJ > 0
//    && morpion->morpion[indI][indJ] == morpion->morpion[indI-1][indJ-1])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indI = indI-1;
//        indJ = indJ-1;
//    }
//    indI = i;
//    indJ = j;
//    while(indI < getTailleMorpion(morpion)&& indJ < getTailleMorpion(morpion)-1
//            && morpion->morpion[indI][indJ] == morpion->morpion[indI+1][indJ+1])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indI = indI+1;
//        indJ = indJ+1;
//    }
//    return cptNbAlignes;
//}
//
//int compterSuccDiagAsc(int i, int j, tpm morpion)
//{
//    int indI;
//    int indJ;
//    int cptNbAlignes;
//
//    cptNbAlignes=1;
//    indI = i;
//    indJ = j;
//
//    while(indI > 0 &&  indJ < getTailleMorpion(morpion)-1
//          && morpion->morpion[indI][indJ] == morpion->morpion[indI-1][indJ+1])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indI = indI-1;
//        indJ = indJ+1;
//    }
//    indI = i;
//    indJ = j;
//    while(indI < getTailleMorpion(morpion)-1 && indJ > 0
//          && morpion->morpion[indI][indJ] == morpion->morpion[indI+1][indJ-1])
//    {
//        cptNbAlignes = cptNbAlignes+1;
//        indI = indI+1;
//        indJ = indJ-1;
//    }
//    return cptNbAlignes;
//}

static int evalF(tpm morpion)
{
    int seriesJ1 = 0;
    int seriesJ2 = 0;
    int pointsJ1 = 0;
    int pointsJ2 = 0;
    int gagnant;

    gagnant = estGainIA(morpion);
    printf("gagnant : %d\n\n", gagnant);

    if(gagnant)
    {
        if(gagnant == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    if(estFin(morpion))
    {
        return 0;
    }

    if(joueurCourant == 1)
    {
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 4, 5);
        pointsJ1 += seriesJ1 * 15;
        pointsJ2 += seriesJ2 * 15;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 3, 5);
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 5;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 2, 5);
        pointsJ1 += seriesJ1;
        pointsJ2 += seriesJ2;
    }
    else    // l'adversaire
    {
        nbSeriesAlign(morpion, & seriesJ2, & seriesJ1, 4, 5);
        pointsJ1 += seriesJ1 * 15;
        pointsJ2 += seriesJ2 * 15;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ2, & seriesJ1, 3, 5);
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 5;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ2, & seriesJ1, 2, 5);
        pointsJ1 += seriesJ1;
        pointsJ2 += seriesJ2;
    }

    return pointsJ1 - pointsJ2;
}

/**
 * Ne verifie pas | | |O|O|O| |, tactique offensive
 * @param morpion
 * @return
 */
static int evalMO(tpm morpion)
{
    int gagnant = 0;
    int seriesJ1;
    int seriesJ2;
    int pointsJ1 = 0;
    int pointsJ2 = 0;

    gagnant = estGainIA(morpion);

    if (gagnant != 0) {
        if (gagnant == -1) {
            return -1000 + morpion->nbCoupsJoues;
        } else {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if (estFin(morpion))
        return 0;

    //On compte le nombre de séries de 2 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(2, & seriesJ1, & seriesJ2, morpion);
    pointsJ1 = seriesJ1;
    pointsJ2 = seriesJ2;

    //On compte le nombre de séries de 3 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(3, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }
    else
    {
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }
    else
    {
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }

    printf("======> JOUEUR COURANT : %d <=======\n", joueurCourant);
    afficherMorpion(morpion);
    if(joueurCourant == 1)
    {
        printf("pointsJ1 - pointsJ2 = %d\n\n", pointsJ1 - pointsJ2);
        return pointsJ1 - pointsJ2;
    }
    else
    {
        printf("pointsJ2 - pointsJ1 = %d\n\n", pointsJ2 - pointsJ1);
        return pointsJ2 - pointsJ1;
    }
}

/**
 * Ne verifie pas | | |O|O|O| |, tactique defensive
 * @param morpion
 * @return
 */
static int evalMD(tpm morpion)
{
    int gagnant = 0;
    int seriesJ1;
    int seriesJ2;
    int pointsJ1 = 0;
    int pointsJ2 = 0;

    gagnant = estGainIA(morpion);

    if (gagnant != 0) {
        if (gagnant == -1) {
            return -1000 + morpion->nbCoupsJoues;
        } else {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if (estFin(morpion))
        return 0;

    //On compte le nombre de séries de 2 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(2, & seriesJ1, & seriesJ2, morpion);
    pointsJ1 = seriesJ1;
    pointsJ2 = seriesJ2;

    //On compte le nombre de séries de 3 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(3, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }
    else
    {
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }
    else
    {
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }

    printf("======> JOUEUR COURANT : %d <=======\n", joueurCourant);
    afficherMorpion(morpion);
    if(joueurCourant == 1)
    {
        printf("pointsJ1 - pointsJ2 = %d\n\n", pointsJ1 - pointsJ2);
        return pointsJ1 - pointsJ2;
    }
    else
    {
        printf("pointsJ2 - pointsJ1 = %d\n\n", pointsJ2 - pointsJ1);
        return pointsJ2 - pointsJ1;
    }
}

/**
 * Meilleur fonction d'évaluation, on évaluera toujours en faveur de l'IA (tactique offensive)
 * @param morpion
 * @return
 */
static int evalFO(tpm morpion)
{
    int gagnant = 0;
    int seriesJ1;
    int seriesJ2;
    int pointsJ1 = 0;
    int pointsJ2 = 0;
    int seriesDangJ1 = 0;
    int seriesDangJ2 = 0;

    gagnant = estGainIA(morpion);

    if (gagnant != 0) {
        if (gagnant == -1) {
            return -1000 + morpion->nbCoupsJoues;
        } else {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if (estFin(morpion))
        return 0;

    //On compte le nombre de séries de 2 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(2, & seriesJ1, & seriesJ2, morpion);
    pointsJ1 = seriesJ1;
    pointsJ2 = seriesJ2;

    //On compte le nombre de séries de 3 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(3, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }
    else
    {
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }
    else
    {
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }
//////---------------------------
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 3, 6);
    if(joueurCourant == 1)
    {
        pointsJ1 += seriesDangJ1 * 25;
        pointsJ2 += seriesDangJ2 * 15;
    }
    else
    {
        pointsJ1 += seriesDangJ1 * 15;
        pointsJ2 += seriesDangJ2 * 25;
    }
    // pour ia (1) bac à sable
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 4, 6);
    if(joueurCourant == 1)
    {
        pointsJ1 += seriesDangJ1 * 30;
        pointsJ2 += seriesDangJ2 * 20;
    }
    else
    {
        pointsJ1 += seriesDangJ1 * 20;
        pointsJ2 += seriesDangJ2 * 30;
    }
//////----------------------------

    printf("======> JOUEUR COURANT : %d <=======\n", joueurCourant);
    afficherMorpion(morpion);
    if(joueurCourant == 1)
    {
        printf("pointsJ1 - pointsJ2 = %d\n\n", pointsJ1 - pointsJ2);
        return pointsJ1 - pointsJ2;
    }
    else
    {
        printf("pointsJ2 - pointsJ1 = %d\n\n", pointsJ2 - pointsJ1);
        return pointsJ2 - pointsJ1;
    }
}

/**
 * Meilleur fonction d'évaluation, on évaluera toujours en faveur de l'adversaire (tactique defensive)
 * @param morpion
 * @return
 */
static int evalFD(tpm morpion)
{
    int gagnant = 0;
    int seriesJ1;
    int seriesJ2;
    int pointsJ1 = 0;
    int pointsJ2 = 0;
    int seriesDangJ1 = 0;
    int seriesDangJ2 = 0;

    gagnant = estGainIA(morpion);

    if (gagnant != 0) {
        if (gagnant == -1) {
            return -1000 + morpion->nbCoupsJoues;
        } else {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if (estFin(morpion))
        return 0;

    //On compte le nombre de séries de 2 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(2, & seriesJ1, & seriesJ2, morpion);
    pointsJ1 = seriesJ1;
    pointsJ2 = seriesJ2;

    //On compte le nombre de séries de 3 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(3, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }
    else
    {
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);
    if (joueurCourant == 1)
    {
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }
    else
    {
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }
//////---------------------------
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 3, 6);
    if(joueurCourant == 1)
    {
        pointsJ1 += seriesDangJ1 * 15;
        pointsJ2 += seriesDangJ2 * 25;
    }
    else
    {
        pointsJ1 += seriesDangJ1 * 25;
        pointsJ2 += seriesDangJ2 * 15;
    }
    // pour ia (1) bac à sable
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 4, 6);
    if(joueurCourant == 1)
    {
        pointsJ1 += seriesDangJ1 * 20;
        pointsJ2 += seriesDangJ2 * 30;
    }
    else
    {
        pointsJ1 += seriesDangJ1 * 30;
        pointsJ2 += seriesDangJ2 * 20;
    }
//////----------------------------

    printf("======> JOUEUR COURANT : %d <=======\n", joueurCourant);
    afficherMorpion(morpion);
    if(joueurCourant == 1)
    {
        printf("pointsJ1 - pointsJ2 = %d\n\n", pointsJ1 - pointsJ2);
        return pointsJ1 - pointsJ2;
    }
    else
    {
        printf("pointsJ2 - pointsJ1 = %d\n\n", pointsJ2 - pointsJ1);
        return pointsJ2 - pointsJ1;
    }
}

static int evalBacASable(tpm morpion)
{
    int vainqueur;
    int seriesX = 0;
    int seriesO = 0;


    printf("======> JOUEUR COURANT : %d <=======\n", joueurCourant);    //affichage test
    afficherMorpion(morpion);   //affichage test
    vainqueur = estGainIA(morpion);
    if(vainqueur != 0)
    {
        if(vainqueur == 1)
        {
            return 1000 - morpion->nbCoupsJoues;
        }
        else if(vainqueur == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
        else    //jamais
            return 0;
    }
    if(joueurCourant == 1)
        nbSeriesAlign(morpion, & seriesX, & seriesO, 3, 5);
    else
        nbSeriesAlign(morpion, & seriesO, & seriesX, 3, 5);
    return seriesX - seriesO;
}

static int lancerEval(int evaluer(tpm), tpm morpion)
{
    return evaluer(morpion);
}

/**
 * lance la bonne fonction d'évaluation
 * @param morpion
 * @return le resultat de la fonction d'évaluation
 */
static int eval(tpm morpion)
{
    if(joueurCourant == 1)
    {
        switch(fonctionEval1)
        {
            case 1:
                printf("IA lancee : evalFG\n");
                return lancerEval(evalFD, morpion);
            case 2:
                printf("IA lancee : evalFO\n");
                return lancerEval(evalFO, morpion);
            case 3:
                printf("IA lancee : evalMD\n");
                return lancerEval(evalMD, morpion);
            case 4:
                printf("IA lancee : evalMO\n");
                return lancerEval(evalMO, morpion);
            case 5:
                printf("IA lancee : evalF\n");
                return lancerEval(evalF, morpion);
            case 6:
                printf("IA lancee : evalBacASable\n");
                return lancerEval(evalBacASable, morpion);
            default:
                printf("IA lancee : evalFD\n");
                return lancerEval(evalFD, morpion);
        }
    }
    else
    {
        switch(fonctionEval2)
        {
            case 1:
                printf("IA lancee : evalFG\n");
                return lancerEval(evalFD, morpion);
            case 2:
                printf("IA lancee : evalFO\n");
                return lancerEval(evalFO, morpion);
            case 3:
                printf("IA lancee : evalMD\n");
                return lancerEval(evalMD, morpion);
            case 4:
                printf("IA lancee : evalMO\n");
                return lancerEval(evalMO, morpion);
            case 5:
                printf("IA lancee : evalF\n");
                return lancerEval(evalF, morpion);
            case 6:
                printf("IA lancee : evalBacASable\n");
                return lancerEval(evalBacASable, morpion);
            default:
                printf("IA lancee : evalFD\n");
                return lancerEval(evalFD, morpion);
        }
    }
}

/**
 * MinMax classique
 * @param morpion
 * @param profondeur jusqu'à la quelle l'IA doit anticiper
 * @param estMax pour savoir si on est dans un min ou dans un max
 * @param joueur le joueur au quel c'est le tour de jouer
 * @return
 */
static int minMax(tpm morpion ,int profondeur, int estMax, int joueur)
{
    //TODO enlever le random pour tester des IA !
    int poidsM;
    int i, j, tmp;
    int estFctMax;
    tpl listeTmp;
    tpl liste;

    int random; //  pour ne pas avoir tout le temps la même partie

    if(estMax)
        poidsM = -1000;
    else
        poidsM = 1000;

    if(profondeur == 0 || estGainIA(morpion) || estFin(morpion))
    {
        return eval(morpion);
    }

    liste = trouverJouables(morpion);
    listeTmp = liste;
    joueur = changerJoueur(joueur);

    if(estMax)
        estFctMax = 0;
    else
        estFctMax = 1;

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        afficherListe(liste);
        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        tmp = minMax(morpion, profondeur-1, estFctMax, joueur);
        printf("tpm : %d\n", tmp);// affichage test
        if(estMax)
        {
            if (tmp >= poidsM)
            {
                if (tmp == poidsM)      //pour ne pas avoir tout le temps la même partie
                {
                    random = rand() % 2;
                    printf("RANDOM : %d\n", random);    //affichage test
                    if(random)
                        poidsM = tmp;
                }
                else
                    poidsM = tmp;
            }
        }
        else
        {
            if (tmp <= poidsM)
            {
                if (tmp == poidsM)      //pour ne pas avoir tout le temps la même partie
                {
                    random = rand() % 2;
                    printf("RANDOM : %d\n", random);    //affichage test
                    if(random)
                        poidsM = tmp;
                }
                else
                    poidsM = tmp;
            }
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
    }
    printf("On suppime la liste\n");
    listeTmp = supprimerListe(listeTmp);
    afficherListe(listeTmp);
    free(listeTmp);
    free(liste);

    return poidsM;
}

/**
 * Message qui demande de passienter (appelé lorsque l'IA "calcule")
 * @param cpt
 */
static void afficherPatienter(int cpt)
{
    //clear le terminal
    //printf("\033[H\033[J  ");
    switch (cpt % 4)
    {
        case 0 :
            printf("veuillez patienter\n");
            break;
        case 1 :
            printf("veuillez patienter .\n");
            break;
        case 2 :
            printf("veuillez patienter ..\n");
            break;
        case 3 :
            printf("veuillez patienter ...\n");
            break;
        default :
            break;
    }
}

static void setIA1(int numIA)
{
    fonctionEval1 = numIA;
}

static void setIA2(int numIA)
{
    fonctionEval2 = numIA;
}

void choisirIAenJvIA()
{
    int numIA = 0;
    while(numIA > 6 || numIA < 1)
    {
        printf("Selectionnez la complexite de l'IA :\n"
                       " - (1) forte defensive\n"
                       " - (2) forte offensive\n"
                       " - (3) moyenne defensive\n"
                       " - (4) moyenne offensive\n"
                       " - (5) faible\n"
                       " - (6) tres faible\n>");
        scanf("%d", & numIA);
    }
    setIA1(numIA);
}

void choisirIAenIAvIA()
{
    int numIA = 0;
    while(numIA > 6 || numIA < 1)
    {
        printf("Selectionnez la complexite de la premiere IA :\n"
                       " - (1) forte defensive\n"
                       " - (2) forte offensive\n"
                       " - (3) moyenne defensive\n"
                       " - (4) moyenne offensive\n"
                       " - (5) faible\n"
                       " - (6) tres faible\n>");
        scanf("%d", & numIA);
    }
    setIA1(numIA);
    numIA = 0;
    while(numIA > 6 || numIA < 1)
    {
        printf("Selectionnez la complexite de la deuxieme IA :\n"
                       " - (1) forte defensive\n"
                       " - (2) forte offensive\n"
                       " - (3) moyenne defensive\n"
                       " - (4) moyenne offensive\n"
                       " - (5) faible\n"
                       " - (6) tres faible\n>");
        scanf("%d", & numIA);
    }
    setIA2(numIA);
}

tpl jouerIA(tpm morpion, int joueur)
{
    int maxi = -10000;
    int tmp, indI, indJ;
    int i, j;
    int cpt = 0;    //pour l'affichage
    tpl liste = creerVide();

    liste = trouverJouables(morpion);
    tpl listeTmp = liste;

    joueurCourant = joueur;
    printf("joueurCourant : %d\n", joueurCourant);

    while(liste != NULL)
    {
        afficherPatienter(cpt); // affichage
        ++cpt;                  // affichage

        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        tmp = minMax(morpion, profondeur-1, 0, joueur);
        if(tmp > maxi)
        {
            maxi = tmp;
            indI = i;
            printf("meilleur i : %d\n", i); //affichage test
            indJ = j;
            printf("meilleur j : %d\n", j); //affichage test
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
        printf("\n\n\n-------- ON CHANGE ---------\n\n\n");
    }
    printf("on suppime la premiere liste\n");
    listeTmp = supprimerListe(listeTmp);
    //free(liste);
    free(listeTmp);
    jouerJoueur(indI, indJ, joueur, morpion);

    liste = trouverJouables(morpion);
    printf("et on sorts\n");
    return liste;
}

////meilleurCoups(eval())
