#include "fichierH/liste.h"
#include "fichierH/ia.h"
#include "fichierH/joueur.h"
#include "fichierH/morpion.h"
#include "fichierH/jeu.h"
#include "fichierH/gainFin.h"
#include "fichierH/parcoursMorpion.h"


static int estBloqueH(int i, int j, tpm morpion);
static int estBloqueV(int i, int j, tpm morpion);
static int estBloqueDA(int i, int j, tpm morpion);
static int estBloqueDD(int i, int j, tpm morpion);

/**
 * Fonction plus lente que trouverCasesJouables(), mais n'oblige pas à une copie de liste
 * @param morpion
 * @return
 */

static int profondeur = 0;
static int joueurCourant = -1;

void choisirDifficulte()
{
    while(profondeur < 1 || profondeur > 4) {
        printf("Veuillez choisir une difficulté : \n"
                       "- (1) bac a sable\n"
                       "- (2) facile\n"
                       "- (3) normal\n"
                       "- (4) difficile (deconseille, trop long)\n"
                       ">");
        scanf("%d", &profondeur);
    }
}

void nbSeriesAlign(tpm morpion, int* seriesX, int* seriesO, int nbAlign) //TODO mettre ca en static et l'enlever du .h
{
    int i, j, k, l;
    int cptO, cptX;

    *seriesX = 0;
    *seriesO = 0;
    //ligne
    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        cptO = 0;
        cptX = 0;

        //horizontale
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[i][j] == 'O')
            {
                ++cptO;
                cptX = 0;
                if(cptO == nbAlign && !estBloqueH(i, j, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[i][j] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueH(i, j, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
        }

        cptO = 0;
        cptX = 0;

        //verticalement
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[j][i] == 'O')
            {
                ++cptO;
                cptX = 0;

                if(cptO == nbAlign && !estBloqueV(j, i, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[j][i] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueV(j, i, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
        }
    }

    cptO = 0;
    cptX = 0;

    //diag desc
    for(i = 0; i < getTailleMorpion(morpion) - 4; ++i)
    {
        cptO = 0;
        cptX = 0;
        k = i;
        l = 0;
        while(k < getTailleMorpion(morpion))
        {
            if(morpion->morpion[k][l] == 'O')
            {
                ++cptO;
                cptX = 0;

                if(cptO == nbAlign && !estBloqueDD(k, l, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDD(k, l, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
            ++k;
            ++l;
        }
    }
    for(j = 1; j < getTailleMorpion(morpion) - 4; ++j) // on commence à 1 pour pas refaire la meme diag qu'avec i = 0
    {
        cptO = 0;
        cptX = 0;
        k = 0;
        l = j;
        while(l < getTailleMorpion(morpion))
        {
            if(morpion->morpion[k][l] == 'O')
            {
                ++cptO;
                cptX = 0;

                if(cptO == nbAlign && !estBloqueDD(k, l, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDD(k, l, morpion)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
            ++k;
            ++l;
        }
    }

    //diag asc

    for(i = 0; i < getTailleMorpion(morpion) - 4; ++i)
    {
        cptO = 0;
        cptX = 0;
        k = i;
        l = getTailleMorpion(morpion) - 1;
        while(k < getTailleMorpion(morpion))
        {
            if(morpion->morpion[k][l] == 'O')
            {
                ++cptO;
                cptX = 0;

                if(cptO == nbAlign && !estBloqueDA(k, l, morpion)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDA(k, l, morpion)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
            ++k;
            --l;
        }
    }
    for(j = getTailleMorpion(morpion)-2; j > 3; --j) // on commence à taille - 2 pour pas refaire la meme diag qu'avec i = 0
    {
        cptO = 0;
        cptX = 0;
        k = 0;
        l = j;
        while(l >= 0)
        {
            if(morpion->morpion[k][l] == 'O')
            {
                ++cptO;
                cptX = 0;

                if(cptO == nbAlign && !estBloqueDA(k, l, morpion)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDA(k, l, morpion)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
            ++k;
            --l;
        }
    }

    printf("seriesX : %d nbAlign : %d\n", *seriesX, nbAlign);
    printf("seriesO : %d nbAlign : %d\n", *seriesO, nbAlign);
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

    nbSeriesAlign(morpion, &seriesJ1, &seriesJ2, 5);

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

/**
 * Compte le nombre de mêmes symboles succéssifs horizontalement à partir des coordonnées i j
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
    while(indI < getTailleMorpion(morpion)&& indJ < getTailleMorpion(morpion)-1
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
    while(indI > 0 && (morpion->morpion[indI][j] == morpion->morpion[indI-1][j]
            || morpion->morpion[indI-1][j] == ' '))
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

    cptPlaceDispo = 1;
    indI = i;
    indJ = j;

    while(indI > 0 && indJ < getTailleMorpion(morpion)-1
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
              || morpion->morpion[indI+1][indJ-1] == ' '))
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
/**
 * Evalue juste le dernier coups joué
 * @param indI
 * @param indJ
 * @param morpion
 * @param estMax
 * @return
 */
static int eval(int indI, int indJ, tpm morpion, int estMax) //TODO prendre en compte le dernier coup joué, et tout les coups intermédiaires (pas juste le dernier)
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

    printf("meilleurPoids : %d\n", meilleurPoids);

    if(estMax)
        return meilleurPoids;
    else
        return meilleurPoids * (-1);
}


//static int eval2(tpm morpion)
//{
//    int cptHorizontale, cptVerticale, cptDiagDes, cptDiagAsc;
//    int seriesJ1 = 0;
//    int seriesJ2 = 0;
//
//    printf("-----------------------\n");
//    afficherMorpion(morpion); // affichageTest
//
//    // compter nb pions successifs et si ils sont bloqués
//
//    if(estGainIA(morpion))
//    {
//        if(!estMax)  //il faut faire l'inverse car ici on aura déjà changé de joueur (et donc de estMax)
//        {
//            printf("eval2 EstMax Gain : %d\n", -1000 + morpion->nbCoupsJoues); // affichageTest
//            return -1000 + morpion->nbCoupsJoues;       //TODO inverser ?
//        }
//        else
//        {
//            printf("eval2 NonEstMax Gain : %d\n", 1000 - morpion->nbCoupsJoues); // affichageTest
//            return 1000 - morpion->nbCoupsJoues;        //TODO inverser ?
//        }
//    }
//
//    // on compte le nombre de séries de 3 pions non bloqués de chaque joueur
//    nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 3);
//
//    if(!estMax && (seriesJ1 || seriesJ2))
//    {
//        printf("eval2 EstMax 3 Align : %d\n", seriesJ2 - seriesJ1); // affichageTest
//        return 2*(seriesJ2 - seriesJ1);     //TODO inverser ?
//    }
//    else if(estMax &&(seriesJ1 || seriesJ2))
//    {
//        printf("eval2 NonEstMax 3 Align : %d\n", seriesJ1 - seriesJ2); // affichageTest
//        return 2*(seriesJ1 - seriesJ2);         //TODO inverser ?
//    }
//        //on compte le nombre de séries de 2 pions non bloqués de chaque joueur
//    else
//    {
//        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 2);
//        if(!estMax)
//        {
//            printf("eval2 EstMax 2 Align : %d\n", seriesJ2 - seriesJ1); // affichageTest
//            return seriesJ2 - seriesJ1;     //TODO inverser ?
//        }
//        else
//        {
//            printf("eval2 NonEstMax 2 Align : %d\n", seriesJ1 - seriesJ2); // affichageTest
//            return seriesJ1 - seriesJ2;         //TODO inverser ?
//        }
//    }
//}

static int eval2(tpm morpion)
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
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 4);
        pointsJ1 += seriesJ1 * 15;
        pointsJ2 += seriesJ2 * 15;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 3);
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 5;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 2);
        pointsJ1 += seriesJ1;
        pointsJ2 += seriesJ2;
    }
    else    // l'adversaire
    {
        nbSeriesAlign(morpion, & seriesJ2, & seriesJ1, 4);
        pointsJ1 += seriesJ1 * 15;
        pointsJ2 += seriesJ2 * 15;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ2, & seriesJ1, 3);
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 5;

        seriesJ1 = 0;
        seriesJ2 = 0;
        nbSeriesAlign(morpion, & seriesJ2, & seriesJ1, 2);
        pointsJ1 += seriesJ1;
        pointsJ2 += seriesJ2;
    }

    return pointsJ1 - pointsJ2;
}
/**
 * Ne fonctionne pas en IA vs IA
 * @param morpion
 * @return
 */
static int evalF(tpm morpion)
{
    int gagnant = 0;
    int seriesJ1, seriesJ2;
    int pointsJ1 = 0, pointsJ2 = 0;

    gagnant = estGainIA(morpion);

    if(gagnant != 0)
    {
        if (gagnant == -1) {
            return -1000 + morpion->nbCoupsJoues;
        }
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if(estFin(morpion))
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
    pointsJ1 += seriesJ1 * 5;
    pointsJ2 += seriesJ2 * 5;

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);
    pointsJ1 += seriesJ1 * 12;
    pointsJ2 += seriesJ2 * 12;

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

static int evalF2(tpm morpion) {
    int gagnant = 0;
    int seriesJ1, seriesJ2;
    int pointsJ1 = 0, pointsJ2 = 0;

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

static int evalBacASable(tpm morpion)
{
    int vainqueur;
    int seriesX = 0;
    int seriesO = 0;

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
        nbSeriesAlign(morpion, & seriesX, & seriesO, 3);
    else
        nbSeriesAlign(morpion, & seriesO, & seriesX, 3);
    return seriesX - seriesO;
}

static int minMax(tpm morpion ,int profondeur, int estMax, int joueur)
{
    int poidsM;
    int i, j, tmp;
    int estFctMax;
    tpl listeTmp;
    tpl liste = creerVide();

    if(estMax)
        poidsM = -1000;
    else
        poidsM = 1000;

    if(profondeur == 0 || estGainIA(morpion) || estFin(morpion))
    {
        //return evalBacASable(morpion);
        return evalF2(morpion);
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
            if (tmp > poidsM)
                poidsM = tmp;
        }
        else
        {
            if (tmp < poidsM)
                poidsM = tmp;
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
