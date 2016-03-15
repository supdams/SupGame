// outils.hpp

#ifndef _OUTILS
#define _OUTILS

#include<iostream>
#include<string>
#include<sstream>

#include "constantes.hpp"

#include "defs.hpp"
#include "structs.hpp"


using namespace std;
 
void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor);
void drawString2(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor);
void drawImage(SDL_Surface *image, int x, int y);

int AABBCollision(int curseur_x ,int curseur_y ,AABB box);
	extern Gestion jeu;
/*
Petite classe utilitaire permettant de faire qqes conversions bien pratiques.
*/
class outils
{
	public :
		static string itos(int i);
		static string ctos(char * c);
		static string charToString(char c);
		static string extraire_message( string msg, char typeMessage, string * nom, string * id, int * posx, int * posy, string * race, string * acces, string * acces_suite);
};

#endif