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

void outils::extraire_message(string msg, char typeMessage, string * nom, string * id, int *posx, int *posy)
{
	int i;

	switch(typeMessage){

		case _NOUVEAU_CLIENT_:		
			i = msg.find_first_of("!");
			(*nom) = msg.substr(1,i-1);
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

		default :
			break;
	}
}
