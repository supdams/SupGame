#include "load.hpp"

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