
#include <stdlib.h>
#include <stdio.h>
#include "fichierH/morpion.h"
#include "fichierH/liste.h"
#include "fichierH/jeu.h"
#include "fichierH/ia.h"
#include "fichierH/gainFin.h"
#include "fichierH/joueur.h"


int main(void)
{
    int i = -1; //pour ne pas le trouver dans la lsite
    int j = -1; //pour ne pas le trouver dans la lsite
    int joueur = 0;
    tpl liste = creerVide();

    tpm morpion = initialiserMorpion();

    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    //on supprime la case jouée
    liste = supprimerElmt(i, j, liste);
    joueur = changerJoueur(joueur);
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
        jouerJoueur(i, j, joueur, morpion);
        //on supprime la case jouée
        liste = supprimerElmt(i, j, liste);
        liste = trouverCasesJouables(i, j, liste, morpion);
        joueur = changerJoueur(joueur);
        ++morpion->nbCoupsJoues;
    } while(!estGain(i, j, morpion) && !estFin(morpion));

    return 0;
}
