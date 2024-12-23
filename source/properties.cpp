//----------------------------------------------------------------------------
//
//  $Workfile: locationCount.cpp$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for the location
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include "propertyFile.h"
#include "loggerSingleton.h"
#include "options.h"
#include "properties.h"

//----------------------------------------------------------------------------
//  Global
//----------------------------------------------------------------------------
std::string gDatabaseConn = "/var/connections/jimsfridge_db.json";
std::string gMQTTConn =     "/var/connections/jimsfridge_mqtt.json";
std::string gAnalogConn =   "/var/connections/jimsfridge_analog.json";
std::string gLDCConn =      "/var/connections/jimsfridge_lcd.json";
std::string gFridgeConn =   "/var/connections/jimsfridge_temp1.json";
std::string gFreezerConn =  "/var/connections/jimsfridge_temp2.json";
std::string gBlueskyConn =  "/var/connections/jimsfridge_bluesky.json";
std::string gPythonScript = "";
int gMsPerSecond = 1000;
int gSecondsPerSecond = 1;
int gSubVotes = 100;
int gAddVotes = 1;
int gMinRandSecs;
int gMaxRandSecs;
int gPost;
uint32_t gFridgeDoorLockout = 1200;
uint32_t gFreezerDoorLockout = 1200;

//----------------------------------------------------------------------------
//  Purpose:
//   Build and parse the command line settings
//
//  Notes:
//
//----------------------------------------------------------------------------
void loadProperties()
{
  PropertyFile propertyFile(gProperties);

  gDatabaseConn = propertyFile.getString("databaseConnection","/var/connections/jimsfridge_db.json");
  gMQTTConn =     propertyFile.getString("mqttConnection","/var/connections/jimsfridge_mqtt.json");
  gAnalogConn =   propertyFile.getString("analogConnection","/var/connections/jimsfridge_analog.json");
  gLDCConn =      propertyFile.getString("ldcConnection","/var/connections/jimsfridge_lcd.json");
  gFridgeConn =   propertyFile.getString("fridgeConnection","/var/connections/jimsfridge_temp1.json");
  gFreezerConn =  propertyFile.getString("freezerConnection","/var/connections/jimsfridge_temp2.json");
  gBlueskyConn =  propertyFile.getString("blueskyConnection","/var/connections/jimsfridge_bluesky.json");
  gPythonScript = propertyFile.getString("pythonScript","/usr/local/bin/postALine.py");

  gMsPerSecond = propertyFile.getInt("msPerSecond",1000);
  gSecondsPerSecond = propertyFile.getInt("secondsPerSecond",1);
  gSubVotes = propertyFile.getInt("subVotes",100);
  gAddVotes = propertyFile.getInt("addVotes",1);
  gMinRandSecs = propertyFile.getInt("minRandSecs",120);
  gMaxRandSecs = propertyFile.getInt("maxRandSecs",180);
  gPost =  propertyFile.getInt("post",1);
  gFridgeDoorLockout =  propertyFile.getInt("fridgeDoorLockout",1200);
  gFreezerDoorLockout =  propertyFile.getInt("freezerDoorLockout",1200);

  int verbose = propertyFile.getInt("verbose",3);
  std::string logfile = propertyFile.getString("logFile","/var/log/jimsFridge.log");
  bool STDOUT = ("true"==propertyFile.getString("toStdout","false"))?true:false;

  LoggerSingleton::getInstance()->setFilename(logfile,STDOUT);
  switch(verbose)
  {
    case(1):
      LoggerSingleton::getInstance()->setSeverity(SEVERITY_WARNING);
      break;
    case(2):
      LoggerSingleton::getInstance()->setSeverity(SEVERITY_INFO);
      break;
    case(3):
      LoggerSingleton::getInstance()->setSeverity(SEVERITY_DEBUG);
      break;
    default:
      LoggerSingleton::getInstance()->setSeverity(SEVERITY_ERROR);
      break;
  }

  propertyFile.printTree();
}