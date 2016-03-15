//serveur.hpp

#ifndef _SERVEUR_H_
#define _SERVEUR_H_ 1

#include "constantes.hpp"
#include "outils.hpp"
#include "perso.hpp"
#include "database.hpp"
#include "map.hpp"


class serveur
{
	public :
		serveur(int port);
		~serveur();
		void lancer(JeuComplet *jeucomplet);
		
		void new_maj_monstre(JeuComplet *jeucomplet);
		void lire_monstre(JeuComplet *jeucomplet);
		void alloc_monstre(JeuComplet *jeucomplet);
		void envoyer_message(char type_message, int exclu);
		void traiter_message(JeuComplet *jeucomplet,char type_message, string msg, int envoyeur);
		void nouvelle_connexion(JeuComplet *jeucomplet);
		void deconnexion(JeuComplet *jeucomplet,int numCli);

		string creer_message_nvp(int exclu, int flag);
		string creer_message_pos(int dest);
		string creer_message_dec(int exclu);
		string creer_message_pos_monstre(int dest);
		perso * getPerso(int i);
		int getIndicePerso(string id);
		int connecte(char *host, char *table, char *user, char *pass);

	private :
		TCPsocket tcpsock_;
		TCPsocket tabsock_[_NB_MAX_CHAT_], tabSrvSock_[_NB_MAX_CHAT_];
		SDLNet_SocketSet set_;

		char msg_[_MAXLEN_];
		int nbCliC_;
		int nbSockActif_;
		int port_;

		perso * perso_[_NB_MAX_CHAT_];

		string lastId_;

		bool quit_;
		database BDDS;
};

#endif