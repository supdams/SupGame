#include "common.h"
#include "PseuGUI.h"
#include "PseuWoW.h"
#include "Scene.h"


SceneGuiStart::SceneGuiStart(PseuGUI *gui) : Scene(gui)
{
    /*
    irrlogo = guienv->addImage(driver->getTexture("data/misc/irrlichtlogo.png"), core::position2d<s32>(5,5));
    const char *fn;
    switch(gui->_driverType)
    {
        case video::EDT_DIRECT3D8:
        case video::EDT_DIRECT3D9:
        fn = "data/misc/directxlogo.png";
        break;

        case video::EDT_OPENGL:
        fn = "data/misc/opengllogo.png";
        break;

        default:
        fn = "data/misc/burninglogo.png";
    }
    driverlogo = guienv->addImage(driver->getTexture(fn), core::position2d<s32>(50,50));
	*/
}

void SceneGuiStart::OnDelete(void)
{
    // not necessary to delete the images, because they are deleted by guienv->clear()
}
