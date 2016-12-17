#include "fichierH/liste.h"
#include "fichierH/ia.h"
#include "fichierH/joueur.h"
#include "fichierH/morpion.h"
#include "fichierH/jeu.h"
#include "fichierH/gainFin.h"

/**
 * Fonction plus lente que trouverCasesJouables(), mais n'oblige pas à une copie de liste
 * @param morpion
 * @return
 */
static tpl trouverJouables(tpm morpion)
{
    tpl liste = creerVide();
    int i, j, k, l;
    printf("%d\n", getTailleMorpion(morpion));
    for(i = 0; i < getTailleMorpion(morpion); ++i)
    {
        for(j = 0; j < getTailleMorpion(morpion); ++j)
        {
            if(morpion->morpion[i][j] != ' ')
            {
                for(k = i-1; k <= i+1; ++k)
                {
                    printf("k : %d\n", k);
                    for(l = j-1; l <= j+1; ++l)
                    {
                        printf("l : %d\n", k);
                        if(k >= 0 && k < getTailleMorpion(morpion) && l >= 0 && l < getTailleMorpion(morpion))
                        {
                            if(morpion->morpion[l][k] == ' ' && !rechercherElmt(l, k, liste))
                            {
                                liste = ajoutListe(l, k, liste);
                                afficherListe(liste);
                            }
                        }
                    }
                }
            }
        }
    }
    return liste;
}

//void IA_jouer(tpm morpion, int profondeur, tpl liste, int joueur)
//{
//    int maxi = -10000;
//    int tmp, indI, indJ;
//    int i, j;
//
//    while(liste != NULL)
//    {
//        i = teteListeI(liste);
//        j = teteListeJ(liste);
//        tpl listeCopie = copierListe(liste); // on copie la liste originale
//        jouerJoueur(i, j, joueur, morpion);
//        joueur = changerJoueur(joueur);
//        listeCopie = supprimerElmt(i, j, liste); // on supprime la case jouée dans la liste copiée
//        listeCopie = trouverCasesJouables(i, j, liste, morpion);    // on ajoute les nouvelles positions possibles
//
//
//        tmp = minMax(morpion, profondeur-1, 0, listeCopie, joueur);
//        if(tmp > maxi)
//        {
//             maxi = tmp;
//             indI = i;
//             indJ = j;
//        }
//        morpion->morpion[i][j] = ' ';
//
//        liste = queueListe(liste);
//        supprimerListe(listeCopie);
//    }
//    if(joueur == 0)
//        morpion->morpion[indI][indJ] = 'O';
//    else
//        morpion->morpion[indI][indJ] = 'X';
//}
//
//int minMax(tpm morpion ,int profondeur, int estMax, tpl liste, int joueur)
//{
//    int poidsM;
//    int i,j,tmp;
//    tpl listeCopie;
//    const tpl listeOriginale = liste;
//
//    if(estMax)
//        poidsM = -1000;
//    else
//        poidsM = 1000;
//
//
//    if(profondeur == 0 || /*gagnant(morpion) != 0 ||TODO*/ estFin(morpion))
//     {
//          return eval(morpion);
//     }
//
//     while(liste != NULL)
//     {
//        i = teteListeI(liste);
//        j = teteListeJ(liste);
//        listeCopie = copierListe(listeOriginale); // on copie la liste originale
//        jouerJoueur(i, j, joueur, morpion);
//        joueur = changerJoueur(joueur);
//        listeCopie = supprimerElmt(i, j, liste); // on supprime la case jouée dans la liste copiée
//        listeCopie = trouverCasesJouables(i, j, liste, morpion);    // on ajoute les nouvelles positions possibles
//
//         if(estMax)
//             estMax = 0;
//         else
//             estMax = 1;
//
//        tmp = minMax(morpion, profondeur-1, estMax, listeCopie, joueur);
//        if(estMax){
//            if(tmp > poidsM)
//            {
//                 poidsM = tmp;
//            }
//        }
//        else
//        {
//            if(tmp < poidsM)
//            {
//                 poidsM = tmp;
//            }
//        }
//        // on remet la case testée à defaut
//        dejouer(i, j, morpion);
//
//        liste = queueListe(liste);
//        supprimerListe(listeCopie);
//     }
//
//     return poidsM;
//}
//
//void compterSymbSucc(tpm morpion, int *nbCroixAlign, int *nbRondsAlign, int nbAlign)
//{
//    int i;
//    int j;
//    int nbCroix;
//    //horizontalement
//    for(i = 0; i < getTailleMorpion(morpion), ++i)
//    {
//        for(j = 0; j < getTailleMorpion(morpion), ++j)
//        {
//
//        }
//    }
//    //veticalement
//
//    //diag asc
//
//    //diag desc
//}
//
//static void dejouer(int i, int j, tpm morpion)
//{
//    morpion->morpion[i][j] = ' ';
//}
//
////TODO bloque
////int eval(tpm morpion)
////int gagnant(tpm morpion)
//// meilleurCoups(eval())
