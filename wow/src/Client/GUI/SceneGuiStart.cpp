#include "common.h"
#include "PseuGUI.h"
#include "PseuWoW.h"
#include "Scene.h"
#include "fonction.h"

SceneGuiStart::SceneGuiStart(PseuGUI *gui) : Scene(gui)
{
	logdetail("@@@@@@@@@@@@@@@@@@@@@@@@@@");
	logdetail("SceneGuiStart::SceneGuiStart(PseuGUI *gui) : Scene(gui)");
	logdetail("@@@@@@@@@@@@@@@@@@@@@@@@@@");
	
    SDL_WM_SetCaption("SceneGuiStart", NULL);

    SDL_Surface *ecran = NULL, *imageDeFond = NULL;
    SDL_Rect positionFond;
 
    positionFond.x = 0;
    positionFond.y = 0;
 
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
 
    imageDeFond = load_image("data/misc/burninglogo.png");
    SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
 
 
    SDL_Flip(ecran);
	
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

}
