#ifndef CONNECTORTRACKER_H
#define CONNECTORTRACKER_H

class DatasetConnector;
class AlgorithmControler;
class IncomingDataController;
class DataLoader;

class ConnectorTracker
{
public:
    ConnectorTracker();
    DatasetConnector *dataset;
    AlgorithmControler *anomalyDetection;
    IncomingDataController *incomingData;
    DataLoader *loader;
    void initialise();
};

#endif // CONNECTORTRACKER_H
