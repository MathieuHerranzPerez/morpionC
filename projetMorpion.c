#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

typedef struct morp{
    int taille;//
    char** morpion;//
} t_morpion, *tpm;



int getTailleMorpion(tpm morpion)
{
    return morpion->taille;
}

/**
 * @brief initialiserMorpion demande la taille du plateau
 * et initialise le plateau
 * @param morpion
 * @return renvoie le plateau initialisé
 */
tpm initialiserMorpion()
{
    int i, j, taille = 0;
    tpm morpion = (tpm)malloc(sizeof(t_morpion));

    //On demande à l'utilisteur de saisir la taille du plateau
    while(taille < 5 || taille > 25)
    {
        printf("saisir la taille du morpion (>5 et <25) : ");
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
    return morpion;
}

/**
 * @brief afficherMorpion efface le terminal et
 * affiche la matrice (après chaque coups)
 * @param morpion
 */
void afficherMorpion(tpm morpion)
{
    int i, j;
    //clear le terminal
    printf("\033[H\033[J  ");
    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        printf("%d ", i);
    }
    printf("\n");

    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        printf("%d|", i);
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[i][j] == 'X')
            {
                printf("\033[31m");
                printf("X");
                printf("\033[30m");
                printf("|");
            }
            else if(morpion->morpion[i][j] == 'O')
            {
                printf("\033[32m");
                printf("O");
                printf("\033[30m");
                printf("|");
            }
            else
                printf("%c|", morpion->morpion[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief afficherJoueurActuel affiche le joueur dont
 * c’est le tour de jouer
 * @param joueur
 */
void afficherJoueurActuel(int joueur)
{
    if(joueur)
    {
        printf("C'est au joueur ");
        printf("\033[31m");
        printf("X");
        printf("\033[30m");
        printf(" de jouer.\n");
    }
    else
    {
        printf("C'est au joueur ");
        printf("\033[32m");
        printf("O");
        printf("\033[30m");
        printf(" de jouer.\n");
    }
}

int changerJoueur(int joueur)
{
    return (joueur ? 0 : 1);
}

/**
 * @brief estFin Verifie si la matrice est pleine grâce à
 * un compteur et donc si il n’y a pas de gagnant
 * @param nbCoupsJoues
 * @return booléen
 */
int estFin(int nbCoupsJoues, tpm morpion)
{
    return nbCoupsJoues == getTailleMorpion(morpion) * getTailleMorpion(morpion);
}

/**
 * @brief verifierCaseSaisieJouable verifie si la
 * case saisie par le joueur est jouable
 * @param iCase
 * @param jCase
 * @return
 */
int verifierCaseSaisieJouable(int i, int j, tpl liste)
{
    //si on trouve l'élément
    if(rechercherElmt(i, j, liste) != creerVide())
        return 1;
    else
        return 0;
}

/**
 * @brief trouverCasesJouables
 * @param i
 * @param j
 * @param liste
 * @param morpion
 * @return
 */
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
    //on supprime la case jouée
    if(!estVide(liste))
        liste = supprimerElmt(i, j, liste);
    return liste;
}

/**
 * @brief estGainDiagAsc
 * @param i
 * @param j
 * @param morpion
 * @return
 */
int estGainDiagAsc(int i, int j, tpm morpion)
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
 * @brief estGainDiagDes
 * @param i
 * @param j
 * @param morpion
 * @return
 */
int estGainDiagDes(int i, int j, tpm morpion)
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
 * @brief estGainVertical
 * @param i
 * @param j
 * @param morpion
 * @return
 */
int estGainVertical(int i, int j, tpm morpion)
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
 * @brief estGainHorizontal
 * @param i
 * @param j
 * @param morpion
 * @return
 */
int estGainHorizontal(int i, int j, tpm morpion)
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
 * @brief estGain
 * @param i
 * @param j
 * @param morpion
 * @return
 */
int estGain(int i, int j, tpm morpion)
{
    return (estGainDiagAsc(i, j, morpion) || estGainDiagDes(i, j, morpion) ||
            estGainHorizontal(i, j, morpion) || estGainVertical(i, j, morpion));
}

/**
 * @brief jouerJoueur
 * @param i
 * @param j
 * @param joueur
 * @param morpion
 * @param liste
 */
void jouerJoueur(int i, int j, int joueur, tpm morpion)
{
    morpion->morpion[i][j] = (joueur == 1 ? 'X' : 'O');
}

void test()
{
    int i = -1; //pour ne pas le trouver dans la lsite
    int j = -1; //pour ne pas le trouver dans la lsite
    int cptTour = 1;
    int joueur = 1;
    tpl liste = creerVide();

    tpm morpion = initialiserMorpion();
    morpion->morpion[getTailleMorpion(morpion)/2][getTailleMorpion(morpion)/2] = 'O'; //premier coups au milieu
    liste = ajoutListe(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    do
    {
        i = -1;
        j = -1;
        afficherMorpion(morpion);
        afficherJoueurActuel(joueur);
        //on saisie i et j
        while(rechercherElmt(i, j, liste) == NULL)
        {
            printf("Entrez ligne : ");
            scanf("%d", &i);
            printf("Entrez colonne : ");
            scanf("%d", &j);
            if(!verifierCaseSaisieJouable(i, j, liste))
            {
                printf("Mauvaises coordonnées, veuillez en entrer de nouvelles.\n");
                afficherListe(liste);
            }
        }
        jouerJoueur(i, j, joueur, morpion, liste);
        liste = trouverCasesJouables(i, j, liste, morpion);
        joueur = changerJoueur(joueur);
        ++cptTour;
    } while(!estGain(i, j, morpion) && !estFin(cptTour, morpion));
}

int main(void)
{
    test();
    return 0;
}
