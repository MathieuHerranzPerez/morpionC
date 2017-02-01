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
#include "headers/sauvegarde.h"


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
        //des qu'une coordonnée n'est pas trouvée ou qu'on est en fin de liste, on sort
        while (liste != creerVide() && rechercherElmt(teteListeI(liste), teteListeJ(liste), listeF))
        {
            liste = queueListe(liste);
        }
    }
    return liste;
}

/**
 * Boucle principale du JvJ. On passe le morpion la liste et le joueur initialisés
 * @param morpion
 * @param liste
 * @param joueur
 */
static void boucleJvJ(tpm morpion, tpl liste, int joueur)
{
    int i, j;
    do
    {
        i = -1;
        j = -1;

        //clear terminal
        printf("\033[H\033[J");
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

            //saisir 20 pour sauver
            if(i == 20 || j == 20)
            {
                sauvegarder(morpion, 1, 0, 0);
            }

                //saisir 18 pour de l'aide
            else if(i == 18 || j == 18)
            {
                aiderJoueur(morpion, joueur, 1);
            }

                //on verifie que la case saisie est bien dispo
            else if(!estCaseSaisieJouable(i, j, liste))
            {
                printf("Mauvaises coordonnees, veuillez en entrer de nouvelles.\n");
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

    //si quelqu'un a gagné
    if(estGain(i, j, morpion))
    {
        joueur = changerJoueur(joueur);
        printf("LE JOUEUR %d A GAGNE !\n", joueur);
    }
        //sinon si il y a égalité
    else if(estFin(morpion))
    {
        printf("EGALITE\n");
    }
}

/**
 * Boucle principale du JvIA. On passe le morpion la liste et le joueur initialisés
 * @param morpion
 * @param liste
 * @param joueur
 */
static void boucleJvIA(tpm morpion, tpl liste, int joueur)
{
    tpl listeTmp = creerVide();

    // pour chercher la case jouée par l'IA
    tpl coordListe;
    int i = -1;
    int j = -1;

    do
    {
        // c'est au joueur de jouer (humain)
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

                // pour sauver
                if(i == 20 || j == 20)
                {
                    sauvegarder(morpion, 2, getProfondeur(), getFonctionEval1());
                }
                    // pour demander de l'aide
                else if(i == 18 || j == 18)
                {
                    aiderJoueur(morpion, joueur, 1);
                }
                    // on verifie si la case est jouable
                else if(!estCaseSaisieJouable(i, j, liste))
                {
                    printf("Mauvaises coordonnees, veuillez en entrer de nouvelles.\n");
                    afficherListe(liste);
                }
            }

            //on joue le coup saisi
            jouerJoueur(i, j, joueur, morpion);

            liste = supprimerElmt(i, j, liste);
            listeTmp = copierListe(liste); //pour trouver l'element joué par l'IA

            liste = supprimerListe(liste);
        }

            // c'est à l'IA de jouer
        else
        {
            liste = jouerIA(morpion, joueur, 1);

            coordListe = trouverCaseJouee(listeTmp, liste); // pour sortir en cas de victoire de l'IA
            if(coordListe != creerVide())
            {
                i = teteListeI(coordListe);
                j = teteListeJ(coordListe);
            }

            //clear terminal
            printf("\033[H\033[J");
            printf("i jouee par IA : %d\n", i);
            printf("j jouee par IA : %d\n", j);

            supprimerListe(listeTmp);
        }
        joueur = changerJoueur(joueur);
        ++morpion->nbCoupsJoues;

    } while(!estGain(i, j, morpion) && !estFin(morpion));

    // si il y a un gagnant
    if(estGain(i, j, morpion))
    {
        joueur = changerJoueur(joueur);
        afficherMorpion(morpion);
        if(joueur == 0)
        {
            printf("VOUS AVEZ GAGNE\n");
        }
        else
        {
            printf("VOUS AVEZ PERDU, ESSAYEZ PEUT-ETRE AVEC UNE DIFFICULTE MOINDRE\n");
        }
    }
        // sinon si il y a égalité
    else if(estFin(morpion))
    {
        printf("EGALITE\n");
    }
}

/**
 * lance le programme du joueur contre joueur
 */
static void jouerJvJ()
{
    // On initialise
    int joueur = 0;
    tpl liste = creerVide();

    tpm morpion = initialiserMorpion();

    //on joue le premier coup au milieu (cachier des charges ...)
    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    joueur = changerJoueur(joueur);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    printf("LE JOUEUR 1 COMMENCE, BONNE CHANCE, "
                   "- vous pouvez sauver la partie en tapant comme ligne ou colonne '20'.\n"
                   "toute nouvelle sauvegarde ecrasera l'ancienne\n"
                   "- vous pouvez egalement demander de l'aide en tapant '18'\n\n");

    // On lance la boucle principale
    boucleJvJ(morpion, liste, joueur);
}

/**
 * lance le programme joueur contre IA
 */
static void jouerJvIA()
{
    // On initialise
    int joueur = 1;
    tpl liste = creerVide();

    choisirDifficulte();
    choisirIAenJvIA();
    tpm morpion = initialiserMorpion();

    // On joue le premier coup au milieu (cachier des charges ...)
    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion); // premier coup au milieu
    joueur = changerJoueur(joueur);
    liste = trouverCasesJouables(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, liste, morpion);

    printf("L'ORDINATEUR COMMENCE, BONNE CHANCE, "
                   "- vous pouvez sauver la partie en tapant comme ligne ou colonne '20'.\n"
                   "toute nouvelle sauvegarde ecrasera l'ancienne\n"
                   "- vous pouvez egalement demander de l'aide en tapant '18'\n\n");

    // On lance la boucle principale
    boucleJvIA(morpion, liste, joueur);
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

    // On joue le premier coup au milieu (cachier des charges ...)
    jouerJoueur(getTailleMorpion(morpion)/2, getTailleMorpion(morpion)/2, joueur, morpion);
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
        sleep(2);
    } while(!estGain(i, j, morpion) && !estFin(morpion));

    // Si il y a un gagnant
    if(estGain(i, j, morpion))
    {
        afficherMorpion(morpion);
        joueur = changerJoueur(joueur);
        printf("LE JOUEUR %d A GAGNE !\n", joueur);
    }
        // Sinon si il y a égalité
    else if(estFin(morpion))
    {
        printf("EGALITE\n");
    }
}

/**
 * On cherche les cases adjacentes aux autres en regardant toutes les cases
 * (moins rapide que trouverCasesJouables,
 * mais plus portable : on a pas besoin de connaitre la derniere case jouee)
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

static void jouerJvJRestaure(tpm morpion, int joueur)
{
    // On initialise
    tpl liste = trouverJouables(morpion);

    // On lance la boucle principale
    boucleJvJ(morpion, liste, joueur);
}

static void jouerJvIARestaure(tpm morpion, int difficulte, int numIA)
{
    // On initialise
    int joueur = 0;
    tpl liste = trouverJouables(morpion);
    setProfondeur(difficulte);
    setFonctionEval1(numIA);

    // On lance la boucle principale
    boucleJvIA(morpion, liste, joueur);
}

/**
 * Traite les differentes sauvegardes aux quelles on peut etre confronte (JvJ ou JvIA)
 */
static void jouerRestauration()
{
    int mode = 0;
    int difficulte = 0;
    int numIA = 0;
    int joueur;
    tpm morpion;

    morpion = restaurerSauvegarde(& mode, & difficulte, & numIA, & joueur);

    switch(mode)
    {
        case 1 :
            jouerJvJRestaure(morpion, joueur);
            break;

        case 2 :
            jouerJvIARestaure(morpion, difficulte, numIA);
            break;

        default :
            printf("une erreur s'est produite\n");
    }
}

int main(void)
{
    int choix = 0;
    int sortir = 0;
    int restaurer = 0;

    // On recommence tant que l'utilisteur ne saisie pas 0 quand demande
    while(sortir == 0)
    {
        // Si le fichier sauvegarde.txt n'est pas vide, on demande si on veut restaurer
        if(!estFichierSauvegardeVide())
        {
            printf("Voulez-vous restaurer la sauvegarde ?(0 / 1)\n>");
            scanf("%d", & restaurer);
            if(restaurer != 0)
            {
                jouerRestauration();
            }
        }
        // On demande a l'utilisateur ce qu'il veut faire
        while (choix <= 0 || choix > 4)
        {
            printf("Voulez-vous jouer\n - (1) en J vs J\n - (2) en J vs IA\n - (3) en IA vs IA, et donc ne pas jouer ...\n"
                           " (4) Ou lire les regles du jeu ?\n> ");
            scanf("%d", &choix);

            fflush(stdin);
            printf("%d\n", choix);
            if (choix == 4)
            {
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