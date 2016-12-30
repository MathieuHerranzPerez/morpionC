#include "fichierH/parcoursMorpion.h"
#include "fichierH/morpion.h"

static int estBloque(char pion, int i, int j, tpm morpion){
    if( i >= getTailleMorpion(morpion) || j >= getTailleMorpion(morpion))
        return 1;
    if(i <  0 || j <  0)
        return 1;

    // si on tombe sur un pion de l'adversaire, return 1
    if(morpion->morpion[i][j] != ' ' && morpion->morpion[i][j] != pion)
        return 1;

    return 0;
}

static int peutGagnerDiagAsc(char pion, int i, int j, int tailleSerie, int *nbCasesLibres, int sens, tpm morpion)
{
    int nbCasesLibresG;
    int nbCasesLibresD;

    if(estBloque(pion, i, j, morpion))
        return 0;

    //lors de la premiere iteration , on lance recursivement l'algorithme avant la serie et apres la serie
    if(tailleSerie)
    {
        *nbCasesLibres = tailleSerie;
        nbCasesLibresG = *nbCasesLibres;
        nbCasesLibresD = *nbCasesLibres;
        peutGagnerDiagAsc(pion, i + 1, j - 1, 0, &nbCasesLibresG, 1, morpion);
        peutGagnerDiagAsc(pion, i - tailleSerie, j + tailleSerie, 0, &nbCasesLibresD, -1, morpion);

        nbCasesLibresG -= tailleSerie;
        nbCasesLibresD -= tailleSerie;
        (*nbCasesLibres) = nbCasesLibresD+nbCasesLibresG;

        if(*nbCasesLibres >= 5-tailleSerie)
            return 1;

        return 0;
    }
    ++*nbCasesLibres;

    // si on a trouvé que tailleSerie + nbCasesLibres = 5, c est que la serie peut etre gagnante
    if(*nbCasesLibres == 5)
        return 1;
    //lors des iterations suivantes , on lance recursivement l'algorithme pour les cases suivantes (selon le sens)
    return peutGagnerDiagAsc(pion, i + sens, j - sens, 0, nbCasesLibres, sens, morpion);
}

static int peutGagnerDiagDesc(char pion, int i, int j, int tailleSerie, int *nbCasesLibres, int sens, tpm morpion)
{
    int nbCasesLibresG;
    int nbCasesLibresD;

    if(estBloque(pion, i, j, morpion))
        return 0;

    //lors de la premiere iteration , on lance recursivement l'algorithme avant la serie et apres la serie
    if(tailleSerie)
    {
        (*nbCasesLibres) = tailleSerie;
        nbCasesLibresG = *nbCasesLibres;
        nbCasesLibresD = *nbCasesLibres;
        peutGagnerDiagDesc(pion, i + 1, j + 1, 0, &nbCasesLibresD, 1, morpion);
        peutGagnerDiagDesc(pion, i - tailleSerie, j - tailleSerie, 0, &nbCasesLibresG, -1, morpion);

        nbCasesLibresG -= tailleSerie;
        nbCasesLibresD -= tailleSerie;
        (*nbCasesLibres) = nbCasesLibresD+nbCasesLibresG;

        if(*nbCasesLibres >= 5-tailleSerie)
            return 1;

        return 0;
    }
    ++*nbCasesLibres;

    // si on a trouvé que tailleSerie + nbCasesLibres = 5, c est que la serie peut etre gagnante
    if(*nbCasesLibres == 5)
        return 1;
    //lors des iterations suivantes , on lance recursivement l'algorithme pour les cases suivantes (selon le sens)
    return peutGagnerDiagDesc(pion, i + sens, j + sens, 0, nbCasesLibres, sens, morpion);
}

static int peutGagnerVert(char pion, int i, int j, int tailleSerie, int *nbCasesLibres, int sens, tpm morpion){

    int nbCasesLibresB;
    int nbCasesLibresH;

    if(estBloque(pion, i, j, morpion))
        return 0;

    //lors de la premiere iteration , on lance recursivement l'algorithme avant la serie et apres la serie
    if(tailleSerie)
    {
        *nbCasesLibres = tailleSerie;
        nbCasesLibresB = *nbCasesLibres;
        nbCasesLibresH = *nbCasesLibres;
        peutGagnerVert(pion, i + 1, j, 0, & nbCasesLibresB, 1, morpion);
        peutGagnerVert(pion, i - tailleSerie, j, 0, & nbCasesLibresH, -1, morpion) ;

        nbCasesLibresH -= tailleSerie;
        nbCasesLibresB -= tailleSerie;
        (*nbCasesLibres) = nbCasesLibresH+nbCasesLibresB;
        if(*nbCasesLibres >= 5-tailleSerie)
            return 1;

        return 0;
    }
    ++*nbCasesLibres;

    // si on a trouvé que tailleSerie + nbCasesLibres == 5, c est que la serie peut etre gagnante
    if(*nbCasesLibres == 5)
        return 1;
    //lors des iterations suivantes , on lance recursivement l'algorithme pour les cases suivantes (selon le sens)
    return peutGagnerVert(pion, i + sens, j, 0, nbCasesLibres, sens, morpion);
}

static int peutGagnerHori(char pion, int i, int j, int tailleSerie, int *nbCasesLibres, int sens, tpm morpion){

    int nbCasesLibresD;
    int nbCasesLibresG;

    if(estBloque(pion, i, j, morpion))
        return 0;

    //lors de la premiere iteration , on lance recursivement l'algorithme avant la serie et apres la serie
    if(tailleSerie)
    {
        *nbCasesLibres = tailleSerie;
        nbCasesLibresD = *nbCasesLibres;
        nbCasesLibresG = *nbCasesLibres;
        peutGagnerHori(pion, i, j + 1, 0, &nbCasesLibresD, 1, morpion);
        peutGagnerHori(pion, i, j - tailleSerie, 0, &nbCasesLibresG, -1, morpion) ;

        nbCasesLibresD -= tailleSerie;
        nbCasesLibresG -= tailleSerie;
        *nbCasesLibres = nbCasesLibresD+nbCasesLibresG;
        if(*nbCasesLibres >= 5-tailleSerie)
            return 1;

        return 0;
    }
    ++*nbCasesLibres;

    // si on a trouvé que tailleSerie + nbCasesLibres = 5, c est que la serie peut etre gagnante
    if(*nbCasesLibres == 5)
        return 1;
    //lors des iterations suivantes , on lance recursivement l'algorithme pour les cases suivantes (selon le sens)
    return peutGagnerHori(pion, i, j + sens, 0, nbCasesLibres, sens, morpion);
}

//on aurait pu faire autrement, c'est juste pour tester comment ca marche
static void comparerSeries2Succ(int nbMaxOcc, int *nbOcc, char *pion, int i, int j, int *series0, int *series1,
                                void compar(int, int*, char*, int, int, int*, int*, tpm), tpm morpion)
{
    compar(nbMaxOcc, nbOcc, pion, i, j, series0, series1, morpion);
}

static void comparerSerie2SuccDiagDes(int nbMaxOcc, int *nbOccu, char *pion, int i, int j, int *series0, int *series1,
                                      tpm morpion)
{
    int nbCasesLibres;
    if(morpion->morpion[i][j] == ' ')
    {
        *nbOccu = 0;
        *pion = ' ';
        return;
    }

    if(morpion->morpion[i][j] == *pion && *pion != ' '){
        ++*nbOccu;
        if(*nbOccu == nbMaxOcc){
            nbCasesLibres = 0;
            if(peutGagnerDiagAsc(*pion, i, j, nbMaxOcc, & nbCasesLibres, 0, morpion)){
                if(*pion == 'X')
                    ++*series0;
                else
                    ++*series1 ;
            }
            *nbOccu = 0;
            *pion = ' ';
        }
    }
    else
    {
        *pion = morpion->morpion[i][j];
        *nbOccu = 1;
    }
}

static void comparerSerie2SuccDiagAsc(int nbMaxOcc, int *nbOccu, char *pion, int i, int j, int *series0, int *series1,
                                      tpm morpion)
{
    int nbCasesLibres;
    if(morpion->morpion[i][j] == ' ')
    {
        *nbOccu = 0;
        *pion = ' ';
        return;
    }

    if(morpion->morpion[i][j] == *pion && *pion != ' ')
    {
        ++*nbOccu;
        if(*nbOccu == nbMaxOcc)
        {
            nbCasesLibres = 0;
            if(peutGagnerDiagDesc(*pion, i, j, nbMaxOcc, & nbCasesLibres, 0, morpion)){
                if(*pion == 'X')
                    ++*series0;
                else
                    ++*series1;
            }
            *nbOccu = 0;
            *pion = ' ';
        }
    }
    else
    {
        *pion = morpion->morpion[i][j];
        *nbOccu = 1;
    }
}

static void comparerSerie2Bas(int nbMaxOcc, int *nbOcc, char *pion, int i, int j, int *series0, int *series1,
                              tpm morpion)
{
    if(morpion->morpion[i][j] == ' '){
        *nbOcc = 0;
        *pion = ' ';
        return;
    }

    if(morpion->morpion[i][j] == *pion && *pion != ' '){
        ++*nbOcc;
        if(*nbOcc == nbMaxOcc){
            int nbCasesLibres = 0;
            if(peutGagnerVert(*pion, i, j, nbMaxOcc, & nbCasesLibres, 0, morpion))
            {
                if(*pion == 'X')
                    ++*series0;
                else
                    ++*series1;
            }
            *nbOcc = 0;
            *pion = ' ';
        }
    }
    else
    {
        *pion = morpion->morpion[i][j];
        *nbOcc = 1;
    }
}


static void comparerSerie2Droite(int nbMaxOcc, int *nbOcc, char *pion, int i, int j, int *series0, int *series1,
                                 tpm morpion)
{
    if(morpion->morpion[i][j] == ' '){
        *nbOcc = 0;
        *pion = ' ';
        return;
    }

    if(morpion->morpion[i][j] == *pion && *pion != ' '){
        ++*nbOcc;
        if(*nbOcc == nbMaxOcc)
        {
            int nbCasesLibres = 0;
            if(peutGagnerHori(*pion, i, j, nbMaxOcc, & nbCasesLibres, 0, morpion)){
                if(*pion == 'X')
                    ++*series0;
                else
                    ++*series1;
            }
            *nbOcc = 0;
            *pion = ' ';
        }
    }
    else{
        *pion = morpion->morpion[i][j];
        *nbOcc = 1;
    }
}


static void rechercherSeriesSuccDiagASc(int nbMaxOcc, int departX, int departY, int *series0, int *series1, tpm morpion)
{
    int i,j;
    int nbOcc = 0;
    char pion = ' ';

    for(i = departY, j = departX; i < morpion->taille && j < morpion->taille; ++i, ++j)
        comparerSeries2Succ(nbMaxOcc, & nbOcc, & pion, i, j, series0, series1, comparerSerie2SuccDiagAsc,
                            morpion);
}

static void rechercherSeriesSuccDiagDes(int nbMaxOcc, int departX, int departY, int *series0, int *series1, tpm morpion)
{
    int i,j;
    int nbOcc = 0;
    char pion = ' ';

    for(i = departY, j = departX; i < morpion->taille && j >= 0; ++i, --j)
        comparerSeries2Succ(nbMaxOcc, &nbOcc, &pion, i, j, series0, series1, comparerSerie2SuccDiagDes,
                            morpion);
}

static void rechercherSeriesSuccHaut(int nbMaxOcc, int colonne, int *series0, int *series1, tpm morpion)
{
    int i;
    int nbOccu = 0;
    char pion = ' ';
    
    for(i = 0; i < morpion->taille ; ++i)
        comparerSeries2Succ(nbMaxOcc, & nbOccu, & pion, i, colonne, series0, series1, comparerSerie2Bas, morpion);
}

static void rechercherSeriesSuccLarg(int nbMaxOcc, int ligne, int *series0, int *series1, tpm morpion)
{
    int i;
    int nbOccu = 0;
    char pion = ' ';
    
    for(i = 0; i < morpion->taille ; ++i)
        comparerSeries2Succ(nbMaxOcc, & nbOccu, & pion, ligne, i, series0, series1, comparerSerie2Droite, morpion);
}

static void rechercherSeriesSuccDiags(int nbMaxOcc, int *series0, int *series1, tpm morpion)
{
    int i;

    // parcours du premier element
    rechercherSeriesSuccDiagASc(nbMaxOcc, 0, 0, series0, series1, morpion);

    for(i = 1; i < morpion->taille - nbMaxOcc +1 ; ++i){
        // parcours les colonnes de la premiere ligne
        rechercherSeriesSuccDiagASc(nbMaxOcc, i, 0, series0, series1, morpion);

        // parcours les lignes de la premiere colonne
        rechercherSeriesSuccDiagASc(nbMaxOcc, 0, i, series0, series1, morpion);
    }

    rechercherSeriesSuccDiagDes(nbMaxOcc, morpion->taille - 1, 0, series0, series1, morpion);

    for(i = morpion->taille -2; i >= nbMaxOcc -1 ; --i)
    {
        // parcours les colonnes de la premiere ligne
        rechercherSeriesSuccDiagDes(nbMaxOcc, i, 0, series0, series1, morpion);
        // parcours les lignes de la derniere colonne
        rechercherSeriesSuccDiagDes(nbMaxOcc, morpion->taille - 1, morpion->taille - 1 - i, series0, series1, morpion);
    }
}

static void rechercherSeriesSuccVert(int nbMaxOcc, int *series0, int *series1, tpm morpion){
    int i;
    for(i = 0; i < morpion->taille ; ++i)
        rechercherSeriesSuccHaut(nbMaxOcc, i, series0, series1, morpion);
}

static void rechercherSeriesSuccHori(int nbMaxOcc, int *series0, int *series1, tpm morpion){
    int i;
    for(i = 0; i < morpion->taille ; ++i)
        rechercherSeriesSuccLarg(nbMaxOcc, i, series0, series1, morpion);
}

//Compte le nombre de séries nbMaxOcc de pions alignés de chacun des joueurs
void rechercherSeriesSucc(int nbMaxOcc, int *series0, int *series1, tpm morpion)
{
    rechercherSeriesSuccDiags(nbMaxOcc, series0, series1, morpion);

    rechercherSeriesSuccVert(nbMaxOcc, series0, series1, morpion);

    rechercherSeriesSuccHori(nbMaxOcc, series0, series1, morpion);
}


int estBloqueH(int i, int j, tpm morpion, int placeDispo)
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

    if(cptPlaceDispo >= placeDispo)
        return 0;
    return 1;
}

int estBloqueV(int i, int j, tpm morpion, int placeDispo)
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
    if(cptPlaceDispo >= placeDispo)
        return 0;
    return 1;
}

int estBloqueDA(int i, int j, tpm morpion, int placeDispo)
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
    if(cptPlaceDispo >= placeDispo)
        return 0;
    return 1;
}

int estBloqueDD(int i, int j, tpm morpion, int placeDispo)
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
    if(cptPlaceDispo >= placeDispo)
        return 0;
    return 1;
}

void nbSeriesAlign(tpm morpion, int* seriesX, int* seriesO, int nbAlign, int nbNonBloque)
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
                if(cptO == nbAlign && !estBloqueH(i, j, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[i][j] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueH(i, j, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
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

                if(cptO == nbAlign && !estBloqueV(j, i, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[j][i] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueV(j, i, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
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

                if(cptO == nbAlign && !estBloqueDD(k, l, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDD(k, l, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
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

                if(cptO == nbAlign && !estBloqueDD(k, l, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDD(k, l, morpion, nbNonBloque)) // on vérifie en même temps que ce n'est pas bloqué
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

                if(cptO == nbAlign && !estBloqueDA(k, l, morpion, nbNonBloque)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDA(k, l, morpion, nbNonBloque)) // onvérifie en même temps que ce n'est pas bloqué
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

                if(cptO == nbAlign && !estBloqueDA(k, l, morpion, nbNonBloque)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesO;
            }
            else if(morpion->morpion[k][l] == 'X')
            {
                ++cptX;
                cptO = 0;
                if(cptX == nbAlign && !estBloqueDA(k, l, morpion, nbNonBloque)) // onvérifie en même temps que ce n'est pas bloqué
                    ++*seriesX;
            }
            ++k;
            --l;
        }
    }

    printf("seriesX : %d nbAlign : %d\n", *seriesX, nbAlign);  // affichage test
    printf("seriesO : %d nbAlign : %d\n", *seriesO, nbAlign);  // affichage test
}