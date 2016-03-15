// perso.cpp

#include "perso.hpp"

using namespace std;

perso::perso()
{
	this->fic_ = "perso.bmp";

	this->image_ = SDL_LoadBMP(fic_.c_str());
	if (image_ == NULL) {
        printf("Couldn't load %s: %s\n", fic_.c_str(), SDL_GetError());
        return;
    }

	SDL_SetColorKey(this->image_, SDL_SRCCOLORKEY, SDL_MapRGB(this->image_->format, 0, 255, 0));

	this->posx_ = 50;
	this->posy_ = 50;
	this->w_ = this->image_->w;
	this->h_ = this->image_->h;
	this->id_ = " ";
}

perso::perso(string fic)
{
	this->image_ = SDL_LoadBMP(fic.c_str());
	if (image_ == NULL) {
        printf("Couldn't load %s: %s\n", fic.c_str(), SDL_GetError());
        return;
    }

	SDL_SetColorKey(this->image_, SDL_SRCCOLORKEY, SDL_MapRGB(this->image_->format, 0, 255, 0));

	this->posx_ = 50;
	this->posy_ = 50;
	this->w_ = this->image_->w;
	this->h_ = this->image_->h;
	this->id_ = " ";
}

perso::perso(string fic, string nom, int posx, int posy)
{
	this->image_ = SDL_LoadBMP(fic.c_str());
	if (image_ == NULL) {
        printf("Couldn't load %s: %s\n", fic.c_str(), SDL_GetError());
        return;
    }

	SDL_SetColorKey(this->image_, SDL_SRCCOLORKEY, SDL_MapRGB(this->image_->format, 0, 255, 0));

	this->fic_=fic;
	this->nom_=nom;
	this->posx_=posx;
	this->posy_=posy;
	this->w_ = this->image_->w;
	this->h_ = this->image_->h;
	this->id_ = " ";
}


perso::~perso()
{
	SDL_FreeSurface(this->image_);
}

void perso::afficher(SDL_Surface * screen)
{
	SDL_Rect rect;

	rect.w = this->image_->w;
	rect.h = this->image_->h;
	rect.x = this->posx_;
	rect.y = this->posy_;

//	SDL_FillRect(screen, NULL, 0);
	
	if(SDL_BlitSurface(this->image_, NULL, screen, &rect) < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	
	
	SDL_UpdateRect(screen, 0, 0, 0,0);
//	SDL_Flip(screen);
	
	
}


string perso::creerMsg()
{
	char msg[_MAXLEN_];
	string ret = "";

	ret = outils::charToString(_MESSAGE_NORMAL_);	
	sprintf(msg,"%-3d%-3d",this->posx_, this->posy_);
	ret = ret + this->id_ + outils::ctos(msg) + outils::charToString(_FIN_MESSAGE_);

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

/********************************************************/
/*					Accesseurs							*/
void perso::moveX(int dep){	this->posx_ += dep; }
void perso::moveY(int dep){	this->posy_ += dep; }
int perso::getX(){ return(this->posx_); }
int perso::getY(){ return(this->posy_); }
int perso::getW(){ return this->w_; }
int perso::getH(){ return this->h_; }
string perso::getId(){ return this->id_; }

