// constantes.hpp

#ifndef _CONSTANTES
#define _CONSTANTES

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_net.h"
using namespace std;

#define HOST					"localhost"
#define USER					"root"
#define PASS					"123456"
#define DATABASE				"comptes"

#define _NOUVEAU_CLIENT_		'N'
#define _BIENVENUE_ID_			'B'
#define _DECONNEXION_CLIENT_	'D'
#define	_MESSAGE_NORMAL_		'P'
#define	_MESSAGE_CONNECTION_	'L'
#define	_MESSAGE_GUID_CONNECT_	'G'

#define	_MESSAGE_CREATION_PERSO_	'R'
#define	_MESSAGE_CREATURE_INFO_	'C'
#define	_MESSAGE_CREATURE_MAJ_	'M'

#define _SEPARATEUR_MESSAGE_PERSO_	'!'
#define _SEPARATEUR_MESSAGE_	'@'
#define _SEPARATEUR_PERSO_	'#'
#define _FIN_MESSAGE_		'$'


#define	_NB_CHAR_ID_		4

#define _NB_MAX_CHAT_		12
#define _MAXLEN_			1024
#define _MAXNOM_			12
#define _PORT_				8000

#define _TICK_INTERVAL_		10





#define COMPTEUR_MONSTRE1 10
#define FRAME_MONSTRE1 4
#define COMPTEUR_MONSTRE_BOUGE1 100

#define DIMENSION_MONSTRE1_X 32
#define DIMENSION_MONSTRE1_Y 48

#define MAX_CHAINE 100

#define NB_TILE_TILESET 4871
#define TILE_LG 32
#define TILE_HT 32


#define TILE_X 8/*Pour le blit des tiles*/
#define TILE_Y 8

#define COMPTEUR_CHANGE_TILESET 25

enum{HEROS , ISPNJ , ISPNJNONBOUGE , PNJ_BOUGE_TETE , MONSTRE , BIG_ANIM , MONSTRE_NONBOUGE , ARC};

#define NB_COUCHES 3

#define MAX_X 100
#define MAX_Y 100


#define VIDE -1 
#define MUR 1
#define TELEPORTATION 2
#define HAUT_CHANGE 3
#define BAS_CHANGE 4
#define DROITE_CHANGE 5
#define GAUCHE_CHANGE 6
#define ACTION_VIDE 7
#define NAGE_PROPS 8


#define AUCUNE_DIRECTION 4
#define AUCUNE_DIRECTION1 5
#define AUCUNE_DIRECTION2 6

enum{BAS , GAUCHE , DROITE , HAUT};
enum{MORT , VIVANT};

enum{TILESET_1 , TILESET_2};
#define DIMENSION_PERSO_X 32
#define DIMENSION_PERSO_Y 48

typedef struct
{
    SDL_Surface *image;/*l'image de l'animation contenant tout les frames*/
    SDL_Rect src,dest;/*la destination en x , y et les source pour découper l'image des frames*/
    /*int bouge; Definit si l'anim bouge ou pas : temporaire pour test*/
    int animCouranteX , animCouranteY;/*Ou on est dans l'animation*/
    int compteurDefinitife , compteurTemporaire;/*Le temps avant de passer à l'animation suivante*/
    int nbFrame;/*Nombre d'animation dans l'image*/
    int classe;/*Si l'anim est un monstre, un pnj etc ..*/
    int activeAnim;/*Si l'image ne s'anim pas activeAnim = OFF*/
	int orientation;
}Anim;



typedef struct
{
    int largeurScroll , hauteurScroll , xscroll , yscroll , xlim , ylim , wlim, hlim;
    int minx , maxx , miny , maxy;
}Scroll;

typedef struct
{
	
	SDL_Rect destScrollMonstre;
    Anim animMonstre;/*L'anim du monstre*/
	int id;
	int position_x;
	int position_y;
	int spawntimesecs;
	int map;
	int modelid;
	char* nom_monstre;
	int CompteurRepop;
	int hp;
	int mindmg;
	int maxdmg;
    int DeathState;/*Si le monstre est vivant ou pas - boelan*/
	int compteurMonstreBouge;
	 int vivant;
}Monstre;

typedef struct
{
    char props[MAX_CHAINE];
    int num;

}Tileprops;


typedef struct
{
    int tilesetActuelle;
    int compteurTileset;
    int mapDecor[NB_COUCHES][MAX_X][MAX_Y];
    SDL_Surface *imageTileset[2];
    Tileprops *tileprops;
    Scroll scroll;
}Decor;

typedef struct
{
    int nbPnj;
    int nBteleportation;
    int quelMap;
	Decor decor;
	Monstre *monstre;
	int nbMonstre;
}Jeu;

typedef struct
{
    Jeu jeu;
}JeuComplet;



#endif