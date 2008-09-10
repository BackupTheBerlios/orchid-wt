cmake_minimum_required(VERSION 2.6)
project(Orchid-Resources)

ADD_DEFINITIONS(-DDEBUG -ggdb -O)

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

add_subdirectory(Orchid)
add_subdirectory(Wheat)
