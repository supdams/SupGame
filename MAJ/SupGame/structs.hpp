#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include "defs.hpp"
#include "constantes.hpp"

typedef struct Gestion
{
	SDL_Surface *screen, *tmps, *background, *COUCHE0, *COUCHE1, *AFFICHAGE_PERSO, *BGUI3, *ICONE_CHAT, *SOURIEACTIF, *SOURIEINACTIF,
		*Portrait_bg, *HUD_config, *HUD_carte, *HUD_quete, *HUD_portrait_personnage, *HUD_sac;

	int onMenu, menuType, choice;
} Gestion;


typedef struct
{
    char props[MAX_CHAINE];
    int num;

}Tileprops;

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
    Anim animMonstre;/*L'anim du monstre*/
    int compteurMonstreBouge;
	int id;
	int map;
	int modelid;
	char *nom_monstre;
	SDL_Rect destScrollMonstre;
	int CompteurRepop;
	int hp;
	int mindmg;
	int maxdmg;
    int vivant;/*Si le monstre est vivant ou pas - boelan*/

}Monstre;

typedef struct
{
    int x , y , w , h;/*Les clics , pour voir ou se situe la box à tester (les coordonées de commencer par exemples ...)*/

}AABB;


typedef struct
{
    SDL_Rect dest;
    int animActuelleNormale , animActuelleObjet; /*Represente les différentes animations du perso*/
    Anim *animPerso;
    Anim animPersoTempo;
    int activeNage;/*Dit si le perso est entrain de nager*/
    int collisionPnjPerso;
    SDL_Surface *persoCamouflage;
    int activeCamouflage;
	int hp;
	int race;
	int guid;
	int acc_compte;
	int map;
	string nom;
	string acces;
	string acces_suite;
	string creation_reussi;
	string data_creatures;
}Perso;

typedef struct
{
    int largeurScroll , hauteurScroll , xscroll , yscroll , xlim , ylim , wlim, hlim;
    int minx , maxx , miny , maxy;
}Scroll;

typedef struct
{
    int active;
    SDL_Surface *ligneAfficher[3];
    char ligne[NB_LIGNES][MAX_CHAINE];
    SDL_Surface *imageTexte;
    SDL_Rect posTexte;
    int activeTexte , activeSuprimmeTexte , activeTextePanneau , quelFace;
    int modeImage;
    char nomModeImage[MAX_CHAINE];
    TTF_Font *font;
    SDL_Color couleurBlanche;
    SDL_Color couleurRouge;
    char quiParle[MAX_CHAINE];
    char ifSuite[MAX_CHAINE];
    SDL_Rect posLigneBulle[NB_LIGNES];

}Texte;


typedef struct
{
    SDL_Surface *image;
    SDL_Rect position;
    AABB boxinterface[1];

}Interface_jeu;

typedef struct
{
    SDL_Rect dest[NB_DIRECTION];

}ChangeMap;

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
    int quelMap;
    int nBteleportation;
	int nbMonstre;
    Perso perso[_MAXPERSO_];
	Monstre *monstre;
	Decor decor;
    Texte texte;
    Interface_jeu interface_jeu;
    ChangeMap changemap;
	SDL_TimerID timer;
	int clic_activer;
	int num_mob_clic;
}Jeu;


typedef struct
{
    int haut , bas , droite , gauche , espace ,entrer , pause;
    int attaqueBas , attaqueHaut , attaqeBas , attaqueGauche;
    int clic , lshift , c;
    SDL_Rect posSouris;

	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
	char quit;

}Touche;



typedef struct
{
    Jeu jeu;
    Touche touche;
    int continuer;
    SDL_Surface *ecran;
}JeuComplet;




/* Structure pour gérer l'input (clavier puis joystick) */
typedef struct Input
{
	int left, right, up, down, jump, attack, enter, erase, pause, extra, inventaire, espace;
	int add, remove;
	int previous, next, load, save, copy, reinit;
	int mouseX, mouseY;
} Input;
# endif /* !STRUCTS_HPP */