// partie.cpp

#include "partie.hpp"


extern char MSG_[_MAXLEN_];


partie::partie()
{
	for(int i=0;i<_MAXPERSO_;i++)
		this->perso_[i]=NULL;

	this->perso_[_PERSO_LOCAL_] = new perso("perso.bmp");
	this->client_ = new client(_PORT_);
	this->client_->connecter( this->perso_[_PERSO_LOCAL_]->creerMsg() );

	this->pleinEcran_ = false;
}

partie::~partie(){}


Uint32 time_left(Uint32 next_time)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}


/*
void partie::jouer(SDL_Surface *screen, bool pe, SDL_Surface *tmps); g�re le d�roulement de 
la boucle de jeu.
*/
void partie::jouer(SDL_Surface *screen, bool pe, SDL_Surface *tmps)
{
	string msg;

	Uint32 next_time=0;

	this->pleinEcran_ = pe;

	while(this->perso_[_PERSO_LOCAL_]->getId() == " "){
		this->client_->lire_messages();
		traiter_messages();
	}

	while(true){


		// Cr�ation du message contenant les infos � envoyer au serveur pour l'informer
		// notement de notre position dans l'�cran (pero->posx_ et perso->posy);
		msg=this->perso_[_PERSO_LOCAL_]->creerMsg();
		
		// Envoie du message au serveur
		this->client_->envoyer_message(msg);		
		
		// On traite les messages lus par le thread cr�� pr�c�dement (c la m�thode lire_messages();
		// de la classe client qui tourne dans ce thread).
		// Ce traitement permet notement de mettre � jour les positions des autres joueurs connect�s.
		traiter_messages();

		// Gestion des touches du clavier pour bouger le perso ou quitter l'appli
		gerer_touches(screen, tmps);

		this->client_->lire_messages();

		// Rendu des diff�rentes donn�es � l'�cran.
		animer(screen, tmps);		
		
		//cout<<time_left(next_time)<<endl;
		SDL_Delay(time_left(next_time));
		next_time = SDL_GetTicks() + _TICK_INTERVAL_;	
	}
}


/*
void partie::animer(SDL_Surface * screen, SDL_Surface *tmps); se charge d'afficher les 
persos � l'�cran.
*/

void partie::animer(SDL_Surface * screen, SDL_Surface *tmps)
{
	// On commence par effacer l'�cran et la surface temporaire.
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 255));
	SDL_FillRect(tmps, NULL, SDL_MapRGB(screen->format, 0, 255, 0));

	// On blitte les perso un � un sur la surface temporaire.
	for(int i=0 ; i<_MAXPERSO_ ; i++){
		if(this->perso_[i])
			this->perso_[i]->afficher(tmps);
	}
	
	// On blitte la surface temporaire sur l'�cran
	if(SDL_BlitSurface(tmps, NULL, screen, NULL) < 0)
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());	
	
	// Mise � jour de l'�cran ... le tour est jou� !
	SDL_UpdateRect(screen, 0, 0, 0,0);	
}

/*
void partie::traiter_messages(); est surement la m�thode la plus obscure.
Elle est charg�e d'interpr�ter les messages re�us du serveur (par exemple 
l'arriv�e d'un nouveau perso ou l'actualisation des coordonn�es d'un perso, ...)
et de faire les actions correspondantes.
*/
void partie::traiter_messages()
{
	int i, posx, posy;
	string msg, nom, id;

	// On switch sur le type de message re�u (pour l'instant seuls 2 type de message :
	//	- nouveau client
	//	- position du client
	// Le type de message est cod� par le premier caract�re du message suivant un protocole 
	// � la con que g d�fini : 'N' pour nouveau client et 'P' pour position.
	switch(MSG_[0]){
		case _NOUVEAU_CLIENT_:		// Message indiquant la connexion d'un nouveau perso
		case _BIENVENUE_ID_:
			msg = outils::ctos(MSG_);

			if( MSG_[0] == _BIENVENUE_ID_ ){
				msg = outils::extraire_message(msg, MSG_[0], &nom, &id, &posx, &posy);
				this->perso_[_PERSO_LOCAL_]->setId(id);				
			}

			// On va rabotter msg � chaque fois qu'on a trait� un perso						
			while( msg.length()>3 ){

				for(i=1 ; i<_MAXPERSO_ && this->perso_[i] ; i++);
				if(i==_MAXPERSO_)
					cout<<"Erreur de conception, il y a trop de persos..."<<endl;

				msg = outils::extraire_message(msg, _NOUVEAU_CLIENT_, &nom, &id, &posx, &posy);

				this->perso_[i] = new perso("perso.bmp", nom, posx, posy);
				this->perso_[i]->setId(id);
			}
				
			MSG_[0]=' ';
			break;
		

		case _MESSAGE_NORMAL_:
			msg = outils::ctos(MSG_);
			while( msg.length()>3 ){

				msg = outils::extraire_message(msg, MSG_[0], &nom, &id, &posx, &posy);
				i = this->getIndicePerso(id);
				if(i>0)
					this->perso_[i]->update_position(posx, posy);
				else
					cout<<"BOG ; id non trouve dans la liste des persos..."<<endl;				
			}
			MSG_[0]=' ';
			break;

		case _DECONNEXION_CLIENT_:
			msg = outils::ctos(MSG_);
			msg = outils::extraire_message(msg, MSG_[0], &nom, &id, &posx, &posy);
			i = this->getIndicePerso(id);
			delete this->perso_[i];
			this->perso_[i] = NULL;
			break;

		default:
			break;
	}
}

void partie::gerer_touches(SDL_Surface * screen, SDL_Surface *tmps)
{
	SDL_Event event;
	Uint8 *keys;

	while(SDL_PollEvent(&event)) {
		switch(event.type){
		case SDL_QUIT:
			exit(1);
			break;
		}
	}
	keys = SDL_GetKeyState(NULL);
	if(keys[SDLK_ESCAPE])
		exit(1);
	
	if(keys[SDLK_UP])
	{
		if(this->perso_[0]->getY() > 1 )
			this->perso_[0]->moveY(-_DEPL_);
	}
	if(keys[SDLK_DOWN])
	{
		if(this->perso_[0]->getY() < (screen->h - this->perso_[0]->getH()))
			this->perso_[0]->moveY(+_DEPL_);
	}
	if(keys[SDLK_RIGHT])
	{
		if(this->perso_[0]->getX() < (screen->w - this->perso_[0]->getW()))
			this->perso_[0]->moveX(+_DEPL_);
	}
	if(keys[SDLK_LEFT])
	{
		if(this->perso_[0]->getX() > 1)
			this->perso_[0]->moveX(-_DEPL_);
	}

	if(keys[SDLK_SPACE]){
		int j=-8000;
		j = SDL_WM_ToggleFullScreen(screen);
		cout<<"j --> "<<j<<endl;
		tmps = SDL_ConvertSurface(screen, screen->format, screen->flags);
	}
	
}

int partie::getIndicePerso(string id)
{
	int i;

	for(i=0 ; i<_MAXPERSO_ ; i++){
		if(this->perso_[i] && (this->perso_[i]->getId()==id))
			break;
	}

	if( i<_MAXPERSO_ )
		return(i);
	else
		return(-1);
}