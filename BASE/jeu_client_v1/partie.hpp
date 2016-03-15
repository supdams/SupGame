// partie.hpp

#ifndef _PARTIE
#define _PARTIE

#include "constantes.hpp"

#include "perso.hpp"
#include "client.hpp"


/*
Gère le déroulement d'une partie.
*/
class partie
{
	public :
		partie();
		~partie();
		void jouer(SDL_Surface *screen, bool pleinEcran, SDL_Surface *tmps);
		void traiter_messages();
		void animer(SDL_Surface *screen,SDL_Surface *tmps);
		void gerer_touches(SDL_Surface *screen,SDL_Surface *tmps);

		int getIndicePerso(string id);

	private:
		bool pleinEcran_;
		
		perso *perso_[_MAXPERSO_];
		client * client_;

};


#endif