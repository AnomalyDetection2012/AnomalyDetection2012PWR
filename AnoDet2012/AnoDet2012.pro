#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T20:38:50
#
#-------------------------------------------------

QT       += core gui

TARGET = AnoDet2012
TEMPLATE = app


SOURCES += main.cpp\
        guimainwindow.cpp \
    method.cpp \
    RANDOM\randommethod.cpp \
    SOM\neuron.cpp \
    SOM\somnetwork.cpp \
    SOM\topologymap.cpp \
    NEIGHBOUR/space.cpp \
    NEIGHBOUR/nearest_neighbor.cpp \
    BAYES/naive_bayes.cpp \
    BAYES/class_variable.cpp \
    DENSITY/neighbor.cpp \
    DENSITY/densitymethod.cpp

HEADERS  += guimainwindow.h \
    method.h \
    RANDOM\randommethod.h \
    SOM\neuron.h \
    SOM\somnetwork.h \
    SOM\topologymap.h \
    NEIGHBOUR/space.h \
    NEIGHBOUR/nearest_neighbor.h \
    BAYES/naive_bayes.h \
    BAYES/class_variable.h \
    DENSITY/neighbor.h \
    DENSITY/densitymethod.h

FORMS    += guimainwindow.ui
