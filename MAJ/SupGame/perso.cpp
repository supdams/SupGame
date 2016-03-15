// perso.cpp

#include "perso.hpp"

using namespace std;



perso::perso()
{
	this->posx_ = 50;
	this->posy_ = 50;
	this->id_ = " ";
	this->race_ = "";
}

perso::perso(string nom, int posx, int posy, string race)
{

	this->nom_=nom;
	this->posx_=posx;
	this->posy_=posy;
	this->id_ = " ";
	this->race_=race;
}


perso::~perso(){}

void perso::afficher(JeuComplet *jeucomplet, int i)
{
 	SDL_Color foregroundColor;

	foregroundColor.r = 1;
	foregroundColor.g = 255;
	foregroundColor.b = 0;

	if(i==0){
		SDL_Rect dest;
		
		dest.x = jeucomplet->jeu.perso[i].dest.x - jeucomplet->jeu.decor.scroll.xscroll;
		dest.y = jeucomplet->jeu.perso[i].dest.y - jeucomplet->jeu.decor.scroll.yscroll;
		
		if(SDL_BlitSurface(jeucomplet->jeu.perso[i].animPerso[jeucomplet->jeu.perso[i].animActuelleObjet].image , &jeucomplet->jeu.perso[i].animPerso[jeucomplet->jeu.perso[i].animActuelleObjet].src , jeu.tmps , &dest) < 0)
			cout<<"BlitSurface error: "<<SDL_GetError()<<endl;

	//	char nom[200];
	//	memset(&nom, 0, sizeof(nom));
	//	sprintf(nom, "%s", this->nom_.c_str());
	//	drawString(nom, dest.x, dest.y-18, font[5],foregroundColor); 	

    }else{
		SDL_Rect dest;
		dest.x = this->posx_ - jeucomplet->jeu.decor.scroll.xscroll;
		dest.y = this->posy_ - jeucomplet->jeu.decor.scroll.yscroll;
		
		if(SDL_BlitSurface(jeucomplet->jeu.perso[i].animPerso[jeucomplet->jeu.perso[i].animActuelleObjet].image , &jeucomplet->jeu.perso[i].animPerso[jeucomplet->jeu.perso[i].animActuelleObjet].src , jeu.tmps , &dest) < 0)
			cout<<"BlitSurface error: "<<SDL_GetError()<<endl;
		char nom[200];
		memset(&nom, 0, sizeof(nom));
		sprintf(nom, "%s", jeucomplet->jeu.perso[i].nom.c_str());
		drawString(nom, dest.x, dest.y-18, font[5],foregroundColor); 	

		}
}


string perso::creerMsg(JeuComplet *jeucomplet)
{
	char msg[_MAXLEN_];
	string ret = "";

	ret = outils::charToString(_MESSAGE_NORMAL_);	

	sprintf(msg,"%-3d%-3d",jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x, jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y);
	//sprintf(msg,"%-3d%-3d",this->posx_, this->posy_);
	ret = ret + this->id_ + outils::ctos(msg) + outils::charToString(_FIN_MESSAGE_);

	return ret;

}

string perso::creerMsg_logger(string SPseudochaine, string SMdpchaine, JeuComplet *jeucomplet)
{
	string ret = "";

	ret = "L" + SPseudochaine + "!" + SMdpchaine + "!" + outils::charToString(_FIN_MESSAGE_);

	return ret;
}

string perso::creerMsg_connecte_perso(string guide, JeuComplet *jeucomplet)
{
	string ret = "";

	ret = "G" + guide + "!" + outils::charToString(_FIN_MESSAGE_);

	return ret;
}

string perso::creerMsg_creation_perso(string SPseudochaine, JeuComplet *jeucomplet)
{
	string ret = "";
	
	ret = "R" + SPseudochaine + "!" + outils::itos(jeucomplet->jeu.perso[_PERSO_LOCAL_].acc_compte) + "!" + outils::charToString(_FIN_MESSAGE_);

	return ret;
}

void perso::update_position(int posx, int posy)
{
	this->posx_ = posx;
	this->posy_ = posy;
}


void perso::setId(string id)
{
	this->id_ = id;
}

void perso::setrace(string race)
{
	this->race_ = race;
}

/********************************************************/
/*					Accesseurs							*/
void perso::moveX(int dep){	this->posx_ += dep; }
void perso::moveY(int dep){	this->posy_ += dep; }
int perso::getX(JeuComplet *jeucomplet){ return(jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.x); }
int perso::getY(JeuComplet *jeucomplet){ return(jeucomplet->jeu.perso[_PERSO_LOCAL_].dest.y); }

int perso::getW(){ return this->w_; }
int perso::getH(){ return this->h_; }
string perso::getId(){ return this->id_; }
string perso::getRace()
{
	return this->race_; 
}
