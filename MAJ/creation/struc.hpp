#ifndef DEF_STRUC
#define DEF_STRUC


#define MAX_CHAINE 100

typedef struct
{
    int x , y , w , h;/*Les clics , pour voir ou se situe la box à tester (les coordonées de commencer par exemples ...)*/

}AABB;

typedef struct Gestion
{
	SDL_Surface *screen, *tmps, *background, *COUCHE0, *COUCHE1, *AFFICHAGE_PERSO, *BGUI3, *ICONE_CHAT, *SOURIEACTIF, *SOURIEINACTIF,
		*Portrait_bg, *HUD_config, *HUD_carte, *HUD_quete, *HUD_portrait_personnage, *HUD_sac;

	int onMenu, menuType, choice;
} Gestion;

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


#endif


