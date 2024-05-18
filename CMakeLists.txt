cmake_minimum_required(VERSION 3.26)
project(NetCommandExecutor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_LIBS  OFF)
set(Boost_USE_RELEASE_LIBS ON)

find_package(Boost)

if(Boost_FOUND)
    message("Boost found!")
    include_directories( ${Boost_INCLUDE_DIRS} )
    link_directories(${Boost_LIBRARY_DIRS})
endif()

set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src" )

message( "SourceDir: ${PROJECT_SOURCE_DIR}" )
set(SRC_FILES ${PROJECT_SOURCE_DIR}/net/CommandHandler.cpp
              ${PROJECT_SOURCE_DIR}/net/Client.cpp
              ${PROJECT_SOURCE_DIR}/net/Command.cpp
              ${PROJECT_SOURCE_DIR}/net/TcpServer.cpp
              ${PROJECT_SOURCE_DIR}/net/CommandInterpreter.cpp
              ${PROJECT_SOURCE_DIR}/net/CommandExecutor.cpp
              ${PROJECT_SOURCE_DIR}/net/CommandExecutionInterface.cpp
)

add_library(NetCommandExecutor STATIC ${SRC_FILES})
target_link_libraries(NetCommandExecutor ${Boost_LIBRARIES})