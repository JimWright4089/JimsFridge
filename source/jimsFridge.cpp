//----------------------------------------------------------------------------
//
//  $Workfile: sqlConnection.cpp$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for the Battery object
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <signal.h>
#include <thread>
#include "loggerSingleton.h"
#include "mySqlDBSingleton.h"
#include "mqttSingleton.h"
#include "randomSingleton.h"
#include "stopWatch.h"
#include "connection.h"
#include "allSayings.h"
#include "options.h"
#include "properties.h"
#include "ads1015.h"
#include "tmp117.h"
#include "serLCD.h"
#include "section.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------
//  Consts
//----------------------------------------------------------------------------
const std::chrono::milliseconds TIME_PER_TICK = 100ms;

//----------------------------------------------------------------------------
//  Globals
//----------------------------------------------------------------------------
Connection gDatabaseConnection;
MySQLDB &gDatabase = MySQLDBSingleton::getInstance();
Connection mMQTTConnection;
Connection gBluesky;

bool gRunTheThread = true;
bool gStateChange = false;

std::shared_ptr<SerLCD> gSerLCD;
std::shared_ptr<TMP117> gFridgeTemperature;
std::shared_ptr<TMP117> gFreezerTemperature;
std::shared_ptr<ADS1015> gAnalogPort;
std::shared_ptr<Section> gFridgeSection;
std::shared_ptr<Section> gFreezerSection;

int gMajorVersion = 3;
int gMinorVersion = 1;
int gSayingID = 0;

//----------------------------------------------------------------------------
//  Purpose:
//   Stop all of the threads
//
//  Notes:
//
//----------------------------------------------------------------------------
void stopThread(int s)
{
  LoggerSingleton::getInstance()->writeWarning("Stopping app");
  gRunTheThread = false;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Post
//
//  Notes:
//
//----------------------------------------------------------------------------
void post(std::string thePost)
{
  LoggerSingleton::getInstance()->writeInfo(thePost);
  std::string comandToSend = "python3 " + gPythonScript + " \"" + 
    gBluesky.getUser() + "\" \"" + gBluesky.getPassword() + "\" \"" + thePost + "\"";
  LoggerSingleton::getInstance()->writeDebug(comandToSend);

  if(1 == gPost)
  {
    system(comandToSend.c_str());
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Post
//
//  Notes:
//
//----------------------------------------------------------------------------
void redrawLCD(int id, int secs)
{
  char text[64];
  gSerLCD->clearAll();

  int len = snprintf(text,64,"%3d  secs=%6d              ",id,secs);
  gSerLCD->writeText((uint8_t*)text,16);

  gSerLCD->moveCursor(0, 1);
  len = snprintf(text,64,"%6.2f %6.2f %d%d             ",
    gFridgeSection->GetC(),gFreezerSection->GetC(),
    gFridgeSection->GetDoorState(),gFreezerSection->GetDoorState());
  gSerLCD->writeText((uint8_t*)text,16);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Run the system
//
//  Notes:
//
//----------------------------------------------------------------------------
void runThread()
{
  StopWatch fiveSecond(5000ul);
  StopWatch sayingsStopWatch;
  AllSayings &allSayings = AllSayings::getInstance();

  post("JimsFridge Ver "+std::to_string(gMajorVersion) + "." + std::to_string(gMinorVersion));

  sayingsStopWatch.setTime((gMinRandSecs + 
          RandomSingleton::getInstance().next(gMaxRandSecs - gMinRandSecs)) *
          1000);
  fiveSecond.reset();
  sayingsStopWatch.reset();

  while(true == gRunTheThread)
  {
    if(true == fiveSecond.isExpired())
    {
      fiveSecond.reset();
      redrawLCD(gSayingID, sayingsStopWatch.getTimeLeft()/1000);
    }

    if(true == sayingsStopWatch.isExpired())
    {
      gSayingID = allSayings.GetRandomSaying();
      post(allSayings.GetSaying(gSayingID));

      sayingsStopWatch.setTime((gMinRandSecs + 
          RandomSingleton::getInstance().next(gMaxRandSecs - gMinRandSecs)) *
          1000);
      sayingsStopWatch.reset();
      redrawLCD(gSayingID, sayingsStopWatch.getTimeLeft()/1000);
    }

    gFridgeSection->Tick();
    gFreezerSection->Tick();

    if(gFridgeSection->GetDoorState() != gFridgeSection->GetOldDoorState())
    {
      if(Section::DOOR_IS_OPEN == gFridgeSection->GetDoorState())
      {
        post("Fridge Door Opened temperature = "+std::to_string(gFridgeSection->GetStartTemp()));
      }
      else
      {
        post("Fridge Door Closed temperature = "+std::to_string(gFridgeSection->GetC()) +
          " temp changed = " + std::to_string(gFridgeSection->GetC()-gFridgeSection->GetStartTemp()));
      }      
      gFridgeSection->UpdateOldDoorState();
    }

    if(gFreezerSection->GetDoorState() != gFreezerSection->GetOldDoorState())
    {
      if(Section::DOOR_IS_OPEN == gFreezerSection->GetDoorState())
      {
        post("Freezer Door Opened temperature = "+std::to_string(gFreezerSection->GetStartTemp()));
      }
      else
      {
        post("FreezerDoor Closed temperature = "+std::to_string(gFreezerSection->GetC()) + 
          " temp changed = " + std::to_string(gFreezerSection->GetC()-gFreezerSection->GetStartTemp()));
      }      
      gFreezerSection->UpdateOldDoorState();
    }


    std::this_thread::sleep_for(TIME_PER_TICK);
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Entry point
//
//  Notes:
//
//----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  setFlags(argc, argv);

  if(true == gJustHelp)
  {
      return -1;
  }

  loadProperties();

  gBluesky.loadFromFile(gBlueskyConn);

  gDatabase.loadConfiguration(gDatabaseConn);
  gDatabase.openDatabase();

  MQTTSingleton::getInstance().loadConfiguration(gMQTTConn);
  MQTTSingleton::getInstance().openMQTT();

  gSerLCD = std::shared_ptr<SerLCD>(new SerLCD());
  gFridgeTemperature = std::shared_ptr<TMP117>(new TMP117());
  gFreezerTemperature = std::shared_ptr<TMP117>(new TMP117());
  gAnalogPort = std::shared_ptr<ADS1015>(new ADS1015());

  gFridgeSection = std::shared_ptr<Section>(new Section(gFridgeTemperature, gAnalogPort, 1, (uint64_t)(gFridgeDoorLockout*1000))); 
  gFreezerSection = std::shared_ptr<Section>(new Section(gFreezerTemperature, gAnalogPort, 0, (uint64_t)(gFreezerDoorLockout*1000))); 

  gSerLCD->loadConfiguration(gLDCConn);
  gSerLCD->openSerLCD();

  gAnalogPort->loadConfiguration(gAnalogConn);
  gAnalogPort->openADS1015();

  gFridgeTemperature->loadConfiguration(gFridgeConn);
  gFridgeTemperature->openTMP117();

  gFreezerTemperature->loadConfiguration(gFreezerConn);
  gFreezerTemperature->openTMP117();

  signal(SIGTERM, stopThread);
  signal(SIGINT, stopThread);

  gSerLCD->reset();

  gSerLCD->setContrast(5);
  gSerLCD->setBlacklight(29,29,29);

  gRunTheThread = true;
  std::thread doStuffThread{runThread};

  while(true == gRunTheThread)
  {
    std::this_thread::sleep_for(100ms);
  }

  doStuffThread.join();

  std::this_thread::sleep_for(2000ms);

  LoggerSingleton::getInstance()->writeInfo("finishing");
  LoggerSingleton::getInstance()->flush();

  return 0;
}

