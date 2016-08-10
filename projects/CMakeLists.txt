cmake_minimum_required( VERSION 2.8.8 )

################
## version and path
#########################
set(MyCSS_PROJECT_NAME "mycss")
set(MyCSS_PROJECT_DIR_ROOT "..")
set(MyCSS_PROJECT_DIR_LIB "${MyCSS_PROJECT_DIR_ROOT}/lib")
set(MyCSS_PROJECT_API_HEADER "${MyCSS_PROJECT_DIR_ROOT}/include/mycss")
set(MyCSS_PROJECT_SOURCE "${MyCSS_PROJECT_DIR_ROOT}/source")

set(MyCSS_VERSION_MAJOR 0)
set(MyCSS_VERSION_MINOR 0)
set(MyCSS_VERSION_PATCH 9)
set(MyCSS_VERSION_STRING "${MyCSS_VERSION_MAJOR}.${MyCSS_VERSION_MINOR}.${MyCSS_VERSION_PATCH}")

project(${MyCSS_PROJECT_NAME})

################
## lib param
#########################
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 OLD)
endif()

set(MyCSS_PROJECT_LIB_NAME "${MyCSS_PROJECT_NAME}")
set(MyCSS_PROJECT_LIB_NAME_STATIC "${MyCSS_PROJECT_NAME}_static")

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if(NOT DEFINED MyCSS_OPTIMIZATION_LEVEL)
  set(MyCSS_OPTIMIZATION_LEVEL "-O2")
endif()

if(NOT DEFINED MyCSS_BUILD_WITHOUT_THREADS)
    set(MyCSS_BUILD_WITHOUT_THREADS "NO")
endif()

set(INSTALL_DLL_EXE_DIR "bin")

option(MyCSS_BUILD_SHARED "Build shared library" ON)
option(MyCSS_BUILD_STATIC "Build static library" ON)
option(MyCSS_INSTALL_HEADER "Install header files" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MyCSS_OPTIMIZATION_LEVEL}")

include_directories(${MyCSS_PROJECT_SOURCE})
FILE(GLOB ALL_SOURCE_LIST 
  "${MyCSS_PROJECT_SOURCE}/mycss/*.c"
  "${MyCSS_PROJECT_SOURCE}/mycss/selectors/*.c"
  "${MyCSS_PROJECT_SOURCE}/mycss/namespace/*.c"
  "${MyCSS_PROJECT_SOURCE}/mycss/media/*.c"
  "${MyCSS_PROJECT_SOURCE}/mycss/values/*.c"
  "${MyCSS_PROJECT_SOURCE}/mycss/property/*.c"
  "${MyCSS_PROJECT_SOURCE}/mycss/declaration/*.c")

if (!UNIX AND WIN32)
  if(${CMAKE_CL_64})
    add_definitions(-D_WIN64)
  else()
    add_definitions(-D_WIN32)
  endif()
endif()

if(${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
  add_definitions(/wd4100 /wd4255 /wd4820 /wd4668)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  
  message(STATUS "Windows set definitions")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall --std=c99 -fPIC")
  
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-D_POSIX_C_SOURCE=199309L)
  endif()
  
  if(MyCSS_BUILD_WITHOUT_THREADS STREQUAL "NO")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
  endif()
endif()

if(MyCSS_BUILD_WITHOUT_THREADS STREQUAL "YES")
  message(STATUS "Build without POSIX Threads")
  add_definitions(-DMyCSS_BUILD_WITHOUT_THREADS)
else()
  message(STATUS "Build with POSIX Threads")
endif()

################
## build
#########################
if(MyCSS_BUILD_SHARED)
  add_library(${MyCSS_PROJECT_NAME} SHARED ${ALL_SOURCE_LIST})
  set_target_properties(${MyCSS_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${MyCSS_PROJECT_NAME})
  set_target_properties(${MyCSS_PROJECT_NAME} PROPERTIES VERSION ${MyCSS_VERSION_STRING} SOVERSION ${MyCSS_VERSION_MAJOR})
endif()

if(MyCSS_BUILD_STATIC)
  add_library(${MyCSS_PROJECT_LIB_NAME_STATIC} STATIC ${ALL_SOURCE_LIST})
  set_target_properties(${MyCSS_PROJECT_LIB_NAME_STATIC} PROPERTIES OUTPUT_NAME ${MyCSS_PROJECT_LIB_NAME_STATIC})
  set_target_properties(${MyCSS_PROJECT_LIB_NAME_STATIC} PROPERTIES VERSION ${MyCSS_VERSION_STRING} SOVERSION ${MyCSS_VERSION_MAJOR})
endif()

target_link_libraries(${MyCSS_PROJECT_NAME} myhtml)
target_link_libraries(${MyCSS_PROJECT_LIB_NAME_STATIC} myhtml)

################
## install
#########################
if(MyCSS_INSTALL_HEADER)
  install(DIRECTORY "${MyCSS_PROJECT_API_HEADER}" DESTINATION "include" FILES_MATCHING PATTERN "*.h")
endif()

if(MyCSS_BUILD_SHARED)
  install(TARGETS ${MyCSS_PROJECT_LIB_NAME}
  RUNTIME DESTINATION "${INSTALL_DLL_EXE_DIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()

if(MyCSS_BUILD_STATIC)
  install(TARGETS ${MyCSS_PROJECT_LIB_NAME_STATIC}
  RUNTIME DESTINATION "${INSTALL_DLL_EXE_DIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()

