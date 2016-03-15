// main.cpp

#include <string>
#include <iostream>
#include "perso.hpp"
#include "partie.hpp"
#include "main.hpp"
#include "load.hpp"
#include "menu.hpp"
#include "textedition.hpp"

#include "client.hpp"

#include "input.hpp"

#include <windows.h>


// Désolé pour les puristes mais g pas trouvé d'autre solution que d'utiliser
// MSG_ en variable globale pour communiquer facilement entre le thread et le
// reste du programme.
char MSG_[_MAXLEN_];

extern int TE_SetEditionText(TextEdition *te, const char text[]);

int main(int nbarg, char * tabarg[])
{
	if(SDLNet_Init()==-1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return(-2);
	}
	atexit(SDLNet_Quit);


	partie *p = new partie(&jeucomplet);

	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);


	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(TTF_Quit);


	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_Surface *bouton = NULL, *bouton2 = NULL;

	font[0] = loadFont("./font/Kraash_Black.ttf", 48);
	font[1] = loadFont("./font/GenBasB.ttf", 24);
	font[2] = loadFont("./font/Super_Mario_Bros.ttf", 32);
	font[3] = loadFont("./font/MORPHEUS.TTF", 48);
	font[4] = loadFont("./font/MORPHEUS.TTF", 18);
	font[5] = loadFont("./font/GenBasB.TTF", 18);
	font[6] = loadFont("./font/Kraash_Black.TTF", 16);
	font[7] = loadFont("verdana.ttf", 16);

	jeu.screen = SDL_SetVideoMode(800, 600, 32, SDL_HWPALETTE|SDL_DOUBLEBUF );

	if ( jeu.screen == NULL ) {
		fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}

	bouton = load_image("img/Image_432.png");
	bouton2 = load_image("img/Image_436.png");

	jeu.tmps = SDL_ConvertSurface(jeu.screen, jeu.screen->format, jeu.screen->flags);

	SDL_SetColorKey(jeu.tmps, SDL_SRCCOLORKEY, SDL_MapRGB(jeu.tmps->format, 255, 255, 255));

	int go = 1;
	SDL_Color Couleur_Noir;
	Couleur_Noir.r = 0;
	Couleur_Noir.g = 0;
	Couleur_Noir.b = 0;

	SDL_Event event;
	int done = 0;
	int testlogin =0;

	TextEdition pseudo;
	memset(&pseudo, 0, sizeof(TextEdition));
	pseudo.blitStyle = TE_BLITSTYLE_BLENDED;
	pseudo.colorBG = Couleur_Noir;

	SDL_Rect pos;
	pos.x = 305;
	pos.y = 250;
	pos.w = 200;
	pos.h = 30;
	TE_NewTextEdition(&pseudo, 20, pos, font[7], Couleur_Noir, TE_STYLE_NORMAL | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA);
	TE_SetEditionText(&pseudo, "admin");

	TextEdition mot_de_passe;
	memset(&mot_de_passe, 0, sizeof(TextEdition));
	mot_de_passe.blitStyle = TE_BLITSTYLE_BLENDED;
	mot_de_passe.colorBG = Couleur_Noir;

	SDL_Rect pos2;
	pos2.x = 305;
	pos2.y = 380;
	pos2.w = 200;
	pos2.h = 30;
	TE_NewTextEdition(&mot_de_passe, 20, pos2, font[7], Couleur_Noir, TE_STYLE_NORMAL | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA);
	TE_SetEditionText(&mot_de_passe, "admin");

	SDL_Rect destB2;
	destB2.x = 320;
	destB2.y = 550;
	SDL_BlitSurface(bouton , NULL , jeu.screen , &destB2);
	drawString("Valider", 350, 560, font[6],Couleur_Noir);

	if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
		fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

	SDL_Flip(jeu.screen);

	SDL_Rect posSouris;
	posSouris.x = 0;
	posSouris.y = 0;
	int clic=0;

	while (go == 1)
	{

		if (testlogin==0)
		{
			SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
			while (!done)
			{
				SDL_WaitEvent(&event);

				switch (event.type)
				{

				case SDL_QUIT:
					done = 1;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = 1;
					break;


				case SDL_MOUSEBUTTONDOWN :
					switch (event.button.button)
					{
					case SDL_BUTTON_LEFT :
						clic = 1;
						break;
					default :
						break;
					}
					break;
				case SDL_MOUSEBUTTONUP :
					clic = 0;
					break;

				case SDL_MOUSEMOTION:
					posSouris.x = event.motion.x;
					posSouris.y = event.motion.y;
					break;

				default:
					break;
				}

				if (!done)
				{
					SDL_FillRect(jeu.screen , NULL , SDL_MapRGB(jeu.screen->format, 255, 255, 255));
					SDL_FillRect(jeu.tmps , NULL , SDL_MapRGB(jeu.tmps->format, 255, 255, 255));
					SDL_FillRect(jeu.screen, &pos, SDL_MapRGB(jeu.screen->format, 255,0,0));
					SDL_FillRect(jeu.screen, &pos2, SDL_MapRGB(jeu.screen->format, 255,0,0));


					drawString("Thé Fan Oc", 250, 0, font[3],Couleur_Noir);
					drawString("page connexion jeu", 50, 90, font[4],Couleur_Noir);

					drawString("Pseudo:", 360, 220, font[5],Couleur_Noir);
					drawString("Mot de passe: ", 350, 350, font[5],Couleur_Noir);

					TE_HoldTextEdition(&pseudo, event);
					TE_DisplayTextEdition(&pseudo);

					TE_HoldTextEdition(&mot_de_passe, event);
					TE_DisplayTextEdition(&mot_de_passe);

					printf("pseudo: %s\n", pseudo.text);
					printf("mot_de_passe: %s\n", mot_de_passe.text);


					test_touche(&jeucomplet);

					AABB AB;
					AB.w = destB2.w;
					AB.h = destB2.h;
					AB.x = destB2.x;
					AB.y = destB2.y;

					SDL_Rect pos_bouton;
					pos_bouton.x = 320;
					pos_bouton.y = 550;

					if(AABBCollision(posSouris.x , posSouris.y , AB))/*On teste si on appuis sur le bouton */
					{
						SDL_BlitSurface(bouton2 , NULL , jeu.screen , &pos_bouton);
						drawString("Valider", 350, 560, font[5],Couleur_Noir);

						if(clic)/*Si on clic avec le clic gauche de la souris*/
						{
							string SPseudochaine(pseudo.text);
							string SMdpchaine(mot_de_passe.text);

							p->logger(SPseudochaine,SMdpchaine,&jeucomplet);
							p->traitement_complet_message(&jeucomplet);
							done = 1;
						}
					}else{

						SDL_BlitSurface(bouton , NULL , jeu.screen , &pos_bouton);
						drawString("Valider", 350, 560, font[5],Couleur_Noir);
					}

					if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
						fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

					SDL_Flip(jeu.screen);
				}

			}

			/////fin new text box
			if(p->logger_test(&jeucomplet)==0){


				SDL_Color foregroundColor;
				foregroundColor.r = 255;
				foregroundColor.g = 0;
				foregroundColor.b = 0;

				drawString("Login ou mot de passe incorrect ", 250, 450, font[4],foregroundColor);
				drawString("recommence dans 5 secondes!!!!", 250, 500, font[4],foregroundColor);

				if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
					fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

				SDL_Flip(jeu.screen);
				SDL_Delay(3000);
				done=0;
			}
		}
	}

	return 0;
}