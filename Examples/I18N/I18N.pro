TEMPLATE      = lib
CONFIG       += plugin debug
INCLUDEPATH  += ../../Orchid
LIBS         += -L../../Orchid/stem \
                -lorchid-stem \
                -L../../Orchid/flower \
                -lorchid-flower
HEADERS       = i18ndocplugin.h \
                i18ndocsample.h
SOURCES       = i18ndocplugin.cpp \
                i18ndocsample.cpp
TARGET        = $$qtLibraryTarget(i18ndocplugin)
