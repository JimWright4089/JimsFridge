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
#ifndef ALL_SAYINGS_H
#define ALL_SAYINGS_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <memory>
#include "saying.h"
#include "mySqlDBSingleton.h"

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
class AllSayings
{
  public:

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static AllSayings &getInstance();
    void print();
    int  GetRandomSaying();
    std::string GetSaying(int id);
    ~AllSayings();

  private:
    const std::string UNKNOWN = "unknown";
    const int BAD_ID = -1;

    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    std::vector<std::shared_ptr<Saying>> mSayings;
    static std::shared_ptr<AllSayings> sInstance;
    MySQLDB &mDatabase;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    AllSayings();
    void ReloadSayings();
    void AddToDBAllSayingVotes(int num);
    void AddToDBOneSayingVotes(int num, int id);
    void UpdateDBOneSayingVotes(int num, int id);
    void AddToDBOneSayingCount(int num, int id);
};
#endif
