TEMPLATE      = app
CONFIG       += debug
INCLUDEPATH  += ../../Orchid
QT            = core gui network webkit xml
LIBS         += -L../../Orchid/stem \
                -lorchid-stem \
                -L../../Orchid/flower \
                -lorchid-flower \
                -L../../Orchid/root \
                -lorchid-root
HEADERS       = mainwindow.h \
                newresourcedialog.h \
                resourceconfig.h \
                stringlistedit.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                newresourcedialog.cpp \
                resourceconfig.cpp \
                stringlistedit.cpp
FORMS         = mainwindow.ui \
                newresourcedialog.ui \
                resourceconfig.ui \
				stringlistedit.ui
RESOURCES     = browser.qrc
TARGET        = orchid-browser
