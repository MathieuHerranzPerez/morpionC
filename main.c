/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.3.2 / 08-01-2017
 * @file main.c
 */

#include <stdio.h>
#include "headers/morpion.h"
#include "headers/jeu.h"
#include "headers/ia.h"
#include "headers/gainFin.h"
#include "headers/joueur.h"


#ifdef __WIN32__
#include <windows.h>
#define Sleep(n) Sleep(n)

#else /*Le reste, UNIX en particulier*/
#include <unistd.h>
#include <ctype.h>

#define Sleep(n) usleep(n)
#endif

/**
 * trouver les coordonnees qui sont dans liste mais pas dans la liste finale
 * @param liste la liste
 * @param listeF la liste finale
 * @return la case jouee
 */
static tpl trouverCaseJouee(tpl liste, tpl listeF)
{
    if(listeF != creerVide())
    {
        while (liste != creerVide() && rechercherElmt(teteListeI(liste), teteListeJ(liste), listeF))
        {
            liste = queueListe(liste);
        }
    }
    return liste;
}

/**
 * lance le programme du joueur contre joueur
 */
static void jouerJvJ()
{
    int i = -1; //pour ne pas le trouver dans la liste
    int j = -1; //pour ne pas le trouver dans la liste
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
        printf("\033[H\033[J");     //clear terminal
        afficherMorpion(morpion);
        afficherJoueurActuel(joueur);

        //on saisie i et j
        while(rechercherElmt(i, j, liste) == NULL)
        {
            printf("Entrez ligne\n>");
            scanf("%d", &i);
            fflush(stdin);
            printf("Entrez colonne\n>");

            scanf("%d", &j);
            fflush(stdin);
            if(!estCaseSaisieJouable(i, j, liste))
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

/**
 * lance le programme joueur contre IA
 */
static void jouerJvIA()
{
    int i = -1; //pour ne pas le trouver dans la liste
    int j = -1; //pour ne pas le trouver dans la liste
    int joueur = 1;
    tpl listeTmp = creerVide();
    tpl coordListe; // pour chercher la case jouée par l'IA
    tpl liste = creerVide();

    choisirDifficulte();
    choisirIAenJvIA();
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
            while (rechercherElmt(i, j, liste) == NULL)
            {
                printf("Entrez ligne\n>");
                scanf("%d", &i);
                fflush(stdin);
                printf("Entrez colonne\n>");
                scanf("%d", &j);
                fflush(stdin);
                if (!estCaseSaisieJouable(i, j, liste))
                {
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
            liste = jouerIA(morpion, joueur, 1);

            coordListe = trouverCaseJouee(listeTmp, liste); // pour sortir en cas de victoire de l'IA
            if(coordListe != creerVide())
            {
                i = teteListeI(coordListe);
                j = teteListeJ(coordListe);
            }
            printf("\033[H\033[J");     //clear terminal
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

/**
 * lance le programme IA contre IA
 */
static void jouerIAvIA()
{
    int i = -1; //pour ne pas le trouver dans la liste
    int j = -1; //pour ne pas le trouver dans la liste
    int joueur = 1;
    tpl listeTmp;
    tpl coordListe; // pour chercher la case jouée par l'IA
    tpl liste = creerVide();

    choisirDifficulte();
    choisirIAenIAvIA();
    tpm morpion = initialiserMorpion();

    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    joueur = changerJoueur(joueur);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    do
    {
        listeTmp = copierListe(liste); //pour trouver l'element joué par l'IA
        liste = supprimerListe(liste);
        liste = jouerIA(morpion, joueur, 0);

        coordListe = trouverCaseJouee(listeTmp, liste); // pour sortir en cas de victoire de l'IA
        if(coordListe != creerVide())
        {
            i = teteListeI(coordListe);
            j = teteListeJ(coordListe);
        }

        // clear terminal
        printf("\033[H\033[J");
        printf("i jouee par IA : %d\n", i);
        printf("j jouee par IA : %d\n", j);

        supprimerListe(listeTmp);
        joueur = changerJoueur(joueur);
        ++morpion->nbCoupsJoues;

        afficherMorpion(morpion);
        sleep(1);
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
    int sortir = 0;

    while(sortir == 0)
    {
        while (choix <= 0 || choix > 4)
        {
            printf("========= MORPION =========\n\n");
            printf("Voulez-vous jouer en\n - (1) J vs J\n - (2) J vs IA\n - (3) IA vs IA, et donc ne pas jouer ...\n"
                           " (4) Ou lire les regles du jeu ?\n> ");
            scanf("%d", &choix);

            fflush(stdin);
            printf("%d\n", choix);
            if (choix == 4) {
                afficherRegles();
                choix = 0;
            }
        }
        switch (choix) {
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
        choix = 0;
        printf("Partie finie, voulez-vous quitter (0 / 1) ?\n>");
        scanf("%d", & sortir);
    }
    return 0;
}
