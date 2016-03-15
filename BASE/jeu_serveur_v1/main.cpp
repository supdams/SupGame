#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_net.h"

#include "serveur.hpp"



int main(int argc, char **argv)
{

	if(SDL_Init(0)==-1)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}

	if(SDLNet_Init()==-1)
	{
	    printf("SDLNet_Init: %s\n", SDLNet_GetError());
	    exit(2);
	}

	serveur S1(_PORT_);

	S1.lancer();

	return(0);

}