// outils.hpp

#ifndef _OUTILS
#define _OUTILS

#include<iostream>
#include<string>
#include<sstream>

#include "constantes.hpp"

;
using namespace std;

/*
Petite classe utilitaire permettant de faire qqes conversions bien pratiques.
*/
class outils
{
	public :
		static string itos(int i);
		static string ctos(char * c);
		static string charToString(char c);
		static string extraire_message( string msg, char typeMessage, string * nom, string * id, int * posx, int * posy );
};

#endif