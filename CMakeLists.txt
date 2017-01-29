cmake_minimum_required(VERSION 3.3)
project(morpionC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

set(SOURCE_FILES main.c
        headers/gainFin.h
                 gainFin.c
        headers/ia.h
                 ia.c
        headers/jeu.h
                 jeu.c
        headers/joueur.h
                 joueur.c
        headers/liste.h
                 liste.c
        headers/morpion.h
                 morpion.c parcoursMorpion.c headers/parcoursMorpion.h headers/sauvegarde.h sauvegarde.c)
add_executable(morpionC ${SOURCE_FILES})