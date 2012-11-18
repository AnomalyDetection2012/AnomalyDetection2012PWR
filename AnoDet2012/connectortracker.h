#ifndef CONNECTORTRACKER_H
#define CONNECTORTRACKER_H

#include <QtSql>

class ConfigurationHandler;
class DatasetConnector;
class AlgorithmControler;
class IncomingDataController;
class DataLoader;
class GUIController;

class ConnectorTracker
{
public:
    ConnectorTracker();
    void initialise();


private:



public:
    QSqlDatabase *dbConnection;
    ConfigurationHandler *configuration;
    DatasetConnector *dataset;
    AlgorithmControler *anomalyDetection;
    IncomingDataController *incomingData;
    DataLoader *loader;
    GUIController *guiController;



private:



};

#endif // CONNECTORTRACKER_H
