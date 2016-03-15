// perso.hpp


#ifndef _PERSO
#define _PERSO

#include "outils.hpp"

class perso{

	public:
		perso(string nom, int posx, int posy, string id);
		void update_position(int posx, int posy);
		int getX();
		int getY();

		string getPos();
		string getNom();
		string getId();

	private:
		string nom_;
		string id_;
		int posx_,posy_;

};

#endif