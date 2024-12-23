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
#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include "loggerSingleton.h"

//----------------------------------------------------------------------------
//  Global
//----------------------------------------------------------------------------
bool gJustHelp = false;
std::string gProperties = "/var/jimsfridge/jimsfridge.prop";

//----------------------------------------------------------------------------
//  Purpose:
//   Build and parse the command line settings
//
//  Notes:
//
//----------------------------------------------------------------------------
void setFlags(int argc, char* argv[])
{
  try {
    boost::program_options::options_description general("General options");
    general.add_options()
        ("help,h", "produce a help message")
        ("properties,p", boost::program_options::value<std::string>(&gProperties)->
          default_value("/var/jimsfridge/jimsfridge.prop"), "The properties file")
    ;

    boost::program_options::variables_map options_entered;
    store(parse_command_line(argc, argv, general), options_entered);

    boost::program_options::variables_map optionsEntered;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, general), optionsEntered);
    boost::program_options::notify(optionsEntered);

    if (optionsEntered.count("help")) {
        std::cout << general << "\n";
        gJustHelp = true;
    }
  }
  catch(std::exception& e)  
  {
    std::cout << "Options Exception:"+std::string(e.what())<<"\n";
  }

  std::cout << "gProperties:" + gProperties<<"\n";
}
