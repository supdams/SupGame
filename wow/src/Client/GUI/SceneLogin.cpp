#include "common.h"
#include "PseuGUI.h"
#include "PseuWoW.h"
#include "Scene.h"
#include "Realm/RealmSession.h"
#include "GUIEventReceiver.h"


enum GuiElementID
{
	TEXTBOX_NAME = 0x1,
	TEXTBOX_PASSWORD = 0x2,
	BUTTON_QUIT = 0x4,
	BUTTON_COMMUNITY = 0x8,
	BUTTON_LOGON = 0x10,
};

//TODO: Reposition elements on resize
//the code happens only ones, We need to, in the main loop ( usually while(driver->run()) ) set the
//positions of each gui element based on the current screensize ( gotten with driver->getScreenSize(); )

SceneLogin::SceneLogin(PseuGUI *gui) : Scene(gui)
{
	logdetail("@@@@@@@@@@@@@@@@@@@@@@@@@@");
	logdetail("@@@@  SceneLogin      @@@@");
	logdetail("@@@@@@@@@@@@@@@@@@@@@@@@@@");
	SDL_SetWindowTitle(gui->screen, "SceneLogin");
	SDL_RenderClear(gui->renderer);

	SDL_Color Couleur_Noir;
	Couleur_Noir.r = 0;
	Couleur_Noir.g = 0;
	Couleur_Noir.b = 0;

	SDL_Rect pos;
	pos.x = 305;
	pos.y = 250;
	pos.w = 200;
	pos.h = 30;

	memset(&gui->pseudo, 0, sizeof(TextEdition));
	gui->pseudo.blitStyle = TE_BLITSTYLE_BLENDED;
	gui->pseudo.colorBG = Couleur_Noir;
	TE_NewTextEdition(&gui->pseudo, 30, pos,  gui->font[7], Couleur_Noir, TE_STYLE_NORMAL | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA, gui->renderer);

	TE_SetEditionText(&gui->pseudo, "player");


	SDL_Rect pos2;
	pos2.x = 305;
	pos2.y = 380;
	pos2.w = 200;
	pos2.h = 30;

	memset(&gui->mot_de_passe, 0, sizeof(TextEdition));
	gui->mot_de_passe.blitStyle = TE_BLITSTYLE_BLENDED;
	gui->mot_de_passe.colorBG = Couleur_Noir;
	TE_NewTextEdition(&gui->mot_de_passe, 20, pos2,  gui->font[7], Couleur_Noir, TE_STYLE_NORMAL | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA, gui->renderer);

	TE_SetEditionText(&gui->mot_de_passe, "player");

	textdb = instance->dbmgr.GetDB("gui_login_text");
	msgbox_textid = 0;
	clic = 0;
	bouton = gui->loadImage("img/Image_432.png");
	bouton2 = gui->loadImage("img/Image_436.png");

	posSouris.x = 0;
	posSouris.y = 0;

	text_nom_du_jeu = gui->renderText("Thé Fan Oc", gui->font[3], Couleur_Noir);
	text_titre_page = gui->renderText("page connexion jeu", gui->font[4], Couleur_Noir);
	text_Pseudo = gui->renderText("Pseudo:", gui->font[5], Couleur_Noir);
	text_mot_de_passe =	gui->renderText("Mot de passe: ", gui->font[5], Couleur_Noir);
	text_Bouton_Valider = gui->renderText("Valider", gui->font[5],Couleur_Noir);


}

void SceneLogin::OnUpdate(uint32 timepassed)
{

	logdetail("@@@@@@@@@@@OnUpdate@@@@@@@@@@@@@@@");


	SDL_Color Couleur_Noir;
	Couleur_Noir.r = 0;
	Couleur_Noir.g = 0;
	Couleur_Noir.b = 0;
	SDL_Delay(100);
	getInput();
	 
	//SDL_Event event;

	//SDL_WaitEvent(&gui->events);


	SDL_RenderClear(gui->renderer);

	TE_HoldTextEdition(&gui->pseudo, events);
	TE_DisplayTextEdition(&gui->pseudo);

	TE_HoldTextEdition(&gui->mot_de_passe, events);
	TE_DisplayTextEdition(&gui->mot_de_passe);


	gui->Affficher_Texture(text_nom_du_jeu, 250, 0);
	gui->Affficher_Texture(text_titre_page, 50, 90);


	gui->Affficher_Texture(text_Pseudo, 360,220);
	gui->Affficher_Texture(text_mot_de_passe, 350, 350);



	msgbox_textid = scenedata[ISCENE_LOGIN_CONN_STATUS];
	std::string str = GetStringFromDB(ISCENE_LOGIN_CONN_STATUS,msgbox_textid).c_str();
	char* chr = strdup(str.c_str());
	gui->drawString(chr, 250, 120, gui->font[5], Couleur_Noir);
	logerror("msgbox_textid %s,",GetStringFromDB(ISCENE_LOGIN_CONN_STATUS,msgbox_textid).c_str());


/*
if(eventrecv->buttons & BUTTON_QUIT)
{
instance->Stop();
}
*/

	SDL_Rect AB;
	AB.x = 320;
	AB.y = 550;
	AB.h = 35;
	AB.w= 172;

	if(gui->AABBCollision(posSouris.x , posSouris.y , AB))
	{
		gui->Affficher_Texture(bouton2, 320, 550);
		gui->Affficher_Texture(text_Bouton_Valider,350, 560);				

		if(clic)/*Si on clic avec le clic gauche de la souris*/
		{

			std::string accname = std::string(gui->pseudo.text);
			std::string accpass = std::string(gui->mot_de_passe.text);

			if(accname.size() && accpass.size())
			{
				SetData(ISCENE_LOGIN_CONN_STATUS,DSCENE_LOGIN_CONN_ATTEMPT);
				logdebug("Trying to set Logon Data %u, %u", accname.size(), accpass.size());
				// we can safely override the conf settings
				instance->GetConf()->accname = accname;
				instance->GetConf()->accpass = accpass;
				// ...but do not set the defscript vars; its just not safe
				instance->CreateRealmSession();
			}else{
				//popup = guienv->addMessageBox(GetStringFromDB(ISCENE_LOGIN_MSGBOX_DUMMY,0).c_str(),GetStringFromDB(ISCENE_LOGIN_MSGBOX_DUMMY,1).c_str());
				eventrecv->react_to_keys = false; // prevent main window from processing key input; it must be processed by the msgbox's event recv!
				// our eventrecv will handle msgbox close event by itself and enable input again.
			}

		}
	}
	else
	{
		gui->Affficher_Texture(bouton, 320, 550);
		gui->Affficher_Texture(text_Bouton_Valider,350, 560);		
	}
	SDL_RenderPresent(gui->renderer);


}


/*



if(msgbox_textid != scenedata[ISCENE_LOGIN_CONN_STATUS])
{
msgbox_textid = scenedata[ISCENE_LOGIN_CONN_STATUS];
msgbox->setText(GetStringFromDB(ISCENE_LOGIN_CONN_STATUS,msgbox_textid).c_str());
}

if(eventrecv->buttons & BUTTON_QUIT)
{
instance->Stop();
}

if(eventrecv->buttons & BUTTON_LOGON)
{
logdebug("Commencing Logon");
core::stringc tmp;
tmp=rootgui->getElementFromId(TEXTBOX_NAME,true)->getText();
std::string accname =tmp.c_str();
tmp=rootgui->getElementFromId(TEXTBOX_PASSWORD,true)->getText();
std::string accpass=tmp.c_str();
if(accname.size() && accpass.size())
{
SetData(ISCENE_LOGIN_CONN_STATUS,DSCENE_LOGIN_CONN_ATTEMPT);
logdebug("Trying to set Logon Data %u, %u", accname.size(), accpass.size());
// we can safely override the conf settings
instance->GetConf()->accname = accname;
instance->GetConf()->accpass = accpass;
// ...but do not set the defscript vars; its just not safe
instance->CreateRealmSession();
}
else
{
popup = guienv->addMessageBox(GetStringFromDB(ISCENE_LOGIN_MSGBOX_DUMMY,0).c_str(),
GetStringFromDB(ISCENE_LOGIN_MSGBOX_DUMMY,1).c_str());
eventrecv->react_to_keys = false; // prevent main window from processing key input; it must be processed by the msgbox's event recv!
// our eventrecv will handle msgbox close event by itself and enable input again.
}

}
if(eventrecv->buttons & BUTTON_COMMUNITY)
{
#if PLATFORM == PLATFORM_WIN32
ShellExecute(NULL, "open", "http://www.mangosclient.org", NULL, NULL, SW_SHOWNORMAL);
#elif PLATFORM == PLATFORM_UNIX
// linux code here
#elif PLATFORM == PLATFORM_APPLE
// mac code here
#endif
}
eventrecv->buttons = 0;
*/



void SceneLogin::OnDelete(void)
{
	// sound will be stopped after char selection
	// not necessary to delete the images, because they are deleted by guienv->clear()

	logdetail("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
	logdetail("@@@@   SceneLogin  OnDelete   @@@@");
	logdetail("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");

	TE_DeleteTextEdition(&gui->mot_de_passe);// les truc d ecriture mdp
	TE_DeleteTextEdition(&gui->pseudo);// les truc d ecriture pseudo

	SDL_DestroyTexture(bouton); // Libération de la mémoire associée à la texture
	bouton = NULL;

	SDL_DestroyTexture(bouton2);
	bouton2 = NULL;

	SDL_DestroyTexture(text_titre_page);
	text_titre_page = NULL;

	SDL_DestroyTexture(text_titre_page);
	text_titre_page = NULL;

	SDL_DestroyTexture(text_Pseudo);
	text_Pseudo = NULL;

	SDL_DestroyTexture(text_mot_de_passe);
	text_mot_de_passe = NULL;

	SDL_DestroyTexture(text_Bouton_Valider);
	text_Pseudo = NULL;

	SDL_RenderClear(gui->renderer);

	SDL_SetRenderDrawColor(gui->renderer, 255, 255, 255, 255);
}

