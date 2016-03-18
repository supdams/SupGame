#include "common.h"
#include "PseuGUI.h"
#include "PseuWoW.h"
#include "Scene.h"

Scene::Scene(PseuGUI *g)
{
    memset(scenedata, 0, sizeof(uint32) * SCENEDATA_SIZE);
    textdb = NULL;
    gui = g;
    instance = gui->GetInstance();
//    device = gui->_device;
  //  driver = gui->_driver;
    //smgr = gui->_smgr;
  //  guienv = gui->_guienv;
   // rootgui = guienv->getRootGUIElement();
   // cursor = new CCursorController(device->getCursorControl(), driver);
    //cursor->setOSCursorVisible(true);
  //   cursor->setVisible(false);
  //   cursor->render(); // apply above settings

   // soundengine = gui->_soundengine;
}

void Scene::OnDraw(void)
{
  //   device->yield();
}

void Scene::OnDelete(void)
{
}



void Scene::OnManualUpdate(void)
{
    OnResize();
}

void Scene::OnResize(void)
{
}

void Scene::OnDrawBegin(void)
{
}

void Scene::OnUpdate(uint32 timepassed)
{

}



Scene::~Scene()
{
//    delete cursor;
    DEBUG(logdebug("Scene::~Scene()"));
}

