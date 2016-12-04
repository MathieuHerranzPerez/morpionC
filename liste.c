/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 *
 * @version 0.1.0 / 04-11-2016
 * @file liste.c
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include "liste.h"


/**
 * @brief creer une liste null
 * @return NULL
 */
tpl creerVide(void)
{
    return (tpl)NULL;
}

/**
 * @brief renvoie 1 si la liste liste est vide
 * @param liste
 * @return boolean
 */
int estVide(tpl liste)
{
    return liste == creerVide();       //NULL
}

/**
 * @brief ajout un maillon à la liste
 * @param valeur la valeur du maillon à ajouter
 * @param liste la liste où ajouter le maillon
 * @return tpl la nouvelle liste
 */
tpl ajoutListe(int i, int j, tpl liste)
{
    tpl newListe;
    newListe = (tpl)malloc(sizeof(t_liste));
    newListe->i = i;
    newListe->j = j;
    newListe->suiv = liste;
    return newListe;
}

/**
 * @brief renvoie la valeur i du premier maillon de la liste
 * @param liste la maillon
 * @return int la valeur du maillon
 */
int teteListeI(tpl liste)
{
    assert(liste);
    return(liste->i);
}

/**
 * @brief renvoie la valeur j du premier maillon de la liste
 * @param liste la maillon
 * @return int la valeur du maillon
 */
int teteListeJ(tpl liste)
{
    assert(liste);
    return(liste->j);
}

/**
 * @brief renvoie le maillon suivant de la liste
 * @param liste
 * @return tpl le maillon suivant
 */
tpl queueListe(tpl liste)
{
    assert(liste);
    return liste->suiv;
}

/**
 * @brief demande à l'utilisateur de saisir des valeurs,
 * et creer une liste à partir de celles-ci
 * @return tpl la nouvelle liste
 */
tpl creerListe()
{
    int i;
    int j;
    i = 0;
    j = 0;
    tpl liste = creerVide();

    do{
        printf("saisir nouvelle valeur i : ");
        scanf("%d", & i);
        printf("saisir nouvelle valeur j : ");
        scanf("%d", & j);
        if(i >= 0)
            liste = ajoutListe(i, j, liste);
    } while(i >= 0);

    return liste;
}

/**
 * @brief affiche une liste passé en param
 * @param liste la liste à afficher
 */
void afficherListe(tpl liste)
{
    printf("(");
    while(!estVide(liste))
    {
        printf("%d ", teteListeI(liste));
        printf("%d", teteListeJ(liste));
        liste = liste->suiv;
        if(!estVide(liste))
            printf(", ");
    }
    printf(")\n");
}

/**
 * @brief compte la longueur d'une liste (recursif)
 * @param liste dont on souhaite connaitre la longueur
 * @return int la taille de la liste
 */
int compterLongueur(tpl liste)
{
    if(estVide(liste))
        return 0;
    else
        return 1 + (compterLongueur(queueListe(liste)));
}

/**
 * @brief copie la liste passée en param
 * @param liste
 * @return tpl la nouvelle liste
 */
tpl copierListe(tpl liste)
{
    if(estVide(liste))
        return liste;
    else
        return ajoutListe(teteListeI(liste), teteListeJ(liste), copierListe(queueListe(liste)));
}

/**
 * @brief recherche l'element passé en param et renvoie un pointeur sur cet element (null sinon)
 * @param X l'element à chercher
 * @param liste liste où chercher X
 * @return tpl la sous-liste contenant l'element X
 */
tpl rechercherElmt(int i, int j, tpl liste)
{
    while(!estVide(liste) && (teteListeI(liste) != i
          || teteListeJ(liste) != j))
    {
        liste = queueListe(liste);
    }
    return liste;
}

/**
 * @brief cherche et supprime les elements contenant la valeur passée en param
 * @param X la valeur à supprimer de la liste L
 * @param L liste dans laquelle on souhaite supprimer un element
 * @return tpl la nouvelle liste sans l'element supprimé
 */
tpl supprimerElmt(int i, int j, tpl liste)
{
    tpl listeTmp = liste;
    tpl tmpListe = liste;
    while(liste != creerVide() && teteListeI(liste) == i && teteListeJ(liste) == j)   // on traite le premier à part
    {
        tmpListe = queueListe(liste);
        free(liste);
        liste = tmpListe;
    }

    while(queueListe(liste) != creerVide())
    {
        if(teteListeI(queueListe(liste)) == i &&
            teteListeJ(queueListe(liste)) == j)
        {
            listeTmp = queueListe(liste);
            liste->suiv = queueListe(queueListe(liste));
            free(listeTmp);
        }
        else
            liste = queueListe(liste);
    }
    return tmpListe;
}

/**
 * @brief recopie une liste à l'envers
 * @param liste liste à recopier
 * @return tpl la liste recopiée
 */
tpl recopierInverse(tpl liste)
{
    tpl newListe = creerVide();
    while(!estVide(liste))
    {
        newListe = ajoutListe(teteListeI(liste), teteListeJ(liste), newListe);
        liste = queueListe(liste);
    }
    return newListe;
}
