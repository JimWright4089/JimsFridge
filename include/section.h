//----------------------------------------------------------------------------
//
//  $Workfile: allLocations.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for handling the locations
//
//----------------------------------------------------------------------------
#ifndef SECTION_H
#define SECTION_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <memory>
#include "stopWatch.h"
#include "ads1015.h"
#include "tmp117.h"

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: AllLocations
// 
// Purpose:
//      The ground locations
//
//----------------------------------------------------------------------------
class Section
{
  public:
    //----------------------------------------------------------------------------
    //  Class Const
    //----------------------------------------------------------------------------
    const uint8_t STATE_CLOSED = 0;
    const uint8_t STATE_OPENED = 1;

    const uint16_t DOOR_MIDDLE = 500;

    static const bool DOOR_IS_OPEN = false;
    static const bool DOOR_IS_CLOSED = true;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    Section(std::shared_ptr<TMP117> temperature, std::shared_ptr<ADS1015> analog, 
            int analogPort, uint64_t lockout);
    ~Section();
    void Tick();
    double GetC();
    bool GetDoor();
    bool GetDoorState();
    bool GetOldDoorState();
    void UpdateOldDoorState();
    void GetState();
    double GetStartTemp();
  private:

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::shared_ptr<TMP117> mTemperature;
    std::shared_ptr<ADS1015> mAnalogDevice;
    int mAnalogPort = 0;
    std::shared_ptr<StopWatch> mReadSensors;
    std::shared_ptr<StopWatch> mDoorLockOut;
    uint16_t mDoor=0;
    bool mOldDoorState=DOOR_IS_CLOSED;
    bool mDoorState=DOOR_IS_CLOSED;
    double mTheTemp=0.0;
    uint8_t mState=STATE_CLOSED;
    double mStartTemp = 0.0;
    double mEndTemp = 0.0;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
