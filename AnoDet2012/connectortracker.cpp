#include "connectortracker.h"
#include "Dataset/DatasetConnector.h"
#include "DataLoader/dataloader.h"
#include "Alarms/notificationsender.h"
#include "ConfigurationHandler/configurationhandler.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "GUI_COMPONENTS/guicontroller.h"
#include "GUI/mainwindow.h"
#include "Reports/Report.h"

ConnectorTracker::ConnectorTracker()
{
    configuration = new ConfigurationHandler();
    sender = new NotificationSender(configuration);
    dataset = new DatasetConnector();
    anomalyDetection = new AlgorithmControler();
    guiController = new GUIController(); // remember to set components
    isConnected = false;
    dataRecordSize = 0;
}

void ConnectorTracker::initialise(){
    dataset->initialiseConnectors(this);
    anomalyDetection->initialiseConnectors(this);
}

bool ConnectorTracker::createConnection(int objectId){
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
        qDebug() << "An error was encountered: " << QSqlError(dbConnection->lastError()).text();
        return false;
    }
    else
    {
        qDebug() << "The database connection was established successfully.";
    }

    incomingData = new IncomingDataController(dbConnection);
    loader = new DataLoader(objectId, dbConnection);
    reports = new Report(dbConnection);
    reports->initialiseConnectors(this);
    incomingData->initialiseConnectors(this);
    loader->initialiseConnectors(this);
    isConnected = true;

    return true;
}
