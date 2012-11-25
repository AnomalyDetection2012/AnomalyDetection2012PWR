#include "IncomingDataController.h"
#include <sstream>

static const QString statement_base = "SELECT CT.Rekord_ID, E.Obiekt_ID, E.Data, E.Typ_polaczenia FROM dbo.Rekord E JOIN CHANGETABLE(CHANGES dbo.Rekord, %1) AS CT ON E.Rekord_ID = CT.Rekord_ID WHERE CT.SYS_CHANGE_OPERATION='I' AND CT.SYS_CHANGE_VERSION <= %2 AND E.Obiekt_ID = %3;";
static const QString statement_measurement = "SELECT  [Wyniki_pomiar].[Program_pomiar_ID],[Wartosc],  [Program_pomiar].[AlertMin],  [Program_pomiar].[AlertMax], [Nazwa_pomiaru] FROM  [Wyniki_pomiar] inner join  [Program_pomiar] on  [Wyniki_pomiar].[Program_pomiar_ID]= [Program_pomiar].[Program_pomiar_ID] WHERE [Rekord_ID]=%1;";

IncomingDataController::IncomingDataController(QString server, QString dbName, QString username, QString password)
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(processNewData()));

	QString connectionString("Driver={SQL Server};Server=");
	connectionString.append(server).append(";Database=").append(dbName);

    db = QSqlDatabase::addDatabase("QODBC3");
	db.setDatabaseName(connectionString);
	db.setUserName(username);
	db.setPassword(password);

    if (!db.open())
	{
        qDebug() << "An error was encountered: "<< QSqlError(db.lastError()).text();
    }
	else
	{
        qDebug() << "The database connection was established successfully.";
    }

    lastVersionID = getCurrVersionID();
}

IncomingDataController::IncomingDataController(QSqlDatabase *dbConn)
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(processNewData()));

    this->db = *dbConn;

    lastVersionID = getCurrVersionID();
}

IncomingDataController::~IncomingDataController()
{

}

void IncomingDataController::initialiseConnectors(ConnectorTracker *con){
    anomalyDetection = con->anomalyDetection;
    datasetConnector = con->dataset;
    configuration = con->configuration;
    guiController = con->guiController;
    mainWindow = con->mainWindow;
    notificationSender = con->sender;
}

void IncomingDataController::startListening()
{
    timer->start(10000);    //default 10 sec
}

void IncomingDataController::startListening(unsigned long interval)
{
    timer->start(interval);
}

void IncomingDataController::stopListening()
{
    timer->stop();
}

void IncomingDataController::setRefreshInterval(unsigned interval)
{
    this->timer->setInterval(interval);
}

unsigned IncomingDataController::getRefreshInterval()
{
    return this->timer->interval();
}

int IncomingDataController::getCurrVersionID()
{
    if(!performDatabaseConnection())
        return -1;

    int currVersion;
    QString statement = "SELECT CHANGE_TRACKING_CURRENT_VERSION() AS VersionID";
    QSqlQuery query(statement, db);

    if (!query.isActive())
    {
        qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
    }
    else
    {
        if (query.next())
        {
            currVersion = query.value(0).toString().toInt();
            qDebug() << "Current version: " << currVersion;
            return currVersion;
        }
    }

    return -1;
}

bool IncomingDataController::checkNewData()
{
    int currVersion = getCurrVersionID();
    return currVersion > lastVersionID;
}

int IncomingDataController::getLastVersionID()
{
	return lastVersionID;
}

DataRecord *IncomingDataController::getNewData()
{
    if(!performDatabaseConnection())
        return NULL;//new DataRecord[0];

    return NULL;//new DataRecord[0];
	
}

void IncomingDataController::processNewData()
{
    qDebug() << "Checking for new data...";
    int currVersion = getCurrVersionID();
    if(currVersion < 0)
    {
        qDebug() << "An error was encountered when getting change tracking current version";
        return;
    }

    int chosenObjId = configuration->getPropertyValue("SelectedObject","Object_ID").toInt();

    QString statement = statement_base.arg(lastVersionID, 0, 10).arg(currVersion, 0, 10).arg(chosenObjId, 0, 10);

    QSqlQuery query(statement, db);

    if (!query.isActive())
    {
        qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
    }
    else
    {
        vector <vector <double> > data;

        while (query.next())
        {
            int record_id = query.value(0).toInt();
            int object_id = query.value(1).toInt();
            QDateTime dateTime = query.value(2).toDateTime();
            int connection = query.value(3).toInt();


            qDebug() << "New item:\t" << record_id << "\t" << object_id << "\t" << dateTime.toString("dd.MM.yyyy hh:mm:ss.zzz") << "\t" << connection;

            QString stat_measurements = statement_measurement.arg(record_id, 0, 10);
            QSqlQuery query_measurements(stat_measurements, db);

            if(!query_measurements.isActive())
            {
                 qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
            }
            else
            {
                vector <double> dataValues;
                vector <double> mins, maxs;
                vector <int> infos;

                while(query_measurements.next())
                {
                    int program_measurement_ID = query_measurements.value(0).toInt();
                    infos.push_back(program_measurement_ID);

                    double value = query_measurements.value(1).toDouble();

                    double min = datasetConnector->getMinValue(program_measurement_ID);
                    double max = datasetConnector->getMaxValue(program_measurement_ID);

                    dataValues.push_back(value);
                    mins.push_back(min);
                    maxs.push_back(max);
                    qDebug() << "\t " << value << "\tmin: " << min << "\tmax: " << max;
                }

                // test
                vector < vector<double> > wrap;
                wrap.push_back(dataValues);
                double *minsT = new double[mins.size()];
                double *maxsT = new double[maxs.size()];

                for(int k=0;k<mins.size();k++)
                {
                    minsT[k] = mins[k];
                    maxsT[k] = maxs[k];
                }
                vector <bool> result = anomalyDetection->test(mainWindow->getSelectedMethodId(),wrap, minsT, maxsT);

                // add DataRecord to dataset
                vector <double> nonInf(0);

                datasetConnector->newRecord((time_t)dateTime.toTime_t(), dataValues, nonInf, infos, result[0]);

                // redraw LiveLineChart
                guiController->refreshLiveLineChart();

                mainWindow->newRecords(1);

                if(result[0])
                {
                    mainWindow->newAnomalies(1);

                    std::vector<QString> units;
                    for(int i=0; i<infos.size(); i++)
                    {
                        units.push_back(datasetConnector->datasetControler->dataset->dataTable->measurementsInfos->getUnit(infos[i]));
                    }
                    //send notifications
                    notificationSender->sendNotifications(mainWindow->getSelectedObjectName(), dateTime, datasetConnector->datasetControler->dataset->dataTable->dataNames, dataValues, units, mins, maxs);
                }
            }
        }

        lastVersionID = currVersion;
    }

}

bool IncomingDataController::performDatabaseConnection()
{
    if(!db.isOpen())
    {
        if(!db.open())
        {
            qDebug() << "An error was encountered: "<< QSqlError(db.lastError()).text();
            return false;
        }
    }

    return true;
}
