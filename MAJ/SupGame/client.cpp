#include "client.hpp"
#include "outils.hpp"

extern char MSG_[_MAXLEN_];

using namespace std;

client::client(int port)
{
	this->port_ = port;
	this->tcpSock_ = NULL;
	this->set_ = SDLNet_AllocSocketSet(5);
}


// Se connecte à un serveur et lui envoie la position courante du perso
void client::connecter(JeuComplet *jeucomplet, string position )
{

	string st_ip;
	const char * ch_ip;
	char * nom;
	string msg;
	int numused, res;
	IPaddress ip;

	char str[512];


	
	jeucomplet->jeu.perso[_PERSO_LOCAL_].nom = nom_;

		printf("Entrer l'adresse ip du serveur ou faite enter pour utiliser 127.0.0.1\n");
		gets_s(str);
		if (str[0]==0){
			strcpy_s(str, "127.0.0.1");
		}	

		ch_ip=str;
	if(SDLNet_ResolveHost(&ip,ch_ip,this->port_)==-1) {
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(1);
	}

	this->tcpSock_ = SDLNet_TCP_Open(&ip);
	if(!this->tcpSock_) {
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(2);
	}

	// A ce stade la connection est établie (sauf erreur)

	// On ajoute le socket qui a servi à la connexion dans le set
	numused = SDLNet_TCP_AddSocket(this->set_, this->tcpSock_);
	if(numused==-1) {
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
	}	

	// Construction du message à envoyer au serveur pour lui dire bonjour, je suis un nouveau client
	msg = "N" + position;
	nom = strdup(msg.c_str());

	cout<<" envoie de : "<<nom<<endl;
	// On envoie le message
	res = SDLNet_TCP_Send(this->tcpSock_, nom , strlen(nom)+1);
}

/*
int client::envoyer_message(string msg); permet d'envoyer le message 'msg' au serveur sur 
lequel on est connecté.
Elle se charge de transformer la string en char * (et oui, SDLNet_TCP_Send demande un char * 
et d'envoyer le tout au serveur.
*/
int client::envoyer_message(string msg)
{
	const char * ch_msg;
	char * ch_msg2;
	int res=0, len=0;
	
	ch_msg = msg.c_str();
	ch_msg2 = strdup(ch_msg);
	len = strlen(ch_msg2)+1;
	//cout<<"envoyer_message :"<<ch_msg2<<endl;
	if(ch_msg2[0]=='N')
		cout<<"SDLNet_TCP_Send : "<<ch_msg2<<endl;
	res = SDLNet_TCP_Send(this->tcpSock_, ch_msg2, len);

	if(res<len){
		cout<<endl<<"ERREUR LORS DE L'ENVOI DU MESSAGE !!!"<<endl;
	}
	free(ch_msg2);

	return (0);
}

/*
void client::lire_messages(); va écouter le socket en attente de message et placer le contenu 
des messages reçus dans la variable globale MSG_.
Cette fonction est appelée dans un thread à part pour ne pas bloquer le reste du programme.
*/
void client::lire_messages()
{
	int res, numready;

		numready = SDLNet_CheckSockets(this->set_, -1);
		if(numready==-1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			//most of the time this is a system error, where perror might help you.
			perror("SDLNet_CheckSockets");
		} else if (numready>0) {
			if( SDLNet_SocketReady(this->tcpSock_) ){
				res = SDLNet_TCP_Recv(this->tcpSock_, MSG_, _MAXLEN_);
					//cout<<"lire_messages()  MSG_: "<<MSG_<<endl;
			/*	if(res>0)
					cout<<"recu : "<<MSG_<<endl;*/
			}
		}
}
