TEMPLATE      = lib
CONFIG       += plugin debug
INCLUDEPATH  += ../..
QT            = core script
LIBS         += -L../../stem -lorchid-stem
HEADERS       = scriptplugin.h \
                scriptedresource.h
SOURCES       = scriptplugin.cpp \
                scriptedresource.cpp
TARGET        = $$qtLibraryTarget(scriptplugin)
 
