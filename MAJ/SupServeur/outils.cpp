// outils.cpp

#include "outils.hpp"

string outils::itos(int i)
{
	stringstream s;
	s << i;
	return s.str();
}

string outils::ctos(char * c)
{
	string ret;
	stringstream s;

	s << c;
	ret = s.str();
	return ret;
	
}

string outils::charToString(char c)
{
	string ret;
	stringstream s;

	s << c;
	ret = s.str();
	return ret;
	
}



string outils::creerId(string lastId)
{
	
	char *cid;
	int len, idc;
	
	cid = strdup(lastId.c_str());
	len = strlen(cid)-1;	
	
	idc = cid[len];
	idc++;
	
	while(idc>127){
		cid[len]=48;
		len--;
		if(len<0){
			cout<<"Nombre max d'id atteint ..."<<endl;
			return lastId;
		}
		idc = cid[len];
		idc++;
	}
	cid[len]=idc;
	
	lastId = ctos(cid);
	return lastId;
}

void outils::extraire_message(string msg, char typeMessage, string * nom, string * id, int *posx, int *posy, string * race, string * pseudo, string * mdp, string * guide)
{
	int i;
	
			cout<<"extraire_message --> '"<<typeMessage<<"'"<<endl;
	switch(typeMessage){

		case _NOUVEAU_CLIENT_:		
			//cout<<"_NOUVEAU_CLIENT_ --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*nom) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());

			//cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 2eme ! la race du perso
			(*race) = msg.substr(0,1);
			msg = msg.substr(i+1, msg.length());						
			//cout<<"posx ? --> '"<<msg.substr(1,3)<<"'"<<endl;
			//cout<<"posy ? --> '"<<msg.substr(4,3)<<"'"<<endl;
			(*posx) = atoi( msg.substr(1,3).c_str() ) ;
			(*posy) = atoi( msg.substr(4,3).c_str() ) ;
			return;
			break;

		case _DECONNEXION_CLIENT_:
			(*id) = msg.substr(1,3);
			return;
			break;

		case _MESSAGE_NORMAL_:
//			cout<<"msg ..."<<msg<<endl;
			(*id) = msg.substr(1,3);
			(*posx) = atoi( msg.substr(4,3).c_str() ) ;	
			(*posy) = atoi( msg.substr(7,3).c_str() ) ;
			return;
			break;

		case _MESSAGE_CONNECTION_:
			//cout<<"_MESSAGE_CONNECTION_ --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*pseudo) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());

			//cout<<"_MESSAGE_CONNECTION_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 2eme ! la race du perso
			(*mdp) = msg.substr(0,i);
			msg = msg.substr(i+1, msg.length());
			return;
			break;

		case _MESSAGE_GUID_CONNECT_:
			cout<<"_MESSAGE_GUID_CONNECT_ --> '"<<msg<<"'"<<endl;

			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*guide) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			return;
			break;

		case _MESSAGE_CREATION_PERSO_:
			cout<<"_MESSAGE_CREATION_PERSO_ --> '"<<msg<<"'"<<endl;

			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*pseudo) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			//cout<<"_MESSAGE_CONNECTION_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");//acc
			(*guide) = msg.substr(0,i);// acc
			msg = msg.substr(i+1, msg.length());
			return;
			break;

			


		default :
			break;
	}
}
