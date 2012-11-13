#ifndef CONNECTORTRACKER_H
#define CONNECTORTRACKER_H

class ConfigurationHandler;
class DatasetConnector;
class AlgorithmControler;
class IncomingDataController;
class DataLoader;

class ConnectorTracker
{
public:
    ConnectorTracker();
    void initialise();


private:



public:
    ConfigurationHandler *configuration;
    DatasetConnector *dataset;
    AlgorithmControler *anomalyDetection;
    IncomingDataController *incomingData;
    DataLoader *loader;


private:



};

#endif // CONNECTORTRACKER_H
