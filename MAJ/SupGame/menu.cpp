#include "menu.hpp"
#include "box.hpp"


void updateStartMenu(void)
{
	if(input.down == 1)
	{
		if(jeu.choice == 0)
			jeu.choice++;
		printf("input.down == 1    \n");
		input.down = 0;
	}

	if(input.up == 1)
	{
		if(jeu.choice == 1)
			jeu.choice--;
		printf("input.up == 1    \n");
		input.up = 0;
	}

	if(input.enter)
	{
		 if(jeu.choice == 0)
		{
			//initializePlayer();
			//InitInventaire();
			//initGame();
			printf("if(input.enter)   if(choice == 0)  \n");
			jeu.onMenu = 0;
		}
		
		else if(jeu.choice == 1)//Sinon, on quitte le jeu
		{
			exit(0);
			printf(" if(input.enter)   if(choice == 1)    \n");
		}
		input.enter = 0;
	}	
}


void drawStartMenu(void)
{
	SDL_Color foregroundColor;

	foregroundColor.r = 255;
	foregroundColor.g = 255;
	foregroundColor.b = 255;


	drawString("Titre du projet", 300, 50, font[2],foregroundColor);




	//Si l'option n'est pas en surbrillance, on l'affiche normalement, sinon on rajoute ">"
	if(jeu.choice != 0)
	{
		drawString("START", 353, 200, font[0],foregroundColor);
	}
	if(jeu.choice != 1)
	{
		drawString("QUIT", 353, 280, font[0],foregroundColor);
	}

	 //Si l'option est en surbrillance, on rajoute ">"
	if(jeu.choice == 0)
	{
		drawString("> START", 330, 200, font[0],foregroundColor);
	}
	else if(jeu.choice == 1)
	{
		drawString("> QUIT", 330, 280, font[0],foregroundColor);
	}

	//Le nom du studio
	drawString("SupGame Game Studio, 2012", 200, 520, font[2],foregroundColor);
	
}

