#include "fichierH/liste.h"
#include "fichierH/ia.h"
#include "fichierH/joueur.h"
#include "fichierH/morpion.h"
#include "fichierH/jeu.h"
#include "fichierH/gainFin.h"


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

    printf("seriesX : %d\n", *seriesX);
    printf("seriesO : %d\n", *seriesO);
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

    nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 5);
    if(seriesJ1 || seriesJ2)
        return 1;
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

    afficherMorpion(morpion); // affichageTest

    // compter nb pions successifs et si ils sont bloqués

    printf("%d %d\n", indI, indJ); // affichageTest

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


static int eval2(int indI, int indJ, tpm morpion, int estMax) //TODO prendre en compte le dernier coup joué, et tout les coups intermédiaires (pas juste le dernier)
{
    int cptHorizontale, cptVerticale, cptDiagDes, cptDiagAsc;
    int seriesJ1 = 0;
    int seriesJ2 = 0;

    printf("-----------------------\n");
    afficherMorpion(morpion); // affichageTest

    // compter nb pions successifs et si ils sont bloqués

    printf("%d %d\n", indI, indJ); // affichageTest

    if(estGainIA(morpion))
    {
        if(estMax)  //il faut faire l'inverse car ici on aura déjà changé de joueur (et donc de estMax)
        {
            printf("eval2 : %d\n", -1000 + morpion->nbCoupsJoues); // affichageTest
            return -1000 + morpion->nbCoupsJoues;       //TODO inverser ?
        }
        else
        {
            printf("eval2 : %d\n", 1000 - morpion->nbCoupsJoues); // affichageTest
            return 1000 - morpion->nbCoupsJoues;        //TODO inverser ?
        }
    }

    // on compte le nombre de séries de 3 pions non bloqués de chaque joueur
    nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 3);

    if(estMax && (seriesJ1 || seriesJ2))
    {
        printf("eval2 : %d\n", seriesJ2 - seriesJ1); // affichageTest
        return 2*(seriesJ2 - seriesJ1);     //TODO inverser ?
    }
    else if(!estMax &&(seriesJ1 || seriesJ2))
    {
        printf("eval2 : %d\n", seriesJ1 - seriesJ2); // affichageTest
        return 2*(seriesJ1 - seriesJ2);         //TODO inverser ?
    }
    else
    {
        nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 2);
        if(estMax)
        {
            printf("eval2 : %d\n", seriesJ2 - seriesJ1); // affichageTest
            return seriesJ2 - seriesJ1;     //TODO inverser ?
        }
        else
        {
            printf("eval2 : %d\n", seriesJ1 - seriesJ2); // affichageTest
            return seriesJ1 - seriesJ2;         //TODO inverser ?
        }
    }
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
        return eval2(teteListeI(listeCoord), teteListeJ(listeCoord), morpion, estMax);
    }

    liste = trouverJouables(morpion);
    listeTmp = liste;
    joueur = changerJoueur(joueur);

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        if(estMax)
            estMax = 0;
        else
            estMax = 1;

        tmp = minMax(morpion, profondeur-1, estMax, liste, joueur);
        printf("tpm : %d\n", tmp);// affichage test
        if(estMax)
            if(tmp > poidsM)
                poidsM = tmp;
        else
            if(tmp < poidsM)
                poidsM = tmp;

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
    }
    listeTmp = supprimerListe(listeTmp);
    free(listeTmp);
    free(liste);

    printf("poidsM : %d\n", poidsM);    //affichage test
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
        ++morpion->nbCoupsJoues;

        tmp = minMax(morpion, profondeur-1, 0, liste, joueur);
        if(tmp > maxi)
        {
            maxi = tmp;
            indI = i;
            printf("meilleur i : %d\n", i); //affichage test
            indJ = j;
            printf("meilleur j : %d\n", j); //affichage test
        }
        printf("maxi : %d\n\n", maxi);// affichage test

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

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


