#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif
#include "fmap.h"
#include "fevent.h"

#define MAX_LARGEUR_FENETRE 800
#define MAX_HAUTEUR_FENETRE 600 
Map m;

typedef struct
{
	char levelfic[500];
	Input I;
	int toggletileset;
	int toggleHV;
	int reaffiche;
	int selectedtile;
	Map* m;
} Context;

void MoveMap(Map* m,Input* in,int movespeed)
{
	if (in->key[SDLK_LEFT])
		m->xscroll-=movespeed;
	if (in->key[SDLK_RIGHT])
		m->xscroll+=movespeed;
	if (in->key[SDLK_UP])
		m->yscroll-=movespeed;
	if (in->key[SDLK_DOWN])
		m->yscroll+=movespeed;
// limitation
	if (m->xscroll<0)
		m->xscroll=0;
	if (m->yscroll<0)
		m->yscroll=0;
	if (m->xscroll>m->nbtiles_largeur_monde*m->LARGEUR_TILE-m->largeur_fenetre-1)
		m->xscroll=m->nbtiles_largeur_monde*m->LARGEUR_TILE-m->largeur_fenetre-1;
	if (m->yscroll>m->nbtiles_hauteur_monde*m->HAUTEUR_TILE-m->hauteur_fenetre-1)
		m->yscroll=m->nbtiles_hauteur_monde*m->HAUTEUR_TILE-m->hauteur_fenetre-1;
}

void VideoResizeForLevel(Map* m)
{
	char buf[50];
	int xtaillereelle,ytaillereelle;
	xtaillereelle = m->LARGEUR_TILE*m->nbtiles_largeur_monde-1;
	ytaillereelle = m->HAUTEUR_TILE*m->nbtiles_hauteur_monde-1;
	if (xtaillereelle>MAX_LARGEUR_FENETRE)
		xtaillereelle = MAX_LARGEUR_FENETRE;
	if (ytaillereelle>MAX_HAUTEUR_FENETRE)
		ytaillereelle = MAX_HAUTEUR_FENETRE;
	SDL_SetVideoMode(xtaillereelle, ytaillereelle, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	m->largeur_fenetre = xtaillereelle;
	m->hauteur_fenetre = ytaillereelle;
	sprintf(buf,"Taille : %d x %d",m->nbtiles_largeur_monde,m->nbtiles_hauteur_monde);
	SDL_WM_SetCaption(buf,NULL);
}

void SizeChangeCheck(Context* C)
{
	int nsize[2];
	nsize[0] = C->m->nbtiles_largeur_monde;
	nsize[1] = C->m->nbtiles_hauteur_monde;

	if (C->I.key[SDLK_h])
		C->toggleHV = 0;
	if (C->I.key[SDLK_v])
		C->toggleHV = 1;


	if (C->I.mousebuttons[SDL_BUTTON_WHEELDOWN])
		nsize[C->toggleHV]++;
	if (C->I.mousebuttons[SDL_BUTTON_WHEELUP])
		nsize[C->toggleHV]--;

	if (nsize[0]<1 || nsize[1]<1)
		return;  // taille invalide : ignore.
	if (nsize[0]==C->m->nbtiles_largeur_monde && nsize[1]==C->m->nbtiles_hauteur_monde)
		return;  // pas de changement
	
	ResizeMap(C->m,nsize[0],nsize[1]);
 	VideoResizeForLevel(C->m);
	while(0);
}

int EditOnMap(Context* C)
{
	int xpos,ypos;
	SDL_Rect Rdest;
	if (C->reaffiche)
		VideoResizeForLevel(C->m);
	MoveMap(C->m,&C->I,1);
	AfficherMap(C->m,SDL_GetVideoSurface());
	xpos = (C->I.mousex+C->m->xscroll)/C->m->LARGEUR_TILE;
	ypos = (C->I.mousey+C->m->yscroll)/C->m->HAUTEUR_TILE;
	Rdest.x = xpos * C->m->LARGEUR_TILE - C->m->xscroll;
	Rdest.y = ypos * C->m->HAUTEUR_TILE - C->m->yscroll;
	SDL_BlitSurface(C->m->tileset,&C->m->props[C->selectedtile].R,SDL_GetVideoSurface(),&Rdest);
	if (C->I.mousebuttons[SDL_BUTTON_LEFT])
		C->m->schema[xpos][ypos] = C->selectedtile;
	if (C->I.key[SDLK_LCTRL] && C->I.key[SDLK_s])
	{
		SauverMap(C->levelfic,C->m);
		SDL_WM_SetCaption("SAUVEGARDE FAITE",NULL);
	}
	SizeChangeCheck(C);
	return 0;
}

int EditOnTileset(Context* C)
{
	SDL_Rect Rect_dest;
	int localselectedtile;
	int page=0;
	//SDL_KEYDOWN SDL_KEYUP
	Rect_dest.x = 0;
	Rect_dest.h = (18*C->m->LARGEUR_TILE)* m.page_actuel;		
	Rect_dest.w = (7*C->m->HAUTEUR_TILE)* m.page_actuel;
	localselectedtile=0;


		if (C->I.key[SDLK_UP])
		{
			C->I.key[SDLK_UP] = 0;
			m.page_actuel=m.page_actuel--;
			Rect_dest.y =(18*C->m->LARGEUR_TILE)* m.page_actuel;
			SDL_FillRect(SDL_GetVideoSurface() , NULL , 0);
			SDL_BlitSurface(C->m->tileset,NULL,SDL_GetVideoSurface(),&Rect_dest);
			SDL_Flip(SDL_GetVideoSurface());
			SDL_Delay(1);

		}else if (C->I.key[SDLK_DOWN])
		{
			C->I.key[SDLK_DOWN] = 0;
			m.page_actuel=m.page_actuel++;
			Rect_dest.y = (18*C->m->LARGEUR_TILE)*m.page_actuel;
			SDL_FillRect(SDL_GetVideoSurface() , NULL , 0);
			SDL_BlitSurface(C->m->tileset,NULL,SDL_GetVideoSurface(),&Rect_dest);
			SDL_Flip(SDL_GetVideoSurface());
			SDL_Delay(1);
		}else{
			Rect_dest.y =(18*C->m->LARGEUR_TILE)* m.page_actuel;
			SDL_FillRect(SDL_GetVideoSurface() , NULL , 0);
			SDL_BlitSurface(C->m->tileset,NULL,SDL_GetVideoSurface(),&Rect_dest);
			SDL_Flip(SDL_GetVideoSurface());
			SDL_Delay(1);
		}

	if (C->reaffiche)
		SDL_SetVideoMode((8*C->m->LARGEUR_TILE), (18*C->m->LARGEUR_TILE), 32,SDL_HWSURFACE|SDL_DOUBLEBUF);

	
			localselectedtile = (C->I.mousex/C->m->LARGEUR_TILE) + C->m->nbtilesX*(C->I.mousey/C->m->HAUTEUR_TILE)-(144*m.page_actuel);
			
			printf("localselectedtile : %d \n",localselectedtile);
		if (C->I.mousebuttons[SDL_BUTTON_LEFT])
		{
			C->I.mousebuttons[SDL_BUTTON_LEFT] = 0;
			C->selectedtile = localselectedtile;
			return 1;
		}

	return 0;
}

void ProposeSave(Map* m,const char* fic)
{
#ifdef WIN32
	int res = MessageBox(0,L"Sauver ?",L"Sauvegarder ?",MB_YESNO);
	if (res==IDYES)
		SauverMap(fic,m);
#else
	// option de confirmation desactivee pour autres systemes, sauver ave CTRL+S
#endif
}



int Editeur(Context* C)
{
	while(!C->I.key[SDLK_ESCAPE] && !C->I.quit)
	{
		UpdateEvents(&C->I);
		if (C->I.mousebuttons[SDL_BUTTON_RIGHT])
		{
			C->I.mousebuttons[SDL_BUTTON_RIGHT] = 0;
			C->toggletileset = 1 - C->toggletileset;
			C->reaffiche = 1;
		}
		if (C->toggletileset==0) // map
			EditOnMap(C);
		else
		{
			if(EditOnTileset(C)==1)
				C->I.mousebuttons[SDL_BUTTON_RIGHT] = 1;
		}
		C->reaffiche = 0;
		SDL_Flip(SDL_GetVideoSurface());
		SDL_Delay(1);
	}
	ProposeSave(C->m,C->levelfic);
	return 0;
}

int LoadLevel(Context* C,const char* fic)
{
	Map* m;
	SDL_SetVideoMode(MAX_LARGEUR_FENETRE, MAX_HAUTEUR_FENETRE, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	m = ChargerMap(fic,MAX_LARGEUR_FENETRE,MAX_HAUTEUR_FENETRE);
	VideoResizeForLevel(m);
	C->m = m;
	strcpy(C->levelfic,fic);
	Editeur(C);
	LibererMap(m);
	return 0;
}

int NewLevel(Context* C,const char* fic)
{
	int nbxtile,nbytile;
	Map* m;
	strcpy(C->levelfic,fic);
	strcpy(C->levelfic+strlen(C->levelfic)-3,"txt");
	nbxtile=8;
	nbytile=608;

	SDL_SetVideoMode(MAX_LARGEUR_FENETRE, MAX_HAUTEUR_FENETRE, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	m = NewMap(fic,nbxtile,nbytile,100,100);
	VideoResizeForLevel(m);
	C->m = m;
	Editeur(C);
	LibererMap(m);
	return 0;
} 

int Lancer(const char* param)
{
	Context C;
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE);		// prepare SDL
	memset(&C,0,sizeof(Context));
	InitEvents(&C.I);
	if (strcmp(param+strlen(param)-3,"txt")==0)
		LoadLevel(&C,param);
	if (strcmp(param+strlen(param)-3,"png")==0)
		NewLevel(&C,param);
	SDL_Quit();
	return 0;
}

void getdirfrom(const char* fic)
{
	char buf[500];
	int i,nb = strlen(fic);
	strcpy(buf,fic);
	for(i=nb-1;i>=0;i--)
		if (buf[i]=='\\' || buf[i]=='/')
		{
			buf[i]='\0';
#ifdef WIN32
			//printf("chdir : %s\n",buf);
			_chdir(buf);
#endif
			break;
		}
}

int main(int argc,char** argv)
{
	char buf[500];
	_getcwd(buf,500);
	//printf("path = %s\n",buf);
	
	m.page_actuel=0;

	if (argc!=2)
	{
		char buf[500];
		FILE* F;
		printf("Nom du fichier png ou txt\n");
		scanf("%s",buf);
		F = fopen(buf,"r");
		if (F==NULL)
		{
			printf("Fichier introuvable\n");
			system("pause");
			return -1;
		}
		fclose(F);
		return Lancer(buf);
	}
	//printf("argv1 : %s\n",argv[1]);
	getdirfrom(argv[1]);
	_chdir(argv[1]);
	return Lancer(argv[1]);
}
