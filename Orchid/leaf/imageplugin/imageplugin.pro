TEMPLATE      = lib
CONFIG       += plugin debug
INCLUDEPATH  += ../..
LIBS         += -L../../stem -lorchid-stem
HEADERS       = imagecollection.h \
                imagecollection_p.h \
                imageplugin.h \
                imageresource.h
SOURCES       = imagecollection.cpp \
                imageplugin.cpp \
                imageresource.cpp
TARGET        = $$qtLibraryTarget(imageplugin)
 
