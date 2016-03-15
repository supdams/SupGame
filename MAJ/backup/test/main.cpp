/* //thunder  anim
#include <SDL.h>
#include <SDL_image.h>
 
#include <stdio.h>
#include <stdlib.h>
 
#include <assert.h>
 
#define W               320
#define H               48
#define BPP             32
#define TICK_INTERVAL   30
 
 
 
#define CHECK(cond, fun)                                                       \
    do{                                                                        \
        if (!(cond))                                                           \
            fprintf(stderr, "%s line : %u\n %s\n", __FILE__, __LINE__, fun()), \
            exit(EXIT_FAILURE);                                                \
    }while(0);
 
 
Uint32 time_left(Uint32 nextTime)
{
    Uint32 now;
 
    now = SDL_GetTicks();
    if(nextTime <= now)
        return 0;
    else
        return nextTime - now;
}
 
 
typedef struct anim_s
{
    SDL_Surface *gfx;
    SDL_Rect srcRect;
    Uint32 nAnims;
    Uint32 nFrames;
    Uint32 cnt;
    Uint32 crtFrame;
}Anim;
 
 
Anim Anim_create(SDL_Surface* gfx, Uint32 nAnim, Uint32 nFrames)
{
    Anim newAnim;
    newAnim.gfx = gfx;
    newAnim.nAnims = nAnim;
    newAnim.nFrames = nFrames;
    newAnim.cnt = 1;
    newAnim.crtFrame = nAnim;
 
    newAnim.srcRect.x = 0;
    newAnim.srcRect.w = gfx->w;
    assert(gfx->h % nAnim == 0);
    newAnim.srcRect.h = gfx->h / nAnim;
 
    return newAnim;
}
 
 
void Anim_update(Anim* anim)
{
    if(!--anim->cnt)
    {
        anim->cnt = anim->nFrames;
        anim->crtFrame = (anim->crtFrame + 1) % anim->nAnims;
        anim->srcRect.y = anim->crtFrame * anim->srcRect.h;
    }
}
 
 
void Anim_display(Anim* anim, SDL_Surface* dstGfx, int x, int y)
{
    SDL_Rect dstRect;
 
    dstRect.x =x;
    dstRect.y = y;
    SDL_BlitSurface(anim->gfx, &anim->srcRect,dstGfx, &dstRect);
}
 
 
 
 
 
 
SDL_Surface* loadSurface(char const* filename)
{
    SDL_Surface* ret = NULL;
    SDL_Surface* tmp = IMG_Load(filename);
    CHECK(tmp != NULL, IMG_GetError);
 
    ret = SDL_DisplayFormat(tmp);
 
    SDL_FreeSurface(tmp), tmp = NULL;
    CHECK(ret != NULL, SDL_GetError);
 
    return ret;
}
 
 
 
void initSDL(int w, int h, int bpp)
{
    CHECK(SDL_Init(SDL_INIT_VIDEO) >= 0, SDL_GetError);
    CHECK(SDL_SetVideoMode(w, h, bpp, SDL_SWSURFACE), SDL_GetError);
}
 
 
void run(void)
{
    Uint32 nextTime;
    SDL_Surface *scr = SDL_GetVideoSurface();
    SDL_Surface *gfx = loadSurface("thunder.png");
 
    Anim thunderAnim = Anim_create(gfx, 4, 5);
 
    nextTime = SDL_GetTicks() + TICK_INTERVAL;
 
    int done = 0;
    while(!done)
    {
        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            if(ev.type == SDL_QUIT)
                done = 1;
        }
        Anim_update(&thunderAnim);
 
        Anim_display(&thunderAnim, scr, 0, 16);
 
        SDL_Delay(time_left(nextTime));
        nextTime += TICK_INTERVAL;
 
        SDL_Flip(scr);
    }
    SDL_FreeSurface(gfx), gfx = NULL;
}
 
 
 
int main(int arg, char* argv[])
{
    initSDL(W, H, BPP);
    run();
    SDL_Quit();
 
    return 0;
}

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_net.h>

#include <iostream>
using namespace std;

#include "box.hpp"
#include<iostream>
#include<string>
#include<sstream>

string ctos(char * c)
{
	string ret;
	stringstream s;

	s << c;
	ret = s.str();
	return ret;
	
}

string charToString(char c)
{
	string ret;
	stringstream s;

	s << c;
	ret = s.str();
	return ret;
	
}
int main(int argc, char *argv[])
{
	SDL_Surface *ecran = NULL;
	int continuer = 1; 
	char Pseudochaine[100] = "";
	char Mdpchaine[100] = "";
	TTF_Font *police = NULL;


	int port_;
	SDLNet_SocketSet set_;
	TCPsocket tcpSock_;
	IPaddress ip;
	
	char * nom;
	string msg;
	const char * ch_ip;
	
	int numused, res;

	if(SDLNet_Init()==-1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return(-2);
	}	

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr,"Erreur lors de l'initialisation de la SDL : %s\n",SDL_GetError());		
		exit(EXIT_FAILURE);
	}
	if(TTF_Init() == -1)
	{
		fprintf(stderr,"Erreur lors de l'initialisation de SDL_TTF : %s\n",TTF_GetError());		
		exit(EXIT_FAILURE);
	}
	police = TTF_OpenFont("./font/times.ttf",40);
	if(police == NULL)
	{
		fprintf(stderr,"Erreur police : %s",TTF_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_EnableUNICODE(1);

	port_ = 8000;
	tcpSock_ = NULL;
	set_ = SDLNet_AllocSocketSet(5);
	ch_ip="127.0.0.1";


	
	if(SDLNet_ResolveHost(&ip,ch_ip,port_)==-1) {
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(1);
	}

	tcpSock_ = SDLNet_TCP_Open(&ip);
	if(!tcpSock_) {
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(2);
	}

	numused = SDLNet_TCP_AddSocket(set_, tcpSock_);
	if(numused==-1) {
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
	}


 	ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
	SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));
	
	while (continuer)
	{
		lireDonneeZoneTexte(ecran,police,Pseudochaine,"Pseudo : ","",99,0,0);
		lireDonneeZoneTexte(ecran,police,Mdpchaine,"Mdp : ","",99,0,0);
	
		string SPseudochaine(Pseudochaine);
		string SMdpchaine(Mdpchaine);
		

	cout<<"SPseudochaine : "<<SPseudochaine<<endl;
	cout<<"SMdpchaine : "<<SMdpchaine<<endl;


	msg = "C" + SPseudochaine + "!" + SMdpchaine + "!";
	nom = _strdup(msg.c_str());
	// On envoie le message
	res = SDLNet_TCP_Send(tcpSock_, nom , strlen(nom)+1);


		//continuer = 0;
    	}
	
	SDL_Flip(ecran);
	TTF_CloseFont(police);
	TTF_Quit();
    	SDL_Quit();
    	return EXIT_SUCCESS;
}
