//----------------------------------------------------------------------------
//
//  $Workfile: allLocations.cpp$
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
#include <thread>
#include "loggerSingleton.h"
#include "randomSingleton.h"
#include "allSayings.h"
#include "properties.h"

using namespace std::chrono_literals;

//----------------------------------------------------------------------------
//  Globals and Statics
//----------------------------------------------------------------------------
std::shared_ptr<AllSayings> AllSayings::sInstance = nullptr;
RandomSingleton &gRandomNumber = RandomSingleton::getInstance();

//----------------------------------------------------------------------------
//  Purpose:
//   Constructor
//
//  Notes:
//
//----------------------------------------------------------------------------
AllSayings::AllSayings() :
  mDatabase(MySQLDBSingleton::getInstance())
{
}

//----------------------------------------------------------------------------
//  Purpose:
//   Get Instance
//
//  Notes:
//
//----------------------------------------------------------------------------
AllSayings &AllSayings::getInstance()
{
    if (sInstance == nullptr) 
    {
        sInstance = std::shared_ptr<AllSayings>(new AllSayings());
    }
    return *sInstance;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Destructor
//
//  Notes:
//
//----------------------------------------------------------------------------
AllSayings::~AllSayings()
{
  for(std::shared_ptr<Saying> saying: mSayings)
  {
    saying.reset();
  }

  if (sInstance != nullptr) {
      sInstance.reset();
      sInstance = nullptr;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
void AllSayings::print()
{
  LoggerSingleton::getInstance()->writeInfo("Sayings:");
  for(std::shared_ptr<Saying> saying: mSayings)
  {
    LoggerSingleton::getInstance()->writeInfo(std::to_string(saying->GetId()) + " " +
      std::to_string(saying->GetVotes()) + " " +
      std::to_string(saying->GetCount()) + " " +
      saying->GetSaying().substr(0,50));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
void AllSayings::ReloadSayings()
{

  for(std::shared_ptr<Saying> saying: mSayings)
  {
    saying.reset();
  }

  mSayings.clear();

  std::string sqlCommand = "select id, saying, votes, count from sayings;";

  int returnValue = EXIT_FAILURE;
  std::shared_ptr<sql::ResultSet> resultSet = mDatabase.executeStatementWithResult(sqlCommand, returnValue);
  
  while (resultSet->next()) 
  {
    std::shared_ptr<Saying> newSaying = std::shared_ptr<Saying>(new Saying(resultSet));
    mSayings.push_back(newSaying);
  }    
//  print();
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
void AllSayings::AddToDBAllSayingVotes(int num)
{
  std::string sqlCommand = "update sayings set votes = votes + "+ std::to_string(num)+";";

  int returnValue = EXIT_FAILURE;
  mDatabase.executeStatement(sqlCommand, returnValue);

  if(0 != returnValue)
  {
    LoggerSingleton::getInstance()->writeError("AddToDBAllSayingVotes returnValue:" + std::to_string(returnValue));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
void AllSayings::AddToDBOneSayingVotes(int num, int id)
{
  std::string sqlCommand = "update sayings set votes = votes + " + 
    std::to_string(num) + " where id = " + std::to_string(id) + ";";

  int returnValue = EXIT_FAILURE;
  mDatabase.executeStatement(sqlCommand, returnValue);

  if(0 != returnValue)
  {
    LoggerSingleton::getInstance()->writeError("AddToDBOneSayingVotes returnValue:" + std::to_string(returnValue));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
void AllSayings::UpdateDBOneSayingVotes(int num, int id)
{
  std::string sqlCommand = "update sayings set votes = " + 
    std::to_string(num) + " where id = " + std::to_string(id) + ";";

  int returnValue = EXIT_FAILURE;
  mDatabase.executeStatement(sqlCommand, returnValue);

  if(0 != returnValue)
  {
    LoggerSingleton::getInstance()->writeError("AddToDBOneSayingVotes returnValue:" + std::to_string(returnValue));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
void AllSayings::AddToDBOneSayingCount(int num, int id)
{
  std::string sqlCommand = "update sayings set count = count + " + 
    std::to_string(num) + " where id = " + std::to_string(id) + ";";

  int returnValue = EXIT_FAILURE;
  mDatabase.executeStatement(sqlCommand, returnValue);

  if(0 != returnValue)
  {
    LoggerSingleton::getInstance()->writeError("AddToDBOneSayingCount returnValue:" + std::to_string(returnValue));
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
int  AllSayings::GetRandomSaying()
{
  ReloadSayings();

  int voteCounts = 0;
  int selectedID = 0;
  for(std::shared_ptr<Saying> saying: mSayings)
  {
    if(saying->GetVotes() > 0)
    {
      voteCounts += saying->GetVotes();
    }
  }

  if(voteCounts <= 0)
  {
    int count = mSayings.size();
    selectedID = gRandomNumber.next(count)+1;
  }
  else
  {
    int randVoteCount = gRandomNumber.next(voteCounts);
    for(std::shared_ptr<Saying> saying: mSayings)
    {
      if(saying->GetVotes() > 0)
      {
        randVoteCount -= saying->GetVotes();
        if(randVoteCount <= 0)
        {
          selectedID =  saying->GetId();
          break;
        }
      }
    }
  }

  AddToDBAllSayingVotes(gAddVotes);
  UpdateDBOneSayingVotes(gSubVotes*-1, selectedID);
  AddToDBOneSayingCount(1, selectedID);

  return selectedID;
}

//----------------------------------------------------------------------------
//  Purpose:
//   Print
//
//  Notes:
//
//----------------------------------------------------------------------------
std::string AllSayings::GetSaying(int id)
{
  for(std::shared_ptr<Saying> saying: mSayings)
  {
    if(id == saying->GetId())
    {
      return saying->GetSaying();
    }
  }

  return "Unknown";
}
