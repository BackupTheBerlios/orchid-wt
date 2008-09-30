TEMPLATE      = lib
CONFIG       += plugin debug
INCLUDEPATH  += ../../Orchid
LIBS         += -L../../Orchid/stem \
                -lorchid-stem \
                -L../../Orchid/flower \
                -lorchid-flower
HEADERS       = docstreamsplugin.h \
                docstreamssample.h
SOURCES       = docstreamsplugin.cpp \
                docstreamssample.cpp
TARGET        = $$qtLibraryTarget(docstreamsplugin)
