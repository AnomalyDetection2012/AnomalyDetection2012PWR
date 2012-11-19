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
    void createConnection(int objectId);

private:


public:
    QSqlDatabase *dbConnection;
    ConfigurationHandler *configuration;
    DatasetConnector *dataset;
    AlgorithmControler *anomalyDetection;
    IncomingDataController *incomingData;
    DataLoader *loader;
    GUIController *guiController;
    bool isConnected;


private:



};

#endif // CONNECTORTRACKER_H
