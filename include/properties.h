//----------------------------------------------------------------------------
//
//  $Workfile: properties.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for the properties
//
//----------------------------------------------------------------------------
#ifndef PROPERTIES_H
#define PROPERTIES_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <string>

//----------------------------------------------------------------------------
//  Consts
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Globals
//----------------------------------------------------------------------------
extern std::string gDatabaseConn;
extern std::string gMQTTConn;
extern std::string gAnalogConn;
extern std::string gLDCConn;
extern std::string gFridgeConn;
extern std::string gFreezerConn;
extern std::string gBlueskyConn;
extern int gSubVotes;
extern int gAddVotes;
extern int gMinRandSecs;
extern int gMaxRandSecs;
extern int gPost;
extern uint32_t gFridgeDoorLockout;
extern uint32_t gFreezerDoorLockout;
extern std::string gPythonScript;

//----------------------------------------------------------------------------
//  Class Methods
//----------------------------------------------------------------------------
void loadProperties();
#endif
