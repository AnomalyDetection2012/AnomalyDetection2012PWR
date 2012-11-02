#ifndef CONNECTORTRACKER_H
#define CONNECTORTRACKER_H

class DatasetConnector;
class AlgorithmControler;

class ConnectorTracker
{
public:
    ConnectorTracker();
    DatasetConnector *dataset;
    AlgorithmControler *anomalyDetection;
    void initialise();
};

#endif // CONNECTORTRACKER_H
