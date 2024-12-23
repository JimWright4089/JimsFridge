//----------------------------------------------------------------------------
//
//  $Workfile: saying.cpp$
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
#include "loggerSingleton.h"
#include "section.h"

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Section::Section(std::shared_ptr<TMP117> temperature, std::shared_ptr<ADS1015> analog, 
    int analogPort, uint64_t lockout) :
  mTemperature(temperature),
  mAnalogDevice(analog),
  mAnalogPort(analogPort),
  mReadSensors(std::shared_ptr<StopWatch>(new StopWatch(50ul))),
  mDoorLockOut(std::shared_ptr<StopWatch>(new StopWatch(lockout)))
{
  mReadSensors->reset();
  mDoorLockOut->reset();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Destructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Section::~Section()
{

}

//----------------------------------------------------------------------------
//  Purpose:
//   Tick
//
//  Notes:
//
//----------------------------------------------------------------------------
void Section::Tick()
{
  if(true == mReadSensors->isExpired())
  {
    mTheTemp = mTemperature->readC();
    mDoor = mAnalogDevice->readPort(mAnalogPort);
    mReadSensors->reset();
  }
  GetState();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the temperature
//
//  Notes:
//
//----------------------------------------------------------------------------
double Section::GetC()
{
  return mTheTemp;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the door state
//
//  Notes:
//
//----------------------------------------------------------------------------
bool Section::GetDoor()
{
  if(DOOR_MIDDLE > mDoor)
  {
    return DOOR_IS_CLOSED;
  }

  return DOOR_IS_OPEN;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the door state
//
//  Notes:
//
//----------------------------------------------------------------------------
bool Section::GetDoorState()
{
  return mDoorState;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the door state
//
//  Notes:
//
//----------------------------------------------------------------------------
bool Section::GetOldDoorState()
{
  return mOldDoorState;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the door state
//
//  Notes:
//
//----------------------------------------------------------------------------
void Section::UpdateOldDoorState()
{
  mOldDoorState = mDoorState;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the starting temp
//
//  Notes:
//
//----------------------------------------------------------------------------
double Section::GetStartTemp()
{
  return mStartTemp;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get the state of the section
//
//  Notes:
//
//----------------------------------------------------------------------------
void Section::GetState()
{
  bool state = GetDoor();
  if(mDoorState != state)
  {
    if (DOOR_IS_OPEN == state)
    {
      if(true == mDoorLockOut->isExpired())
      {
        mStartTemp = mTheTemp;
        mDoorState = state;
      }
    }
    else
    {
        mDoorState = state;
        mDoorLockOut->reset();
    }
  }
}
