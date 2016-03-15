#include "common.h"

#include "Auth/Sha1.h"
#include "Auth/BigNumber.h"
#include "Auth/AuthCrypt.h"
#include "WorldPacket.h"
#include "WorldSocket.h"
#include "Channel.h"
#include "ObjMgr.h"
#include "World.h"
#include "MapTile.h"
#include "WorldSession.h"
#include "MovementInfo.h"
#include "MovementMgr.h"
#include "Realm/RealmSession.h"
#include "Realm/RealmSocket.h"


struct OpcodeHandler
{
    uint16 opcode;
    void (WorldSession::*handler)(WorldPacket& recvPacket);
};

uint32 Object::maxvalues[TYPEID_MAX];
UpdateField Object::updatefields[UPDATEFIELDS_NAME_COUNT];
uint8 MovementInfo::_c=CLIENT_UNKNOWN;

WorldSession::WorldSession(PseuInstance *in)
{
    logdebug("-> Starting WorldSession 0x%X from instance 0x%X",this,in); // should never output a null ptr
    _instance = in;
    _mustdie=false;
    _logged=false;
    _socket=NULL;
    _myGUID=0; // i dont have a guid yet
    _channels = new Channel(this);
    _world = new World(this);
    _sh.SetAutoCloseSockets(false);
    objmgr.SetInstance(in);
    _lag_ms = 0;
    //...

    _SetupObjectFields();
    MovementInfo::_c=in->GetConf()->client;

  //  in->GetScripts()->RunScriptIfExists("_onworldsessioncreate");

    DEBUG(logdebug("WorldSession 0x%X constructor finished",this));
}

WorldSession::~WorldSession()
{
    if(PseuGUI *gui = GetInstance()->GetGUI())
    {
        // if the realm session still exists, the connection to the world server was not successful
        // and we need to show realmlist window again
        if(_instance->GetRSession())
        {
           // gui->SetSceneState(SCENESTATE_REALMSELECT);
        }
        else
        {
          //  gui->SetSceneState(SCENESTATE_LOGINSCREEN); // kick back to login gui
        }
        logdebug("~WorldSession(): Waiting until world GUI is deleted");
      //  while(gui->GetSceneState() == SCENESTATE_WORLD) // .. and wait until the world gui is really deleted
        //    GetInstance()->Sleep(1);                       // (it can cause crash otherwise)
        logdebug("~WorldSession(): ... world GUI deleted, continuing to close session");
    }

//    _instance->GetScripts()->RunScriptIfExists("_onworldsessiondelete");

    logdebug("~WorldSession(): %u packets left unhandled, and %u delayed. deleting.",pktQueue.size(),delayedPktQueue.size());
    WorldPacket *packet;
    // clear the queue
    while(pktQueue.size())
    {
        packet = pktQueue.next();
        delete packet;
    }
    // clear the delayed queue
    while(delayedPktQueue.size())
    {
        packet = delayedPktQueue.back().pkt;
        delayedPktQueue.pop_back();
        delete packet;
    }

    if(_channels)
        delete _channels;
    if(_socket)
        delete _socket;
    if(_world)
        delete _world;
    DEBUG(logdebug("~WorldSession() this=0x%X _instance=0x%X",this,_instance));
}

void WorldSession::SetMustDie(void)
{
    _mustdie = true;
    logdebug("WorldSession: Must die now.");
}

void WorldSession::Start(void)
{
    log("Connecting to '%s' on port %u",GetInstance()->GetConf()->worldhost.c_str(),GetInstance()->GetConf()->worldport);
    _socket=new WorldSocket(_sh,this);
    _socket->Open(GetInstance()->GetConf()->worldhost,GetInstance()->GetConf()->worldport);
    _sh.Add(_socket);

    // if we cant connect, wait until the socket gives up (after 5 secs)
    while( (!MustDie()) && (!_socket->IsOk()) && (!GetInstance()->Stopped()) )
    {
        logdev("WorldSession::Start(): Socket not ok, waiting...");
        _sh.Select(3,0);
        GetInstance()->Sleep(100);
    }
    logdev("WorldSession::Start() done, mustdie:%u, socket_ok:%u stopped:%u",MustDie(),_socket->IsOk(),GetInstance()->Stopped());
}

void WorldSession::_LoadCache(void)
{
    logdetail("Loading Cache...");
    plrNameCache.ReadFromFile(); // load names/guids of known players
    ItemProtoCache_InsertDataToSession(this);
    CreatureTemplateCache_InsertDataToSession(this);
    GOTemplateCache_InsertDataToSession(this);
    //...
}

void WorldSession::AddToPktQueue(WorldPacket *pkt)
{
    pktQueue.add(pkt);
}

void WorldSession::SendWorldPacket(WorldPacket &pkt)
{
    if(GetInstance()->GetConf()->showmyopcodes)
        logcustom(0,BROWN,"<< Opcode %u [%s] (%u bytes)", pkt.GetOpcode(), GetOpcodeName(pkt.GetOpcode()), pkt.size());
    if(_socket && _socket->IsOk())
        _socket->SendWorldPacket(pkt);
    else
    {
        logerror("WorldSession: Can't send WorldPacket, socket doesn't exist or is not ready.");
    }
}

void WorldSession::Update(void)
{
    if( _sh.GetCount() ) // the socket will remove itself from the handler if it got closed
        _sh.Select(0,0);
    else // so we just need to check if the socket doesnt exist or if it exists but isnt valid anymore.
    {    // if thats the case, we dont need the session anymore either
        if(!_socket || (_socket && !_socket->IsOk()))
        {
            _OnLeaveWorld();
            SetMustDie();
        }
    }

    // process the send queue and send packets buffered by other threads
    while(sendPktQueue.size())
    {
        WorldPacket *pkt = sendPktQueue.next();
        SendWorldPacket(*pkt);
        delete pkt;
    }

    // while there are packets on the queue, handle them
    while(pktQueue.size())
    {
        HandleWorldPacket(pktQueue.next());
    }

    // now check if there are packets that couldnt be handled earlier due to missing data
    _HandleDelayedPackets();

    _DoTimedActions();

    if(_world)
        _world->Update();
}

// this func will delete the WorldPacket after it is handled!
void WorldSession::HandleWorldPacket(WorldPacket *packet)
{
//    static DefScriptPackage *sc = GetInstance()->GetScripts();
    static OpcodeHandler *table = _GetOpcodeHandlerTable();

    bool known = false;
    uint16 hpos;
    for (hpos = 0; table[hpos].handler != NULL; hpos++)
    {
        if (table[hpos].opcode == packet->GetOpcode())
        {
            known=true;
            break;
        }
    }

    bool hideOpcode = false;
    bool disabledOpcode = IsOpcodeDisabled(packet->GetOpcode());
    if(disabledOpcode && GetInstance()->GetConf()->hideDisabledOpcodes)
        hideOpcode = true;

    // TODO: Maybe make table or something with all the frequently opcodes
    if (GetInstance()->GetConf()->hidefreqopcodes)
    {
        switch(packet->GetOpcode())
        {
            case SMSG_MONSTER_MOVE:
            hideOpcode = true;
        }
    }

    if( (known && GetInstance()->GetConf()->showopcodes==1)
        || ((!known) && GetInstance()->GetConf()->showopcodes==2)
        || (GetInstance()->GetConf()->showopcodes==3) )
    {
        if(!hideOpcode)
            logcustom(1,YELLOW,">> Opcode %u [%s] (%s, %u bytes)", packet->GetOpcode(), GetOpcodeName(packet->GetOpcode()), (known ? (disabledOpcode ? "Disabled" : "Known") : "UNKNOWN"), packet->size());
    }

    if( (!known) && GetInstance()->GetConf()->dumpPackets > 1)
    {
        DumpPacket(*packet);
    }

    try
    {
        // if there is a script attached to that opcode, call it now.
        // note: the pkt rpos needs to be reset by the scripts!
        std::string scname = "opcode::";
        scname += stringToLower(GetOpcodeName(packet->GetOpcode()));
 


        // call the opcode handler
        if(known && !disabledOpcode)
        {
            packet->rpos(0);
            (this->*table[hpos].handler)(*packet);
        }
    }
    catch (ByteBufferException bbe)
    {
        char errbuf[200];
        sprintf(errbuf,"attempt to \"%s\" %lu bytes at position %lu out of total %lu bytes. (wpos=%lu)", bbe.action, bbe.readsize, bbe.rpos, bbe.cursize, bbe.wpos);
        logerror("Exception while handling opcode %u [%s]!",packet->GetOpcode(),GetOpcodeName(packet->GetOpcode()));
        logerror("WorldSession: ByteBufferException");
        logerror("ByteBuffer reported: %s", errbuf);
        // copied from below
        logerror("Data: pktsize=%u, handler=0x%X queuesize=%u",packet->size(),table[hpos].handler,pktQueue.size());
        logerror("Packet Hexdump:");
        logerror("%s",toHexDump((uint8*)packet->contents(),packet->size(),true).c_str());

        if(GetInstance()->GetConf()->dumpPackets)
            DumpPacket(*packet, bbe.rpos, errbuf);
    }
    catch (...)
    {
        logerror("Exception while handling opcode %u [%s]!",packet->GetOpcode(),GetOpcodeName(packet->GetOpcode()));
        logerror("Data: pktsize=%u, handler=0x%X queuesize=%u",packet->size(),table[hpos].handler,pktQueue.size());
        logerror("Packet Hexdump:");
        logerror("%s",toHexDump((uint8*)packet->contents(),packet->size(),true).c_str());

        if(GetInstance()->GetConf()->dumpPackets)
            DumpPacket(*packet, packet->rpos(), "unknown exception");
    }

    delete packet;
}


OpcodeHandler *WorldSession::_GetOpcodeHandlerTable() const
{
    static OpcodeHandler table[] =
    {
        { SMSG_AUTH_CHALLENGE,       &WorldSession::_HandleAuthChallengeOpcode     },
        { SMSG_AUTH_RESPONSE,        &WorldSession::_HandleAuthResponseOpcode      },
        {SMSG_CHAR_ENUM,     &WorldSession::_HandleCharEnumOpcode},
        {SMSG_SET_PROFICIENCY, &WorldSession::_HandleSetProficiencyOpcode},
        {SMSG_ACCOUNT_DATA_TIMES,  &WorldSession::_HandleAccountDataMD5Opcode},
        {SMSG_MESSAGECHAT, &WorldSession::_HandleMessageChatOpcode},
        {SMSG_NAME_QUERY_RESPONSE, &WorldSession::_HandleNameQueryResponseOpcode},
        {SMSG_PONG, &WorldSession::_HandlePongOpcode},
        {SMSG_TRADE_STATUS, &WorldSession::_HandleTradeStatusOpcode},
        {SMSG_GROUP_INVITE, &WorldSession::_HandleGroupInviteOpcode},
        {SMSG_CHANNEL_NOTIFY, &WorldSession::_HandleChannelNotifyOpcode},

        // movement opcodes
        {MSG_MOVE_SET_FACING, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_FORWARD, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_BACKWARD, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_STOP, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_STRAFE_LEFT, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_STRAFE_RIGHT, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_STOP_STRAFE, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_JUMP, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_TURN_LEFT, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_TURN_RIGHT, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_STOP_TURN, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_START_SWIM, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_STOP_SWIM, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_HEARTBEAT, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_FALL_LAND, &WorldSession::_HandleMovementOpcode},
        {MSG_MOVE_TELEPORT_ACK, &WorldSession::_HandleTelePortAckOpcode},

        // set speed opcodes
        {MSG_MOVE_SET_WALK_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_RUN_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_RUN_BACK_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_SWIM_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_SWIM_BACK_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_TURN_RATE, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_FLIGHT_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_FLIGHT_BACK_SPEED, &WorldSession::_HandleSetSpeedOpcode},
        {MSG_MOVE_SET_PITCH_RATE, &WorldSession::_HandleSetSpeedOpcode},

        // force set speed opcodes
        {SMSG_FORCE_WALK_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_RUN_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_RUN_BACK_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_SWIM_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_SWIM_BACK_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_TURN_RATE_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_FLIGHT_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_FLIGHT_BACK_SPEED_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},
        {SMSG_FORCE_PITCH_RATE_CHANGE, &WorldSession::_HandleForceSetSpeedOpcode},

        {SMSG_COMPRESSED_UPDATE_OBJECT, &WorldSession::_HandleCompressedUpdateObjectOpcode},
        {SMSG_UPDATE_OBJECT, &WorldSession::_HandleUpdateObjectOpcode},
        {SMSG_CAST_FAILED, &WorldSession::_HandleCastResultOpcode},
        /*{SMSG_CLEAR_EXTRA_AURA_INFO_OBSOLETE, &WorldSession::_HandleCastSuccessOpcode},  <--- RECHECK ME ! - cast success removed ?!? */
        {SMSG_ITEM_QUERY_SINGLE_RESPONSE, &WorldSession::_HandleItemQuerySingleResponseOpcode},
        {SMSG_DESTROY_OBJECT, &WorldSession::_HandleDestroyObjectOpcode},
        {SMSG_INITIAL_SPELLS, &WorldSession::_HandleInitialSpellsOpcode},
        {SMSG_LEARNED_SPELL, &WorldSession::_HandleLearnedSpellOpcode},
        {SMSG_REMOVED_SPELL, &WorldSession::_HandleLearnedSpellOpcode},
        {SMSG_CHANNEL_LIST, &WorldSession::_HandleChannelListOpcode},
        {SMSG_EMOTE, &WorldSession::_HandleEmoteOpcode},
        {SMSG_TEXT_EMOTE, &WorldSession::_HandleTextEmoteOpcode},
        {SMSG_NEW_WORLD, &WorldSession::_HandleNewWorldOpcode},
        {SMSG_LOGIN_VERIFY_WORLD, &WorldSession::_HandleLoginVerifyWorldOpcode},
        {SMSG_MOTD, &WorldSession::_HandleMotdOpcode},
        {SMSG_NOTIFICATION, &WorldSession::_HandleNotificationOpcode},
        {SMSG_WHO, &WorldSession::_HandleWhoOpcode},
        {SMSG_CREATURE_QUERY_RESPONSE, &WorldSession::_HandleCreatureQueryResponseOpcode},
        {SMSG_GAMEOBJECT_QUERY_RESPONSE, &WorldSession::_HandleGameobjectQueryResponseOpcode},
        {SMSG_CHAR_CREATE, &WorldSession::_HandleCharCreateOpcode},
        {SMSG_MONSTER_MOVE, &WorldSession::_HandleMonsterMoveOpcode},

        // table termination
        { 0,                         NULL }
    };
    return table;
}

void WorldSession::_DelayWorldPacket(WorldPacket& pkt, uint32 ms)
{
    DEBUG(logdebug("DelayWorldPacket (%s, size: %u, ms: %u)",GetOpcodeName(pkt.GetOpcode()),pkt.size(),ms));
    // need to copy the packet, because the current packet will be deleted after it got handled
    WorldPacket *pktcopy = new WorldPacket(pkt.GetOpcode(),pkt.size());
    pktcopy->append(pkt.contents(),pkt.size());
    delayedPktQueue.push_back(DelayedWorldPacket(pktcopy,ms));
    DEBUG(logdebug("-> WP ptr = 0x%X",pktcopy));
}

void WorldSession::_HandleDelayedPackets(void)
{
    if(delayedPktQueue.size())
    {
        DelayedPacketQueue copy(delayedPktQueue);
        delayedPktQueue.clear(); // clear original, since it might be filled up by newly delayed packets, which would cause endless loop
        while(copy.size())
        {
            DelayedWorldPacket d = copy.front();
            copy.pop_front(); // remove packet from front
            if(clock() >= d.when) // if its time to handle this packet, do so
            {
                DEBUG(logdebug("Handling delayed packet (%s [%u], size: %u, ptr: 0x%X)",GetOpcodeName(d.pkt->GetOpcode()),d.pkt->GetOpcode(),d.pkt->size(),d.pkt));
                HandleWorldPacket(d.pkt);
            }
            else
            {
                delayedPktQueue.push_back(d); // and if not, put it again onto the queue
            }
        }
    }
}

// use this func to send packets from other threads
void WorldSession::AddSendWorldPacket(WorldPacket *pkt)
{
    sendPktQueue.add(pkt);
}
void WorldSession::AddSendWorldPacket(WorldPacket& pkt)
{
    WorldPacket *wp = new WorldPacket(pkt.GetOpcode(),pkt.size());
    if(pkt.size())
        wp->append(pkt.contents(),pkt.size());
    sendPktQueue.add(wp);
}

void WorldSession::SetTarget(uint64 guid)
{
    SendSetSelection(guid);
}

void WorldSession::_OnEnterWorld(void)
{
    if(!InWorld())
    {
        _logged=true;
//        GetInstance()->GetScripts()->variables.Set("@inworld","true");
     //   GetInstance()->GetScripts()->RunScriptIfExists("_enterworld");

    }
}

void WorldSession::_OnLeaveWorld(void)
{
    if(InWorld())
    {
        _logged=false;
    //    GetInstance()->GetScripts()->RunScriptIfExists("_leaveworld");
      //  GetInstance()->GetScripts()->variables.Set("@inworld","false");
    }
}

void WorldSession::_DoTimedActions(void)
{
    static clock_t pingtime=0;
    if(InWorld())
    {
        if(pingtime < clock())
        {
            pingtime=clock() + 30*CLOCKS_PER_SEC;
            SendPing(clock());
        }
        //...
    }
}

std::string WorldSession::DumpPacket(WorldPacket& pkt, int errpos, const char *errstr)
{
    static std::map<uint32,uint32> opstore;
    std::stringstream s;
    s << "TIMESTAMP: " << getDateString() << "\n";
    s << "OPCODE: " << pkt.GetOpcode() << " " << GetOpcodeName(pkt.GetOpcode()) << "\n";
    s << "SIZE: " << pkt.size() << "\n";
    if(errpos > 0)
        s << "ERROR-AT: " << errpos << "\n";
    if(errstr)
        s << "ERROR: " << errstr << "\n";
    if(pkt.size())
    {
        s << "DATA-HEX:\n";
        s << toHexDump((uint8*)pkt.contents(),pkt.size(),true,32);
        s << "\n";

        s << "DATA-TEXT:\n";
        for(uint32 i = 0; i < pkt.size(); i++)
        {
            s << (isprint(pkt[i]) ? (char)pkt[i] : '.');
            if((i+1) % 32 == 0)
                s << "\n";
        }
        s << "\n";

    }
    s << "\n";


    return NULL;
}

std::string WorldSession::GetOrRequestPlayerName(uint64 guid)
{
    if(!guid || GUID_HIPART(guid) != HIGHGUID_PLAYER)
    {
        logerror("WorldSession::GetOrRequestObjectName: "I64FMT" is not player",guid);
        return "<ERR: OBJECT>"; // TODO: temporary, to find bugs with this, if there are any
    }
    std::string name = plrNameCache.GetName(guid);
    if(name.empty())
    {
        if(!objmgr.IsRequestedPlayerGUID(guid))
        {
            objmgr.AddRequestedPlayerGUID(guid);
            SendQueryPlayerName(guid);
        }
    }
    return name;
}





///////////////////////////////////////////////////////////////
// Opcode Handlers
///////////////////////////////////////////////////////////////

void WorldSession::_HandleAuthChallengeOpcode(WorldPacket& recvPacket)
{
    //Read Packet
    uint32 sp, serverseed;
    if(GetInstance()->GetConf()->client > CLIENT_TBC)
    {
      recvPacket >> sp;
    }
    recvPacket >> serverseed;

    // Do stuff with the data
    std::string acc = stringToUpper(GetInstance()->GetConf()->accname);

    logdebug("Auth: serverseed=0x%X",serverseed);
    Sha1Hash digest;
    digest.UpdateData(acc);
    uint32 unk=0;
    uint64 unk64=0;
    digest.UpdateData((uint8*)&unk,sizeof(uint32));
    BigNumber clientseed;
    clientseed.SetRand(8*4);
    uint32 clientseed_uint32=clientseed.AsDword();
    digest.UpdateData((uint8*)&clientseed_uint32,sizeof(uint32));
    digest.UpdateData((uint8*)&serverseed,sizeof(uint32));
    digest.UpdateBigNumbers(GetInstance()->GetSessionKey(),NULL);
    digest.Finalize();

    // Send Reply
    WorldPacket auth;
    if(GetInstance()->GetConf()->client<=CLIENT_TBC)
    {
      auth<<(uint32)(GetInstance()->GetConf()->clientbuild)<<unk<<acc<<clientseed_uint32;
      auth.append(digest.GetDigest(),20);
    }
    else
    {
      auth<<(uint32)(GetInstance()->GetConf()->clientbuild)<<unk<<acc<<unk<<clientseed_uint32<<unk<<unk<<unk<<unk64;
      auth.append(digest.GetDigest(),20);
    }
    auth << (uint32)0; // TODO: this is not correct value, expected: 160 bytes of addon_data
    auth.SetOpcode(CMSG_AUTH_SESSION);

    SendWorldPacket(auth);

    // note that if the sessionkey/auth is wrong or failed, the server sends the following packet UNENCRYPTED!
    // so its not 100% correct to init the crypt here, but it should do the job if authing was correct
    _socket->InitCrypt(GetInstance()->GetSessionKey());

}

void WorldSession::_HandleAuthResponseOpcode(WorldPacket& recvPacket)
{
    uint8 errcode;
    uint8 BillingPlanFlags, expansion; uint32 BillingTimeRemaining, BillingTimeRested;
    recvPacket >> errcode;
    recvPacket >> BillingTimeRemaining >> BillingPlanFlags >> BillingTimeRested;
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
      recvPacket >> expansion;
    // TODO: add data to generic_text.scp and use the strings here
    if(errcode == AUTH_OK)
    {
        logdetail("World Authentication successful, preparing for char list request...");
        WorldPacket pkt(CMSG_CHAR_ENUM, 0);
        SendWorldPacket(pkt);
    }
    else
    {
        logerror("World Authentication failed, errcode=0x%X",(uint8)errcode);
        SetMustDie();
    }
}

void WorldSession::_HandleCharEnumOpcode(WorldPacket& recvPacket)
{
    uint8 num;
    uint8 charId;
    PlayerEnum plr[10]; // max characters per realm is 10
    uint8 dummy8;
    uint32 dummy32;
    bool char_found;
    _charList.clear();

    recvPacket >> num;
    if(num==0)
    {
        logdetail("No chars found!");
        char_found = false;
        //GetInstance()->SetError();
        //return;
    }
    else
    {
        logdetail("Chars in list: %u",num);
        // TODO: load cache on loadingscreen
        for(unsigned int i=0;i<num;i++)
        {
            recvPacket >> plr[i]._guid;
            recvPacket >> plr[i]._name;
            recvPacket >> plr[i]._race;
            recvPacket >> plr[i]._class;
            recvPacket >> plr[i]._gender;
            recvPacket >> plr[i]._bytes1;//skin
            recvPacket >> plr[i]._bytes2;//face
            recvPacket >> plr[i]._bytes3;//hair style
            recvPacket >> plr[i]._bytes4;//hair color
            recvPacket >> plr[i]._bytesx;//facial hair
            recvPacket >> plr[i]._level;
            recvPacket >> plr[i]._zoneId;
            recvPacket >> plr[i]._mapId;
            recvPacket >> plr[i]._x;
            recvPacket >> plr[i]._y;
            recvPacket >> plr[i]._z;
            recvPacket >> plr[i]._guildId;
            recvPacket >> plr[i]._flags;
            if(GetInstance()->GetConf()->client > CLIENT_TBC)
            {
              recvPacket >> dummy32; // at_login_customize
            }
            recvPacket >> dummy8;
            recvPacket >> plr[i]._petInfoId;
            recvPacket >> plr[i]._petLevel;
            recvPacket >> plr[i]._petFamilyId;
            for(unsigned int inv=0;inv<20;inv++)
            {
                recvPacket >> plr[i]._items[inv].displayId >> plr[i]._items[inv].inventorytype ;
                if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
                {
                  recvPacket >> dummy32; //enchant aura id
                }
            }
            plrNameCache.Add(plr[i]._guid, plr[i]._name); // TODO: set after loadingscreen, after loading cache

        }
        char_found=false;

        SCPDatabase *zonedb = GetDBMgr().GetDB("zone"),
                    *racedb = GetDBMgr().GetDB("race"),
                    *mapdb = GetDBMgr().GetDB("map"),
                    *classdb = GetDBMgr().GetDB("class");
        char *zonename, *racename, *mapname, *classname;
        zonename = racename = mapname = classname = "";

        for(unsigned int i=0;i<num;i++)
        {
            if(zonedb)
                zonename = zonedb->GetString(plr[i]._zoneId, "name");
            if(racedb)
                racename = racedb->GetString(plr[i]._race, "name");
            if(mapdb)
                mapname = mapdb->GetString(plr[i]._mapId, "name");
            if(classdb)
                classname = classdb->GetString(plr[i]._class, "name");

            CharacterListExt cx;
            cx.p = plr[i];
            cx.class_ = classname;
            cx.race = racename;
            cx.zone = zonename;
            cx.map_ = mapname;
            _charList.push_back(cx);

            logcustom(0,LGREEN,"## %s (%u) [%s/%s] Map: %s; Zone: %s",
                plr[i]._name.c_str(),
                plr[i]._level,
                racename,
                classname,
                mapname,
                zonename);

            logdetail("-> coords: map=%u zone=%u x=%f y=%f z=%f",
            plr[i]._mapId,plr[i]._zoneId,plr[i]._x,plr[i]._y,plr[i]._z);

            for(unsigned int inv=0;inv<20;inv++)
            {
                if(plr[i]._items[inv].displayId)
                    logdebug("-> Has Item: Model=%u InventoryType=%u",plr[i]._items[inv].displayId,plr[i]._items[inv].inventorytype);
            }
            if(plr[i]._name==GetInstance()->GetConf()->charname)
            {
                charId = i;
                char_found=true;
            }

        }
     }
        if(!char_found)
        {
            if(PseuGUI *gui = GetInstance()->GetGUI())
            {
             //   gui->SetSceneState(SCENESTATE_CHARSELECT);
               // gui->UpdateScene();
            }
            else
            {
                logerror("Character \"%s\" was not found on char list, can't connect!", GetInstance()->GetConf()->charname.c_str());
                GetInstance()->SetError();
                return;
            }
        }
        else
        {
            EnterWorldWithCharacter(plr[charId]._name.c_str());
        }
}

void WorldSession::EnterWorldWithCharacter(std::string name)
{
    logdebug("EnterWorldWithCharacter(%s)",name.c_str());
    _myGUID = 0;
    CharacterListExt charex;
    for(CharList::iterator it = _charList.begin(); it != _charList.end(); it++)
    {


    }
    if(!_myGUID)
    {
        logerror("Character '%s' does not exist on this account");
        return;
    }

    PreloadDataBeforeEnterWorld(charex.p);

    log("Entering World with Character \"%s\"...", name.c_str());

    // create the character and add it to the objmgr.
    // note: this is the only object that has to stay in memory unless its explicitly deleted by the server!
    // that means even if the server sends create object with that guid, do NOT recreate it!!
    MyCharacter *my = new MyCharacter();
    my->Create(_myGUID);
    my->SetName(name);
    objmgr.Add(my);

    // TODO: initialize the world here, and load required maps.
    // must remove appropriate code from _HandleLoginVerifyWorldOpcode() then!!

    WorldPacket pkt(CMSG_PLAYER_LOGIN,8);
    pkt << _myGUID;
    AddSendWorldPacket(pkt); // it can be called from gui thread also, use threadsafe version

    // close realm session when logging into world
    if(!MustDie() && _socket->IsOk() && GetInstance()->GetRSession())
    {
        GetInstance()->GetRSession()->SetMustDie(); // realm session is no longer needed
    }
}

void WorldSession::PreloadDataBeforeEnterWorld(PlayerEnum& pl)
{
    log("Loading data before entering world...");
    _LoadCache(); // we are about to login, so we need cache data


}


void WorldSession::_HandleSetProficiencyOpcode(WorldPacket& recvPacket)
{
    if(recvPacket.size())
    {
        DEBUG(
            logdebug("SetProficiency: Hexdump:");
            logdebug(toHexDump((uint8*)recvPacket.contents(),recvPacket.size(),true).c_str());
            );
    }
}

void WorldSession::_HandleAccountDataMD5Opcode(WorldPacket& recvPacket)
{
	/*
        uint32 time;
        uint8 dummy;
        recvPacket >> time >> dummy;
        ...
	*/
    // packet structure not yet known
}

void WorldSession::_HandleMessageChatOpcode(WorldPacket& recvPacket) //TODO: REWRITE ME!!!
{
    uint8 type, chatTag;
    uint32 lang;
    uint64 source_guid = 0, source_guid2 = 0, listener_guid = 0;
    uint32 msglen;
    uint32 unk32;
    uint32 source_name_len = 0, listener_name_len = 0;
    std::string source_name, listener_name;
    std::string msg, channel = "";

    recvPacket >> type >> lang;

    if(lang == LANG_ADDON && GetInstance()->GetConf()->skipaddonchat)
        return;
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
    {
      recvPacket >> source_guid;
      recvPacket >> unk32;
    }

    switch(type)
    {
        case CHAT_MSG_MONSTER_SAY:
        case CHAT_MSG_MONSTER_PARTY:
        case CHAT_MSG_MONSTER_YELL:
        case CHAT_MSG_MONSTER_WHISPER:
        case CHAT_MSG_MONSTER_EMOTE:
        case CHAT_MSG_RAID_BOSS_WHISPER:
        case CHAT_MSG_RAID_BOSS_EMOTE:
        case CHAT_MSG_BN:
            recvPacket >> source_name_len;
            recvPacket >> source_name;
            // MaNGOS sends nothing for these, not used
            recvPacket >> listener_guid; // always 0
            if(listener_guid && !IS_PLAYER_GUID(listener_guid))
            {
                recvPacket >> listener_name_len; // always 1 (\0)
                recvPacket >> listener_name; // always \0
                logdebug("CHAT: Listener: '%s' (guid="I64FMT" len=%u type=%u)", listener_name.c_str(), listener_guid, listener_name_len, type);
            }
            break;

        default:
            if(type == CHAT_MSG_CHANNEL)
                recvPacket >> channel;
            recvPacket >> source_guid2; // no idea why it is sent twice
    }
    recvPacket >> msglen;
    recvPacket >> msg;
    recvPacket >> chatTag;


    SCPDatabase *langdb = GetDBMgr().GetDB("language");
    const char* ln;
    std::string langname;
    if(langdb)
        langname = langdb->GetString(lang,"name");
    ln = langname.c_str();
    std::string name = source_name;

    if(source_guid && IS_PLAYER_GUID(source_guid))
    {
        name = GetOrRequestPlayerName(source_guid);
        if(name.empty())
        {
            _DelayWorldPacket(recvPacket, uint32(GetLagMS() * 1.2f)); // guess time how long it will take until we got player name from the server
            return; // handle later
        }
    }
//    GetInstance()->GetScripts()->variables.Set("@thismsg_name",name);
//    GetInstance()->GetScripts()->variables.Set("@thismsg",DefScriptTools::toString(source_guid));


    DEBUG(logdebug("Chat packet recieved, type=%u lang=%u src="I64FMT" dst="I64FMT" chn='%s' len=%u",
        type,lang,source_guid,source_guid,channel.c_str(),msglen));

    if (type == CHAT_MSG_SYSTEM)
    {
        logcustom(0,WHITE,"SYSMSG: \"%s\"",msg.c_str());
    }
    else if (type==CHAT_MSG_WHISPER )
    {
        logcustom(0,WHITE,"WHISP: %s [%s]: %s",name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_CHANNEL )
    {
        logcustom(0,WHITE,"CHANNEL: [%s]: %s [%s]: %s",channel.c_str(),name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_SAY )
    {
        logcustom(0,WHITE,"CHAT: %s [%s]: %s",name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_YELL )
    {
        logcustom(0,WHITE,"CHAT: %s yells [%s]: %s ",name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_REPLY )
    {
        logcustom(0,WHITE,"TO %s [%s]: %s",name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_GUILD )
    {
        logcustom(0,WHITE,"GUILD: %s [%s]: %s",name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_PARTY )
    {
        logcustom(0,WHITE,"PARTY: %s [%s]: %s",name.c_str(),ln,msg.c_str());
    }
    else if (type==CHAT_MSG_EMOTE )
    {
        logcustom(0,WHITE,"EMOTE [%s]: %s %s",ln,name.c_str(),msg.c_str());
    }
    else if (type==CHAT_MSG_MONSTER_SAY)
    {
        logcustom(0,WHITE,"NPC: %s [%s]: %s",name.c_str(),ln,msg.c_str());
    }
    else
    {
        logcustom(0,WHITE,"UNK CHAT TYPE (%u): %s [%s]: %s",type,name.c_str(),ln,msg.c_str());
    }

    // DefScript bindings below
    bool isCmd = false;
    if(source_guid != GetGuid() && msg.length() > 1 && msg.at(0) == '-' && GetInstance()->GetConf()->allowgamecmd)
        isCmd = true;




    // the following block searches for items in chat and queries them if they are unknown
    if(!isCmd && source_guid != GetGuid() && msg.length() > strlen(CHAT_ITEM_BEGIN_STRING))
    {
        for(uint32 pos=0;pos<msg.length()-strlen(CHAT_ITEM_BEGIN_STRING);pos++)
        {
            if(!memcmp(msg.c_str()+pos,CHAT_ITEM_BEGIN_STRING,strlen(CHAT_ITEM_BEGIN_STRING)))
            {
                std::string itemid;
                uint32 id;

                while(msg[pos] != ':')
                {
                    itemid += msg[pos+strlen(CHAT_ITEM_BEGIN_STRING)];
                    pos++;
                }
                id = atoi(itemid.c_str());
                if(id)
                {
                    logdebug("Found Item in chat message: %u",id);
                    if(objmgr.GetItemProto(id)==NULL)
                        SendQueryItem(id,0);
                }
                else
                {
                    logdebug("Tried to find ItemID in chat message, but link seems incorrect");
                }
            }
        }
    }
}

void WorldSession::_HandleMotdOpcode(WorldPacket& recvPacket)
{
    uint32 lines;
    std::string line;
    recvPacket >> lines;
    for(uint32 i = 0; i < lines; i++)
    {
        recvPacket >> line;
        logcustom(0,YELLOW,"MOTD: %s",line.c_str());
    }
}

void WorldSession::_HandleNotificationOpcode(WorldPacket& recvPacket)
{
    std::string text;
    recvPacket >> text;
    logcustom(0,YELLOW,"NOTIFY: %s",text.c_str());
}

void WorldSession::_HandleNameQueryResponseOpcode(WorldPacket& recvPacket)
{
    uint64 pguid;
    uint8 unk, declined, crace, cgender, cclass ;
    std::string pname,realm;
    uint32 prace, pgender, pclass;
    if(GetInstance()->GetConf()->client > CLIENT_TBC)
      pguid = recvPacket.readPackGUID();
    else
      recvPacket >> pguid;

    if(GetInstance()->GetConf()->client > CLIENT_TBC)
      recvPacket >> unk;

    recvPacket >> pname >> realm;
    if(GetInstance()->GetConf()->client > CLIENT_TBC)
    {
      recvPacket >> crace >> cgender >> cclass; //Not used yet...
    }
    else
      recvPacket >> prace >> pgender >> pclass; //Not used yet...
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
      recvPacket >> declined;
    if(pname.length()>MAX_PLAYERNAME_LENGTH || pname.length()<MIN_PLAYERNAME_LENGTH)
    {
        logerror("Playername Length outside bounds: %u",pname.length());
        return; // playernames maxlen=12, minlen=2
    }
    // rest of the packet is not interesting for now
    plrNameCache.Add(pguid,pname);
    logdetail("CACHE: Assigned new player name: '%s' = " I64FMTD ,pname.c_str(),pguid);
    WorldObject *wo = (WorldObject*)objmgr.GetObj(pguid);
    if(wo)
        wo->SetName(pname);
}

void WorldSession::_HandlePongOpcode(WorldPacket& recvPacket)
{
    uint32 pong;
    recvPacket >> pong;
    _lag_ms = clock() - pong;
    if(GetInstance()->GetConf()->notifyping)
        log("Received Ping reply: %u ms latency.", _lag_ms);
}
void WorldSession::_HandleTradeStatusOpcode(WorldPacket& recvPacket)
{
    recvPacket.hexlike();
    uint8 unk;
    recvPacket >> unk;
    if(unk==1)
    {
        // TODO: Implement this!!
        SendChatMessage(CHAT_MSG_SAY,0,"It has no sense trying to trade with me, that feature is not yet implemented!","");
        WorldPacket pkt;
        pkt.SetOpcode(CMSG_CANCEL_TRADE);
        SendWorldPacket(pkt);
    }
}

void WorldSession::_HandleGroupInviteOpcode(WorldPacket& recvPacket)
{
    recvPacket.hexlike();
    WorldPacket pkt;
    pkt.SetOpcode(CMSG_GROUP_DECLINE);
    SendWorldPacket(pkt);
}

void WorldSession::_HandleMovementOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    MovementInfo mi;
    guid = recvPacket.readPackGUID();
    recvPacket >> mi;
    DEBUG(logdebug("MOVE: "I64FMT" -> time=%u flags=0x%X x=%.4f y=%.4f z=%.4f o=%.4f",guid,mi.time,mi.flags,mi.pos.x,mi.pos.y,mi.pos.z,mi.pos.o));
    Object *obj = objmgr.GetObj(guid);
    if(obj && obj->IsWorldObject())
    {
        ((WorldObject*)obj)->SetPosition(mi.pos.x,mi.pos.y,mi.pos.z,mi.pos.o);
    }
    //TODO: Eval rest of Packet!!
}

void WorldSession::_HandleSetSpeedOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    float speed;
    uint32 movetype;
    MovementInfo mi;

    switch(recvPacket.GetOpcode())
    {
    case MSG_MOVE_SET_WALK_SPEED:
        movetype = MOVE_WALK;
        break;

    case MSG_MOVE_SET_RUN_SPEED:
        movetype = MOVE_RUN;
        break;

    case MSG_MOVE_SET_RUN_BACK_SPEED:
        movetype = MOVE_WALKBACK;
        break;

    case MSG_MOVE_SET_SWIM_SPEED:
        movetype = MOVE_SWIM;
        break;

    case MSG_MOVE_SET_SWIM_BACK_SPEED:
        movetype = MOVE_SWIMBACK;
        break;

    case MSG_MOVE_SET_TURN_RATE:
        movetype = MOVE_TURN;
        break;

    case MSG_MOVE_SET_FLIGHT_SPEED:
        movetype = MOVE_FLY;
        break;

    case MSG_MOVE_SET_FLIGHT_BACK_SPEED:
        movetype = MOVE_FLYBACK;
        break;

    case MSG_MOVE_SET_PITCH_RATE:
        movetype = MOVE_PITCH_RATE;
        break;

    default:
        logerror("MSG_MOVE_SET speed change unkown case error, opcode %u !", recvPacket.GetOpcode());
        return;
    }

    guid = recvPacket.readPackGUID();
    recvPacket >> mi;
    recvPacket >> speed;

    Object *obj = objmgr.GetObj(guid);
    if(obj && obj->IsUnit())
    {
        ((Unit*)obj)->SetSpeed(movetype, speed);
        ((Unit*)obj)->SetPosition(mi.pos.x, mi.pos.y, mi.pos.z, mi.pos.o);
    }
}

void WorldSession::_HandleForceSetSpeedOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    uint32 unk32, movetype;
    float speed;
    uint8 unk8;

    switch(recvPacket.GetOpcode())
    {
    case SMSG_FORCE_WALK_SPEED_CHANGE:
        movetype = MOVE_WALK;
        break;

    case SMSG_FORCE_RUN_SPEED_CHANGE:
        movetype = MOVE_RUN;
        break;

    case SMSG_FORCE_RUN_BACK_SPEED_CHANGE:
        movetype = MOVE_WALKBACK;
        break;

    case SMSG_FORCE_SWIM_SPEED_CHANGE:
        movetype = MOVE_SWIM;
        break;

    case SMSG_FORCE_SWIM_BACK_SPEED_CHANGE:
        movetype = MOVE_SWIMBACK;
        break;

    case SMSG_FORCE_TURN_RATE_CHANGE:
        movetype = MOVE_TURN;
        break;

    case SMSG_FORCE_FLIGHT_SPEED_CHANGE:
        movetype = MOVE_FLY;
        break;

    case SMSG_FORCE_FLIGHT_BACK_SPEED_CHANGE:
        movetype = MOVE_FLYBACK;
        break;

    case SMSG_FORCE_PITCH_RATE_CHANGE:
        movetype = MOVE_PITCH_RATE;
        break;

    default:
        logerror("MSG_FORCE_ speed change unkown case error, opcode %u !", recvPacket.GetOpcode());
        return;
    }

    guid = recvPacket.readPackGUID();
    recvPacket >> unk32;
    if (movetype == MOVE_RUN)
        recvPacket >> unk8;
    recvPacket >> speed;

    Object *obj = objmgr.GetObj(guid);
    if(obj && obj->IsUnit())
    {
        ((Unit*)obj)->SetSpeed(movetype, speed);
    }
}

void WorldSession::_HandleTelePortAckOpcode(WorldPacket& recvPacket)
{
    uint32 unk32;
    uint64 guid;
    MovementInfo mi;
    guid = recvPacket.readPackGUID();
    recvPacket >> unk32 >> mi;

    logdetail("Got teleported, data: x: %f, y: %f, z: %f, o: %f, guid: "I64FMT, mi.pos.x, mi.pos.y, mi.pos.z, mi.pos.o, guid);

    _world->UpdatePos(mi.pos.x,mi.pos.y);
    _world->Update();

    if(MyCharacter *my = GetMyChar())
    {
        my->SetPosition(mi.pos.x,mi.pos.y,mi.pos.z,mi.pos.o);
    }

    WorldPacket wp(MSG_MOVE_TELEPORT_ACK,8+4+4);
    if(GetInstance()->GetConf()->client > CLIENT_TBC)
      wp.appendPackGUID(guid);    //GUID must be packed!
    else
      wp << guid;
    wp << (uint32)0 << (uint32)getMSTime(); //First value is some counter
    SendWorldPacket(wp);

    _world->GetMoveMgr()->SetFallTime(100);
    _world->GetMoveMgr()->MoveFallLand();



}

void WorldSession::_HandleNewWorldOpcode(WorldPacket& recvPacket)
{
    DEBUG(logdebug("DEBUG: _HandleNewWorldOpcode() objs:%u mychar: ptr=0x%X, guid="I64FMT,objmgr.GetObjectCount(),GetMyChar(),GetMyChar()->GetGUID()));
    uint32 mapid;
    float x,y,z,o;
    // we assume we are NOT on a transport!
    // else we had to do the following before:
    // recvPacket >> tmapid >> tx >> ty >> tz >> to;
    recvPacket >> mapid >> x >> y >> z >> o;

    // when getting teleported, the client sends CMSG_CANCEL_TRADE 2 times.. dont ask me why.
    WorldPacket wp(CMSG_CANCEL_TRADE,8);
    SendWorldPacket(wp);
    SendWorldPacket(wp);

    wp.SetOpcode(MSG_MOVE_WORLDPORT_ACK);
    SendWorldPacket(wp);

    wp.SetOpcode(CMSG_SET_ACTIVE_MOVER);
    wp << GetGuid();
    SendWorldPacket(wp);




    // TODO: clear action buttons

    // clear world data and load required maps
    _world->Clear();
    _world->UpdatePos(x,y,mapid);
    _world->Update();

    if(MyCharacter *my = GetMyChar())
    {
        my->ClearSpells(); // will be resent by server
        my->SetPosition(x,y,z,o,mapid);
    }

    _world->GetMoveMgr()->SetFallTime(100);
    _world->GetMoveMgr()->MoveFallLand();//Must be sent after character was set to new position

    // TODO: need to switch to SCENESTATE_LOGINSCREEN here, and after everything is loaded, back to SCENESTATE_WORLD


}

void WorldSession::_HandleChannelNotifyOpcode(WorldPacket& recvPacket)
{
        _channels->HandleNotifyOpcode(recvPacket);
}

void WorldSession::_HandleCastResultOpcode(WorldPacket& recvPacket)
{
    uint32 spellid,otherr = 0;
    uint8 result, castCount;
    recvPacket >> castCount >> spellid >> result;
    if (recvPacket.rpos()+sizeof(uint32) <= recvPacket.size())
        recvPacket >> otherr;
    logdetail("Cast of spell %u failed. result=%u, cast count=%u, additional info=%u",spellid,result,castCount,otherr);
}

void WorldSession::_HandleCastSuccessOpcode(WorldPacket& recvPacket)
{
    uint32 spellId;
    uint64 casterGuid;

    casterGuid = recvPacket.readPackGUID();
    recvPacket >> spellId;

    if (GetMyChar()->GetGUID() == casterGuid)
        logdetail("Cast of spell %u successful.",spellId);
    else
    {
        Object *caster = objmgr.GetObj(casterGuid);
        if(caster)
            logdetail("%s casted spell %u", caster->GetName().c_str(), spellId);
        else
            logerror("Caster of spell %u (GUID "I64FMT") is unknown object!",spellId,casterGuid);
    }
}

void WorldSession::_HandleInitialSpellsOpcode(WorldPacket& recvPacket)
{
        uint8 unk;
        uint16 not_spellslot,count,spellid16;
        uint32 spellid;
        recvPacket >> unk >> count;
        logdebug("Got initial spells list, %u spells.",count);
        if(GetInstance()->GetConf()->client > CLIENT_TBC)
        {
          for(uint16 i = 0; i < count; i++)
          {
              recvPacket >> spellid >> not_spellslot;
              logdebug("Initial Spell: id=%u slot=%u",spellid,not_spellslot);
              GetMyChar()->AddSpell(spellid, not_spellslot);
          }
        }
        else
        {
          for(uint16 i = 0; i < count; i++)
          {
              recvPacket >> spellid16 >> not_spellslot;
              spellid = spellid16;
              logdebug("Initial Spell: id=%u slot=%u",spellid,not_spellslot);
              GetMyChar()->AddSpell(spellid, not_spellslot);
          }

        }
        //TODO: Parse packet completely
}

void WorldSession::_HandleLearnedSpellOpcode(WorldPacket& recvPacket)
{
        uint32 spellid;
        recvPacket >> spellid;
        GetMyChar()->AddSpell(spellid, 0); // other spells must be moved by +1 in slot?

        logdebug("Learned spell: id=%u",spellid);
}

void WorldSession::_HandleRemovedSpellOpcode(WorldPacket& recvPacket)
{
    uint32 spellid;
    recvPacket >> spellid;
    GetMyChar()->RemoveSpell(spellid);
    logdebug("Unlearned spell: id=%u",spellid);
}

void WorldSession::_HandleChannelListOpcode(WorldPacket& recvPacket)
{
       // _channels->HandleListRequest(recvPacket);
}

void WorldSession::_HandleEmoteOpcode(WorldPacket& recvPacket)
{
    std::string name;
    uint32 anim; // animation id?
    uint64 guid; // guid of the unit performing the emote
    recvPacket >> anim >> guid;

    if(guid)
    {
        Object *o = objmgr.GetObj(guid);
        if(o)
        {
            name = o->GetName();
        }
        if(name.empty())
        {
            if(IS_PLAYER_GUID(guid))
            {
                name = GetOrRequestPlayerName(guid);
                if(name.empty())
                {
                    _DelayWorldPacket(recvPacket, uint32(GetLagMS() * 1.2f));
                    return;
                }
            }
        }
    }

    logdebug(I64FMT " / %s performing emote; anim=%u",guid,name.c_str(),anim);

    // TODO: show emote in GUI :P
}

void WorldSession::_HandleTextEmoteOpcode(WorldPacket& recvPacket)
{
    std::string name; // name of emote target
    std::string name_from; // name of the unit performing the emote
    uint32 emotetext; // emote id
    int32 emotev; // variation (like different texts on /flirt and so on)
    uint32 namelen; // length of name of emote target (without '\0' )
    uint64 guid; // guid of the unit performing the emote
    uint8 c; // temp

    recvPacket >> guid >> emotetext >> *((uint32*)&emotev) >> namelen;

    // get the target name, which is NOT null-terminated
    for(uint32 i = 0; i < namelen; i++)
    {
        recvPacket >> c;
        if(c)
            name += c;
    }

    logdebug(I64FMT " Emote: name=%s text=%u variation=%i len=%u",guid,name.c_str(),emotetext,emotev,namelen);
    SCPDatabaseMgr& dbmgr = GetInstance()->dbmgr;
    SCPDatabase *emotedb = dbmgr.GetDB("emote");
    if(emotedb)
    {
        std::string target,target2;
        bool targeted=false; // if the emote is directed to anyone around or a specific target
        bool targeted_me=false; // if the emote was targeted to us if it was targeted
        bool from_me=false; // if we did the emote
        bool female=false; // if emote causer is female

        if(GetMyChar()->GetGUID() == guid) // we caused the emote
            from_me=true;

        if(name.length()) // is it directed to someone?
        {
            targeted=true; // if yes, we have a target
            if(GetMyChar()->GetName() == name) // if the name is ours, its directed to us
                targeted_me=true;
        }

        Unit *u = (Unit*)objmgr.GetObj(guid);
        if(u)
        {
            if(u->GetGender() != 0) // female
                female=true;
            name_from = u->GetName();
        }

        // if we targeted ourself, the general emote is used!
        if(targeted && from_me && targeted_me)
            targeted_me=false;

        // now build the string that is used to lookup the text in the database
        if(from_me)
            target += "me";
        else
            target += "one";

        if(targeted)
        {
            target += "to";
            if(targeted_me)
                target += "me";
            else
                target += "one";
        }
        else
            target += "general";

        // not all emotes have a female version, so check if there is one in the database
        if(female && emotedb->GetFieldId((char*)(target + "female").c_str()) != SCP_INVALID_INT)
                target += "female";

        logdebug("Looking up 'emote' SCP field %u entry '%s'",emotetext,target.c_str());

        std::string etext;
        etext = emotedb->GetString(emotetext,(char*)target.c_str());

        char out[300]; // should be enough

        if(from_me)
            sprintf(out,etext.c_str(),name.c_str());
        else
            sprintf(out,etext.c_str(),name_from.c_str(),name.c_str());

        logcustom(0,WHITE,"EMOTE: %s",out);

    }
    else
    {
        logerror("Can't display emote text %u, SCP database \"emote\" not loaded.",emotetext);
    }

}

void WorldSession::_HandleLoginVerifyWorldOpcode(WorldPacket& recvPacket)
{
    float x,y,z,o;
    uint32 m;
    recvPacket >> m >> x >> y >> z >> o;
    logdebug("LoginVerifyWorld: map=%u x=%f y=%f z=%f o=%f",m,x,y,z,o);
    _OnEnterWorld();
    // update the world as soon as the server confirmed that we are where we are.
    _world->UpdatePos(x,y,m);
    _world->Update();
    _world->CreateMoveMgr();

    // temp. solution to test terrain rendering
    if(PseuGUI *gui = GetInstance()->GetGUI())
    {
       // gui->SetSceneState(SCENESTATE_WORLD);
    }
}

ByteBuffer& operator>>(ByteBuffer& bb, WhoListEntry& e)
{
    uint8 dummy;
    bb >> e.name >> e.gname >> e.level >> e.classId >> e.raceId >> dummy >> e.zoneId;
    return bb;
}

void WorldSession::_HandleWhoOpcode(WorldPacket& recvPacket)
{
    uint32 count, unk;
    recvPacket >> count >> unk;

    log("Got WHO-List, %u players.         (unk=%u)",count,unk);

    if(count >= 1)
    {
        log("     Name     |Level|  Class     |   Race       |    Zone; Guild");
        log("--------------+-----+------------+--------------+----------------");
        if(count > 1)
        {
            _whoList.clear(); // need to clear current list only if requesting more then one player name
        }
    }

    for(uint32 i = 0; i < count; i++)
    {
        WhoListEntry wle;
        recvPacket >> wle;

        _whoList.push_back(wle);

        // original WhoListEntry is saved, now do some formatting
        while(wle.name.length() < 13)
            wle.name.append(" ");

        SCPDatabaseMgr& db = GetInstance()->dbmgr;
        char classname[20], racename[20];
        std::string zonename;
        memset(classname,0,sizeof(classname));
        memset(racename,0,sizeof(racename));

        SCPDatabase *zonedb = db.GetDB("zone"),
                    *racedb = db.GetDB("race"),
                    *classdb = db.GetDB("class");
        if(zonedb)
            zonename = zonedb->GetString(wle.zoneId, "name");
        if(racedb)
            strcpy(racename,racedb->GetString(wle.raceId, "name"));
        if(classdb)
            strcpy(classname,classdb->GetString(wle.classId, "name"));

        for(uint8 i = strlen(classname); strlen(classname) < 10; i++)
            classname[i] = ' ';
        for(uint8 i = strlen(racename); strlen(racename) < 12; i++)
            racename[i] = ' ';
        char tmp[12];
        sprintf(tmp,"%lu",wle.level);
        std::string lvl_str = tmp;
        while(lvl_str.length() < 3)
            lvl_str = " " + lvl_str;

        log("%s | %s | %s | %s | %s; %s", wle.name.c_str(), lvl_str.c_str(), classname, racename, zonename.c_str(), wle.gname.c_str());
    }
}

void WorldSession::_HandleCreatureQueryResponseOpcode(WorldPacket& recvPacket)
{
    uint32 entry;
    recvPacket >> entry;
    if( (!entry) || (entry & 0x80000000) ) // last bit marks that entry is invalid / does not exist on server side
    {
        uint32 real_entry = entry & ~0x80000000;
        logerror("Creature %u does not exist!", real_entry);
        objmgr.AddNonexistentCreature(real_entry);
        return;
    }

    CreatureTemplate *ct = new CreatureTemplate();
    std::string s;
    uint32 unk;
    float unkf;
    ct->entry = entry;
    recvPacket >> ct->name;
    recvPacket >> s;
    recvPacket >> s;
    recvPacket >> s;
    recvPacket >> ct->subname;
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
      recvPacket >> ct->directions;
    recvPacket >> ct->flag1;
    recvPacket >> ct->type;
    recvPacket >> ct->family;
    recvPacket >> ct->rank;
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
    {
      if(GetInstance()->GetConf()->client == CLIENT_WOTLK)
      {
          for(uint32 i = 0; i < MAX_KILL_CREDIT; i++)
              recvPacket >> ct->killCredit[i];
      }
      recvPacket >> ct->displayid_A;
      recvPacket >> ct->displayid_H;
      recvPacket >> ct->displayid_AF;
      recvPacket >> ct->displayid_HF;
      recvPacket >> unkf;
      recvPacket >> unkf;
      recvPacket >> ct->RacialLeader;
      if(GetInstance()->GetConf()->client == CLIENT_WOTLK)
      {
          for(uint32 i = 0; i < 4; i++)
              recvPacket >> ct->questItems[i];
          recvPacket >> ct->movementId;
      }
    }
    else
    {
      recvPacket >> unk;
      recvPacket >> ct->PetSpellDataId;
      recvPacket >> ct->displayid_A;
      ct->displayid_H = ct->displayid_A;
      ct->displayid_AF = ct->displayid_A;
      ct->displayid_HF = ct->displayid_A;
      recvPacket >> ct->civilian;
    }
    std::stringstream ss;
    ss << "Got info for creature " << entry << ":" << ct->name;
    if(!ct->subname.empty())
        ss << " <" << ct->subname << ">";
    ss << " type " << ct->type;
    ss << " flags " << ct->flag1;
    ss << " models " << ct->displayid_A << "/" << ct->displayid_H;
    logdetail("%s",ss.str().c_str());

    objmgr.Add(ct);
    objmgr.AssignNameToObj(entry, TYPEID_UNIT, ct->name);
}

void WorldSession::_HandleGameobjectQueryResponseOpcode(WorldPacket& recvPacket)
{
    uint32 entry;
    recvPacket >> entry;
    if(entry & 0x80000000)
    {
        uint32 real_entry = entry & ~0x80000000;
        logerror("Gameobject %u does not exist!");
        objmgr.AddNonexistentGO(real_entry);
        return;
    }

    std::string other_names, unks;

    GameobjectTemplate *go = new GameobjectTemplate();
    go->entry = entry;
    recvPacket >> go->type;
    recvPacket >> go->displayId;
    recvPacket >> go->name;
    recvPacket >> other_names; // name1
    recvPacket >> other_names; // name2
    recvPacket >> other_names; // name3 (all unused)
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
    {
      recvPacket >> unks;
      recvPacket >> go->castBarCaption;
      recvPacket >> go->unk1;
    }
    for(uint32 i = 0; i < GAMEOBJECT_DATA_FIELDS; i++)
        recvPacket >> go->raw.data[i];
    if(GetInstance()->GetConf()->client > CLIENT_CLASSIC_WOW)
    {
      recvPacket >> go->size;
      if(GetInstance()->GetConf()->client > CLIENT_TBC)
      {
        for(uint32 i = 0; i < 4; i++)
          recvPacket >> go->questItems[i];
      }
    }
    std::stringstream ss;
    ss << "Got info for gameobject " << entry << ":" << go->name;
    ss << " type " << go->type;
    ss << " displayid " << go->displayId;
    logdetail("%s",ss.str().c_str());

    objmgr.Add(go);
    objmgr.AssignNameToObj(entry, TYPEID_GAMEOBJECT, go->name);
}

void WorldSession::_HandleCharCreateOpcode(WorldPacket& recvPacket)
{
    uint8 response;
    recvPacket >> response;
    if(response == CHAR_CREATE_SUCCESS)
    {
        log("Character created successfully.");
        WorldPacket pkt(CMSG_CHAR_ENUM, 0);
        SendWorldPacket(pkt);
        logdebug("Requested new CMSG_CHAR_ENUM");
    }
    else
    {
        logerror("Character creation error, response=%u", response);
    }
    if(SCPDatabase *db = GetInstance()->dbmgr.GetDB("generic_text"))
    {
        // convert response number to field name (simple int to string)
        char buf[20];
        sprintf(buf,"%u",response);
        std::string response_str = db->GetString(0, buf); // data are expected to be at index 0
        log("Response String: '%s'",response_str.c_str());
    }

    //if(PseuGUI *gui = GetInstance()->GetGUI())
  //      gui->SetSceneData(ISCENE_CHARSEL_ERRMSG, response);
}

void WorldSession::_HandleMonsterMoveOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    guid = recvPacket.readPackGUID();

    Object* obj = objmgr.GetObj(guid);
    if (!obj || !obj->IsWorldObject())
        return;

    uint8 unk, type;
    uint32 time, flags, movetime, waypoints;
    float x, y, z;
    if(GetInstance()->GetConf()->client > CLIENT_TBC)
      recvPacket >> unk;

    recvPacket >> x >> y >> z >> time >> type;

    float oldx = ((WorldObject*)obj)->GetX(),
          oldy = ((WorldObject*)obj)->GetY();
    float o = atan2f(y - oldy, x - oldx);
    // not much good, better than nothing

    ((WorldObject*)obj)->SetPosition(x, y, z, o);
    switch(type)
    {
        case 0: break; // normal packet
        case 1: return; // stop packet
        case 2:
            float unkf;
            recvPacket >> unkf >> unkf >> unkf;
            break;
        case 3:
            uint64 unkguid;
            recvPacket >> unkguid;
            break;
        case 4:
            float angle;
            recvPacket >> angle;
            break;
    }

    //  movement flags, time between waypoints, number of waypoints
    recvPacket >> flags >> movetime >> waypoints;

    /*
    // waypoint data
    for (uint32 i = 0; i < waypoints; i++)
        recvPacket >> x >> y >> z;
    */
}

// TODO: delete world on LogoutComplete once implemented




