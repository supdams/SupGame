#ifndef DEF_ZONETEXTE
#define DEF_ZONETEXTE


#define MAX_CHAINE 100
#include <SDL.h>
#include "struc.hpp"

	void lireDonneeZoneTexte(SDL_Surface *surface, TTF_Font *police, char *chaineEntree, char *introductionInitiale, char *texteDePreremplissage, long nombreDeCaracteresMaximal,int positionx, int positiony);
	void detecterErreurs(SDL_Surface *surface, SDL_Surface *barre, TTF_Font *police, char *chaineEntree, char *texteDePreremplissage, long nombreDeCaracteresMaximal);
	void lireCaractereZoneTexte(SDL_Event event, int *majuscule, char *chaine, long *positionCaractere, int *continuer, long nombreDeCaracteresMaximal);
	void entrerLettre(int *lettreEntree, long numeroCaractere, char *caractere);
	void insererCaractereDansChaine(long positionCaractere, char *chaine, char caractereAInserer);
	void blitterCurseur(char *chaineFinale, SDL_Rect *positionTexte, TTF_Font *police, char *introduction, char *chaineEntree, SDL_Surface *surface, SDL_Surface *barre, long positionCaractere);
	void detecterLettre(SDL_Event event, int *lettreEntree, char *caractereEntre, int *majuscule);
 
	
int AABBCollision(int curseur_x ,int curseur_y ,AABB box);
void test_touche();

void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor);
void drawImage(SDL_Surface *image, int x, int y);
SDL_Surface *load_image(const char file[MAX_CHAINE]);
TTF_Font *loadFont(char *name, int size);







extern Gestion jeu;
extern TTF_Font *font[7];
extern Touche touche;

#endif


