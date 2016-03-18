#ifndef PSEUGUI_H
#define PSEUGUI_H

#include "World.h"

class PseuGUI;
class Object;
class PseuInstance;
class Scene;


enum SceneState
{
    SCENESTATE_NULL = 0,
    SCENESTATE_GUISTART,
    SCENESTATE_LOGINSCREEN,
    SCENESTATE_REALMSELECT, // actually both realm and char select will call the same scene, but with slightly different data
    SCENESTATE_CHARSELECT,
    SCENESTATE_LOADING,
    SCENESTATE_WORLD,
    SCENESTATE_NOSCENE = 0xFFFFFFFF
};

enum DriverIDs
{
    NULLDEVICE = 0,
    SOFTWARE = 1,
    BURNINGSVIDEO = 2,
    OPENGL = 3,
    DIRECTX8 = 4,
    DIRECTX9 = 5,
};

#define MOUSE_SENSIVITY 0.5f
#define ANGLE_STEP (M_PI/180.0f)
#define DEG_TO_RAD(x) ((x)*ANGLE_STEP)
#define RAD_TO_DEG(x) ((x)/ANGLE_STEP)
#define RAD_FIX(x) ( (x)>(2*M_PI) ? ((x)-(2*M_PI)) : ( ((x)<0) ? ((x)+(2*M_PI)) : (x) ) )
#define DEG_FIX(x) ( (x)>360 ? ((x)-360) : ( ((x)<0) ? ((x)+360) : (x) ) )
#define IRR_TO_O(x) RAD_FIX(M_PI-DEG_TO_RAD(x)) // convert World orientation (rad) into irrlicht rotation (deg)
#define O_TO_IRR(x) DEG_FIX(180-RAD_TO_DEG(x)) // ... and the other way around



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

    friend class SceneGuiStart;
    friend class SceneLogin;
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
    inline bool IsInitialized(void) { return _initialized; }
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
    PseuInstance *_instance;
    SceneState _scenestate, _scenestate_new;
    Scene *_scene;
    uint32 _passtime, _lastpasstime, _passtimediff;
    uint32 _throttle;//used for frameratelimiting
    bool _updateScene; // manually update scene?

};


#endif

