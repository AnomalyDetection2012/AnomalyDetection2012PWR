#include "connectortracker.h"
#include "Dataset/DatasetConnector.h"
#include "DataLoader/dataloader.h"
#include "ConfigurationHandler/configurationhandler.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"

ConnectorTracker::ConnectorTracker()
{
    configuration = new ConfigurationHandler();
    dataset = new DatasetConnector();
    anomalyDetection = new AlgorithmControler();

    QString server = configuration->getDatabaseServer();
    QString source = configuration->getDatabaseSource();
    QString userName = configuration->getDatabaseUserName();
    QString userPassword = configuration->getDatabaseUserPassword();

    incomingData = new IncomingDataController(server, source, userName, userPassword);
    loader = new DataLoader(3, server, source, userName, userPassword);
}

void ConnectorTracker::initialise(){
    dataset->initialiseConnectors(this);
    anomalyDetection->initialiseConnectors(this);
    incomingData->initialiseConnectors(this);
    loader->initialiseConnectors(this);
}
