TEMPLATE      = lib
CONFIG       += plugin debug
INCLUDEPATH  += ../..
LIBS         += -L../../stem -lorchid-stem
HEADERS       = modelplugin.h \
                modelresource.h \
                modelresource_p.h \
                xmlmodelresource.h
SOURCES       = modelplugin.cpp \
                modelresource.cpp \
                xmlmodelresource.cpp
TARGET        = $$qtLibraryTarget(modelplugin)
 
