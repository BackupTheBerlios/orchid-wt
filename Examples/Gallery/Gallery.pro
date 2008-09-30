TEMPLATE      = lib
CONFIG       += plugin debug
INCLUDEPATH  += ../../Orchid
LIBS         += -L../../Orchid/stem \
                -lorchid-stem \
                -L../../Orchid/flower \
                -lorchid-flower
HEADERS       = gallerydemo.h \
                gallery.h \
                galleryplugin.h
SOURCES       = gallery.cpp \
                gallerydemo.cpp \
                galleryplugin.cpp
RESOURCES     = gallery.qrc
TARGET        = $$qtLibraryTarget(galleryplugin)
