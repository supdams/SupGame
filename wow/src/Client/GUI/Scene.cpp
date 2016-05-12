#include "common.h"
#include "PseuGUI.h"
#include "PseuWoW.h"
//#include "CCursorController.h"
#include "Scene.h"

Scene::Scene(PseuGUI *g)
{
    memset(scenedata, 0, sizeof(uint32) * SCENEDATA_SIZE);
    textdb = NULL;
    gui = g;
    instance = gui->GetInstance();
	//smgr = gui->_smgr;
   /*
	device = gui->_device;
    driver = gui->_driver;
    smgr = gui->_smgr;
    guienv = gui->_guienv;
    rootgui = guienv->getRootGUIElement();
    cursor = new CCursorController(device->getCursorControl(), driver);
    cursor->setOSCursorVisible(true);
    cursor->setVisible(false);
    cursor->render(); // apply above settings

    soundengine = gui->_soundengine;
	*/
}

void Scene::OnDraw(void)
{
   // device->yield();
}

void Scene::OnDelete(void)
{
}

void Scene::OnUpdate(uint32 timepassed)
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
/*
video::SColor Scene::GetBackgroundColor(void)
{
    return 0;
}
*/
Scene::~Scene()
{
   // delete cursor;
    DEBUG(logdebug("Scene::~Scene()"));
}

std::string Scene::GetStringFromDB(uint32 index, uint32 entry, SCPDatabase *other_db )
{
	std::string r = "";
    SCPDatabase *db = other_db ? other_db : textdb;
    if(!db)
    {
        r += "<string ";
        r += index;
        r += "/";
        r += entry;
        r += " not found>";
        return r;
    }
    char buf[20];
    sprintf(buf,"%u",entry);
    r += db->GetString(index, buf);
    return r;
}




void Scene::getInput(void)
{
	while (SDL_PollEvent(&events))
	{
		switch (events.type)
		{

   case SDL_WINDOWEVENT:
        switch(events.window.event)
        {
        case SDL_WINDOWEVENT_ENTER:
           // cout << "entered" << endl;
            break;

        case SDL_WINDOWEVENT_LEAVE:
          //  cout << "left" << endl;
            break;
        }
        break;




		case SDL_QUIT:
			//exit(0);
			break;


		case SDL_MOUSEBUTTONDOWN :
			switch (events.button.button)
			{
			case SDL_BUTTON_LEFT :
				clic = 1;
				break;
			default :
				break;
			}
			break;

		case SDL_MOUSEBUTTONUP :
			clic = 0;
			break;

		case SDL_MOUSEMOTION:
			posSouris.x = events.motion.x;
			posSouris.y = events.motion.y;
			break;

		default:
			break;
		}
	}
}