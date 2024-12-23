cmake_minimum_required(VERSION 3.10)

# set the project name
project(J1imsFridge)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(BUILD_MODE Debug)
include_directories(./Common)
include_directories(./MQTT)
find_package(jsoncpp REQUIRED)
find_library(paho-mqtt3a NAMES libpaho-mqtt3a.so REQUIRED)

# Find Boost libs.
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE( Boost 1.74.0 COMPONENTS program_options thread log log_setup REQUIRED )
find_package(Threads)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ../JimsCommon/include )
INCLUDE_DIRECTORIES( ../JimsHWCommon/include )
INCLUDE_DIRECTORIES( ./include )

add_executable(jimsFridge
../JimsCommon/source/logger.cpp
../JimsCommon/source/loggerSingleton.cpp
../JimsCommon/source/connection.cpp
../JimsCommon/source/mqtt.cpp
../JimsCommon/source/mqttMessage.cpp
../JimsCommon/source/mqttSingleton.cpp
../JimsCommon/source/stopWatch.cpp
../JimsCommon/source/mySqlDB.cpp
../JimsCommon/source/mySqlDBSingleton.cpp
../JimsCommon/source/randomSingleton.cpp
../JimsCommon/source/propertyFile.cpp
../JimsHWCommon/source/raspi/i2c.cpp
../JimsHWCommon/source/ads1015.cpp
../JimsHWCommon/source/tmp117.cpp
../JimsHWCommon/source/serLCD.cpp
./source/options.cpp
./source/properties.cpp
./source/section.cpp
./source/saying.cpp
./source/allSayings.cpp
./source/jimsFridge.cpp)
target_link_libraries(jimsFridge jsoncpp paho-mqtt3a ${Boost_LIBRARIES} mysqlcppconn)

