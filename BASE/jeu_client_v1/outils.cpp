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



string outils::extraire_message(string msg, char typeMessage, string * nom, string * id, int *posx, int *posy)
{
	int i;

	i=0;

	switch(typeMessage){

		case _NOUVEAU_CLIENT_:			
			i = msg.find_first_of("!");
			(*nom) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			(*id) = msg.substr(0,3);
			(*posx) = atoi( msg.substr(3,3).c_str() ) ;		
			(*posy) = atoi( msg.substr(6,3).c_str() ) ;
			msg = msg.substr(10,msg.length());
			return msg;
			break;

		case _BIENVENUE_ID_:
			(*id) = msg.substr(1,3);
			msg = msg.substr(4, msg.length());
			return msg;
			break;

		case _MESSAGE_NORMAL_:
			(*id) = msg.substr(1,3);
			(*posx) = atoi( msg.substr(4,3).c_str() ) ;	
			(*posy) = atoi( msg.substr(7,3).c_str() ) ;
			msg = msg.substr(10, msg.length());
			return msg;
			break;

		case _DECONNEXION_CLIENT_:
			(*id) = msg.substr(1,3);
			msg = msg.substr(4, msg.length());
			return msg;
			break;

		default :
			break;
	}
}