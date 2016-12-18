cmake_minimum_required(VERSION 3.3)
project(morpionC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

set(SOURCE_FILES main.c
                 fichierH/gainFin.h
                 gainFin.c
                 fichierH/ia.h
                 ia.c
                 fichierH/jeu.h
                 jeu.c
                 fichierH/joueur.h
                 joueur.c
                 fichierH/liste.h
                 liste.c
                 fichierH/morpion.h
                 morpion.c)
add_executable(morpionC ${SOURCE_FILES})