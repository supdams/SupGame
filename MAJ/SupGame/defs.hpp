#ifndef DEFS_HPP
#define DEFS_HPP

/*Toutes les inclusions , les defines , enum etc ... ce situent ici (on inclus ce fichier dans chaque fichier .c)*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <SDL_Mixer.h>
#include <time.h>
#include <math.h>

#include <iostream>
using namespace std;


#define MODE_DEBUG OFF

#define MAX_CHAINE 100
#define LARGEUR 800
#define HAUTEUR 600
#define DIMENSION_PERSO_X 32
#define DIMENSION_PERSO_Y 48

#define COMPTEUR_MONSTRE_BOUGE1 100
#define COMPTEUR_MONSTRE1 10
#define FRAME_MONSTRE1 4

#define DIMENSION_MONSTRE1_X 32
#define DIMENSION_MONSTRE1_Y 48

#define COORDONNE_DEBUT_PERSO_X 568
#define COORDONNE_DEBUT_PERSO_Y 130

#define VITESSE_PERSO 2
#define NB_FRAME_PERSO 4

#define MAX_X 100
#define MAX_Y 100

#define MUR 1
#define TELEPORTATION 2
#define HAUT_CHANGE 3
#define BAS_CHANGE 4
#define DROITE_CHANGE 5
#define GAUCHE_CHANGE 6
#define ACTION_VIDE 7
#define NAGE_PROPS 8

#define ON 1
#define OFF 0

#define AUCUNE_OCCUPATION 1
#define NB_TILESETS 2
#define COMPTEUR_CHANGE_TILESET 25
#define NB_TILE_TILESET 4872
#define TILE_LG 32
#define TILE_HT 32
#define NB_COUCHES 3
#define TILE_X 8/*Pour le blit des tiles*/
#define TILE_Y 8

#define DIMENSION_SPRITE_X 32
#define DIMENSION_SPRITE_Y 48

#define NB_LIGNES 3

#define BULLE 0
#define NORMAL 1

#define VIDE -1 /*Sert si aucun tile n'est mis a [2][3][4]par exemple */

#define NB_ALL_ANIM_PERSO_EVER_OF_THE_WORLD 5
#define NB_MAP 5
#define POS_MAP_GAUCHE 1
#define POS_MAP_DROITE 0
#define POS_MAP_HAUT 3
#define NB_DIRECTION 4

#define AUCUNE_DIRECTION 4
#define AUCUNE_DIRECTION1 5
#define AUCUNE_DIRECTION2 6

enum{BAS , GAUCHE , DROITE , HAUT};
enum{ARC_HAUT , ARC_BAS , ARC_GAUCHE , ARC_DROITE};
enum{HEROS , ISPNJ , ISPNJNONBOUGE , PNJ_BOUGE_TETE , MONSTRE , BIG_ANIM , MONSTRE_NONBOUGE , ARC};
enum{MORT , VIVANT};
enum{EPEE , ARME_ARC , CANNE_A_PECHE};
enum{TILESET_1 , TILESET_2};


#endif
