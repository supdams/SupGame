// perso.hpp

#ifndef _PERSO
#define _PERSO


#include <string>
#include <iostream>

#include "SDL.h"

#include "constantes.hpp"
#include "outils.hpp"
#include "structs.hpp"
#include "map.hpp"

#include "textedition.hpp"
extern Gestion jeu;
using namespace std;
		
class perso{

	public:
		perso();
		perso(string nom, int posx, int posy, string race);
		~perso();

		void afficher(JeuComplet *jeucomplet, int i);
		int getY(JeuComplet *jeucomplet);
		int getX(JeuComplet *jeucomplet);
		int getW();
		int getH();
		int LaRace();
		string getId();
		string getRace();

		void update_position(int posx, int posy);
		void moveX(int dep);
		void moveY(int dep);
		void setId(string id);
		void setrace(string race);

		string creerMsg(JeuComplet *jeucomplet);
		string creerMsg_logger(string SPseudochaine, string SMdpchaine, JeuComplet *jeucomplet);
		string creerMsg_connecte_perso(string guide, JeuComplet *jeucomplet);
		string creerMsg_creation_perso(string SPseudochaine, JeuComplet *jeucomplet);

	private:
		string nom_;
		string id_;
		string race_;
		int w_, h_, posx_, posy_, carte_,race;
		
		
};
extern TTF_Font *font[8];

#endif