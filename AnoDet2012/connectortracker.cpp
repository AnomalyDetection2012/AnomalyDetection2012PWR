#include "connectortracker.h"
#include "Dataset/DatasetConnector.h"
#include "DataLoader/dataloader.h"
#include "ConfigurationHandler/configurationhandler.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "GUI_COMPONENTS/guicontroller.h"

ConnectorTracker::ConnectorTracker()
{
    configuration = new ConfigurationHandler();
    dataset = new DatasetConnector();
    anomalyDetection = new AlgorithmControler();
    guiController = new GUIController(); // remember to set components
    isConnected = false;
}

void ConnectorTracker::initialise(){
    dataset->initialiseConnectors(this);
    anomalyDetection->initialiseConnectors(this);
}

void ConnectorTracker::createConnection(int objectId){
    QString server = configuration->getDatabaseServer();
    QString source = configuration->getDatabaseSource();
    QString userName = configuration->getDatabaseUserName();
    QString userPassword = configuration->getDatabaseUserPassword();

    // create shared database connection:
    QString connectionString("Driver={SQL Server};Server=");
    connectionString.append(server).append(";Database=").append(source);

    dbConnection = &QSqlDatabase::addDatabase("QODBC3");
    dbConnection->setDatabaseName(connectionString);
    dbConnection->setUserName(userName);
    dbConnection->setPassword(userPassword);

    if (!dbConnection->open())
    {
        qDebug() << "An error was encountered: "<< QSqlError(dbConnection->lastError()).text();
    }
    else
    {
        qDebug() << "The database connection was established successfully.";
    }

    incomingData = new IncomingDataController(dbConnection);
    loader = new DataLoader(objectId, dbConnection);
    incomingData->initialiseConnectors(this);
    loader->initialiseConnectors(this);
    isConnected = true;
}
