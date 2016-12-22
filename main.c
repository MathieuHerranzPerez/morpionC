
#include <stdlib.h>
#include <stdio.h>
#include "fichierH/morpion.h"
#include "fichierH/liste.h"
#include "fichierH/jeu.h"
#include "fichierH/ia.h"
#include "fichierH/gainFin.h"
#include "fichierH/joueur.h"


#ifdef __WIN32__
#include <windows.h>
#define Sleep(n) Sleep(n)

#else /*Le reste, UNIX en particulier*/
#include <unistd.h>
#define Sleep(n) usleep(n)
#endif


static tpl trouverCaseJouee(tpl liste, tpl listeF)
{
    if(listeF != creerVide())
    {
        while (liste != creerVide() && rechercherElmt(teteListeI(liste), teteListeJ(liste), listeF)) {
            liste = queueListe(liste);
        }
    }
    return liste;
}

static void jouerJvJ()
{
    int i = -1; //pour ne pas le trouver dans la lsite
    int j = -1; //pour ne pas le trouver dans la lsite
    int joueur = 0;
    tpl liste = creerVide();

    tpm morpion = initialiserMorpion();

    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    joueur = changerJoueur(joueur);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    printf("LE JOUEUR 1 COMMENCE, BONNE CHANCE\n");
    do
    {
        i = -1;
        j = -1;
        afficherMorpion(morpion);
        afficherJoueurActuel(joueur);
        //on saisie i et j
        while(rechercherElmt(i, j, liste) == NULL)
        {
            printf("Entrez ligne\n>");
            scanf("%d", &i);
            printf("Entrez colonne\n>");
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

    if(estGain(i, j, morpion))
    {
        joueur = changerJoueur(joueur);
        printf("LE JOUEUR %d A GAGNE !\n", joueur);
    }
    else if(estFin(morpion))
    {
        printf("EGALITE\n");
    }
}

static void jouerJvIA()
{
    int i = -1; //pour ne pas le trouver dans la lsite
    int j = -1; //pour ne pas le trouver dans la lsite
    int joueur = 1;
    tpl listeTmp = creerVide();
    tpl coordListe = creerVide(); // pour chercher la case jouée par l'IA
    tpl liste = creerVide();

    int seriesX, seriesO;

    choisirDifficulte();
    tpm morpion = initialiserMorpion();

    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    joueur = changerJoueur(joueur);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    do
    {
        if(joueur == 0)
        {
            i = -1;
            j = -1;
            afficherMorpion(morpion);
            afficherJoueurActuel(joueur);
            //on saisie i et j
            while (rechercherElmt(i, j, liste) == NULL) {
                printf("Entrez ligne\n>");
                scanf("%d", &i);
                printf("Entrez colonne\n>");
                scanf("%d", &j);
                if (!verifierCaseSaisieJouable(i, j, liste)) {
                    printf("Mauvaises coordonnées, veuillez en entrer de nouvelles.\n");
                    afficherListe(liste);
                }
            }
            jouerJoueur(i, j, joueur, morpion);

            liste = supprimerElmt(i, j, liste);
            listeTmp = copierListe(liste); //pour trouver l'element joué par l'IA

            liste = supprimerListe(liste);
        }
        else
        {
            afficherListe(listeTmp); // affichage test

            liste = jouerIA(morpion, joueur);

            coordListe = trouverCaseJouee(listeTmp, liste); // pour sortir en cas de victoire de l'IA
            if(coordListe != creerVide())
            {
                i = teteListeI(coordListe);
                j = teteListeJ(coordListe);
            }
            printf("i jouee par IA : %d\n", i);
            printf("j jouee par IA : %d\n", j);

            supprimerListe(listeTmp);
        }
        joueur = changerJoueur(joueur);
        ++morpion->nbCoupsJoues;

    } while(!estGain(i, j, morpion) && !estFin(morpion));
    if(estGain(i, j, morpion))
    {
        joueur = changerJoueur(joueur);
        if(joueur == 0)
            printf("VOUS AVEZ GAGNE\n");
        else
            printf("VOUS AVEZ PERDU, ESSAYEZ PEUT-ETRE AVEC UNE DIFFICULTE MOINDRE\n");
    }
    else if(estFin(morpion))
    {
        printf("EGALITE\n");
    }
}

static void jouerIAvIA()
{
    int i = -1; //pour ne pas le trouver dans la lsite
    int j = -1; //pour ne pas le trouver dans la lsite
    int joueur = 1;
    tpl listeTmp = creerVide();
    tpl coordListe = creerVide(); // pour chercher la case jouée par l'IA
    tpl liste = creerVide();

    int seriesX, seriesO;

    choisirDifficulte();
    tpm morpion = initialiserMorpion();

    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    joueur = changerJoueur(joueur);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    do
    {
        listeTmp = copierListe(liste); //pour trouver l'element joué par l'IA
        liste = supprimerListe(liste);
        liste = jouerIA(morpion, joueur);

        coordListe = trouverCaseJouee(listeTmp, liste); // pour sortir en cas de victoire de l'IA
        if(coordListe != creerVide())
        {
            i = teteListeI(coordListe);
            j = teteListeJ(coordListe);
        }
        printf("i jouee par IA : %d\n", i);
        printf("j jouee par IA : %d\n", j);

        supprimerListe(listeTmp);
        joueur = changerJoueur(joueur);
        ++morpion->nbCoupsJoues;

        printf("------JEU------\n");
        afficherMorpion(morpion);
        printf("----FIN JEU----\n");
        Sleep(1000);
    } while(!estGain(i, j, morpion) && !estFin(morpion));
    if(estGain(i, j, morpion))
    {
        joueur = changerJoueur(joueur);
        printf("LE JOUEUR %d A GAGNE !\n", joueur);
    }
    else if(estFin(morpion))
    {
        printf("EGALITE\n");
    }
}

int main(void)
{
    int choix = 0;
    while(choix <= 0 || choix > 3)
    {
        printf("Voulez-vous jouer\n - (1) en J vs J\n - (2) en J vs IA\n - (3) en IA vs IA, et donc ne pas jouer ...\n> ");
        scanf("%d", &choix);
    }
    switch(choix)
    {
        case 1 :
            jouerJvJ();
            break;

        case 2 :
            jouerJvIA();
            break;

        case 3 :
            jouerIAvIA();
            break;
        default :
            break;
    }
    return 0;
}
