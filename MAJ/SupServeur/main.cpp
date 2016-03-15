#include "constantes.hpp"
#include "serveur.hpp"

#include "ConfigEnv.h"

 typedef unsigned long        uint32;
typedef unsigned short       uint16;
typedef unsigned char        uint8;
uint8 loglevel = 3;
uint8 LogWorld = 1;

JeuComplet jeucomplet;

int main(int argc, char **argv)
{
	string lhostname, lusername, lpassword, ldatabase;
    int lport = 0;
    int ltype = 1;
    // Configure Main Database

    bool result;
    if(Config.MainConfig.SetSource("logonserver.conf"))
    {
      //  sLog->outColor(TGREEN, "ok!");
        //sLog->outColor(TNORMAL, "\n\n");
    }
    else
    {
        //sLog->outColor(TRED, "fail.");
        //sLog->outColor(TNORMAL, "\n\n");
        return true;
    }


    uint8 LogWorld = (uint8)Config.MainConfig.GetIntDefault("LogWorld", 0);
    uint8 loglevel = (uint8)Config.MainConfig.GetIntDefault("LogLevel", 0);
    uint32 cport = Config.MainConfig.GetIntDefault("RealmListPort", 3724);
	string host = Config.MainConfig.GetStringDefault("Host", "0.0.0.0");
	string filename = Config.MainConfig.GetStringDefault("LogFile", "LogFile.log");




    // Configure Logon Database...
    result = Config.MainConfig.GetString("LogonDatabase.Username", &lusername);
    result = !result ? result : Config.MainConfig.GetString("LogonDatabase.Password", &lpassword);
    result = !result ? result : Config.MainConfig.GetString("LogonDatabase.Hostname", &lhostname);
    result = !result ? result : Config.MainConfig.GetString("LogonDatabase.Name", &ldatabase);
    result = !result ? result : Config.MainConfig.GetInt("LogonDatabase.Port", &lport);
    result = !result ? result : Config.MainConfig.GetInt("LogonDatabase.Type", &ltype);


	if(result == false)
    {
		cout<<"sql: Logon database parameters not found."<< endl;
        return false;
    }
	
		


	if(SDL_Init(0)==-1)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}

	if(SDLNet_Init()==-1)
	{
	    printf("SDLNet_Init: %s\n", SDLNet_GetError());
	    exit(2);
	}

	//BDD.openConnection("127.0.0.1","comptes","root","");



	/*
    MYSQL mysql;
    mysql_init(&mysql);
    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(&mysql,"127.0.0.1","root","","comptes",0,NULL,0))
    {
		cout<<"connection a la BDD OK"<< endl;
        mysql_close(&mysql);
    }
    else
    {
		cout<<"Une erreur s'est produite lors de la connexion à la BDD!"<< endl;
	}
	*/
	serveur S1(_PORT_);
	S1.alloc_monstre(&jeucomplet);
	S1.lire_monstre(&jeucomplet);
	
	load_tileprops(&jeucomplet);
	
	load_map(&jeucomplet , "map/map1/map1.txt");
	S1.lancer(&jeucomplet);

	return(0);

}