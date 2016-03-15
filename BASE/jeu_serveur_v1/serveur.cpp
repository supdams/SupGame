//serveur.cpp

#include "serveur.hpp"

using namespace std;

serveur::serveur(int port)
{
	this->port_ = port;
	this->tcpsock_ = NULL;
	
	for(int i=0 ; i<_NB_MAX_CHAT_ ; i++){
		this->tabsock_[i] = NULL;
		this->perso_[i] = NULL;
	}

	this->nbCliC_ = 0;
	this->numCliL_ = 0;
	this->quit_ = false;
	this->lastId_ = "aaa";
}

serveur::~serveur()
{
}

// Petite fonction pour gérer la durée d'une boucle entre deux envois de messages.
Uint32 time_left(Uint32 next_time)
{
    Uint32 now;
    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}


void serveur::lancer()
{
	IPaddress ip;
	string msg;	int cpt, numaj, result;
	Uint32 next_time=0;


	if(SDLNet_ResolveHost(&ip,NULL,port_)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(1);
	}
	
	tcpsock_ = SDLNet_TCP_Open(&ip);
	if(!tcpsock_)
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(2);
	}
	
	set_=SDLNet_AllocSocketSet(_NB_MAX_CHAT_+1);
	
	
	numaj = SDLNet_TCP_AddSocket(set_, tcpsock_);
	cout<<"premier numaj -------> "<<numaj<<endl;
	printf("En attente de connexion... \n");
	
	while(! quit_ )
	{
		if( (nbSockActif_ = SDLNet_CheckSockets(set_, 100)) > 0 )     // On teste si au moins un port est actif
		{
			;
			if( SDLNet_SocketReady(tcpsock_) )    // si c une demande de connexion
			{
				this->nouvelle_connexion();
			}
			else		// Sinon c un envoie de données
			{
				for(cpt=0 ; cpt < _NB_MAX_CHAT_ ; cpt++)         // On balaie la liste des connectés
				{
					if( SDLNet_SocketReady(tabsock_[cpt]) )
					{
						result=SDLNet_TCP_Recv(tabsock_[cpt], msg_, _MAXLEN_);
						
						if( result <=0 ) {						// Il y a eut une erreur (deco)
							this->deconnexion(cpt);
						} else {							
							//cout<<this->tabNom_[cpt]<< " --> " << msg_ <<endl;
							//this->tabMsg_[cpt] = strdup(this->msg_);
							msg = outils::ctos(msg_);
							traiter_message(_MESSAGE_NORMAL_, msg, cpt);
						}
					}
					
				}
			}			
		}

		if( time_left(next_time)<1 ){
			//cout<<"e";
			this->envoyer_message( _MESSAGE_NORMAL_, -1);
			next_time = SDL_GetTicks() + _TICK_INTERVAL_;
		}		
	}
}



void serveur::envoyer_message(char type_message, int exclu)
{
	char * env;
	char * env2;
	int len, len2, i, res;
	string msg="", msg2="";

	switch(type_message){
		case _NOUVEAU_CLIENT_:
			cout<<"envoyer_message("<<type_message<<", "<<exclu<<")"<<endl;

			msg = this->creer_message_nvp(exclu, 0);
			cout<<"   msg  --> "<<msg<<endl;
			env = strdup(msg.c_str());
			len = strlen(env)+1;

			msg2 = this->creer_message_nvp(exclu, 1);
			cout<<"   msg2 --> "<<msg2<<endl;
			env2 = strdup(msg2.c_str());
			len2 = strlen(env2)+1;

			for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
				if(this->tabsock_[i] &&(i!=exclu) ){					
					res=SDLNet_TCP_Send(this->tabsock_[i], env, len);
					cout<<"envoie pour "<<i<<" --> "<<env<<endl;
					if(res<len){
						printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					}
				} else if(i==exclu) {					
					res = SDLNet_TCP_Send(this->tabsock_[exclu], env2, len2);
					cout<<"envoie pour "<<i<<" -->"<<env2<<endl;
					if(res<len){
						printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					}
				}
			}
			SDL_Delay(5);
			break;

		case _DECONNEXION_CLIENT_:
			msg = this->creer_message_dec(exclu);
			cout<<"msg dec ..."<<msg<<endl;
			env = strdup(msg.c_str());
			len = strlen(env)+1;

			for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
				if( this->tabsock_[i] && i!=exclu ){
					res = SDLNet_TCP_Send(this->tabsock_[i], env, len);
				}
			}
			break;


		case _MESSAGE_NORMAL_:			
			for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
				if(this->tabsock_[i]){
					msg = this->creer_message_pos(i);
					env = strdup(msg.c_str());
					len = strlen(env)+1;
					res=SDLNet_TCP_Send(this->tabsock_[i], env, len);
					//free(env);
				}
			}			
			break;

		default:
			break;
	}
}


void serveur::traiter_message(char type_message, string msg, int envoyeur)
{	
	string nom="", id="";
	int posx, posy, i;



	switch(type_message){

		case _NOUVEAU_CLIENT_:
			outils::extraire_message(msg, _NOUVEAU_CLIENT_, &nom, &id, &posx, &posy);
			cout<<"Nouveau client connecte : "<<nom<<" sur, posx : "<<posx<<" - posy : "<<posy<<endl;
			this->lastId_ = outils::creerId(this->lastId_);
			this->perso_[envoyeur] = new perso(nom, posx, posy, this->lastId_);
			envoyer_message(_NOUVEAU_CLIENT_, envoyeur);
			break;


		case _DECONNEXION_CLIENT_:
			outils::extraire_message(msg, _DECONNEXION_CLIENT_, &nom, &id, &posx, &posy);
			i = this->getIndicePerso(id);
			delete this->perso_[i];
			this->perso_[i]=NULL;
			break;


		case _MESSAGE_NORMAL_:
			//cout<<"traiter_message(NORMAL) recoit msg --> "<<msg<<endl;
			outils::extraire_message(msg, _MESSAGE_NORMAL_, &nom, &id, &posx, &posy);
			//cout<<id<<" "<<posx<<" "<<posy<<endl;
			this->perso_[envoyeur]->update_position(posx, posy);
			break;

		default:
			break;
	}
}


/*
void serveur::nouvelle_connexion(); gère la connexion d'un nouveau client sur le serveur.
*/
void serveur::nouvelle_connexion()
{
	int cpt, numaj, result;
	string msg;
	
	for(cpt=0 ; cpt<_NB_MAX_CHAT_ && this->tabsock_[cpt] ; cpt++); // On cherche un socket de libre				
	
	if(  cpt<_NB_MAX_CHAT_ ){			
		this->tabsock_[cpt] = SDLNet_TCP_Accept(this->tcpsock_);
		numaj = SDLNet_TCP_AddSocket(this->set_, this->tabsock_[cpt]);						
		result=SDLNet_TCP_Recv(this->tabsock_[cpt], this->msg_, _MAXLEN_);
		if(result>0){
			msg = outils::ctos(this->msg_);
			cout<<"nouvelle_connexion() recu --> "<<msg_<<endl;
			traiter_message(_NOUVEAU_CLIENT_, msg, cpt);			
			//envoyer_message(_NOUVEAU_CLIENT_,cpt);
		}
		this->nbCliC_++;			
	} else {
		cout<<"Le serveur est plein..."<<endl;
	}
}

/*
void serveur::deconnexion(int numCli); Gère la déconnexion d'un client.
*/
void serveur::deconnexion(int numCli)
{
	cout<<"###### Deconnexion de "<<this->perso_[numCli]->getNom()<< endl;

	envoyer_message(_DECONNEXION_CLIENT_, numCli);
	delete this->perso_[numCli];
	this->perso_[numCli]=NULL;

	SDLNet_TCP_DelSocket(this->set_, this->tabsock_[numCli]);
	this->tabsock_[numCli] = NULL;
	this->nbCliC_--;
	
}





/*
string serveur::creer_message_nvp(serveur * s, int exclu, int flag); Créé le message suivant la 
connexion d'un nouveau client

  Flags :
	0 --> création du message pour tous les autres
	1 --> création du message pour le nouveau

Structure des messages :
------------------------

Si Flag = 0
	1 char  --> Debut de message
	N chars --> Nom du nouveau client
	1 char  --> séparateur de données ( = "!" )
	3 chars --> Id du nouveau client
	6 chars --> position du nouveau client
	1 char  --> Fin de message

Si Flag != 0
	1 char  --> Debut de message
	Puis pour chaque perso déjà connecté :
		3 chars --> Id du perso i
		6 chars --> positions du perso i
		1 char  --> Séparateur de perso ( = '#' )
	Fin Puis
	1 char   --> Fin de message
*/
string serveur::creer_message_nvp(int exclu, int flag)
{
	string ret = "";
	perso * p;
	int i;

	

	if(flag){
		// Il faut créer un message indiquant au nouveau tous les persos déjà existants...		
		// Mais on commence par lui donner son Id !!!
		ret = outils::charToString(_BIENVENUE_ID_);
		ret += this->perso_[exclu]->getId();
		for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
			p = this->perso_[i];
			if( p && i!=exclu ){
				ret = ret + p->getNom() + "!" + p->getId() + p->getPos() + outils::charToString(_SEPARATEUR_PERSO_);
			}
		}
		ret += outils::charToString(_FIN_MESSAGE_);

		// else = il faut juste indiquer l'arrivée du nouveau client (message destiné aux autres)
	} else {
		ret = outils::charToString(_NOUVEAU_CLIENT_);
		p = this->perso_[exclu];
		ret = ret + p->getNom() + "!" + p->getId() + p->getPos()+ outils::charToString(_FIN_MESSAGE_);
	}

	return ret;
}


/*
string serveur::creer_message_pos(int dest); Créé le message à envoyer à dest.
dest doit recevoir les infos sur tous les persos qui le concernent (qui sont dans sa zone).
Actuellement ça représente tous les persos sauf lui même mais plus tard quand il y a aura une 
grande map il faudra trier les persos qui sont dans sa zone.
Structure du message :
1 char  --> type de message ( = _MESSAGE_NORMAL_ = 'P' )
Puis pour chaque personnage :
3 chars --> Id du perso
3 chars --> posx du perso
3 chars --> posy du perso
1 char  --> Indicateur de séparation de perso ( = _SEPARATEUR_PERSO_ = '#' )
Fin puis
1 char  --> Indicateur de fin de message
*/
string serveur::creer_message_pos(int dest)
{
	string ret = " ";
	perso * p;
	int i;

	ret = outils::charToString(_MESSAGE_NORMAL_);
	
	for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
		p = this->perso_[i];
		// Ce if sera à remplacer par la fonction qui gèrera les zones et dira si il faut ou pas
		// envoyer les infos du perso i au perso dest.
		if( p && i!=dest){			
			ret = ret + p->getId() + p->getPos() + outils::charToString(_SEPARATEUR_PERSO_);
		}
	}
	ret += outils::charToString(_FIN_MESSAGE_);

	return ret;
}


/*
string serveur::creer_message_dec(int exclu); Créé un message indiquant la déconnexion d'in client.
Structure du message :
1 char  --> type de message (= _DECONNEXION_CLIENT_ = 'D')
3 chars --> Id du perso qui se deco
1 char  --> fin de message (= _FIN_MESSAGE_ = '$')

exemple : "Daab$"
*/
string serveur::creer_message_dec(int exclu)
{
	string ret = "";
	ret = outils::charToString(_DECONNEXION_CLIENT_) + this->perso_[exclu]->getId() + outils::charToString(_FIN_MESSAGE_);
	return ret;
}


perso * serveur::getPerso(int i)
{
	return(this->perso_[i]);
}


int serveur::getIndicePerso(string id)
{
	int i;

	for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
		if(this->perso_[i] && this->perso_[i]->getId()==id)
			break;
	}
	if(i<_NB_MAX_CHAT_)
		return(i);
	else
		return(-1);
}