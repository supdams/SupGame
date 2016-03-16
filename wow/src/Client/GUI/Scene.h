#ifndef _SCENE_H
#define _SCENE_H


#include "SceneData.h"

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
    //core::stringw GetStringFromDB(u32 index, u32 entry, SCPDatabase *other_db = NULL);
    inline void SetState(SceneState sc) { _scenestate = sc; }
    inline SceneState GetState(void) { return _scenestate; }
  //  virtual void OnUpdate(s32);
    virtual void OnManualUpdate(void);
    virtual void OnDraw(void);
    virtual void OnDrawBegin(void);
    virtual void OnDelete(void);
    virtual void OnResize(void);
  //  virtual video::SColor GetBackgroundColor(void);
    virtual void SetData(uint32 index, uint32 value) { scenedata[index] = value; }

protected:
    PseuInstance *instance;
    PseuGUI *gui;

    CCursorController *cursor;
    SceneState _scenestate;
    uint32 scenedata[SCENEDATA_SIZE]; // generic storage for anything the PseuInstance thread wants to tell us
    SCPDatabase *textdb, *racedb, *classdb;
    ZThread::FastMutex mutex;
};

class SceneGuiStart : public Scene
{
public:
    SceneGuiStart(PseuGUI *gui);
    void OnDelete(void);


};

class SceneLogin : public Scene
{
public:
    SceneLogin(PseuGUI *gui);

    void OnDelete(void);

private:

    uint32 msgbox_textid;
};

class CharSelectGUIEventReceiver;






class ShTlTerrainSceneNode;
class MCameraFPS;
class MCameraOrbit;
class MyEventReceiver;
class MapMgr;
class WorldSession;
class MovementMgr;
class MyCharacter;
/*
class SceneWorld : public Scene
{
    struct SceneNodeWithGridPos
    {
//        scene::ISceneNode *scenenode;
        uint32 gx,gy;
    };

public:
    SceneWorld(PseuGUI *gui);
    void OnDraw(void);
    void OnDrawBegin(void);
    void OnDelete(void);
 //   void OnUpdate(s32);
    void UpdateTerrain(void);
    void InitTerrain(void);
    void RelocateCamera(void);
    void RelocateCameraBehindChar(void);
    void UpdateMapSceneNodes(std::map<uint32,SceneNodeWithGridPos>&);
//    scene::ISceneNode *GetMyCharacterSceneNode(void);
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
//    IGUIStaticText *debugText;
    bool debugmode;
    std::map<uint32,SceneNodeWithGridPos> _doodads;
    std::map<uint32,SceneNodeWithGridPos> _wmos;
    std::map<uint32,SceneNodeWithGridPos> _sound_emitters;
 //   scene::ISceneNode *sky;
   // scene::ISceneNode *selectedNode, *oldSelectedNode, *focusedNode, *oldFocusedNode;
    //video::SColor envBasicColor;
    MovementMgr *movemgr;
    MyCharacter *mychar;
    bool _freeCameraMove;
    void _CalcXYMoveVect(float o);
   // core::vector2df xyCharMovement; // stores sin() and cos() values for current MyCharacter orientation, so that they need to be calculated only if the character turns around
    bool mouse_pressed_left;
    bool mouse_pressed_right;
    float old_char_o;
};
*/


#endif
