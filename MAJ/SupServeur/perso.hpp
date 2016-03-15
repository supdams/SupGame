// perso.hpp
#ifndef _PERSO
#define _PERSO

#include "outils.hpp"

class perso{

	public:
		perso(string nom, int posx, int posy, string id, string race);
		void update_position(int posx, int posy);
		string load_login(string pseudo, string mdp);
		string connect_perso(string guid);
		string recharger_liste_perso(string guid);
		string liste_creature(string map);
		
		string new_liste_creature(string map,JeuComplet *jeucomplet);
		string creation_perso(string pseudo, string guide);

		int getX();
		int getY();
		int id_compte;

		string getPos();
		string getNom();
		string getId();
		string getRace();
		
		string acces_;
		string ecrire_acces_;

	private:
		string nom_;
		string id_;
		string race_;
		int posx_,posy_;
};

#endif