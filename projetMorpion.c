#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

int TAILLE = 0;

/**
 * @brief initialiserMorpion demande la taille du plateau
 * et initialise le plateau
 * @param morpion
 * @return renvoie le plateau initialisé
 */
char** initialiserMorpion(char** morpion)
{
    int i, j;
    
    //On demande à l'utilisteur de saisir la taille du plateau
    while(TAILLE < 5 || TAILLE > 25)
    {
        printf("saisir la taille du morpion (>5 et <25) : ");
        scanf("%d", &TAILLE);
    }

    //On reserve de la place pour notre plateau
    morpion = (char**)malloc(TAILLE * sizeof(char*));
    for(i = 0; i < TAILLE; ++i)
    {
        morpion[i] = (char*)malloc(TAILLE * sizeof(char));
    }

    //On initialise chaque case avec un espace
    for(i = 0; i < TAILLE; ++i)
    {
        for(j = 0; j < TAILLE; ++j)
        {
            morpion[i][j] = ' ';
        }
    }
    return morpion;
}

/**
 * @brief afficherMorpion efface le terminal et
 * affiche la matrice (après chaque coups)
 * @param morpion
 */
void afficherMorpion(char** morpion)
{
    int i, j;
    //clear le terminal
    printf("\033[H\033[J  ");
    for(i = 0; i < TAILLE; ++i)
    {
        printf("%d ", i);
    }
    printf("\n");
    
    for(i = 0; i < TAILLE; ++i)
    {
        printf("%d|", i);
        for(j = 0; j < TAILLE; ++j)
        {
            printf("%c|", morpion[i][j]);
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
int estFin(int nbCoupsJoues)
{
    return nbCoupsJoues == TAILLE;
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


tpl trouverCasesJouables(int i, int j, tpl liste, char** morpion)
{
    int indI;
    int indJ;
            
    //on supprime la case jouée
    supprimerElmt(i, j, liste);
    for(indI = i-1; indI < i+1; ++indI)
    {
        for(indJ = j-1; indJ < j+1; ++indJ)
        {
            if(indI >= 0 && indJ >= 0 && indI < TAILLE && indJ < TAILLE
                    && morpion[indI][indJ] == ' ')
            {
                ajoutListe(indI, indJ, liste);
            }
        }
    }
    return liste;
}

int estGainDiagAsc(int i, int j, char** morpion)
{
    int indI;
    int indJ;
    int cptNbAlignes;
    
    cptNbAlignes=1;
    indI = i;
    indJ=j; 
    
    while(indI > 1 &&  indJ < TAILLE-1 
        && morpion[indI][indJ]==morpion[indI-1][indJ+1])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indI = indI-1;
        indJ = indJ+1;   
    }
    indI = i;
    indJ = j;
    while(indI < TAILLE+1 && indJ > 1 
        && morpion[indI][indJ]==morpion[indI+1][indJ-1])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indI = indI+1;
        indJ = indJ-1;   
    }
    return (cptNbAlignes == 5);
}

int estGainDiagDes(int i, int j, char** morpion)
{
    int indI;
    int indJ;
    int cptNbAlignes;
    
    cptNbAlignes=1;
    indI = i;
    indJ=j; 
    
    while(indI > 1 &&  indJ > 1 
        && morpion[indI][indJ]==morpion[indI-1][indJ-1])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indI = indI-1;
        indJ = indJ-1;   
    }
    indI = i;
    indJ = j;
    while(indI < TAILLE+1 && indJ < TAILLE+1 
        && morpion[indI][indJ]==morpion[indI+1][indJ+1])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indI = indI+1;
        indJ = indJ+1;   
    }
    return (cptNbAlignes == 5);
}

int estGainVertical(int i, int j, char** morpion)
{
    int indI;
    int cptNbAlignes;
    
    cptNbAlignes=1;
    indI = i; 
    
    while(indI < TAILLE-1 && morpion[indI][j]==morpion[indI+1][j])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indI = indI +1;   
    }
    indI = i;
    while(indI > 1 && morpion[indI][j]==morpion[indI-1][j])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indI = indI -1;   
    }
    return (cptNbAlignes == 5);
}

int estGainHorizontal(int i, int j, char** morpion)
{
    int indJ;
    int cptNbAlignes;
    
    cptNbAlignes=1;
    indJ = j;
    
    while(indJ < TAILLE-1 && morpion[i][indJ]==morpion[i][indJ+1])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indJ = indJ +1;   
    }
    indJ = j;
    while(indJ > 1 && morpion[i][indJ]==morpion[i][indJ-1])
    {
        cptNbAlignes = cptNbAlignes+1;          
        indJ = indJ -1;   
    }
    return (cptNbAlignes == 5);
}

int estGain(int i, int j, char** morpion)
{
    return (estGainDiagAsc(i, j, morpion) || estGainDiagDes(i, j, morpion) ||
            estGainHorizontal(i, j, morpion) || estGainVertical(i, j, morpion));
}

void jouerJoueur(int joueur, char** morpion, tpl liste)
{
    int i = -1; //pour ne pas le trouver dans la lsite
    int j = -1; //pour ne pas le trouver dans la lsite
    
    while(rechercherElmt(i, j, liste) != NULL)
    {
        printf("Entrez i : ");
        scanf("%d", &i);
        printf("Entrez j : ");
        scanf("%d", &j);
        if(rechercherElmt(i, j, liste) == NULL)
            printf("Mauvaises coordonnées, veuillez en entrer de nouvelles.\n");
    }
    morpion[i][j] = (joueur == 1 ? 'X' : 'O');
}

int main(void)
{
    //jouer();
    int joueur = 1;
    char ** morpion = initialiserMorpion(morpion);
    afficherMorpion(morpion);
    afficherJoueurActuel(joueur);


    return 0;
}

