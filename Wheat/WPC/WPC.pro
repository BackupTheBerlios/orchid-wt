TEMPLATE      = app
CONFIG       += debug
INCLUDEPATH  += ../../Orchid
QT            = core xml
LIBS         += -L../../Orchid/flower \
                -lorchid-flower
HEADERS       = cppfragmentwriter.h
SOURCES       = cppfragmentwriter.cpp \
                main.cpp
TARGET        = orchid-wpc
