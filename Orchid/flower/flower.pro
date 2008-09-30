TEMPLATE      = lib
CONFIG       += dll debug
INCLUDEPATH  += ..
HEADERS       = documentenums.h \
                documenthead.h \
                documentprocessor.h \
                documentprocessor_p.h \
                documentstreams.h \
                fragmentbuilder.h \
                fragmentdom.h \
                fragment.h \
                fragment_p.h \
                htmlfragmentwriter.h \
                styleattributes.h \
                style.h \
                xhtmlstreamwriter.h \
                xmlfragmentreader.h \
                xmlfragmentwriter.h
SOURCES       = documenthead.cpp \
                documentprocessor.cpp \
                documentstreams.cpp \
                fragmentbuilder.cpp \
                fragment.cpp \
                fragmentdom.cpp \
                htmlfragmentwriter.cpp \
                styleattributes.cpp \
                style.cpp \
                xhtmlstreamwriter.cpp \
                xmlfragmentreader.cpp \
                xmlfragmentwriter.cpp
TARGET        = $$qtLibraryTarget(orchid-flower)
