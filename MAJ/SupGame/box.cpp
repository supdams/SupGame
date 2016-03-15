
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "box.hpp"

 
void lireDonneeZoneTexte(SDL_Surface *surface, TTF_Font *police, char *chaineEntree, char *introductionInitiale, char *texteDePreremplissage, long nombreDeCaracteresMaximal,int positionx, int positiony)
{
	int continuer = 1, majuscule = 0;
	char introduction[100] = "", chaineFinale[100] = "";
	SDL_Surface *texte, *barre = NULL;
	SDL_Color noir = {0,0,0};
	SDL_Rect positionTexte;
	long positionCaractere = 0; 
	SDL_Event event;
	barre = IMG_Load("Curseur.png");
	detecterErreurs(surface,barre,police,chaineEntree,texteDePreremplissage,nombreDeCaracteresMaximal);
	strcpy(introduction,introductionInitiale);
	strcpy(chaineEntree,texteDePreremplissage);
	positionCaractere = strlen(chaineEntree);
	SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,255,255,255));
	SDL_EnableKeyRepeat(100,100);
	SDL_Flip(surface);
	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				lireCaractereZoneTexte(event,&majuscule,chaineEntree,&positionCaractere,&continuer,nombreDeCaracteresMaximal);
				strcat(introduction,chaineEntree);
				strcpy(chaineFinale,introduction);
				strcpy(introduction,introductionInitiale);
				break;
		}
		if(strlen(chaineFinale) <= 0)
		{
			strcat(introduction,chaineEntree);
			strcpy(chaineFinale,introduction);
			strcpy(introduction,introductionInitiale);
		}
		texte = TTF_RenderUTF8_Solid(police,chaineFinale,noir);
		positionTexte.x = positionx;
		positionTexte.y = positiony;
		SDL_BlitSurface(texte,NULL,surface,&positionTexte);
		blitterCurseur(chaineFinale,&positionTexte,police,introduction,chaineEntree,surface,barre, positionCaractere);
		SDL_Flip(surface);
		SDL_FillRect(surface,NULL,SDL_MapRGB(surface->format,255,255,255));
	}
	SDL_FreeSurface(texte);
	SDL_FreeSurface(barre);
 
}
 
void detecterErreurs(SDL_Surface *surface, SDL_Surface *barre, TTF_Font *police, char *chaineEntree, char *texteDePreremplissage, long nombreDeCaracteresMaximal)
{
	if(surface == NULL)
	{
		fprintf(stderr,"Le pointeur sur surface vaut NULL, donc le texte ne peut pas être affiché à l'écran.\n");
		exit(EXIT_FAILURE);
	}
	if(barre == NULL)
	{
		fprintf(stderr,"Erreur lors de l'ouverture de l'image Curseur.png: %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}
	if(police == NULL)
	{
		fprintf(stderr,"Le pointeur sur police vaut NULL, donc aucune police ne peut être ouverte.\n");
		exit(EXIT_FAILURE);
	}
	if(chaineEntree == NULL)
	{
		fprintf(stderr,"La chaîne entrée vaut NULL, donc l'utilisateur ne peut rentrer aucune chaîne.\n");
		exit(EXIT_FAILURE);
	}
	if(nombreDeCaracteresMaximal < 1)
	{
		fprintf(stderr,"La taille maximale est nulle, donc l'utilisateur ne peut entrer aucune chaîne.\n");
		exit(EXIT_FAILURE);
	}
	if(texteDePreremplissage != NULL && strlen(texteDePreremplissage) > nombreDeCaracteresMaximal)
	{
		fprintf(stderr,"Le nombre de caractères du texte de préremplissage est supérieur au nombre de caractères maximal.\n");
		exit(EXIT_FAILURE);
	}
 
}
 
void blitterCurseur(char *chaineFinale, SDL_Rect *positionTexte, TTF_Font *police, char *introduction, char *chaineEntree, SDL_Surface *surface, SDL_Surface *barre, long positionCaractere)
{
	int longueurSurfaceTexte = 0, longueurSurfaceTexte2 = 0, longueurSurfaceTexte3 = 0, largeurSurfaceTexte = 0;
	SDL_Rect positionBarre;
	if(strlen(chaineFinale) > 0)
	{
		positionBarre.x = positionTexte->x;
		if(TTF_SizeUTF8(police,introduction,&longueurSurfaceTexte,&largeurSurfaceTexte) == 0)
		{
			positionBarre.x += longueurSurfaceTexte;
			longueurSurfaceTexte = 0;
		}
		if(TTF_SizeUTF8(police,chaineEntree,&longueurSurfaceTexte2,&largeurSurfaceTexte) == 0)
		{
			if(TTF_SizeUTF8(police,&chaineEntree[positionCaractere],&longueurSurfaceTexte3,&largeurSurfaceTexte) == 0)
			{
				longueurSurfaceTexte = longueurSurfaceTexte2 - longueurSurfaceTexte3;
			}
			positionBarre.x += longueurSurfaceTexte;
		}
		positionBarre.y = positionTexte->y;
		SDL_BlitSurface(barre,NULL,surface,&positionBarre);
	}
 
}
 
void lireCaractereZoneTexte(SDL_Event event, int *majuscule, char *chaine, long *positionCaractere, int *continuer, long nombreDeCaracteresMaximal)
{
	char caractereEntre = 0;
	int lettreEntree = 0, diminuerPositionCaractere = 1;
	long i = 0;
	if(event.key.keysym.mod & KMOD_SHIFT)
	{
		*majuscule = 1;
	}
	else
	{
		*majuscule = 0;
	}
	switch(event.key.keysym.unicode)
	{
		case SDLK_LEFT:
			if(*positionCaractere > 0)
				*positionCaractere -= 1;
			break;
		case SDLK_RIGHT:
			if(*positionCaractere < strlen(chaine))
				*positionCaractere += 1;
			break;
		case SDLK_BACKSPACE:
			if(*positionCaractere > 0)
			{
				for(i = *positionCaractere - 1; i < strlen(chaine); i++)
				{
					if(i > -1)
						chaine[i] = chaine[i+1];
					else
						diminuerPositionCaractere = 0;
				}
				if(diminuerPositionCaractere)
					*positionCaractere -= 1;
				else
					*positionCaractere = 1;
			}
			break;
		case SDLK_ESCAPE:
			*continuer = 0;
			break;
		case SDLK_RETURN:
			*continuer = 0;
			break;
		default:
			break;
	}
	detecterLettre(event,&lettreEntree,&caractereEntre,majuscule);
	if(lettreEntree == 1 && strlen(chaine) <= nombreDeCaracteresMaximal)
	{
		insererCaractereDansChaine(*positionCaractere,chaine,caractereEntre);
		*positionCaractere += 1;
	}
}
 
void detecterLettre(SDL_Event event, int *lettreEntree, char *caractereEntre, int *majuscule)
{
	long i = 0;
	for(i = 0; i < 26; i++) //Lettres sans accents minuscules et majuscules
	{
		if(event.key.keysym.unicode == SDLK_a + i)
		{
			entrerLettre(lettreEntree,65 + i,caractereEntre);
			if(!(*majuscule))
			{
				*caractereEntre += 32;
			}
			break;
		}
	}
	for(i = 0; i < 27; i++)
	{
		if(event.key.keysym.unicode == SDLK_AMPERSAND + i)
		{
			entrerLettre(lettreEntree,38 + i,caractereEntre);
		}
	}
	for(i = 0; i < 5; i++)
	{
		if(event.key.keysym.unicode == SDLK_SPACE + i)
		{
			entrerLettre(lettreEntree,32 + i,caractereEntre);
		}
	}
	switch(event.key.keysym.unicode) //Caractères spéciaux
	{
		case SDLK_BACKSLASH:
			entrerLettre(lettreEntree,92,caractereEntre);
			break;
		default:
			break;
	}
	for(i = 0; i < 10; i++)
	{
		if(event.key.keysym.unicode == SDLK_KP0 + i) //Nombres du pavé numérique
		{
			entrerLettre(lettreEntree,48 + i,caractereEntre);
		}
	}
 
}
 
void entrerLettre(int *lettreEntree, long numeroCaractere, char *caractere)
{
	if(!(*lettreEntree))
	{
		*lettreEntree = 1;
		*caractere = numeroCaractere;
	}
}
 
void insererCaractereDansChaine(long positionCaractere, char *chaine, char caractereAInserer)
{
	char chaine2[100] = "";
	strcpy(chaine2,&chaine[positionCaractere]);
	chaine[positionCaractere] = caractereAInserer;
	strcpy(&chaine[positionCaractere+1],chaine2);
}
