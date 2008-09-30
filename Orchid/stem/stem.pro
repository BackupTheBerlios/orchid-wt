TEMPLATE      = lib
CONFIG       += dll debug
INCLUDEPATH  += ..
HEADERS       = configurationloader.h \
                configurationwriter.h \
                extensionmanager.h \
                extensionplugin.h \
                location.h \
                request.h \
                request_p.h \
                resourcebase.h \
                resourcebase_p.h \
                resourcefactory.h \
                resourceglobals.h \
                resource.h \
                resourcekeep.h \
                resourcemodel.h \
                resourceobject.h
SOURCES       = configurationloader.cpp \
                configurationwriter.cpp \
                extensionmanager.cpp \
                extensionplugin.cpp \
                location.cpp \
                request.cpp \
                resourcebase.cpp \
                resource.cpp \
                resourcefactory.cpp \
                resourcekeep.cpp \
                resourcemodel.cpp \
                resourceobject.cpp
TARGET        = $$qtLibraryTarget(orchid-stem)
 
