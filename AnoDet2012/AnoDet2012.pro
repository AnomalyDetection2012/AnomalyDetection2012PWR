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
    Dataset/InfoTable.cpp \
    Dataset/datasetControler.cpp \
    Dataset/DatasetConnector.cpp \
    Dataset/Dataset.cpp \
    Dataset/DataRecordTable.cpp \
    Dataset/DataRecord.cpp \
    ANOMALY_DETECTION/method.cpp \
    INCOMING_DATA_TRACKING/IncomingDataController.cpp \
    INCOMING_DATA_TRACKING/IncomingDataConnector.cpp \
    ANOMALY_DETECTION/BAYES/naive_bayes.cpp \
    ANOMALY_DETECTION/BAYES/class_variable.cpp \
    ANOMALY_DETECTION/DENSITY/neighbor.cpp \
    ANOMALY_DETECTION/DENSITY/densitymethod.cpp \
    ANOMALY_DETECTION/NEIGHBOUR/space.cpp \
    ANOMALY_DETECTION/NEIGHBOUR/nearest_neighbor.cpp \
    ANOMALY_DETECTION/RANDOM/randommethod.cpp \
    ANOMALY_DETECTION/SOM/topologymap.cpp \
    ANOMALY_DETECTION/SOM/somnetwork.cpp \
    ANOMALY_DETECTION/SOM/neuron.cpp \
    ANOMALY_DETECTION/algorithmcontroler.cpp \
    ANOMALY_DETECTION/RANDOM/randomdetection.cpp \
    ANOMALY_DETECTION/SOM/somdetection.cpp \
    ANOMALY_DETECTION/NEIGHBOUR/neighbourdetection.cpp \
    ANOMALY_DETECTION/DENSITY/densitydetection.cpp \
    ANOMALY_DETECTION/BAYES/bayesdetection.cpp \
    ANOMALY_DETECTION/RBF/RBFNetwork.cpp \
    ANOMALY_DETECTION/RBF/OutputNeuron.cpp \
    ANOMALY_DETECTION/RBF/KMeans.cpp \
    ANOMALY_DETECTION/RBF/HiddenNeuron.cpp \
    ANOMALY_DETECTION/RBF/Cluster.cpp \
    ANOMALY_DETECTION/RBF/BIASNeuron.cpp \
    connectortracker.cpp \
    DataLoader/dataloader.cpp

HEADERS  += guimainwindow.h \
    Dataset/InfoTable.h \
    Dataset/datasetControler.h \
    Dataset/DatasetConnector.h \
    Dataset/Dataset.h \
    Dataset/DataRecordTable.h \
    Dataset/DataRecord.h \
    ANOMALY_DETECTION/method.h \
    INCOMING_DATA_TRACKING/IncomingDataController.h \
    INCOMING_DATA_TRACKING/IncomingDataConnector.h \
    ANOMALY_DETECTION/BAYES/naive_bayes.h \
    ANOMALY_DETECTION/BAYES/class_variable.h \
    ANOMALY_DETECTION/DENSITY/neighbor.h \
    ANOMALY_DETECTION/DENSITY/densitymethod.h \
    ANOMALY_DETECTION/NEIGHBOUR/space.h \
    ANOMALY_DETECTION/NEIGHBOUR/nearest_neighbor.h \
    ANOMALY_DETECTION/RANDOM/randommethod.h \
    ANOMALY_DETECTION/SOM/topologymap.h \
    ANOMALY_DETECTION/SOM/somnetwork.h \
    ANOMALY_DETECTION/SOM/neuron.h \
    ANOMALY_DETECTION/algorithmcontroler.h \
    ANOMALY_DETECTION/RANDOM/randomdetection.h \
    ANOMALY_DETECTION/SOM/somdetection.h \
    ANOMALY_DETECTION/NEIGHBOUR/neighbourdetection.h \
    ANOMALY_DETECTION/DENSITY/densitydetection.h \
    ANOMALY_DETECTION/BAYES/bayesdetection.h \
    ANOMALY_DETECTION/RBF/RBFNetwork.h \
    ANOMALY_DETECTION/RBF/OutputNeuron.h \
    ANOMALY_DETECTION/RBF/KMeans.h \
    ANOMALY_DETECTION/RBF/HiddenNeuron.h \
    ANOMALY_DETECTION/RBF/Cluster.h \
    ANOMALY_DETECTION/RBF/BIASNeuron.h \
    connectortracker.h \
    DataLoader/dataloader.h \
    DataLoader/dataloader.h

FORMS    += guimainwindow.ui
