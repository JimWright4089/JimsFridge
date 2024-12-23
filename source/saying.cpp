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
#include "saying.h"

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Saying::Saying()
{

}

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
Saying::Saying(std::shared_ptr<sql::ResultSet>& resultSet)
{
  mId = resultSet->getInt(1);
  mSaying = std::string(resultSet->getString(2));
  mVotes = resultSet->getInt(3);
  mCount = resultSet->getInt(4);
}

//----------------------------------------------------------------------------
//  Purpose:
//   Getter Setter
//
//  Notes:
//
//----------------------------------------------------------------------------
int Saying::GetId()
{
  return mId;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Getter Setter
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string Saying::GetSaying()
{
  return mSaying;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Getter Setter
//
//  Notes:
//
//----------------------------------------------------------------------------
int Saying::GetVotes()
{
  return mVotes;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Getter Setter
//
//  Notes:
//
//----------------------------------------------------------------------------
int Saying::GetCount()
{
  return mCount;
}
 