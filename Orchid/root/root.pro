TEMPLATE      = lib
CONFIG       += dll debug
INCLUDEPATH  += ..
QT            = core network xml
LIBS         += -L../stem -lorchid-stem
HEADERS       = httpservice.h \
                httpservice_p.h \
                service.h \
                service_p.h
SOURCES       = httpservice.cpp \
                service.cpp
TARGET        = $$qtLibraryTarget(orchid-root)
