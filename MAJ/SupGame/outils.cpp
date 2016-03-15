// outils.cpp

#include "outils.hpp"

int AABBCollision(int curseur_x ,int curseur_y ,AABB box) /*Voit si il y a eu collision avec une AABB (clic bouton)*/
{
    if (curseur_x >= box.x && curseur_x < box.x + box.w
            && curseur_y >= box.y && curseur_y < box.y + box.h)
        return 1;
    else
        return 0;
}

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


string outils::extraire_message(string msg, char typeMessage, string * nom, string * id, int *posx, int *posy, string * race, string * acces, string * acces_suite)
{
	int i;
	string debug ="";
	i=0;

		//cout<<"extraire_message --> '"<<typeMessage<<"'"<<"msg --> '"<<msg<<"'"<<endl;
	switch(typeMessage){

		case _NOUVEAU_CLIENT_:			
			//cout<<"_NOUVEAU_CLIENT_ --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*nom) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());


			
		//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 2eme ! la race du perso
			(*race) = msg.substr(0,1);
			msg = msg.substr(i+1, msg.length());

			(*id) = msg.substr(0,3);
			(*posx) = atoi( msg.substr(3,3).c_str() ) ;		
			(*posy) = atoi( msg.substr(6,3).c_str() ) ;
			msg = msg.substr(10,msg.length());
			return msg;
			break;

		case _BIENVENUE_ID_:
			//cout<<"_BIENVENUE_ID_ --> '"<<msg<<"'"<<endl;
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
			
		case _MESSAGE_CONNECTION_:
			//(*id) = msg.substr(1,3);
			//msg = msg.substr(4, msg.length());
			
			cout<<"_MESSAGE_CONNECTION_  tous--> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*acces) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			cout<<"_MESSAGE_CONNECTION_ decouper --> '"<<msg<<"'"<<endl;



			//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 2eme ! les info des perso
			(*acces_suite) = msg;
			msg = msg.substr(i+1, msg.length());

			return msg;
			break;
			
		case _MESSAGE_GUID_CONNECT_:
			//(*id) = msg.substr(1,3);
			//msg = msg.substr(4, msg.length());
			
			cout<<"_MESSAGE_GUID_CONNECT_  tous--> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*acces) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			cout<<"_MESSAGE_GUID_CONNECT_ decouper --> '"<<msg<<"'"<<endl;



			//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 2eme ! les info des perso
			(*acces_suite) = msg;
			msg = msg.substr(i+1, msg.length());

			return msg;
			break;


		case _MESSAGE_CREATION_PERSO_:
			cout<<"_MESSAGE_CREATION_PERSO_  tous--> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("$");// 1er ! le nom du perso
			(*acces) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			cout<<"_MESSAGE_CREATION_PERSO_ decouper --> '"<<msg<<"'"<<endl;



			//	cout<<"_NOUVEAU_CLIENT_suite --> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 2eme ! les info des perso

			
		 debug = msg.substr(0,1)  ;
		msg = msg.substr(i+1, msg.length());




			(*acces_suite) = msg;
			msg = msg.substr(i+1, msg.length());

			return msg;
			break;

		case _MESSAGE_CREATURE_INFO_:
			//(*id) = msg.substr(1,3);
			//msg = msg.substr(4, msg.length());
			
			cout<<"_MESSAGE_CREATURE_INFO_  tous--> '"<<msg<<"'"<<endl;
			i = msg.find_first_of("!");// 1er ! le nom du perso
			(*acces) = msg.substr(1,i-1);
			msg = msg.substr(i+1, msg.length());
			
			return msg;
			break;

		default :
			break;
	}
}



void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor)
{
	SDL_Rect dest;
	SDL_Surface *tmps;
//	SDL_Color foregroundColor;


	/* On utilise SDL_TTF pour générer une SDL_Surface à partir d'une chaîne de caractères (string) */
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);

	//tmps = TTF_RenderUTF8_Blended(font, text, foregroundColor);
 

	tmps = TTF_RenderText_Blended (font, text, foregroundColor);

	if (tmps == NULL)
	{
		printf("Couldn't create String %s: %s\n", text, SDL_GetError());
		return;
	}

	/* On blitte cette SDL_Surface à l'écran */
	dest.x = x;
	dest.y = y;
	dest.w = tmps->w;
	dest.h = tmps->h;
		if(SDL_BlitSurface(tmps, NULL, jeu.screen, &dest) < 0)
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());


		
	// Mise à jour de l'écran ... le tour est joué !
	//SDL_UpdateRect(jeu.screen, dest.x, dest.y, dest.w,dest.h);

	/* On libère la SDL_Surface temporaire (pour éviter les fuites de mémoire - cf. chapitre dédié) */
	SDL_FreeSurface(tmps);

}

void drawString2(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor)
{
	SDL_Rect dest;
	SDL_Surface *tmps;
//	SDL_Color foregroundColor;


	/* On utilise SDL_TTF pour générer une SDL_Surface à partir d'une chaîne de caractères (string) */
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);

	//tmps = TTF_RenderUTF8_Blended(font, text, foregroundColor);
 

	tmps = TTF_RenderText_Blended (font, text, foregroundColor);

	if (tmps == NULL)
	{
		printf("Couldn't create String %s: %s\n", text, SDL_GetError());
		return;
	}

	/* On blitte cette SDL_Surface à l'écran */
	dest.x = x;
	dest.y = y;
	dest.w = tmps->w;
	dest.h = tmps->h;
		if(SDL_BlitSurface(tmps, NULL, jeu.tmps, &dest) < 0)
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());


		
	// Mise à jour de l'écran ... le tour est joué !
	//SDL_UpdateRect(jeu.screen, dest.x, dest.y, dest.w,dest.h);

	/* On libère la SDL_Surface temporaire (pour éviter les fuites de mémoire - cf. chapitre dédié) */
	SDL_FreeSurface(tmps);

}

void drawImage(SDL_Surface *image, int x, int y)
{
	SDL_Rect dest;

	/* Règle le rectangle à blitter selon la taille de l'image source */

	dest.x = x;
	dest.y = y;
	dest.w = image->w;
	dest.h = image->h;

	/* Blitte l'image entière sur l'écran aux coordonnées x et y */

	SDL_BlitSurface(image, NULL, jeu.screen, &dest);
}

