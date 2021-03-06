#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T20:38:50
#
#-------------------------------------------------


QT       += core gui sql webkit network xml
CONFIG   += exceptions rtti

TARGET = AnoDet2012
TEMPLATE = app


SOURCES += main.cpp \
    Dataset/InfoTable.cpp \
    Dataset/datasetControler.cpp \
    Dataset/DatasetConnector.cpp \
    Dataset/Dataset.cpp \
    Dataset/DataRecordTable.cpp \
    Dataset/DataRecord.cpp \
    ANOMALY_DETECTION/method.cpp \
    INCOMING_DATA_TRACKING/IncomingDataController.cpp \
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
    DataLoader/dataloader.cpp \
    Dataset/MeasurementInfo.cpp \
    ConfigurationHandler/configurationhandler.cpp \
    GUI_COMPONENTS/livelinechart.cpp \
    GUI_COMPONENTS/guicontroller.cpp \
    GUI/mainwindow.cpp \
    GUI/dialogfilter.cpp \
    Alarms/Mail/smtpclient.cpp \
    Alarms/Mail/quotedprintable.cpp \
    Alarms/Mail/mimetext.cpp \
    Alarms/Mail/mimepart.cpp \
    Alarms/Mail/mimemultipart.cpp \
    Alarms/Mail/mimemessagebuilder.cpp \
    Alarms/Mail/mimemessage.cpp \
    Alarms/Mail/mimeinlinefile.cpp \
    Alarms/Mail/mimehtml.cpp \
    Alarms/Mail/mimefile.cpp \
    Alarms/Mail/mimecontentformatter.cpp \
    Alarms/Mail/mimeattachment.cpp \
    Alarms/Mail/mailsender.cpp \
    Alarms/Mail/emailaddress.cpp \
    Alarms/SMS/smssender.cpp \
    Alarms/SMS/smsmessagebuilder.cpp \
    Alarms/notificationsender.cpp \
    Alarms/subscribershandler.cpp \
    GUI/datasettableview.cpp \
    Reports/WebViewPrinter.cpp \
    Reports/ReportGenerator.cpp \
    Reports/Report.cpp \
    GUI/dialogbusy.cpp \
    GUI/dialoghelp.cpp

HEADERS  += \
    Dataset/InfoTable.h \
    Dataset/datasetControler.h \
    Dataset/DatasetConnector.h \
    Dataset/Dataset.h \
    Dataset/DataRecordTable.h \
    Dataset/DataRecord.h \
    ANOMALY_DETECTION/method.h \
    INCOMING_DATA_TRACKING/IncomingDataController.h \
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
    DataLoader/dataloader.h \
    Dataset/MeasurementInfo.h \
    ConfigurationHandler/configurationhandler.h \
    GUI_COMPONENTS/livelinechart.h \
    GUI_COMPONENTS/guicontroller.h \
    GUI/mainwindow.h \
    GUI/dialogfilter.h \
    Alarms/subscriber.h \
    Alarms/Mail/smtpmime.h \
    Alarms/Mail/smtpclient.h \
    Alarms/Mail/quotedprintable.h \
    Alarms/Mail/mimetext.h \
    Alarms/Mail/mimepart.h \
    Alarms/Mail/mimemultipart.h \
    Alarms/Mail/mimemessagebuilder.h \
    Alarms/Mail/mimemessage.h \
    Alarms/Mail/mimeinlinefile.h \
    Alarms/Mail/mimehtml.h \
    Alarms/Mail/mimefile.h \
    Alarms/Mail/mimecontentformatter.h \
    Alarms/Mail/mimeattachment.h \
    Alarms/Mail/mailsender.h \
    Alarms/Mail/emailaddress.h \
    Alarms/SMS/smssender.h \
    Alarms/SMS/smsmessagebuilder.h \
    Alarms/notificationsender.h \
    Alarms/subscribershandler.h \
    GUI/datasettableview.h \
    Reports/WebViewPrinter.h \
    Reports/ReportGenerator.h \
    Reports/Report.h \
    GUI/dialogbusy.h \
    GUI/dialoghelp.h

FORMS    += \
    GUI/mainwindow.ui \
    GUI/dialogfilter.ui \
    GUI/dialogbusy.ui \
    GUI/dialoghelp.ui

OTHER_FILES +=

RESOURCES += \
    Resources/resources.qrc
