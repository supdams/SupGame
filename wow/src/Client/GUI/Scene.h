#ifndef _SCENE_H
#define _SCENE_H
#include "SceneData.h"

/*
#include "irrlicht/irrlicht.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;


inline core::rect<s32> CalcRelativeScreenPos(core::dimension2d<u32> dim, f32 x, f32 y, f32 w, f32 h)
{
    core::rect<s32> r;
    r.UpperLeftCorner.X = dim.Width * x;
    r.UpperLeftCorner.Y = dim.Height* y;
    r.LowerRightCorner.X = r.UpperLeftCorner.X + (dim.Width * w);
    r.LowerRightCorner.Y = r.UpperLeftCorner.Y + (dim.Height * h);
    return r;
}
//Pure and applied stupidity: the driver returns u32, but guiend->addbutton insists on getting s32
inline core::rect<s32> CalcRelativeScreenPos(video::IVideoDriver* drv, f32 x, f32 y, f32 w, f32 h)
{
    return CalcRelativeScreenPos(drv->getScreenSize(),x,y,w,h);
}
*/
class PseuGUI;
class CCursorController;
class GUIEventReceiver;

// base class
class Scene
{
    friend class PseuGUI;
public:
    Scene(PseuGUI *g);
    ~Scene();
    std::string GetStringFromDB(uint32 index, uint32 entry, SCPDatabase *other_db = NULL);
    inline void SetState(SceneState sc) { _scenestate = sc; }
    inline SceneState GetState(void) { return _scenestate; }

    virtual void OnUpdate(uint32 timepassed);
    virtual void OnManualUpdate(void);
    virtual void OnDraw(void);
    virtual void OnDrawBegin(void);
    virtual void OnDelete(void);
    virtual void OnResize(void);
//    virtual video::SColor GetBackgroundColor(void);
    virtual void SetData(uint32 index, uint32 value) { scenedata[index] = value; }

protected:
    PseuInstance *instance;
    PseuGUI *gui;
	
	void getInput(void);
	
	/*
    irr::IrrlichtDevice *device;
    irr::video::IVideoDriver* driver;
    irr::scene::ISceneManager* smgr;
    irr::gui::IGUIEnvironment* guienv;
    irr::gui::IGUIElement* rootgui;
    irrklang::ISoundEngine *soundengine;*/
    CCursorController *cursor;



	SDL_Event events;
	 uint32 clic;
	SDL_Rect posSouris;



    SceneState _scenestate;
    uint32 scenedata[SCENEDATA_SIZE]; // generic storage for anything the PseuInstance thread wants to tell us
    SCPDatabase *textdb, *racedb, *classdb;
    ZThread::FastMutex mutex;
};


class SceneRealmSelection : public Scene
{
public:
    SceneRealmSelection(PseuGUI *gui);
    void OnUpdate(uint32 timepassed);
    void OnDelete(void);
private:
  //  IGUIImage *irrlogo, *driverlogo;
    GUIEventReceiver *eventrecv;
    uint32 msgbox_textid;
};

class SceneGuiStart : public Scene
{
public:
    SceneGuiStart(PseuGUI *gui);
    void OnDelete(void);
private:
  //  IGUIImage *irrlogo, *driverlogo;

};

class SceneLogin : public Scene
{
public:
    SceneLogin(PseuGUI *gui);
    void OnUpdate(uint32 timepassed);
    void OnDelete(void);
	
private:
   // IGUIImage *irrlogo, *background;
	SDL_Texture *bouton, *bouton2, *text_nom_du_jeu, *text_titre_page, *text_mot_de_passe, *text_Pseudo, *text_Bouton_Valider;
    GUIEventReceiver *eventrecv;
   // gui::IGUIElement *msgbox;
   // gui::IGUIWindow *popup;
    uint32 msgbox_textid;
};

class CharSelectGUIEventReceiver;

class SceneCharSelection : public Scene
{
public:
    SceneCharSelection(PseuGUI *gui);
    void OnUpdate(uint32 timepassed);
    void OnManualUpdate(void);
    void OnDelete(void);
    void OnResize(void);

    void FillCharlist(void);

private:
	SDL_Texture *bouton, *bouton2, 
		*nom_du_jeu,
		
	*IMD_BUTTON_ENTER_WORLD ,
	*IMD_BUTTON_BACK ,
	*IMD_BUTTON_NEW_CHARACTER ,
	*IMD_BUTTON_DELETE_CHARACTER ,
	*IMD_BUTTON_SELECT_CHARACTER ,
	*IMD_BUTTON_CHARACTER_OK ,
	*IMD_BUTTON_SELECT_CHARACTER2 ,
	*IMD_BUTTON_CHARACTER_OK2 ,
	*IMD_BUTTON_SELECT_REALM ,
	*IMD_BUTTON_REALMWIN_OK ,
	*IMD_BUTTON_REALMWIN_CANCEL ,
	*IMD_BUTTON_NEWCHARWIN_OK ,
	*IMD_BUTTON_NEWCHARWIN_CANCEL ,	
	*IMD_BUTTON_ENTER_WORLD2 ,
	*IMD_BUTTON_BACK2 ,
	*IMD_BUTTON_NEW_CHARACTER2 ,
	*IMD_BUTTON_DELETE_CHARACTER2 ,
	*IMD_BUTTON_SELECT_REALM2 ,
	*IMD_BUTTON_REALMWIN_OK2 ,
	*IMD_BUTTON_REALMWIN_CANCEL2 ,
	*IMD_BUTTON_NEWCHARWIN_OK2 ,
	*IMD_BUTTON_NEWCHARWIN_CANCEL2 	
		
		;
    GUIEventReceiver *eventrecv;
   // gui::IGUIElement *msgbox;
   // gui::IGUIWindow *popup;
    uint32 msgbox_textid;
	uint32 choix_server;
	uint32 choix_perso;
	int action_bouton;
	int realmwin;
    int newcharwin;

	/*
    IGUIWindow *realmwin;
    IGUIListBox *realmlistbox;
    IGUIListBox *charlistbox; // temporary until something better found
    //Character creation //temporary. maybe a whole new character creation scene should be used?
    IGUIWindow  *newcharwin;
    IGUIComboBox *raceselect;
    IGUIComboBox *classselect;
    IGUIEditBox *charname;
    IGUIElement *msgbox; // display status/result of character creation
    uint32 msgbox_textid; // stores old textid
    std::map<u32,u32> racemap, classmap; //<comboBoxId,dbId> maps DB IDs in db to IDs in the combobox, because irrlicht does not allow custom ids in comboboxes
	*/
	};




class ShTlTerrainSceneNode;
class MCameraFPS;
class MCameraOrbit;
class MyEventReceiver;
class MapMgr;
class WorldSession;
class MovementMgr;
class MyCharacter;

class SceneWorld : public Scene
{
    struct SceneNodeWithGridPos
    {
       // scene::ISceneNode *scenenode;
        uint32 gx,gy;
    };

public:
    SceneWorld(PseuGUI *gui);
    void OnDraw(void);
    void OnDrawBegin(void);
    void OnDelete(void);
    void OnUpdate(uint32 timepassed);
    void UpdateTerrain(void);
    void InitTerrain(void);
    void RelocateCamera(void);
    void RelocateCameraBehindChar(void);
    void UpdateMapSceneNodes(std::map<uint32,SceneNodeWithGridPos>&);
   // scene::ISceneNode *GetMyCharacterSceneNode(void);
 //   video::SColor GetBackgroundColor(void);

    WorldPosition GetWorldPosition(void);

private:
    ShTlTerrainSceneNode *terrain;
    MCameraFPS *camera;
    MyEventReceiver *eventrecv;
    uint32 map_gridX, map_gridY;
    WorldSession *wsession;
    World *world;
    MapMgr *mapmgr;
  //  IGUIStaticText *debugText;
    bool debugmode;
    std::map<uint32,SceneNodeWithGridPos> _doodads;
    std::map<uint32,SceneNodeWithGridPos> _wmos;
    std::map<uint32,SceneNodeWithGridPos> _sound_emitters;
//    scene::ISceneNode *sky;
  //  scene::ISceneNode *selectedNode, *oldSelectedNode, *focusedNode, *oldFocusedNode;
    //video::SColor envBasicColor;
    MovementMgr *movemgr;
    MyCharacter *mychar;
    bool _freeCameraMove;
    void _CalcXYMoveVect(float o);
 //   core::vector2df xyCharMovement; // stores sin() and cos() values for current MyCharacter orientation, so that they need to be calculated only if the character turns around
    bool mouse_pressed_left;
    bool mouse_pressed_right;
    float old_char_o;
};

class SceneLoading : public Scene
{/*
    struct SceneNodeWithGridPos
    {
        scene::ISceneNode *Lscenenode;
        uint32 gx,gy;
    }; */

public:
    SceneLoading(PseuGUI *gui);
    void OnUpdate(uint32 timepassed);
	bool Finished;
	bool LoadingTime;
	uint32 doodadtotal;
	uint32 loaded;
	uint32 percent;
	std::string ls_path;
	std::list<std::string> AllDoodads;

private:
	//IGUIImage *loadingscreen;
	WorldSession *Lwsession;
    World *Lworld;
    MapMgr *Lmapmgr;
    bool Ldebugmode;
};




#endif
