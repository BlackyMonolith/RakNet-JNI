cmake_minimum_required(VERSION 3.2)
project(RakNet_JNI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Detect JNI
find_package(JNI)
include_directories( ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} )

# Detect RakNet Sources:
add_subdirectory(RakNet)
add_subdirectory(JNI)

set(SOURCE_FILES ${RAKNET_SOURCES} ${JNI_SOURCES})
MESSAGE(STATUS "Sources are: ${SOURCE_FILES}")

# Find the JNI libraries:

add_definitions( "-fpermissive" )
add_library(RakNet_JNI SHARED ${SOURCE_FILES})

# Detect Libraries required by RakNet
IF ( WIN32 AND NOT UNIX )
    set(RAKNET_LIBRARIES ws2_32.lib)
ELSE ( WIN32 AND NOT UNIX )
    set(RAKNET_LIBRARIES pthread)
ENDIF ( WIN32 AND NOT UNIX )

TARGET_LINK_LIBRARIES( RakNet_JNI ${RAKNET_LIBRARIES} ${JAVA_JVM_LIBRARY} )

# Specify output directories
set( LIBRARY_OUTPUT_DIRECTORY "./bin" )