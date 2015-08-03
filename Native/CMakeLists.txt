cmake_minimum_required(VERSION 3.0)
project(RakNet_JNI)

set ( CMAKE_RULE_MESSAGES OFF )

# Detect processor architecture:
include ( CMakeDetectArchitecture.cmake )
target_architecture ( BUILD_ARCH )

# Whether or not to build in 64-Bit mode:
IF ( BUILD_ARCH STREQUAL "x64" )
    message ( "Proposing x64 Build" )
    option ( BUILD_X64 "Build in x64 mode when enabled or in x86 mode when disabled" ON )
ELSE ( BUILD_ARCH STREQUAL "x64" )
    message ( "Proposing x86 Build" )
    option ( BUILD_X64 "Build in x64 mode when enabled or in x86 mode when disabled" OFF )
ENDIF ( BUILD_ARCH STREQUAL "x64" )

# Generate different configurations
set ( CMAKE_CONFIGURATION_TYPES "Release" )
set ( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations" FORCE )

# Build Compiler Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

IF ( MINGW OR CMAKE_COMPILER_IS_GNUCXX )
    set ( CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS} -fpermissive -D__NO_INLINE__" )
    IF ( NOT BUILD_X64 )
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
    ENDIF ( NOT BUILD_X64 )
ENDIF ( MINGW OR CMAKE_COMPILER_IS_GNUCXX )

# Detect Libraries:
IF ( WIN32 AND NOT UNIX )
    set ( BUILD_LIBRARIES "ws2_32.lib" )
ELSE ( WIN32 AND NOT UNIX )
    set ( BUILD_LIBRARIES "pthread" )
ENDIF ( WIN32 AND NOT UNIX )

find_package ( JNI )
include_directories ( ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} )

# Detect sources:
add_subdirectory ( RakNet )
add_subdirectory ( JNI )

set ( SOURCE_FILES ${RAKNET_SOURCES} ${JNI_SOURCES} )

# Build the library:
add_library ( RakNet_JNI SHARED ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES ( RakNet_JNI ${BUILD_LIBRARIES} ${JAVA_JVM_LIBRARY} )