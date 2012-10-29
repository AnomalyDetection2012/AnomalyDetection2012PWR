#include "IncomingDataController.h"
#include <sstream>

static const QString statement_base = "SELECT CT.Rekord_ID, E.Obiekt_ID, E.Data, E.Typ_polaczenia FROM dbo.Rekord E JOIN CHANGETABLE(CHANGES dbo.Rekord, %1) AS CT ON E.Rekord_ID = CT.Rekord_ID WHERE CT.SYS_CHANGE_OPERATION='I' AND CT.SYS_CHANGE_VERSION <= %2;";

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

IncomingDataController::~IncomingDataController()
{

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

    QString statement = statement_base.arg(lastVersionID, 0, 10).arg(currVersion, 0, 10);

    QSqlQuery query(statement, db);

    if (!query.isActive())
    {
        qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
    }
    else
    {
        while (query.next())
        {
            int record_id = query.value(0).toInt();
            int object_id = query.value(1).toInt();
            QDateTime date = query.value(2).toDateTime();
            int connection = query.value(3).toInt();
            qDebug() << "New item:\t" << record_id << "\t" << object_id << "\t" << date.toString("dd.MM.yyyy hh:mm:ss.zzz") << "\t" << connection;
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
