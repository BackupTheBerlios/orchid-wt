cmake_minimum_required(VERSION 2.6)
project(Orchid-Resources)

#ADD_DEFINITIONS(-DDEBUG -ggdb)
ADD_DEFINITIONS(-DQT_NO_DEBUG)

set(QT_USE_QTXML true)
set(QT_USE_QTWEBKIT true)
set(QT_USE_QTSCRIPT true)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIRS})
add_definitions(${QT_DEFINITIONS})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Orchid
)

set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/Orchid/plugins)
set(EXE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)

add_subdirectory(Orchid)
add_subdirectory(Wheat)
add_subdirectory(Examples)
