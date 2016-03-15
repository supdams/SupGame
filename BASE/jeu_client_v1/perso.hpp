// perso.hpp

#ifndef _PERSO
#define _PERSO


#include <string>
#include <iostream>

#include "SDL.h"

#include "constantes.hpp"
#include "outils.hpp"

using namespace std;

class perso{

	public:
		perso();
		perso(string fic);
		perso(string fic, string nom, int posx, int posy);
		~perso();

		void afficher(SDL_Surface * screen);
		int getY();
		int getX();
		int getW();
		int getH();
		string getId();

		void update_position(int posx, int posy);
		void moveX(int dep);
		void moveY(int dep);
		void setId(string id);

		string creerMsg();

	private:
		string nom_;
		string id_;
		string fic_;
		SDL_Surface *image_;
		int w_, h_, posx_, posy_;		

};

#endif