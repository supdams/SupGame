#ifndef _CLIENT_H_
#define _CLIENT_H_ 1


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>



#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_net.h"

#include "constantes.hpp"
#include "perso.hpp"

using namespace std;


/************************************************************************
La classe client permet de gérer l'interface réseaux du 'jeu'.
Elle contient les méthodes pour se connecter au serveur et échanger 
des messages avec lui.
**************************************************************************/
class client
{
public:
	client(int port);
	void connecter(JeuComplet *jeucomplet, string position );
	int envoyer_message(string msg);
	void lire_messages();
	~client(){};
	int race_;

private:
	int port_;
	SDLNet_SocketSet set_;
	TCPsocket tcpSock_, srvSock_, lectSock_;

	string nom_;
};

#endif