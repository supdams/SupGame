#include <sdl.h>
#include <SDL_image.h>

#pragma comment (lib,"sdl.lib")      // ignorez ces lignes si vous ne linkez pas les libs de cette façon.
#pragma comment (lib,"sdlmain.lib")

#define FMAP_CACHE_SIZE 500

#define MAX_CHAINE 100
typedef struct
{
	SDL_Rect R;
	int mur;
	// tout ce que vous voulez...
} TileProp;

typedef struct
{
	int LARGEUR_TILE,HAUTEUR_TILE;
	int nbtilesX,nbtilesY;
	SDL_Surface* tileset;
	char tilesetname[FMAP_CACHE_SIZE];
	TileProp* props;
	Uint16** schema;
	int nbtiles_largeur_monde,nbtiles_hauteur_monde;
	int xscroll,yscroll;
	int largeur_fenetre,hauteur_fenetre;
	int page_actuel;
} Map;

Map* ChargerMap(const char* fic,int largeur_fenetre,int hauteur_fenetre);
Map* NewMap(const char* tileset,int nbtilesX,int nbtilesY,int nbtiles_largeur_monde,int nbtiles_hauteur_monde);
int SauverMap(const char* fic,Map* m);
int AfficherMap(Map* m,SDL_Surface* screen);
int LibererMap(Map* m);
int ResizeMap(Map* m,int nouvelletailleX,int nouvelletailleY);

