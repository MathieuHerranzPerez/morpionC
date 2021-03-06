/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.9.2 / 01-02-2017
 * @file ia.c
 */



#include "headers/liste.h"
#include "headers/ia.h"
#include "headers/joueur.h"
#include "headers/jeu.h"
#include "headers/gainFin.h"



static int profondeur = 0;
static int joueurCourant = -1;
static int fonctionEval1;   // la fonction d'évaluation pour JvIA et IAvIA
static int fonctionEval2;   // l'autre fonction d'évaluation pour IAvIA

/**
 * @return Retourne la profondeur de l'arbre de l'IA
 */
int getProfondeur()
{
    return profondeur;
}

/**
 * @return Retourne le numero de la fonctionEval1
 */
int getFonctionEval1()
{
    return fonctionEval1;
}

/**
 * Modifie la valeur de la profondeur
 * @param prof la nouvelle profondeur
 */
void setProfondeur(int prof)
{
    profondeur = prof;
}

/**
 * Modifie la valeur du numéro de la fonctionEval1
 * @param fctEval
 */
void setFonctionEval1(int fctEval)
{
    fonctionEval1 = fctEval;
}

/**
 * Permet à l'utilisateur de selectionner une difficulte d'IA
 */
void choisirDifficulte()
{
    int prof = 0;

    // Tant qu'on a saisi n'importe quoi
    while(prof < 1 || prof > 5) {
        printf("Veuillez choisir une difficulte : \n"
                       "- (1) bac a sable\n"
                       "- (2) facile\n"
                       "- (3) normal (long)\n"
                       "- (4) difficile (deconseille, vraiment long)\n"
                       "- (5) tres difficile (deconseille, vraiment tres tres tres tres (trop) long)\n"
                       ">");
        scanf("%d", & prof);
    }
    setProfondeur(prof);
}


/**
 * Vérifie grâce à nbSeriesAlign si il y a un gagnant
 * @param morpion
 * @return 1, -1 ou 0 suivant le joueur courant et si le joueur courant gagne ou perd
 */
static int estGainIA(tpm morpion)
{
    int seriesJ1 = 0;
    int seriesJ2 = 0;

    // Met dans seriesJ1 et seriesJ2 le nombre de séries de 5 symboles alignés des joueurs
    nbSeriesAlign(morpion, & seriesJ1, & seriesJ2, 5, 5);

    if(joueurCourant == 1 && seriesJ1)
    {
        return 1;
    }
    else if(joueurCourant == 1 && seriesJ2)
    {
        return -1;
    }
    else if(joueurCourant == 0 && seriesJ1)
    {
        return -1;
    }
    else if(joueurCourant == 0 && seriesJ2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Trouve dans le morpion les cases jouables et renvois la liste des coups.
 * fonction plus lente que trouverCasesJouables(), mais n'oblige pas à une copie de liste
 * @param morpion
 * @return la liste
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

/**
 * Fonction d'évaluation faible
 * @param morpion
 * @return un poids
 */
static int evalF(tpm morpion)
{
    int seriesJ1 = 0;
    int seriesJ2 = 0;
    int pointsJ1 = 0;
    int pointsJ2 = 0;
    int gagnant;

    gagnant = estGainIA(morpion);

    if(gagnant)
    {
        // Si l'IA perd
        if(gagnant == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
            // Si l'IA gagne
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    // Si il y a égalité
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

    if (gagnant != 0)
    {
        // Si l'IA perd
        if (gagnant == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
            // Si l'iA gagne
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    // Si egalite
    if (estFin(morpion))
    {
        return 0;
    }

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
        // On maximise car offensive
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }
    else
    {
        // On minimise car offensive
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);

    if (joueurCourant == 1)
    {
        // On maximise car offensive
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }
    else
    {
        // On minimise car offensive
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }

    if(joueurCourant == 1)
    {
        return pointsJ1 - pointsJ2;
    }
    else
    {
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

    if (gagnant != 0)
    {
        if (gagnant == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if (estFin(morpion))
    {
        return 0;
    }

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
        // On minimise car defensive
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }
    else
    {
        // On maximise car defensive
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);

    if (joueurCourant == 1)
    {
        // On minimise car defensive
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }
    else
    {
        // On maximise car defensive
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }

    if(joueurCourant == 1)
    {
        return pointsJ1 - pointsJ2;
    }
    else
    {
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

    if (gagnant != 0)
    {
        if (gagnant == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    // Si egalite
    if (estFin(morpion))
    {
        return 0;
    }

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
        // On maximise car offensive
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }
    else
    {
        // On minimise car offensive
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);

    if (joueurCourant == 1)
    {
        // On maximise car offensive
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }
    else
    {
        // On minimise car offensive
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }
//////---------------------------
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 3, 6);

    if(joueurCourant == 1)
    {
        // On maximise car offensive
        pointsJ1 += seriesDangJ1 * 25;
        pointsJ2 += seriesDangJ2 * 15;
    }
    else
    {
        // On minimise car offensive
        pointsJ1 += seriesDangJ1 * 15;
        pointsJ2 += seriesDangJ2 * 25;
    }
    // pour ia (1) bac à sable
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 4, 6);

    if(joueurCourant == 1)
    {
        // On maximise car offensive
        pointsJ1 += seriesDangJ1 * 30;
        pointsJ2 += seriesDangJ2 * 20;
    }
    else
    {
        // On minimise car offensive
        pointsJ1 += seriesDangJ1 * 20;
        pointsJ2 += seriesDangJ2 * 30;
    }
//////----------------------------

    if(joueurCourant == 1)
    {
        return pointsJ1 - pointsJ2;
    }
    else
    {
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

    if (gagnant != 0)
    {
        if (gagnant == -1)
        {
            return -1000 + morpion->nbCoupsJoues;
        }
        else
        {
            return 1000 - morpion->nbCoupsJoues;
        }
    }

    //si egalite
    if (estFin(morpion))
    {
        return 0;
    }

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
        // On minimise car defensive
        pointsJ1 += seriesJ1 * 4;
        pointsJ2 += seriesJ2 * 5;
    }
    else
    {
        // On maximise car defensive
        pointsJ1 += seriesJ1 * 5;
        pointsJ2 += seriesJ2 * 4;
    }

    //On compte le nombre de séries de 4 pions alignés de chacun des joueurs
    seriesJ1 = 0;
    seriesJ2 = 0;
    rechercherSeriesSucc(4, & seriesJ1, & seriesJ2, morpion);

    if (joueurCourant == 1)
    {
        // On minimise car defensive
        pointsJ1 += seriesJ1 * 10;
        pointsJ2 += seriesJ2 * 12;
    }
    else
    {
        // On minimise car defensive
        pointsJ1 += seriesJ1 * 12;
        pointsJ2 += seriesJ2 * 10;
    }
//////---------------------------
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 3, 6);

    if(joueurCourant == 1)
    {
        // On minimise car defensive
        pointsJ1 += seriesDangJ1 * 15;
        pointsJ2 += seriesDangJ2 * 25;
    }
    else
    {
        // On maximise car defensive
        pointsJ1 += seriesDangJ1 * 25;
        pointsJ2 += seriesDangJ2 * 15;
    }
    // pour ia (1) bac à sable
    nbSeriesAlign(morpion, & seriesDangJ1, & seriesDangJ2, 4, 6);

    if(joueurCourant == 1)
    {
        // On minimise car defensive
        pointsJ1 += seriesDangJ1 * 20;
        pointsJ2 += seriesDangJ2 * 30;
    }
    else
    {
        // On maximise car defensive
        pointsJ1 += seriesDangJ1 * 30;
        pointsJ2 += seriesDangJ2 * 20;
    }
//////----------------------------

    if(joueurCourant == 1)
    {
        return pointsJ1 - pointsJ2;
    }
    else
    {
        return pointsJ2 - pointsJ1;
    }
}

/**
 * Fonction d'évaluation très basique
 * Vérifie si vainqueur ou si trois symboles alignés
 * @param morpion
 * @return un poids
 */
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
        else    // jamais
        {
            return 0;
        }
    }
    if(joueurCourant == 1)
    {
        nbSeriesAlign(morpion, &seriesX, &seriesO, 3, 5);
    }
    else
    {
        nbSeriesAlign(morpion, &seriesO, &seriesX, 3, 5);
    }

    return seriesX - seriesO;
}

/**
 * Lance la bonne fonction d'évaluation
 * @param evaluer la fonction d'évaluation
 * @param morpion
 * @return le poids renvoyé par la fonction d'évaluation
 */
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
                return lancerEval(evalFD, morpion);

            case 2:
                return lancerEval(evalFO, morpion);

            case 3:
                return lancerEval(evalMD, morpion);

            case 4:
                return lancerEval(evalMO, morpion);

            case 5:
                return lancerEval(evalF, morpion);

            case 6:
                return lancerEval(evalBacASable, morpion);

            default:
                return lancerEval(evalFD, morpion);
        }
    }
        // Pour la deuxième IA de IAvsIA
    else
    {
        switch(fonctionEval2)
        {
            case 1:
                return lancerEval(evalFD, morpion);

            case 2:
                return lancerEval(evalFO, morpion);

            case 3:
                return lancerEval(evalMD, morpion);

            case 4:
                return lancerEval(evalMO, morpion);

            case 5:
                return lancerEval(evalF, morpion);

            case 6:
                return lancerEval(evalBacASable, morpion);

            default:
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
    int poidsM;
    int i, j, tmp;
    int estFctMax;  // pour l'IA vs IA, on a besoin de retenir qui appelle la fonction
    tpl listeTmp;
    tpl liste;

    int random; //  pour ne pas avoir tout le temps la même partie

    if(estMax)
    {
        poidsM = -1000;
    }
    else
    {
        poidsM = 1000;
    }

    if(profondeur == 0 || estGainIA(morpion) || estFin(morpion))
    {
        return eval(morpion);
    }

    liste = trouverJouables(morpion);
    listeTmp = liste;
    joueur = changerJoueur(joueur);

    if(estMax)
    {
        estFctMax = 0;
    }
    else
    {
        estFctMax = 1;
    }

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);

        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        tmp = minMax(morpion, profondeur-1, estFctMax, joueur);

        if(estMax)
        {
            if (tmp >= poidsM)
            {
                if (tmp == poidsM)      //pour ne pas avoir tout le temps la même partie
                {
                    random = rand() % 2;
                    if(random)
                    {
                        poidsM = tmp;
                    }
                }
                else
                {
                    poidsM = tmp;
                }
            }
        }
        else
        {
            if (tmp <= poidsM)
            {
                if (tmp == poidsM)      //pour ne pas avoir tout le temps la même partie
                {
                    random = rand() % 2;
                    if(random)
                    {
                        poidsM = tmp;
                    }
                }
                else
                {
                    poidsM = tmp;
                }
            }
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
    }
    listeTmp = supprimerListe(listeTmp);
    free(listeTmp);
    free(liste);

    return poidsM;
}

/**
 * Message qui demande de patienter (appelé lorsque l'IA "calcule")
 */
static void afficherPatienter(int cpt)
{
    if(cpt % 2 == 0)        // il est pair
    {
        if (cpt % 8 == 0)
        {
            printf("\rveuillez patienter        ");
            fflush(stdout);
        } else if (cpt % 8 == 2)
        {
            printf("\rveuillez patienter .      ");
            fflush(stdout);
        } else if (cpt % 8 == 4)
        {
            printf("\rveuillez patienter ..     ");
            fflush(stdout);
        } else if (cpt % 8 == 6)
        {
            printf("\rveuillez patienter ...    ");
            fflush(stdout);
        }
    }
}

/**
 * Change la valeur de fonctionEval1
 * @param numIA la nouvelle valeur
 */
static void setIA1(int numIA)
{
    fonctionEval1 = numIA;
}

/**
 * Change la valeur de fonctionEval2
 * @param numIA la nouvelle valeur
 */
static void setIA2(int numIA)
{
    fonctionEval2 = numIA;
}

/**
 * Permet à l'utilisateur de selectionner une complexite d'IA
 */
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

/**
 * Permet à l'utilisateur de selectionner une complexite d'IA
 */
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



/**
 * AlphaBeta, renvoie un poids suivant la fonction eval utilisée, correspondant au meilleur coup à jouer
 * @param morpion
 * @param profondeur
 * @param estBeta
 * @param joueur
 * @param alpha
 * @param beta
 * @return un poids
 */
static int alphaBeta(tpm morpion ,int profondeur, int estBeta, int joueur, int alpha, int beta)
{
    int i, j, tmp, meilleur;
    int estFctBeta;  // pour l'IA vs IA, on a besoin de retenir qui appelle la fonction
    tpl liste;

    if(profondeur == 0 || estGainIA(morpion) || estFin(morpion))
    {
        return eval(morpion);
    }

    liste = trouverJouables(morpion);
    joueur = changerJoueur(joueur);

    if(estBeta)
    {
        estFctBeta = 0;
        meilleur = -1000;
    }
    else
    {
        estFctBeta = 1;
        meilleur = 1000;
    }

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        tmp = alphaBeta(morpion, profondeur - 1, estFctBeta, joueur, alpha, beta);

        if(estBeta)
        {
            if(tmp > meilleur)
            {
                meilleur = tmp;

                if(meilleur > alpha)
                {
                    alpha = meilleur;
                    if(alpha >= beta)
                    {
                        // on remet la case testée à defaut
                        dejouer(i, j, morpion);
                        --morpion->nbCoupsJoues;

                        return meilleur;
                    }
                }
            }
        }
        else
        {
            if(tmp < meilleur)
            {
                meilleur = tmp;
                if (meilleur < beta)
                {
                    beta = meilleur;
                    if (alpha >= beta)
                    {
                        // on remet la case testée à defaut
                        dejouer(i, j, morpion);
                        --morpion->nbCoupsJoues;

                        return meilleur;
                    }
                }
            }
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
    }

    return meilleur;
}

/**
 * Permet a l'IA de jouer en fonction de l'etat du morpion, suivant le joueur actuel,
 * grace a un minMAx si estAlphaBeta est faux, avec un alphaBeta sinon
 * @param morpion le morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 * @return la liste apres que l'IA ai joue
 */
tpl jouerIA(tpm morpion, int joueur, int estAlphaBeta)
{
    int maxi = -10000;
    int tmp;
    int indI = 0, indJ = 0;
    int i, j;
    int cpt = 0;
    tpl liste;

    int random;

    liste = trouverJouables(morpion);
    tpl listeTmp = liste;

    joueurCourant = joueur;

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        afficherPatienter(cpt); // affichage
        ++cpt;
        if(estAlphaBeta)
        {
            tmp = alphaBeta(morpion, profondeur - 1, 0, joueur, -1000, 1000);
        }
        else
        {
            tmp = minMax(morpion, profondeur - 1, 0, joueur);
        }

        if(tmp >= maxi)
        {
            if(tmp == maxi)
            {
                random = rand() % 2;
                if(random)
                {
                    maxi = tmp;
                    indI = i;
                    indJ = j;
                }
            }
            else
            {
                maxi = tmp;
                indI = i;
                indJ = j;
            }
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
    }
    listeTmp = supprimerListe(listeTmp);

    //free(liste);
    free(listeTmp);
    jouerJoueur(indI, indJ, joueur, morpion);

    liste = trouverJouables(morpion);
    return liste;
}

/**
 * Indique une case pas trop mal à jouer en l'affichant
 * @param morpion le morpion
 * @param joueur (vaut 1 ou 0) determine si il faut jouer O ou X
 * @param estAlphaBeta (0 ou 1) si on veux utiliser un algo AlphaBeta ou minMax
 */
void aiderJoueur(tpm morpion, int joueur, int estAlphaBeta)
{
    int maxi = -10000;
    int tmp;
    int indI = 0, indJ = 0;
    int i, j;
    int cpt = 0;
    tpl liste;

    liste = trouverJouables(morpion);
    tpl listeTmp = liste;

    joueurCourant = joueur;

    while(liste != NULL)
    {
        i = teteListeI(liste);
        j = teteListeJ(liste);
        jouerJoueur(i, j, joueur, morpion);
        ++morpion->nbCoupsJoues;

        afficherPatienter(cpt); // affichage
        ++cpt;
        if(estAlphaBeta)
        {
            tmp = alphaBeta(morpion, 4, 0, joueur, -1000, 1000);
        }
        else
        {
            tmp = minMax(morpion, 4, 0, joueur);
        }

        if(tmp > maxi)
        {
            maxi = tmp;
            indI = i;
            indJ = j;
        }

        // on remet la case testée à defaut
        dejouer(i, j, morpion);
        --morpion->nbCoupsJoues;

        liste = queueListe(liste);
    }

    listeTmp = supprimerListe(listeTmp);
    //free(liste);
    free(listeTmp);

    printf("\nL'ordinateur vous suggere de jouer en (%d, %d)\n", indI, indJ);
}