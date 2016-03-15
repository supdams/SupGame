
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
using namespace std;

#include "box.hpp"

#include "struc.hpp"
#include<string>
#include<sstream>

Gestion jeu;
TTF_Font *font[7];
Touche touche;
SDL_Surface *box_choix_perso = NULL,*bouton = NULL,*bouton2 = NULL, *image_perso1 = NULL, *image_perso2 = NULL, *image_perso3 = NULL;

void init ()
{

}
void affiche()
{


}


void pause()
{
	int continuer = 1;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
		case SDL_QUIT:
			continuer = 0;
		}
	}
}


int main(int argc, char *argv[])
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	/* Initialise SDL_TTF */
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}

	font[0] = loadFont("./font/Kraash_Black.ttf", 48);
	font[1] = loadFont("./font/GenBasB.ttf", 24);
	font[2] = loadFont("./font/Super_Mario_Bros.ttf", 32);
	font[3] = loadFont("./font/MORPHEUS.TTF", 48);
	font[4] = loadFont("./font/MORPHEUS.TTF", 18);
	font[5] = loadFont("./font/GenBasB.TTF", 18);
	font[6] = loadFont("./font/Kraash_Black.TTF", 16);

	atexit(SDL_Quit);

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	// Initialisation du mode vidéo, on peut remplacer SDL_HWSURFACE par SDL_SWSURFACE
	// pour créer la surface en mémoire system au lieu d'utiliser la mémoire vidéo.
	// L'inconvéniant de SDL_HWSURFACE c'est qd plein écran mon image scintille :(.	
	jeu.screen = SDL_SetVideoMode(800, 600, 32, SDL_HWPALETTE|SDL_DOUBLEBUF );

	if ( jeu.screen == NULL ) {
		fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
			SDL_GetError());
		exit(1);
	}
	memset(&touche , 0 , sizeof(Touche));/*Toute les variable de la structure Touche sont miss à 0*/

	jeu.tmps = SDL_ConvertSurface(jeu.screen, jeu.screen->format, jeu.screen->flags);

	SDL_SetColorKey(jeu.tmps, SDL_SRCCOLORKEY, SDL_MapRGB(jeu.tmps->format, 255, 255, 255));

	int go;
	go=1;

	int afficher;
	afficher=0;

	int text;
	text =0;

	box_choix_perso = load_image("img/box-choix-perso.png");

	bouton = load_image("img/Image_432.png");
	bouton2 = load_image("img/Image_436.png");

	image_perso1 = load_image("img/jeu/heros/Magriavs/perso_marche1.png");
	image_perso2 = load_image("img/jeu/heros/Roukrses/perso_marche1.png");
	image_perso3 = load_image("img/jeu/heros/Skrills/perso_marche1.png");
		char Pseudochaine[100] = "";
				char Mdpchaine[100] = "";
		
				lireDonneeZoneTexte(jeu.tmps,font[5],Pseudochaine,"Pseudo : ","",99,0,0);

	while (go == 1)
	{

		SDL_FillRect(jeu.screen , NULL , SDL_MapRGB(jeu.screen->format, 255, 255, 255));/*On rempli l'écran de noir*/
		SDL_FillRect(jeu.tmps , NULL , SDL_MapRGB(jeu.tmps->format, 255, 255, 255));/*On rempli l'écran de noir*/
		//cout<<"touche.posSouris.x:"<<touche.posSouris.x<<" touche.posSouris.y:"<<touche.posSouris.y<<endl;
		test_touche();

		//affiche();
		SDL_Color foregroundColor;

		foregroundColor.r = 0;
		foregroundColor.g = 0;
		foregroundColor.b = 0;


		////////////////1

		drawString("Thé Fan Oc", 250, 0, font[3],foregroundColor);


		drawString("Création de perso :", 50, 90, font[4],foregroundColor);
		drawString("1: choisi une classe clic decu ", 50, 120, font[5],foregroundColor);
		drawString("2: puis clique sur le bouton valider pour choisir cette classe", 50, 140, font[5],foregroundColor);



		drawString("Magriavs", 100, 180, font[4],foregroundColor);
		SDL_Rect dest1;
		dest1.x = 80;
		dest1.y = 200;

		SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest1);	


		SDL_Rect desti2;
		desti2.x = 130;
		desti2.y = 240;

		SDL_Rect source2;
		source2.w = 32;
		source2.h = 48;
		source2.x = 32;
		source2.y = 48;
		SDL_BlitSurface(image_perso1 , &source2 , jeu.tmps , &desti2);	// le perso 1

		//////////////:fin 1

		drawString("Roukrses", 360, 180, font[4],foregroundColor);
		SDL_Rect dest3;
		dest3.x = 340;
		dest3.y = 200;
		SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest3);

		SDL_Rect desti4;
		desti4.x = 390;
		desti4.y = 240;

		SDL_Rect source4;
		source4.w = 32;
		source4.h = 48;
		source4.x = 32;
		source4.y = 48;
		SDL_BlitSurface(image_perso2 , &source4 , jeu.tmps , &desti4);		// le perso 2	
		//////////////:fin 2

		drawString("Skrills", 620, 180, font[4],foregroundColor);

		SDL_Rect dest5;
		dest5.x = 600;
		dest5.y = 200;
		SDL_BlitSurface(box_choix_perso , NULL , jeu.tmps , &dest5);	


		SDL_Rect desti6;
		desti6.x = 650;
		desti6.y = 240;


		SDL_Rect source6;
		source6.w = 32;
		source6.h = 48;
		source6.x = 32;
		source6.y = 48;
		SDL_BlitSurface(image_perso3 , &source6 , jeu.tmps , &desti6);// le perso 2




		// if clic ^^


		// 1 box
		AABB AB;
		AB.w = dest1.w;
		AB.h = dest1.h;
		AB.x = dest1.x;
		AB.y = dest1.y;

		if(AABBCollision(touche.posSouris.x , touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{
			if(touche.clic)/*Si on clic avec le clic gauche de la souris*/
			{
				text=1;
			}

		}
		// 2 box
		AB.w = dest3.w;
		AB.h = dest3.h;
		AB.x = dest3.x;
		AB.y = dest3.y;

		if(AABBCollision(touche.posSouris.x , touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{			
			if(touche.clic)/*Si on clic avec le clic gauche de la souris*/
			{
				text=2;
			}
		}

		// 3 box
		AB.w = dest5.w;
		AB.h = dest5.h;
		AB.x = dest5.x;
		AB.y = dest5.y;

		if(AABBCollision(touche.posSouris.x , touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
		{
			if(touche.clic)/*Si on clic avec le clic gauche de la souris*/
			{
				text=3;
			}
		}

		if(text==1)
		{
			text==1;
			drawString("Magriavs", 150, 350, font[4],foregroundColor);
			drawString("Info: Magriavs", 350, 380, font[5],foregroundColor);
		}
		else if(text==2)
		{
			text==1;
			drawString("Roukrses", 150, 350, font[4],foregroundColor);
			drawString("Info: Roukrses", 350, 380, font[5],foregroundColor);
		}
		else if(text==3)
		{
			text==3;
			drawString("Skrills", 150, 350, font[4],foregroundColor);
			drawString("Info: Skrills", 350, 380, font[5],foregroundColor);
		}else if(text==4)
		{
			text==3;
			drawString("CREATIN ENVOYER", 150, 350, font[4],foregroundColor);
			drawString("text 1PPPPPPPPPPPPPPP", 350, 380, font[5],foregroundColor);
		}else
		{
			text==0;
			drawString("Clic sur une classe pour afficher les detail", 150, 350, font[6],foregroundColor);
		}



		if(text==1||text==2||text==3)
		{
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 550;
			SDL_BlitSurface(bouton2 , NULL , jeu.tmps , &destB2);
			drawString("Valider", 350, 560, font[6],foregroundColor);


			// 2 box
			AB.w = destB2.w;
			AB.h = destB2.h;
			AB.x = destB2.x;
			AB.y = destB2.y;

			if(AABBCollision(touche.posSouris.x , touche.posSouris.y , AB))/*On teste si on appuis sur le bouton */
			{			
				if(touche.clic)/*Si on clic avec le clic gauche de la souris*/
				{
					text=4;
				}
			}

		}else{
			///////////// bouton valider
			SDL_Rect destB2;
			destB2.x = 320;
			destB2.y = 550;
			SDL_FillRect(jeu.screen , &destB2 , SDL_MapRGB(jeu.screen->format, 255, 255, 255));

			SDL_BlitSurface(bouton , NULL , jeu.tmps , &destB2);
			drawString("Valider", 350, 560, font[6],foregroundColor);

		
		}
			
		if(SDL_BlitSurface(jeu.tmps, NULL, jeu.screen, NULL) < 0)
			fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		SDL_Flip(jeu.screen);
		//pause();

	}

	return 0;
}
