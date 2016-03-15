// perso.cpp

#include "perso.hpp"

perso::perso(string nom, int posx, int posy, string id)
{
	this->nom_=nom;
	this->posx_=posx;
	this->posy_=posy;
	this->id_=id;
}

void perso::update_position(int posx, int posy)
{
	this->posx_=posx;
	this->posy_=posy;
}


string perso::getPos()
{
	string ret="";
	char pos[_MAXLEN_];

	sprintf(pos,"%-3d%-3d", this->posx_,this->posy_);
	ret = outils::ctos(pos);	
	return ret;

}

int perso::getX(){	return (this->posx_); }
int perso::getY(){	return (this->posy_); }

string perso::getNom(){ return (this->nom_); }
string perso::getId(){ return (this->id_); }