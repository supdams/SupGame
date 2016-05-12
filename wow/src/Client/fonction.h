#ifndef FONCTION_H
#define FONCTION_H

#include "common.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define MAX_CHAINE 100

struct Input
{
	int left, right, up, down, jump, attack, enter, erase, pause, extra, inventaire, espace;
	int add, remove;
	int previous, next, load, save, copy, reinit;
	int mouseX, mouseY;
};

SDL_Surface *load_image(const char file[MAX_CHAINE]);
TTF_Font *loadFont(char *name, int size);

void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor,SDL_Surface *ecran);
void drawImage(SDL_Surface *image, int x, int y);

void getInput(void);

#endif

