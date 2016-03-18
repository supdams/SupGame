
#include "fonction.h"


SDL_Surface *load_image(const char file[MAX_CHAINE])
{
    SDL_Surface *imgTemporaire = NULL;
    SDL_Surface *imgNew = NULL;

    imgTemporaire = IMG_Load(file);

    if (imgTemporaire == NULL)
    {
        logerror("load_image Erreur avec %s\n--> %s\n" , file, IMG_GetError());
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
        logerror("Failed to open Font %s: %s\n", name, TTF_GetError());
        return NULL;
	}

	return font;
}


void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor,SDL_Surface *ecran)
{
	SDL_Rect dest;
	SDL_Surface *tmps;

	/* On utilise SDL_TTF pour g�n�rer une SDL_Surface � partir d'une cha�ne de caract�res (string) */
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);

	//tmps = TTF_RenderUTF8_Blended(font, text, foregroundColor);
	tmps = TTF_RenderText_Blended (font, text, foregroundColor);

	if (tmps == NULL)
	{
		logerror("Couldn't create String %s: %s\n", text, SDL_GetError());
		return;
	}

	/* On blitte cette SDL_Surface � l'�cran */
	dest.x = x;
	dest.y = y;
	dest.w = tmps->w;
	dest.h = tmps->h;
		if(SDL_BlitSurface(tmps, NULL, ecran, &dest) < 0)
		logerror("BlitSurface error: %s\n", SDL_GetError());


		
	// Mise � jour de l'�cran ... le tour est jou� !
	//SDL_UpdateRect(jeu.screen, dest.x, dest.y, dest.w,dest.h);

	/* On lib�re la SDL_Surface temporaire (pour �viter les fuites de m�moire - cf. chapitre d�di�) */
	SDL_FreeSurface(tmps);

}

void drawImage(SDL_Surface *image, int x, int y)
{
	SDL_Rect dest;
	
	SDL_Surface *tmps;
	/* R�gle le rectangle � blitter selon la taille de l'image source */

	dest.x = x;
	dest.y = y;
	dest.w = image->w;
	dest.h = image->h;

	/* Blitte l'image enti�re sur l'�cran aux coordonn�es x et y */

	SDL_BlitSurface(image, NULL, tmps, &dest);
}
