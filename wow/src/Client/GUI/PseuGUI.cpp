#include "common.h"
//#include "irrlicht/irrlicht.h"
//#include "CM2MeshFileLoader.h"
//#include "CWMOMeshFileLoader.h"
#include "World/Object.h"
//#include "DrawObject.h"
#include "PseuWoW.h"
#include "Scene.h"
#include "PseuGUI.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>



int PseuGUI::AABBCollision(int curseur_x ,int curseur_y ,SDL_Rect box) /*Voit si il y a eu collision avec une AABB (clic bouton)*/
{
    if (curseur_x >= box.x && curseur_x < box.x + box.w
            && curseur_y >= box.y && curseur_y < box.y + box.h)
        return 1;
    else
        return 0;
}

int PseuGUI::intersects(SDL_Rect box1 , SDL_Rect box2)/*Fonction qui teste la collision entre deux SDL_Rect*/
{
    if ((box2.x >= box1.x + box1.w)     /* trop à droite*/
            || (box2.x + box2.w <= box1.x) /*trop à gauche*/
            || (box2.y >= box1.y + box1.h) /* trop en bas*/
            || (box2.y + box2.h <= box1.y))  /* trop en haut*/
        return 0;
    else
        return 1;
}

bool PseuGUI::HandleEvent(SDL_Event *Event)
{
    bool ShouldQuit = false;
 
    switch(Event->type)
    {
        case SDL_QUIT:
        {
            logdetail("SDL_QUIT\n");
            ShouldQuit = true;
        } break;

        case SDL_WINDOWEVENT:
        {
            switch(Event->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    logdetail("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", Event->window.data1, Event->window.data2);
                } break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    logdetail("SDL_WINDOWEVENT_FOCUS_GAINED\n");
                } break;

				/*
                case SDL_WINDOWEVENT_EXPOSED:
                {
                    screen = SDL_GetWindowFromID(Event->window.windowID);
                    renderer = SDL_GetRenderer(screen);
                    static bool IsWhite = true;
                    if (IsWhite == true)
                    {
                      //  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        IsWhite = false;
                    }
                    else
                    {
                      //  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        IsWhite = true;
                    }
                    SDL_RenderClear(renderer);
                    SDL_RenderPresent(renderer);
                } break;
				*/
            }
        } break;
    }
    
    return(ShouldQuit);
}



TTF_Font *PseuGUI::loadFont(char *name, int size)
{
	/* Use SDL_TTF to load the font at the specified size */

	TTF_Font *font = TTF_OpenFont(name, size);

	if (font == NULL)
	{
		logerror("Failed to open Font %s: %s\n", name, TTF_GetError());
		//exit(1);
	}

	return font;
}


void PseuGUI::closeFont(TTF_Font *font)
{
	/* Close the font once we're done with it */

	if (font != NULL)
	{
		TTF_CloseFont(font);
	}
}

SDL_Texture* PseuGUI::renderText(char *text, TTF_Font *font, SDL_Color color) 
{
	SDL_Surface *surf; //Pour écrire le texte
	SDL_Texture *texture; //Pour convertir la surface en texture
   
	
	surf = TTF_RenderText_Blended(font, text, color);

	// Conversion de l'image en texture
    texture = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
    return texture;
}

void PseuGUI::drawString(char *text, int x, int y, TTF_Font *font,SDL_Color foregroundColor)
{
	SDL_Rect dest;
	SDL_Surface *surface; //Pour écrire le texte
	SDL_Texture *texture; //Pour convertir la surface en texture

	   surface = TTF_RenderUTF8_Blended(font, text, foregroundColor);

	if (surface != NULL)
	{
		/* NOUS MODIFIONS QUELQUE PEU NOTRE CODE POUR PROFITER DE LA MEMOIRE GRAPHIQUE
		   QUI GERE LES TEXTURES  */
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		// On se débarrasse du pointeur vers une surface
		/* On libère la SDL_Surface temporaire (pour éviter les fuites de mémoire - cf. chapitre 
		   dédié) */
		SDL_FreeSurface(surface);
		surface = NULL;

	    // On dessine cette texture à l'écran 
	    dest.x = x;
	    dest.y = y;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(renderer, texture, NULL, &dest);

		//On supprime la texture
		SDL_DestroyTexture(texture);

	}
	else
	{
		logerror("La chaine n'a pu être écrite %s: %s\n", text, TTF_GetError());

	//	exit(0);
	}	
}



void PseuGUI::Affficher_Texture(SDL_Texture *image, int x, int y)
{
	SDL_Rect dest;

	/* Règle le rectangle à dessiner selon la taille de l'image source */
	dest.x = x;
	dest.y = y;

	/* Dessine l'image entière sur l'écran aux coordonnées x et y */
	SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, image, NULL, &dest);
}

void PseuGUI::drawImagePlus(SDL_Texture *image, int x, int y, double rotation, SDL_Point center, SDL_RendererFlip flip)
{
	//Nouvelle fonction en SDL2 qui gère le flip et les rotations

	SDL_Rect dest;

	/* Règle le rectangle à dessiner selon la taille de l'image source */
	dest.x = x;
	dest.y = y;

	/* Dessine l'image entière sur l'écran aux coordonnées x et y */
	SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopyEx(renderer, image, NULL, &dest, rotation, &center, flip);
}


void PseuGUI::drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy)
{
    /* Rectangle de destination à dessiner */
	SDL_Rect dest;

	dest.x = destx;
	dest.y = desty;
	dest.w = 20;//TILE_SIZE;
	dest.h = 20;//TILE_SIZE;

	/* Rectangle source */
	SDL_Rect src;

	src.x = srcx;
	src.y = srcy;
	src.w = 20;//TILE_SIZE;
	src.h = 20;//TILE_SIZE;

	/* Dessine la tile choisie sur l'écran aux coordonnées x et y */
	SDL_RenderCopy(renderer, image, &src, &dest);
}


void PseuGUI::delay(unsigned int frameLimit)
{

    /* Gestion des 60 fps */
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}


SDL_Texture *PseuGUI::loadImage(char *name)
{
	/* Charge les images avec SDL Image dans une SDL_Surface */

	SDL_Surface *loadedImage = NULL;
	SDL_Texture *texture = NULL;
    loadedImage = IMG_Load(name);

	if (loadedImage != NULL)
	{
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);

		// On se débarrasse du pointeur vers une surface
		SDL_FreeSurface(loadedImage);
		loadedImage = NULL;
	}
	else
		logerror("L'image n'a pas pu être chargée! SDL_Error :  %s\n", SDL_GetError());

		return texture;
}



PseuGUIRunnable::PseuGUIRunnable()
{
    _gui = new PseuGUI();
}

void PseuGUIRunnable::run(void)
{
    _gui->Run();
}

PseuGUI *PseuGUIRunnable::GetGUI(void)
{
    return _gui;
}

PseuGUIRunnable::~PseuGUIRunnable()
{
    delete _gui;
}

PseuGUI::PseuGUI()
{
    _xres = 640;
    _yres = 480;
    _colordepth = 32;
    _shadows = false;
    _windowed = true;
    _vsync = false;
    _initialized = false;
    _mustdie = false;
//    _driverType = video::EDT_BURNINGSVIDEO; // nulldevice makes not really a sense to display stuff
    _scenestate = _scenestate_new = SCENESTATE_NULL;
   
	/*
	_smgr = NULL;
    _device = NULL;
    _guienv = NULL;
	*/
    _scene = NULL;
    _passtime = _lastpasstime = _passtimediff = 0;
  //  _soundengine = NULL;
    _usesound = false;

}

PseuGUI::~PseuGUI()
{
  //  domgr.Clear();
    this->Cancel();
    _instance->DeleteGUI(); // this makes the instance set its gui ptr to NULL
    logdebug("PseuGUI::~PseuGUI()");
}

void PseuGUI::SetDriver(uint8 driverId)
{
	/*
    switch(driverId)
    {
    case DIRECTX9:      _driverType = video::EDT_DIRECT3D9;     break;
    case DIRECTX8:      _driverType = video::EDT_DIRECT3D8;     break;
    case OPENGL:        _driverType = video::EDT_OPENGL;        break;
    case SOFTWARE:      _driverType = video::EDT_SOFTWARE;      break;
    case BURNINGSVIDEO: _driverType = video::EDT_BURNINGSVIDEO; break;
    case NULLDEVICE:    _driverType = video::EDT_NULL;          break;
    default:            _driverType = video::EDT_BURNINGSVIDEO; // if no valid driver detected, use software
    }
	*/
    // TODO: add support for changing driver during runtime?
}

void PseuGUI::SetResolution(uint16 x, uint16 y, uint16 depth)
{
    _xres = x;
    _yres = y;
    _colordepth = depth;
}

void PseuGUI::SetWindowed(bool b)
{
    _windowed = b;
    // see PseuGUI::Init(): !_windowed == fullscreen
}

void PseuGUI::SetVSync(bool b)
{
    _vsync = b;
}

void PseuGUI::UseShadows(bool b)
{
    _shadows = b;
}

// if this fuction is called from another thread the device will not work correctly. o_O


void PseuGUI::_Init(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)//initialisation de la SDL
    {
       logerror("Erreur d'initialisation de la SDL : %s",SDL_GetError());
     //  return EXIT_FAILURE;
    }
	// Initialise SDL Video 
	screen =  SDL_CreateWindow("Handmade Hero",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,
                                          600,
                                          SDL_WINDOW_RESIZABLE);

	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	
	
	
	/*
	/////
    // Create window and renderer.
   /// screen = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

   // renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
   */
    if (screen == NULL || renderer == NULL)
        {
            logerror("Impossible d'initialiser le mode écran à %d x %d: %s\n", 800, 600, SDL_GetError());
           // exit(1);
        }

	//Initialisation du chargement des images png avec la SDL 2.0 - Voir les commentaires du chapitre 9
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		logerror( "SDL_image n'a pu être initialisée! SDL_image Error: %s\n", IMG_GetError() );
		//exit(1);
	}

	// Cache le curseur de la souris 
	//SDL_ShowCursor(SDL_DISABLE);

	/* Initialise SDL_TTF */

	if (TTF_Init() < 0)
	{
		logerror("Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());

	//	exit(1);
	}

	 SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    

	font[0] = loadFont("./font/Kraash_Black.ttf", 48);
	font[1] = loadFont("./font/GenBasB.ttf", 24);
	font[2] = loadFont("./font/Super_Mario_Bros.ttf", 32);
	font[3] = loadFont("./font/MORPHEUS.TTF", 48);
	font[4] = loadFont("./font/MORPHEUS.TTF", 18);
	font[5] = loadFont("./font/GenBasB.TTF", 18);
	font[6] = loadFont("./font/Kraash_Black.TTF", 16);
	font[7] = loadFont("verdana.ttf", 16);
	

	//SDL_Texture *background;
	/*
    _device = createDevice(_driverType,dimension2d<u32>(_xres,_yres),_colordepth,!_windowed,_shadows,_vsync);
    if(!_device)
    {
        logerror("PseuGUI: Can't use specified video driver, trying software mode...");
        _device = createDevice(video::EDT_SOFTWARE,dimension2d<u32>(_xres,_yres),_colordepth,!_windowed,false,false);
        if(!_device)
        {
            logerror("ERROR: PseuGUI::_Init() failed, no video driver available!");
            return;
        }
        else
        {
            logerror("PseuGUI: Software mode OK!");
        }
    }
    DEBUG(logdebug("PseuGUI::Init() _device=%X",_device));
    
	
	_device->setWindowCaption(L"PseuWoW - Initializing");
    _device->setResizable(true);
	*/
    //_driver = _device->getVideoDriver();
   // _smgr = _device->getSceneManager();
   // _guienv = _device->getGUIEnvironment();
    _timer = SDL_GetTicks();
   // _screendimension = _driver->getScreenSize();

    //...

    // disable crappy irrlicht logging
 //   _device->getLogger()->setLogLevel(ELL_NONE);

    // register external loaders for not supported filetypes
  //  scene::CM2MeshFileLoader* m2loader = new scene::CM2MeshFileLoader(_device);
    //_smgr->addExternalMeshLoader(m2loader);
   // scene::CWMOMeshFileLoader* wmoloader = new scene::CWMOMeshFileLoader(_device);
    //_smgr->addExternalMeshLoader(wmoloader);
    _throttle=0;
    _initialized = true;

}

void PseuGUI::Cancel(void)
{
    DEBUG(logdebug("PseuGUI::Cancel()"));

    if(_scene)
    {
        _scene->OnDelete();
        delete _scene;
        _scene = NULL;
    }
	/*
    if(_device)
    {
        _device->drop();
        _device = NULL;
    }
    if(_soundengine)
    {
        _soundengine->drop();
        _soundengine = NULL;
    }
	*/
    _mustdie = true;
}

void PseuGUI::Shutdown(void)
{
     DEBUG(logdebug("PseuGUI::Shutdown()"));
    _mustdie = true;
}

void PseuGUI::Run(void)
{
    if(!_initialized)
        this->_Init();
    if(!_initialized) // recheck
    {
        logerror("PseuGUI: not initialized, using non-GUI mode");
        Cancel();
        return;
    }



    int lastFPS = -1, fps = -1;

    while(/*_device && _device->run() &&*/ !_mustdie)
    {
		
        _lastpasstime = _passtime;
        _passtime = SDL_GetTicks();
        _passtimediff = _passtime - _lastpasstime;
		/*
        if (!_device->isWindowActive())
        {
            _device->sleep(10); // save cpu & gpu power if not focused
        }
		*/
        _UpdateSceneState();
		
        if(!_scene)
        {
			SDL_Delay(10);
          //  _device->sleep(10);
            continue;
        }
		/*
        if(_screendimension != _driver->getScreenSize())
        {
            _scene->OnResize();
            _screendimension = _driver->getScreenSize();
        }
		*/
        if(_updateScene)
        {
            _updateScene = false;
            _scene->OnManualUpdate();
        }
		
		
        _scene->OnUpdate(_passtimediff); // custom: process input, set camera, etc
     //   _driver->beginScene(true, true, _scene->GetBackgroundColor()); // irr: call driver to start drawing
        _scene->OnDrawBegin(); // custom: draw everything before irrlicht draws everything by itself
       // _smgr->drawAll(); // irr: draw all scene nodes
        //_guienv->drawAll(); // irr: draw gui elements
        _scene->OnDraw(); // custom: draw everything that has to be draw late (post-processing also belongs here)
        //_driver->endScene(); // irr: drawing done
        if(/*_driver->getFPS()>100 && */_throttle < 10)//Primitive FPS-Limiter - upper cap hardcoded 100 FPS.
            _throttle++;                           //lowercap 60 (if it drops below, limiting is eased).
        if(/*_driver->getFPS()<60 &&*/ _throttle>0)    //but honestly, a 10 msec delay is not worth this amount of code.
            _throttle--;                           //If the FPS is down, it will never be because of this
      
		
		if(_throttle>0)
			SDL_Delay(_throttle);//Thus i opt for dropping the charade and using a fixed conf value of max 10.
            //sleeps max 10 msec (=_throttle) here.

		/*

        fps = _driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"PseuWoW [";
            str += _driver->getName();
            str += "] FPS:";
            str += fps;

            _device->setWindowCaption(str.c_str());

            lastFPS = fps;
        }
		*/
    }
  //  domgr.UnlinkAll(); // At this point the irr::device is probably closed and deleted already, which means it deleted
                       // all SceneNodes and everything. the ptrs are still stored in the DrawObjects, means they need to be unlinked now not to cause a crash.
    DEBUG(logdebug("PseuGUI::Run() finished"));
    Cancel(); // already got shut down somehow, we can now safely cancel and drop the device
}

// called from ObjMgr::Remove(guid)
void PseuGUI::NotifyObjectDeletion(uint64 guid)
{
   // domgr.Delete(guid);
}

// called from ObjMgr::Add(Object*)
void PseuGUI::NotifyObjectCreation(Object *o)
{
 //   DrawObject *d = new DrawObject(_device,o,_instance);
   // domgr.Add(o->GetGUID(),d);
}

void PseuGUI::NotifyAllObjectsDeletion(void)
{
  //  domgr.Clear();
}

void PseuGUI::SetInstance(PseuInstance* in)
{
    _instance = in;
}

void PseuGUI::SetSceneState(SceneState state)
{
    _scenestate_new = state; // will be applied at next cycle
}

void PseuGUI::_UpdateSceneState(void)
{
    if(_scenestate != _scenestate_new/* && _smgr*/)
    {
        if(_scene)
        {
            _scene->OnDelete();
            delete _scene;
            _scene = NULL;
        }
    //    _smgr->clear();
      //  _guienv->clear();



        logdebug("PseuGUI: switching to SceneState %u", _scenestate_new);

        switch (_scenestate_new)
        {
            case SCENESTATE_GUISTART: _scene = new SceneGuiStart(this); break;
            case SCENESTATE_LOGINSCREEN: _scene = new SceneLogin(this); break;
		//	case SCENESTATE_REALMSELECT: _scene = new SceneRealmSelection(this); break;
			case SCENESTATE_LOADING: _scene = new SceneLoading(this); break;
		//	case SCENESTATE_CHARSELECT: _scene = new SceneCharSelection(this); break;
            case SCENESTATE_WORLD: _scene = new SceneWorld(this); break;

			
			case SCENESTATE_REALMSELECT:
                _scene = new SceneCharSelection(this);
                _scene->SetData(ISCENE_CHARSEL_REALMFIRST, 1);
                _scenestate_new = SCENESTATE_CHARSELECT;
                break;
            case SCENESTATE_CHARSELECT:
                _scene = new SceneCharSelection(this);
                _scene->SetData(ISCENE_CHARSEL_REALMFIRST, 0);
                break;

            default: _scene = new Scene(this); // will draw nothing, just yield the gui
        }
        _scene->SetState(_scenestate_new);
        // current scenestate can be set safely after scene is created and ready
        _scenestate = _scenestate_new;


        logdebug("PseuGUI: scene created.");
    }
}

bool PseuGUI::SetSceneData(uint32 index, uint32 value)
{
    if(!_scene)
        return false;
    _scene->SetData(index, value);
    return true;
}

uint32 PseuGUI::GetSceneState(void)
{
    /* // not good, not threadsafe! (can crash)
    if(!_scene)
        return SCENESTATE_NOSCENE;
    return _scene->GetState();*/
    return _scenestate;
}

/*
// used to get our current WorldPosition
WorldPosition PseuGUI::GetWorldPosition(void)
{
    if(_scene && _scene->GetState() == SCENESTATE_WORLD)
    {
        return ((SceneWorld*)_scene)->GetWorldPosition();
    }
    return WorldPosition();
}
*/
