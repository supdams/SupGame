#include "common.h"
#include "PseuWoW.h"
#include <time.h>
#include <openssl/rand.h>

#include "ByteBuffer.h"

#include "Auth/BigNumber.h"
#include "Realm/RealmSession.h"
#include "World/WorldSession.h"
#include "World/CacheHandler.h"
#include "PseuGUI.h"
#include "RemoteController.h"
#include "Cli.h"



//###### Start of program code #######

PseuInstanceRunnable::PseuInstanceRunnable()
{
}

void PseuInstanceRunnable::run(void)
{
    _i = new PseuInstance(this);
    if(_i->Init())
    {
        _i->Run();
    }
    else
    {
        getchar(); // if init failed, wait for keypress before exit
    }
    delete _i;
}

void PseuInstanceRunnable::sleep(uint32 msecs)
{
    ZThread::Thread::sleep(msecs);
}

PseuInstance::PseuInstance(PseuInstanceRunnable *run)
{
    _runnable=run;
    _ver="PseuWoW Alpha Build 13.51" DEBUG_APPENDIX;
    _ver_short="A13.51" DEBUG_APPENDIX;
    _wsession=NULL;
    _rsession=NULL;
    _conf=NULL;
    _cli=NULL;
    _rmcontrol=NULL;
    _gui=NULL;
    _guithread=NULL;
    _stop=false;
    _fastquit=false;
    _startrealm=true;
    _createws=false;
    _creaters=false;
    _error=false;
    _initialized=false;
    for(uint32 i = 0; i < COND_MAX; i++)
    {
        _condition[i] = new ZThread::Condition(_mutex);
    }

}

PseuInstance::~PseuInstance()
{
    if(_cli)
    {
        _cli->stop();
        // delete _cli; // ok this is a little mem leak... can be fixed sometime in future
    }


    logdebug("Waiting for GUI to quit...");
    while(_gui)
        Sleep(1);

    if(_guithread)
        _guithread->wait();

    if(_rmcontrol)
        delete _rmcontrol;
    if(_rsession)
        delete _rsession;
    if(_wsession)
        delete _wsession;


    delete _conf;

    for(uint32 i = 0; i < COND_MAX; i++)
    {
        delete _condition[i];
    }

    log("--- Instance shut down ---");
}

bool PseuInstance::Init(void)
{
    log_setloglevel(0);
    log("");
    log("--- Initializing Instance ---");

    if(_confdir.empty())
        _confdir="./conf/";
    if(_scpdir.empty())
        _scpdir="./scripts/";

    srand((unsigned)time(NULL));
    RAND_set_rand_method(RAND_SSLeay()); // init openssl randomizer


    _conf=new PseuInstanceConf();



    CreateDir("cache");

    dbmgr.AddSearchPath("./cache");
    dbmgr.SetCompression(6);



    // TODO: find a better loaction where to place this block!
    if(GetConf()->enablegui)
    {
        if(InitGUI())
            logdebug("GUI: Init successful.");
        else
            logerror("GUI: Init failed!");
    }

    if(GetConf()->rmcontrolport)
    {
        _rmcontrol = new RemoteController(this,GetConf()->rmcontrolport);
    }

#if !(PLATFORM == PLATFORM_WIN32 && !defined(_CONSOLE))
    if(GetConf()->enablecli)
    {
        log("Starting CLI...");
        _cli = new CliRunnable(this);
        ZThread::Thread t(_cli);
    }
#endif

    if(_error)
    {
        logcritical("Errors while initializing!");
        return false;
    }

    log("Init complete.");
    _initialized=true;
    return true;
}

bool PseuInstance::InitGUI(void)
{
    if(GetGUI())
    {
        logerror("GUI: Aborting init, GUI already exists!");
        return false;
    }

        logerror("GUI: InitGUI fin!");
    return false;
}

void PseuInstance::Run(void)
{
    if(!_initialized)
        return;

    logdetail("PseuInstance: Initialized and running!");

    if(GetGUI())
    {
        while(!GetGUI()->IsInitialized())
            Sleep(1); // wait until the gui is ready. it will crash otherwise
        logdebug("GUI: switching to startup display...");
      //  GetGUI()->SetSceneState(SCENESTATE_GUISTART);
    }
    // TODO: as soon as username and password can be inputted into the gui, wait until it was set by user.

    if(GetConf()->realmlist.empty() || GetConf()->realmport==0)
    {
        logcritical("Realmlist address not set, can't connect.");
        SetError();
    }
    else
    {

        if(!GetConf()->enablegui || !(GetConf()->accname.empty() || GetConf()->accpass.empty()) )
        {
            logdebug("GUI not active or Login data pre-entered, skipping Login GUI");
            CreateRealmSession();
        }
        else
        {
          //  GetGUI()->SetSceneState(SCENESTATE_LOGINSCREEN);
        }

        // this is the mainloop
        while(!_stop)
        {
            Update();
            if(_error)
                _stop=true;
        }


    }

    // fastquit is defined if we clicked [X] (on windows)
    // If softquit is set, do not terminate forcefully, but shut it down instead
    if(_fastquit && !_conf->softquit)
    {
        log("Aborting Instance...");
        return;
    }

    log("Shutting down instance...");

    // if there was an error, better dont save, as the data might be damaged
    if(!_error)
    {
        SaveAllCache();
        //...
    }



    if(GetConf()->exitonerror == false && _error)
    {
        log("Exiting on error is disabled, PseuWoW is now IDLE");
        log("-- Press enter to exit --");
        char crap[100];
        fgets(crap,sizeof(crap),stdin); // workaround, need to press enter 2x for now
    }

}

void PseuInstance::Update()
{
    // if the user typed anything into the console, process it before anything else.
    // note that it can also be used for simulated cli commands sent by other threads, so it needs to be checked even if cli is disabled
    ProcessCliQueue();

    // delete sessions if they are no longer needed
    if(_rsession && _rsession->MustDie())
    {
        delete _rsession;
        _rsession = NULL;
    }

    if(_wsession && _wsession->MustDie())
    {
        delete _wsession;
        _wsession = NULL;
    }

    if(_createws)
    {
        _createws = false;
        if(_wsession)
            delete _wsession;
        _wsession = new WorldSession(this);
        _wsession->Start();
    }

    if(_creaters)
    {
        _creaters = false;
        if(_rsession)
            delete _rsession;
        ConnectToRealm();
    }

    // if we have no active sessions, we may reconnect, if no GUI is active for login
    if((!_rsession) && (!_wsession) && GetConf()->reconnect && !_gui)
    {
        if(GetConf()->accname.empty() || GetConf()->accpass.empty())
        {
            logdev("Skipping reconnect, acc name or password not set");
        }
        else
        {   // everything fine, we have all data
            logdetail("Waiting %u ms before reconnecting.",GetConf()->reconnect);
            for(uint32 t = 0; t < GetConf()->reconnect && !this->Stopped(); t+=100) Sleep(100);
            this->Sleep(1000); // wait 1 sec before reconnecting
            CreateRealmSession();
        }
    }
    if((!_rsession) && (!_wsession) && _gui)
    {
     
            logdetail("Disconnected, switching GUI back to Loginscreen.");
           // _gui->SetSceneState(SCENESTATE_LOGINSCREEN);
          //  while(_gui && _gui->GetSceneState() != SCENESTATE_LOGINSCREEN) // .. and wait until scenestate is set
            //    Sleep(1);
        
    }

    // update currently existing/active sessions
    if(_rsession)
        _rsession->Update();
    if(_wsession)
        try { _wsession->Update(); } catch (...)
        {
            logerror("Unhandled exception in WorldSession::Update()");
        }


    if(_rmcontrol)
    {
        _rmcontrol->Update();
        if(_rmcontrol->MustDie())
        {
            delete _rmcontrol;
            _rmcontrol = NULL;
        }
    }

//    GetScripts()->GetEventMgr()->Update();

    this->Sleep(GetConf()->networksleeptime);
}

void PseuInstance::ProcessCliQueue(void)
{


}

void PseuInstance::AddCliCommand(std::string cmd)
{
    _cliQueue.add(cmd);
}

void PseuInstance::SaveAllCache(void)
{
    //...
    if(GetWSession())
    {
        GetWSession()->plrNameCache.SaveToFile();
        ItemProtoCache_WriteDataToCache(GetWSession());
        CreatureTemplateCache_WriteDataToCache(GetWSession());
        GOTemplateCache_WriteDataToCache(GetWSession());
        //...
    }
}

void PseuInstance::Sleep(uint32 msecs)
{
    GetRunnable()->sleep(msecs);
}

void PseuInstance::DeleteGUI(void)
{
    _gui = NULL;
    delete _guithread; // since it was allocated with new
    _guithread = NULL;
//    if(GetScripts()->ScriptExists("_onguiclose"))
  //      AddCliCommand("_onguiclose"); // since this func is called from another thread, use threadsafe variant via CLI

    // if console mode is disabled in windows, closing the gui needs to close the app
#if PLATFORM == PLATFORM_WIN32 && !defined(_CONSOLE)
    this->Stop();
#endif
}

bool PseuInstance::ConnectToRealm(void)
{
    _rsession = new RealmSession(this);
    _rsession->SetLogonData(); // get accname & accpass from PseuInstanceConfig and set it in the realm session
    _rsession->Connect();
    if(_rsession->MustDie() || !_rsession->SocketGood()) // something failed. it will be deleted in next Update() call
    {
        logerror("PseuInstance: Connecting to Realm failed!");
  //      if(_gui)
//            _gui->SetSceneData(ISCENE_LOGIN_CONN_STATUS, DSCENE_LOGIN_CONN_FAILED);
        return false;
    }

    _rsession->SendLogonChallenge();
    return true;
}

void PseuInstance::WaitForCondition(InstanceConditions c, uint32 timeout /* = 0 */)
{
    _mutex.acquire();
    if(timeout)
        _condition[c]->wait(timeout);
    else
        _condition[c]->wait();
    _mutex.release();
}

PseuInstanceConf::PseuInstanceConf()
{
    enablecli=false;
    enablegui=false;
    exitonerror=false;
    debug=0;
    rmcontrolport=0;
}

void PseuInstanceConf::ApplyFromVarSet()
{
    debug=3;
    realmlist="mangos";
    accname="test";
    accpass="test";
    exitonerror=0;
    reconnect=5000;
    realmport=9090;
    client=1;
    clientlang="";
    realmname="mangos";
    charname="dams";
    networksleeptime=1;
    showopcodes=1;
    hidefreqopcodes=1;
    hideDisabledOpcodes=0;
    enablecli=0;
    allowgamecmd=0;
    notifyping=0;
    showmyopcodes=0;
    disablespellcheck=0;
    enablegui=1;
    rmcontrolport=8010;
    rmcontrolhost="localhost";
    rmcontrolpass="blabldddd";
    useMaps=0;
    skipaddonchat=0;
    dumpPackets=1;
    softquit=0;
    dataLoaderThreads=2;
    useMPQ=0;



















    switch(client)
    {
      case CLIENT_CLASSIC_WOW:
      {
        clientbuild = 6005;
        clientversion_string="1.12.2";
        break;
      }
      case CLIENT_TBC:
      {
        clientbuild = 8606;
        clientversion_string="2.4.3";
        break;
      }
      case CLIENT_WOTLK:
      {
        clientbuild = 12340;
        clientversion_string="3.3.5";
        break;
      }
      case CLIENT_CATA:
      default:
      {
        logerror("Unknown client - check conf");
      }
    }

    // clientversion is a bit more complicated to add
    std::string opt=clientversion_string + ".";
    std::string num;
    uint8 p=0;
    for(uint8 i=0;i<opt.length();i++)
    {
        if(!isdigit(opt.at(i)))
        {
            clientversion[p]=(unsigned char)atoi(num.c_str());
            num.clear();
            p++;
            if(p>2)
                break;
            continue;
        }
        num+=opt.at(i);
    }


    // cleanups, internal settings, etc.
    log_setloglevel(debug);
    log_setlogtime(1);
//    MemoryDataHolder::SetThreadCount(dataLoaderThreads);
  //  MemoryDataHolder::SetUseMPQ(clientlang);
}




PseuInstanceConf::~PseuInstanceConf()
{
        //...
}




