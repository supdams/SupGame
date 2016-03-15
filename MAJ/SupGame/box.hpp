#ifndef DEF_ZONETEXTE
#define DEF_ZONETEXTE

#include <SDL.h>

#include "structs.hpp"

	void lireDonneeZoneTexte(SDL_Surface *surface, TTF_Font *police, char *chaineEntree, char *introductionInitiale, char *texteDePreremplissage, long nombreDeCaracteresMaximal,int positionx, int positiony);
	void detecterErreurs(SDL_Surface *surface, SDL_Surface *barre, TTF_Font *police, char *chaineEntree, char *texteDePreremplissage, long nombreDeCaracteresMaximal);
	void lireCaractereZoneTexte(SDL_Event event, int *majuscule, char *chaine, long *positionCaractere, int *continuer, long nombreDeCaracteresMaximal);
	void entrerLettre(int *lettreEntree, long numeroCaractere, char *caractere);
	void insererCaractereDansChaine(long positionCaractere, char *chaine, char caractereAInserer);
	void blitterCurseur(char *chaineFinale, SDL_Rect *positionTexte, TTF_Font *police, char *introduction, char *chaineEntree, SDL_Surface *surface, SDL_Surface *barre, long positionCaractere);
	void detecterLettre(SDL_Event event, int *lettreEntree, char *caractereEntre, int *majuscule);
 
#endif