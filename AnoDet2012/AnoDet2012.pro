#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T20:38:50
#
#-------------------------------------------------

QT       += core gui sql

TARGET = AnoDet2012
TEMPLATE = app


SOURCES += main.cpp \
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
    DENSITY/densitymethod.cpp \
    Dataset/InfoTable.cpp \
    Dataset/datasetControler.cpp \
    Dataset/DatasetConnector.cpp \
    Dataset/Dataset.cpp \
    Dataset/DataRecordTable.cpp \
    Dataset/DataRecord.cpp \
    INCOMING_DATA_TRACKING/IncomingDataController.cpp \
    INCOMING_DATA_TRACKING/IncomingDataConnector.cpp \
    RBF/OutputNeuron.cpp \
    RBF/KMeans.cpp \
    RBF/HiddenNeuron.cpp \
    RBF/Cluster.cpp \
    RBF/BIASNeuron.cpp \
    RBF/main.cpp \
    RBF/main.cpp \
    RBF/KMeans.cpp \
    RBF/RBFNetwork.cpp

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
    DENSITY/densitymethod.h \
    Dataset/InfoTable.h \
    Dataset/datasetControler.h \
    Dataset/DatasetConnector.h \
    Dataset/Dataset.h \
    Dataset/DataRecordTable.h \
    Dataset/DataRecord.h \
    INCOMING_DATA_TRACKING/IncomingDataController.h \
    INCOMING_DATA_TRACKING/IncomingDataConnector.h \
    RBF/OutputNeuron.h \
    RBF/KMeans.h \
    RBF/HiddenNeuron.h \
    RBF/Cluster.h \
    RBF/BIASNeuron.h \
    RBF/RBFNetwork.h

FORMS    += guimainwindow.ui
