#ifndef PSEUGUI_H
#define PSEUGUI_H

//#include "irrlicht/irrlicht.h"
//#include "irrklang/irrKlang.h"
#include "SceneData.h"
//#include "DrawObjMgr.h"
#include "World/World.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "GUI/sdl/textedition.h"



#include <string>
class PseuGUI;
class Object;
class PseuInstance;
class Scene;

enum SceneState
{
	SCENESTATE_NULL = 0,
	SCENESTATE_GUISTART,
	SCENESTATE_LOGINSCREEN,
	SCENESTATE_REALMSELECT,
	SCENESTATE_CHARSELECT,
	SCENESTATE_LOADING,
	SCENESTATE_WORLD,
	SCENESTATE_NOSCENE = 0xFFFFFFFF
};



/*
inline irr::core::vector3df WPToIrr(WorldPosition wp)
{
return irr::core::vector3df(-wp.x, wp.z, -wp.y);
}

inline WorldPosition IrrToWP(irr::core::vector3df v, float o_rad)
{
return WorldPosition(-v.X, -v.Z, v.Y, IRR_TO_O(o_rad)); // rotate by 90° and fix value
}
*/

class PseuGUIRunnable : public ZThread::Runnable
{
public:
	PseuGUIRunnable();
	~PseuGUIRunnable();
	void run(void);
	PseuGUI *GetGUI(void);
private:
	PseuGUI* _gui;
};


class PseuGUI
{
	//  too bad friends are not inherited...
	friend class Scene;
	friend class SceneLoading;
	friend class SceneWorld;
	friend class SceneGuiStart;
	friend class SceneLogin;
	friend class SceneCharSelection;
	friend class SceneRealmSelection;
	// ...

public:
	PseuGUI();
	~PseuGUI();


	void SetInstance(PseuInstance*);
	inline PseuInstance *GetInstance(void) { return _instance; }
	void Run(void);
	void SetDriver(uint8);
	void SetResolution(uint16 x, uint16 y, uint16 depth=32);
	void SetWindowed(bool);
	void SetVSync(bool);
	void UseShadows(bool);
	void Cancel(void);
	void Shutdown(void);
	inline bool IsInitialized(void) { return _initialized /*&& _device*/; }
	inline void SetUseSound(bool b) { _usesound = b; }

	inline bool MustDie(void) { return _mustdie; }

	// interfaces to tell the gui what to draw
	void NotifyObjectDeletion(uint64 guid);
	void NotifyObjectCreation(Object *o);
	void NotifyAllObjectsDeletion(void);

	// scenes
	void SetSceneState(SceneState);
	bool SetSceneData(uint32, uint32);
	uint32 GetSceneState(void);
	inline void UpdateScene(void) { _updateScene = true; }

	// helpers
	WorldPosition GetWorldPosition(void);

private:
	void _Init(void);
	void _UpdateSceneState(void);
	uint16 _xres,_yres,_colordepth;
	bool _windowed,_vsync,_shadows;
	bool _initialized,_mustdie;
	bool _usesound;
	TextEdition pseudo;
	TextEdition mot_de_passe;
	/*
	irr::IrrlichtDevice *_device;
	irr::video::IVideoDriver* _driver;
	irr::scene::ISceneManager* _smgr;
	irr::gui::IGUIEnvironment* _guienv;
	irr::video::E_DRIVER_TYPE _driverType;
	irrklang::ISoundEngine *_soundengine;
	*/

	SDL_Window* screen;  // Holds window properties.
	SDL_Renderer* renderer;  // Holds rendering surface properties.
	
	TTF_Font *font[8];


	void Affficher_Texture(SDL_Texture *image, int x, int y);
	void drawImagePlus(SDL_Texture *image, int x, int y, double rotation, SDL_Point center, SDL_RendererFlip flip);
	void drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy);
	void delay(unsigned int frameLimit);
	SDL_Texture *loadImage(char *name);
	SDL_Texture* renderText(char *text, TTF_Font *font, SDL_Color color); 
	TTF_Font *loadFont(char *name, int size);
	void closeFont(TTF_Font *font);
	void drawString(char *text, int x, int y, TTF_Font *font, SDL_Color foregroundColor);

	bool PseuGUI::HandleEvent(SDL_Event *Event);
	int AABBCollision(int curseur_x ,int curseur_y ,SDL_Rect box);
	int intersects(SDL_Rect box1 , SDL_Rect box2);

	// DrawObjMgr domgr;
	PseuInstance *_instance;
	SceneState _scenestate, _scenestate_new;
	Scene *_scene;
	// ITimer *_timer;
	uint32 _passtime, _lastpasstime, _passtimediff;
	// irr::core::dimension2d<irr::u32> _screendimension;
	uint32 _timer;
	uint32 _throttle;//used for frameratelimiting
	bool _updateScene; // manually update scene?
	unsigned int frameLimit;
};



#endif

