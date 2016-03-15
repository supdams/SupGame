
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "box.hpp"
#include "struc.hpp"

int AABBCollision(int curseur_x ,int curseur_y ,AABB box) /*Voit si il y a eu collision avec une AABB (clic bouton)*/
{
    if (curseur_x >= box.x && curseur_x < box.x + box.w
            && curseur_y >= box.y && curseur_y < box.y + box.h)
        return 1;
    else
        return 0;
}

void test_touche()
{

    /*On teste si l'utilisateur appuis sur une touche , si oui on met la variable qui correspon à la touche */
    /*appuyée à 1 si la touche est relachée on met la variable correspondante à 0 . Ce qui sert pour la fonction update*/
    SDL_Event event;

	bool onclick = false;
	      

	touche.mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	
	touche.mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT :
			exit(1);
            break;
        case SDL_KEYDOWN :
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN :
                touche.entrer = 1;
                break;
            case SDLK_SPACE :
                touche.espace = 1;
                break;
            case SDLK_RIGHT :
                touche.droite = 1;
                break;
            case SDLK_LEFT :
                touche.gauche = 1;
                break;
            case SDLK_UP :
                touche.haut = 1;
                break;
            case SDLK_DOWN:
                touche.bas = 1;
                break;
            case SDLK_LSHIFT :
                touche.lshift = 1;
                break;
             case SDLK_c :
                touche.c = 1;
                break;
            default :
                ;
            }
            break;
        case SDL_KEYUP :
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN :
                touche.entrer = 0;
                break;
            case SDLK_RIGHT :
                touche.droite = 0;
                break;
            case SDLK_LEFT :
                touche.gauche = 0;
                break;
            case SDLK_UP :
                touche.haut = 0;
                break;
            case SDLK_DOWN:
                touche.bas = 0;
                break;
            case SDLK_SPACE :
                touche.espace = 0;
                break;
            case SDLK_LSHIFT :
                touche.lshift = 0;
                break;
            case SDLK_c :
                touche.c = 0;
                break;
            default :
                break;
            }
            break;



        case SDL_MOUSEBUTTONDOWN :
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT :
                touche.clic = 1;
                break;
            default :
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP :
            touche.clic = 0;
            break;
        case SDL_MOUSEMOTION:
            touche.posSouris.x = event.motion.x; 
            touche.posSouris.y = event.motion.y;
            break;
      
		
		
		printf("input.mouseX : %d\n", touche.posSouris.x);
		
		
		
		
		
		default :
            break;

		
}
    
}

}

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
	detecterErreurs(jeu.tmps,barre,police,chaineEntree,texteDePreremplissage,nombreDeCaracteresMaximal);
	strcpy(introduction,introductionInitiale);
	strcpy(chaineEntree,texteDePreremplissage);
	positionCaractere = strlen(chaineEntree);
	//SDL_FillRect(jeu.tmps,NULL,SDL_MapRGB(jeu.tmps->format,255,255,255));
	SDL_EnableKeyRepeat(100,100);
	//SDL_Flip(jeu.tmps);
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
		SDL_BlitSurface(texte,NULL,jeu.tmps,&positionTexte);
		blitterCurseur(chaineFinale,&positionTexte,police,introduction,chaineEntree,jeu.tmps,barre, positionCaractere);
		//SDL_Flip(jeu.tmps);
		//SDL_FillRect(jeu.tmps,NULL,SDL_MapRGB(jeu.tmps->format,255,255,255));
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
		SDL_BlitSurface(barre,NULL,jeu.tmps,&positionBarre);
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



SDL_Surface *load_image(const char file[MAX_CHAINE])/*charge une image*/
{
    SDL_Surface *imgTemporaire = NULL;
    SDL_Surface *imgNew = NULL;

    imgTemporaire = IMG_Load(file);

    if (imgTemporaire == NULL)
    {
        fprintf(stderr, "load_image Erreur avec %s\n--> %s\n" , file, IMG_GetError());
        return NULL;
    }

    imgNew = SDL_DisplayFormatAlpha(imgTemporaire);

    SDL_FreeSurface(imgTemporaire);

    return imgNew;
}

TTF_Font *loadFont(char *name, int size)
{
	TTF_Font *font = TTF_OpenFont(name, size);

	if (font == NULL)
	{
		printf("Failed to open Font %s: %s\n", name, TTF_GetError());
		exit(1);
	}

	return font;
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


