#include "common.h"

#include "World/Object.h"

#include "PseuWoW.h"
#include "PseuGUI.h"

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
    _passtime = _lastpasstime = _passtimediff = 0;
    _usesound = false;
}

PseuGUI::~PseuGUI()
{

    _instance->DeleteGUI(); // this makes the instance set its gui ptr to NULL
    logdebug("PseuGUI::~PseuGUI()");
}



// if this fuction is called from another thread the device will not work correctly. o_O
void PseuGUI::_Init(void)
{


    logdebug("PseuGUI::Init()");






    _initialized = true;// pour la boucle ,
}

void PseuGUI::Cancel(void)
{
    DEBUG(logdebug("PseuGUI::Cancel()"));

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


                     // all SceneNodes and everything. the ptrs are still stored in the DrawObjects, means they need to be unlinked now not to cause a crash.
    DEBUG(logdebug("PseuGUI::Run() finished"));
    Cancel(); // already got shut down somehow, we can now safely cancel and drop the device
}





void PseuGUI::SetInstance(PseuInstance* in)
{
    _instance = in;
}



