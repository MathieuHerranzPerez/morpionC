/**
 * @author HERRANZ PEREZ Mathieu <mathieu.herranz-perez@etu.univ-amu.fr>
 * @author ALIE-SANDEVOIR Isis <isis.alie-sandevoir@etu.univ-amu.fr>
 *
 * @version 0.1.1 / 028-01-2017
 * @file sauvegarde.c
 */

#include "headers/morpion.h"

void sauvegarder(tpm morpion, int mode/*1 ou 2*/, int difficulte, int numIA)
{
    int i, j;
    FILE* fichier = NULL;

    fichier = fopen("../sauvegarde.txt", "w");

    if(fichier != NULL)
    {
        fprintf(fichier, "%d\n"
                "%d\n"
                "%d\n"
                "%d\n", mode, difficulte, numIA, morpion->taille);

        //ecrit la grille dans le fichier
        for(i = 0; i < morpion->taille; ++i)
        {
            for(j = 0; j < morpion->taille; ++j)
            {
                fprintf(fichier, "%c", morpion->morpion[i][j]);
            }
            fprintf(fichier, "\n");
        }
        printf("Sauvegarde terminee\n");
    }
    else
    {
        printf("erreur lors de la sauvegarde, fichier inexistant\n");
    }
    fclose(fichier);
}

tpm restaurerSauvegarde(int * mode, int * difficulte, int * numIA, int * joueur)
{
    int i, j;
    int taille;
    char** grille;
    int nbCoups = 0;

    char buffer[16];

    FILE* fichier = NULL;

    fichier = fopen("../sauvegarde.txt", "r");

    if(fichier != NULL)
    {
        fgets(buffer, 3, fichier);
        *mode = atoi(buffer);


        fgets(buffer, 3, fichier);
        *difficulte = atoi(buffer);

        fgets(buffer, 3, fichier);
        *numIA = atoi(buffer);

        fgets(buffer, 4, fichier);
        taille = atoi(buffer);


        //alloue l'esapce pour notre grille
        grille = (char**)malloc(taille * sizeof(char*));
        for(i = 0; i < taille; ++i)
        {
            grille[i] = (char*)malloc(taille * sizeof(char));
        }

        //on remplie la grille
        for(i = 0; i < taille; ++i)
        {
            fgets(buffer, taille+1, fichier);
            for(j = 0; j < taille; ++j)
            {
                grille[i][j] = buffer[j];
                if(grille[i][j] != ' ')
                {
                    ++nbCoups;
                }
            }
            fgets(buffer, 2, fichier); // pour sauter le \n\0
        }

        fclose(fichier);

        if(nbCoups % 2 == 0)
            *joueur = 0;
        else
            *joueur = 1;
        return initialiserMorpionRestauration(taille, grille, nbCoups);
    }
    else
    {
        printf("restauration echouee\n");
        return initialiserMorpion();
    }
}

int estFichierSauvegardeVide()
{
    FILE* fichier = NULL;
    int caractere;

    fichier = fopen("../sauvegarde.txt", "r");
    if(fichier != NULL)
    {
        caractere = fgetc(fichier);
        if(caractere != EOF)
            return 0;
        else
            return 1;
    }
    else
    {
        return 1;
    }
}

void supprimerFichierSauvegarde()
{
    remove("../sauvegarde.txt");
}