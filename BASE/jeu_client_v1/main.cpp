// main.cpp

#include <string>
#include <iostream>
#include "perso.hpp"
#include "partie.hpp"

// Désolé pour les puristes mais g pas trouvé d'autre solution que d'utiliser 
// MSG_ en variable globale pour communiquer facilement entre le thread et le 
// reste du programme.
char MSG_[_MAXLEN_];


int main(int nbarg, char * tabarg[])
{
	// screen c'est l'écran et tmps la surface de blittage temporaire
	// (on blitte tout sur tmps et qd on a fini on blitte tmps sur screen, ça évite les scintillements.
	SDL_Surface *screen;
	SDL_Surface *tmps;
	
	
	if(SDLNet_Init()==-1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return(-2);
	}
	
	partie *p = new partie();	
	
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
	
	atexit(SDL_Quit);

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // Initialisation du mode vidéo, on peut remplacer SDL_HWSURFACE par SDL_SWSURFACE
	// pour créer la surface en mémoire system au lieu d'utiliser la mémoire vidéo.
	// L'inconvéniant de SDL_HWSURFACE c'est qd plein écran mon image scintille :(.	
	screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE | SDL_RESIZABLE );
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
			SDL_GetError());
        exit(1);
    }

	//tmps = (SDL_Surface *)malloc(sizeof(SDL_Surface));
	tmps = SDL_ConvertSurface(screen, screen->format, screen->flags);
	// On met tmps au même format que screen pour pouvoir utiliser la ruse du blittage temporaire

	SDL_SetColorKey(tmps, SDL_SRCCOLORKEY, SDL_MapRGB(tmps->format, 0, 255, 0));

	p->jouer(screen, false, tmps);	
	
	return 0;
}