//serveur.hpp

#ifndef _SERVEUR_H_
#define _SERVEUR_H_ 1



#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_net.h"


#include "constantes.hpp"
#include "outils.hpp"
#include "perso.hpp"

using namespace std;

class serveur
{
	public :
		serveur(int port);
		~serveur();
		void lancer();
		void envoyer_message(char type_message, int exclu);
		void traiter_message(char type_message, string msg, int envoyeur);
		void nouvelle_connexion();
		void deconnexion(int numCli);

		string creer_message_nvp(int exclu, int flag);
		string creer_message_pos(int dest);
		string creer_message_dec(int exclu);

		perso * getPerso(int i);
		int getIndicePerso(string id);

	private :
		TCPsocket tcpsock_;
		TCPsocket tabsock_[_NB_MAX_CHAT_], tabSrvSock_[_NB_MAX_CHAT_];
		SDLNet_SocketSet set_;

		char msg_[_MAXLEN_];
		int nbCliC_, numCliL_;
		int nbSockActif_;
		int port_;		

		char msgEnvoie_[_MAXLEN_];

		perso * perso_[_NB_MAX_CHAT_];

		string lastId_;

		bool quit_;
};

#endif