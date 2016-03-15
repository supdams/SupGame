// outils.hpp

#ifndef _OUTILS
#define _OUTILS

#include<iostream>
#include<string>
#include<sstream>

#include "constantes.hpp"

;
using namespace std;


class outils
{
	public :
		static string itos(int i);
		static string ctos(char * c);
		static string charToString(char c);
		static string creerId(string lastId);
		static void extraire_message(string msg, char typeMessage, string * nom, string * id, int *posx, int *posy);		
};

#endif