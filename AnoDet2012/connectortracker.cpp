#include "connectortracker.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "Dataset/DatasetConnector.h"

ConnectorTracker::ConnectorTracker()
{
    // CONSTRUCTORS
    dataset = new DatasetConnector();
    anomalyDetection = new AlgorithmControler();
}

void ConnectorTracker::initialise(){
    // INITS getting connectors
    dataset->initialiseConnectors(this);
    anomalyDetection->initialiseConnectors(this);
}
