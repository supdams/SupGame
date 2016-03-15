#ifndef _MENU_H_
#define _MENU_H_

#include "structs.hpp"


/* Structures globales */
extern Gestion jeu;
extern TTF_Font *font[8];

extern Input input;

void drawStartMenu(void);
void updateStartMenu();

extern void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor);



#endif