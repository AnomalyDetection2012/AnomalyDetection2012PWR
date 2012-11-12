#include "connectortracker.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "Dataset/DatasetConnector.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "DataLoader/dataloader.h"

ConnectorTracker::ConnectorTracker()
{
    // CONSTRUCTORS
    dataset = new DatasetConnector();
    anomalyDetection = new AlgorithmControler();
//    incomingData = new IncomingDataController("RAFAL-PC\\SQLEXPRESS","SCSWin", "sa", "tajnehaslo"); //TODO: parametryzacja gdy bedzie juz czytanie configow z plikow
    loader = new DataLoader(3,"RAFAL-PC\\SQLEXPRESS","SCSWin", "sa", "tajnehaslo");
}

void ConnectorTracker::initialise(){
    // INITS getting connectors
    dataset->initialiseConnectors(this);
    anomalyDetection->initialiseConnectors(this);
//    incomingData->initialiseConnectors(this);
    loader->initialiseConnectors(this);
}
