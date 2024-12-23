//----------------------------------------------------------------------------
//
//  $Workfile: saying.h$
//
//  $Revision: X$
//
//  Modification History:
//  $Log:
//  $
//
//  Notes:
//     This is the code for handling a location
//
//----------------------------------------------------------------------------
#ifndef SAYING_H
#define SAYING_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <stdint.h>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <vector>
#include "mySqlDBSingleton.h"

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: Saying
// 
// Purpose:
//      The location
//
//----------------------------------------------------------------------------
class Saying
{
  public:
    //----------------------------------------------------------------------------
    //  Consts
    //----------------------------------------------------------------------------

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    Saying();
    Saying(std::shared_ptr<sql::ResultSet>& resultSet);
    int GetId();
    std::string GetSaying();
    int GetVotes();
    int GetCount();
    std::string GetJSON();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    int mId = 0;
    std::string mSaying = "";
    int mVotes = 0;
    int mCount = 0;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
};
#endif
