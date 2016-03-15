//serveur.cpp

#include "serveur.hpp"
#include "database.hpp"


serveur::serveur(int port)
{
	this->port_ = port;
	this->tcpsock_ = NULL;

	for(int i=0 ; i<_NB_MAX_CHAT_ ; i++){
		this->tabsock_[i] = NULL;
		this->perso_[i] = NULL;
	}

	this->nbCliC_ = 0;
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

int serveur::connecte(char *host, char *table, char *user, char *pass)
{
	int errormsg = 0; //catch any database errors
	// dbConnection = true; //connection started, toggle flag

	//connect to the database
	errormsg = BDDS.openConnection(host, table, user, pass);

	cout<<"sql: "<<errormsg<< endl;    
	//return the database message
	return errormsg;
}

void serveur::alloc_monstre(JeuComplet *jeucomplet)
{	
	cout<<"alloc_monstre^^^^^^^^^^^^"<< endl; 

	if (connecte(HOST, DATABASE, USER, PASS) == SUCCESS)
	{
		cout<<"connect alloc_monstre"<< endl; 

		char query[500];
		sprintf(query, "SELECT * FROM creature");
		cout<<"SQL:"<<query<< endl; 

		if (BDDS.query(query))
		{
			jeucomplet->jeu.nbMonstre =BDDS.NumRows();
			jeucomplet->jeu.monstre = (Monstre*)malloc(jeucomplet->jeu.nbMonstre * sizeof(Monstre));
		} 
	}
}


void serveur::lire_monstre(JeuComplet *jeucomplet)
{
	cout<<"lire monstre ^^"<<endl;
	char query4[500];
	sprintf(query4, "SELECT * FROM creature");
	cout<<"SQL:"<<query4<< endl; 

	MYSQL_ROW row=NULL;
	int c=0;

	while ((row = BDDS.GetRow()) != NULL)
	{

		cout
			<<" c :"<<c
			<<" id :"<<row[0]
		<<" map :"<<row[1]
		<<" modelid :"<<row[2]
		<<" nom :"<<row[3]
		<<" position_x :"<<row[4]
		<<" position_y :"<<row[5]
		<<" spawntimesecs :"<<row[6]
		<<" hp :"<<row[7]
		<<" mindmg :"<<row[8]
		<<" maxdmg :"<<row[9]
		<<" DeathState :"<<row[10]<< endl;

		jeucomplet->jeu.monstre[c].id= atoi(row[0]);
		jeucomplet->jeu.monstre[c].map= atoi(row[1]);
		jeucomplet->jeu.monstre[c].modelid= atoi(row[2]);
		jeucomplet->jeu.monstre[c].nom_monstre= row[3];
		jeucomplet->jeu.monstre[c].position_x= atoi(row[4]);
		jeucomplet->jeu.monstre[c].position_y= atoi(row[5]);
		jeucomplet->jeu.monstre[c].spawntimesecs= atoi(row[6]);
		jeucomplet->jeu.monstre[c].hp= atoi(row[7]);
		jeucomplet->jeu.monstre[c].mindmg= atoi(row[8]);
		jeucomplet->jeu.monstre[c].maxdmg= atoi(row[9]);
		jeucomplet->jeu.monstre[c].DeathState= atoi(row[10]);
		jeucomplet->jeu.monstre[c].compteurMonstreBouge=10;
		jeucomplet->jeu.monstre[c].animMonstre.orientation=0;

		put_anim_monstre(c, jeucomplet->jeu.monstre[c].position_x , jeucomplet->jeu.monstre[c].position_y , jeucomplet , "img/jeu/monstre/monstre1.png");
		c++;
	}
}


void serveur::new_maj_monstre(JeuComplet *jeucomplet)
{
	string tmp_data;
	string data="M"+ outils::itos(jeucomplet->jeu.nbMonstre)+ outils::charToString(_FIN_MESSAGE_);
	for(int x = 0 ; x < jeucomplet->jeu.nbMonstre;)
	{
		switch(jeucomplet->jeu.monstre[x].animMonstre.orientation)
		{
		case BAS :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;
		case HAUT :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;
		case DROITE :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;
		case GAUCHE :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;
		case AUCUNE_DIRECTION :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;
		case AUCUNE_DIRECTION1 :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;
		case AUCUNE_DIRECTION2 :
			tmp_data  =outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.dest.y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.monstre[x].animMonstre.orientation) + outils::charToString(_FIN_MESSAGE_);
			break;

		default :
			break;
		}

		data+= tmp_data;
		x++;

	}

	if (data!="")
	{
		char * env;
		int len, i, res;
		string msg="", msg2="";

		for(i=0 ; i<_NB_MAX_CHAT_ ; i++){
			if(this->tabsock_[i]){
				msg = data;
				env = strdup(msg.c_str());
				len = strlen(env)+1;
				res=SDLNet_TCP_Send(this->tabsock_[i], env, len);
				free(env);// a verif
			}
		}
	}
}

void serveur::lancer(JeuComplet *jeucomplet)
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
			//cout<<"nbSockActif_: "<<nbSockActif_<<endl;

			if(nbSockActif_==-1)    // si c une demande de connexion
			{
				cout<<"rien buggg nbSockActif_: "<<nbSockActif_<<"  SDLNet_GetError: "<<SDLNet_GetError()<<endl;
			}

			if( SDLNet_SocketReady(tcpsock_) )    // si c une demande de connexion
			{
				this->nouvelle_connexion(jeucomplet);
			}
			else		// Sinon c un envoie de données
			{
				for(cpt=0 ; cpt < _NB_MAX_CHAT_ ; cpt++)         // On balaie la liste des connectés
				{

					if( SDLNet_SocketReady(tabsock_[cpt]) )
					{
						result=SDLNet_TCP_Recv(tabsock_[cpt], msg_, _MAXLEN_);
						msg = outils::ctos(msg_);
						//cout<<"BUG 1  result: "<<result<<" cpt: "<<cpt<<" msg: "<<msg<<endl;

						cout<<"RECU msg: "<<msg<<endl;
						string flag ="";
						(flag) = msg.substr(0,1);


						if(result <= 0) {// Il y a eut une erreur (deco)
							cout<<"SDLNet_GetError: " << SDLNet_GetError() << endl;
							//cout<<"BUG result ?  result: "<<result<<" cpt: "<<cpt<<endl;

							this->deconnexion(jeucomplet,cpt);

						} else {

							if(flag == "L") {

								traiter_message(jeucomplet,_MESSAGE_CONNECTION_, msg, cpt); 
							}else if(flag == "G") {

								traiter_message(jeucomplet,_MESSAGE_GUID_CONNECT_, msg, cpt);
							}else if(flag == "R") {

								traiter_message(jeucomplet,_MESSAGE_CREATION_PERSO_, msg, cpt);
							}else if(flag == "C") {

								traiter_message(jeucomplet,_MESSAGE_CREATURE_INFO_, msg, cpt);
							}else {
								//cout<<this->tabNom_[cpt]<< " --> " << msg_ <<endl;
								//this->tabMsg_[cpt] = strdup(this->msg_);
								traiter_message(jeucomplet,_MESSAGE_NORMAL_, msg, cpt);
								//cout<<"BUG 10 "<<endl;
							}
						}
					}
				}
			}
		}

		if( time_left(next_time)<1 )
		{	
			load_valeur_scrool(jeucomplet);
			update_dest_scroll_monstre(jeucomplet);
			new_maj_monstre(jeucomplet);
			SDL_Delay(5);
			update_monstres(jeucomplet);
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
		cout<<"message pour tous les autres --> "<<msg<<endl;
		env = strdup(msg.c_str());
		len = strlen(env)+1;

		msg2 = this->creer_message_nvp(exclu, 1);
		cout<<"message pour le nouveau --> "<<msg2<<endl;
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
				cout<<"envoie pour l envoyeur "<<i<<" -->"<<env2<<endl;
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
		SDL_Delay(5);
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
		SDL_Delay(5);
		break;



	case _MESSAGE_CONNECTION_:

		for(i=0 ; i<_NB_MAX_CHAT_ ; i++){

			if(i==exclu) {					

				cout<<"envoyer_message _MESSAGE_CONNECTION_("<<type_message<<",acces_: "<<this->perso_[i]->acces_<<")"<<endl;

				msg = "L" + this->perso_[i]->acces_ + "!";
				env2 = strdup(msg.c_str());
				len2 = strlen(env2)+1;


				res = SDLNet_TCP_Send(this->tabsock_[exclu], env2, len2);
				cout<<"envoie pour l envoyeur i-->"<<i<<" env2-->"<<env2<<endl;
				if(res<len2){
					printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				}
			}
		}
		SDL_Delay(5);
		break;

	case _MESSAGE_GUID_CONNECT_:

		for(i=0 ; i<_NB_MAX_CHAT_ ; i++){

			if(i==exclu) {					

				cout<<"envoyer_message _MESSAGE_GUID_CONNECT_("<<type_message<<",acces_: "<<this->perso_[i]->acces_<<")"<<endl;

				msg = "G" + this->perso_[i]->acces_ ;
				env2 = strdup(msg.c_str());
				len2 = strlen(env2)+1;


				res = SDLNet_TCP_Send(this->tabsock_[exclu], env2, len2);
				cout<<"envoie pour l envoyeur i-->"<<i<<" env2-->"<<env2<<endl;
				if(res<len2){
					printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				}
			}
		}
		SDL_Delay(5);
		break;

	case _MESSAGE_CREATION_PERSO_:

		for(i=0 ; i<_NB_MAX_CHAT_ ; i++){

			if(i==exclu) {					

				cout<<"envoyer_message _MESSAGE_CREATION_PERSO_("<<type_message<<",acces_: "<<this->perso_[i]->acces_<<")"<<endl;

				msg = "R" + this->perso_[i]->acces_ ;
				env2 = strdup(msg.c_str());
				len2 = strlen(env2)+1;


				res = SDLNet_TCP_Send(this->tabsock_[exclu], env2, len2);
				cout<<"envoie pour l envoyeur i-->"<<i<<" env2-->"<<env2<<endl;
				if(res<len2){
					printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				}
			}
		}
		SDL_Delay(5);
		break;

	case _MESSAGE_CREATURE_INFO_:

		for(i=0 ; i<_NB_MAX_CHAT_ ; i++){

			if(i==exclu) {					

				cout<<"envoyer_message _MESSAGE_CREATURE_INFO_("<<type_message<<",acces_: "<<this->perso_[i]->acces_<<")"<<endl;

				msg = "C" + this->perso_[i]->acces_ ;
				env2 = strdup(msg.c_str());
				len2 = strlen(env2)+1;


				res = SDLNet_TCP_Send(this->tabsock_[exclu], env2, len2);
				cout<<"envoie pour l envoyeur i-->"<<i<<" env2-->"<<env2<<endl;
				if(res<len2){
					printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				}
			}
		}
		SDL_Delay(5);
		break;

	case _MESSAGE_CREATURE_MAJ_:			
		SDL_Delay(5);
		break;

	default:
		break;
	}
}


void serveur::traiter_message(JeuComplet *jeucomplet, char type_message, string msg, int envoyeur)
{	
	string nom="", id="", race="", pseudo="", mdp="", guide="";
	int posx, posy, i;

	//cout<<"traiter_message: "<<type_message<<endl;


	switch(type_message){

	case _NOUVEAU_CLIENT_:
		outils::extraire_message(msg, _NOUVEAU_CLIENT_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);
		cout<<"Nouveau client connecte"<<endl;
		this->lastId_ = outils::creerId(this->lastId_);
		nom="";
		posx=0;
		posy=0;
		this->perso_[envoyeur] = new perso(nom, posx, posy, this->lastId_,race);
		envoyer_message(_NOUVEAU_CLIENT_, envoyeur);
		break;


	case _DECONNEXION_CLIENT_:
		outils::extraire_message(msg, _DECONNEXION_CLIENT_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);
		i = this->getIndicePerso(id);
		delete this->perso_[i];
		this->perso_[i]=NULL;
		break;


	case _MESSAGE_NORMAL_:
		//cout<<"traiter_message(NORMAL) recoit msg --> "<<msg<<endl;
		outils::extraire_message(msg, _MESSAGE_NORMAL_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);
		//cout<<id<<" "<<posx<<" "<<posy<<endl;
		this->perso_[envoyeur]->update_position(posx, posy);
		break;

	case _MESSAGE_CONNECTION_:
		outils::extraire_message(msg, _MESSAGE_CONNECTION_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);


		this->perso_[envoyeur]->load_login(pseudo, mdp);
		cout<<"traiter_message acces_: "<<this->perso_[envoyeur]->acces_<<endl;


		envoyer_message(_MESSAGE_CONNECTION_, envoyeur);
		cout<<"_MESSAGE_CONNECTION_  pseudo: "<<pseudo<<" , mdp: "<<mdp<<endl;
		break;


	case _MESSAGE_GUID_CONNECT_:
		outils::extraire_message(msg, _MESSAGE_GUID_CONNECT_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);

		this->perso_[envoyeur]->acces_ = "";
		this->perso_[envoyeur]->connect_perso(guide);

		cout<<"_MESSAGE_GUID_CONNECT_ traiter_message acces_: "<<this->perso_[envoyeur]->acces_<<endl;		

		envoyer_message(_MESSAGE_GUID_CONNECT_, envoyeur);



		this->perso_[envoyeur]->acces_ = "";
		this->perso_[envoyeur]->new_liste_creature(guide, jeucomplet);// map

		cout<<"_MESSAGE_CREATURE_INFO_ traiter_message acces_: "<<this->perso_[envoyeur]->acces_<<endl;	
		cout<<"envoyeur: "<<envoyeur<<endl;	
		envoyer_message(_MESSAGE_CREATURE_INFO_, envoyeur);


		break;


	case _MESSAGE_CREATION_PERSO_:
		outils::extraire_message(msg, _MESSAGE_CREATION_PERSO_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);

		this->perso_[envoyeur]->acces_ = "";
		this->perso_[envoyeur]->creation_perso(pseudo,guide);
		this->perso_[envoyeur]->ecrire_acces_="";

		this->perso_[envoyeur]->ecrire_acces_=this->perso_[envoyeur]->acces_;
		cout<<"!!!!!! guide: "<<guide<<endl;		


		this->perso_[envoyeur]->acces_ = "";
		this->perso_[envoyeur]->recharger_liste_perso(guide);


		this->perso_[envoyeur]->acces_ = this->perso_[envoyeur]->ecrire_acces_ + this->perso_[envoyeur]->acces_;
		cout<<"!!!!!!_MESSAGE_CREATION_PERSO_ traiter_message nouveau acces_: "<<this->perso_[envoyeur]->acces_<<endl;		

		envoyer_message(_MESSAGE_CREATION_PERSO_, envoyeur);


		break;

	case _MESSAGE_CREATURE_INFO_:
		outils::extraire_message(msg, _MESSAGE_CREATURE_INFO_, &nom, &id, &posx, &posy, &race, &pseudo, &mdp, &guide);

		this->perso_[envoyeur]->acces_ = "";
		this->perso_[envoyeur]->new_liste_creature(guide, jeucomplet);// map

		cout<<"_MESSAGE_CREATURE_INFO_ traiter_message acces_: "<<this->perso_[envoyeur]->acces_<<endl;		

		envoyer_message(_MESSAGE_CREATURE_INFO_, envoyeur);


		break;

	default:
		break;
	}
}


/*
void serveur::nouvelle_connexion(); gère la connexion d'un nouveau client sur le serveur.
*/
void serveur::nouvelle_connexion(JeuComplet *jeucomplet)
{
	int cpt, numaj, result;
	string msg;

	for(cpt=0 ; cpt<_NB_MAX_CHAT_ && this->tabsock_[cpt] ; cpt++); // On cherche un socket de libre				

	if(  cpt<_NB_MAX_CHAT_ ){			
		this->tabsock_[cpt] = SDLNet_TCP_Accept(this->tcpsock_);
		numaj = SDLNet_TCP_AddSocket(this->set_, this->tabsock_[cpt]);						
		result=SDLNet_TCP_Recv(this->tabsock_[cpt], this->msg_, _MAXLEN_);
		cout<<"RECU msg: "<<this->msg_<<endl;

		if(result>0){
			msg = outils::ctos(this->msg_);
			//cout<<"nouvelle_connexion() msg_ --> "<<msg_<<"cpt --> "<<cpt<<endl;
			//traiter_message(_MESSAGE_CONNECTION_, msg, cpt);// avan _NOUVEAU_CLIENT_
			traiter_message(jeucomplet,_NOUVEAU_CLIENT_, msg, cpt);// avan _NOUVEAU_CLIENT_			
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
void serveur::deconnexion(JeuComplet *jeucomplet,int numCli)
{
	cout<<"BUG deco "<<endl;
	cout<<"###### Deconnexion de "<<numCli<< endl;

	envoyer_message(_DECONNEXION_CLIENT_, numCli);
	cout<<"BUG ###### Deconnexion de "<<numCli<< endl;
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
				// 1 er separateur de perso pour le bug des nom ...
				ret = ret + outils::charToString(_SEPARATEUR_PERSO_) + p->getNom() + "!" + p->getRace() + "!" + p->getId() + p->getPos() + outils::charToString(_SEPARATEUR_PERSO_);
			}
		}
		ret += outils::charToString(_FIN_MESSAGE_);

		// else = il faut juste indiquer l'arrivée du nouveau client (message destiné aux autres)
	} else {
		ret = outils::charToString(_NOUVEAU_CLIENT_);
		p = this->perso_[exclu];
		ret = ret + p->getNom() + "!" + p->getRace() + "!" + p->getId() + p->getPos()+ outils::charToString(_FIN_MESSAGE_);
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

string serveur::creer_message_pos_monstre(int dest)
{
	string ret = " ";
	perso * p;
	int i;

	ret = outils::charToString(_MESSAGE_CREATURE_MAJ_);

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


