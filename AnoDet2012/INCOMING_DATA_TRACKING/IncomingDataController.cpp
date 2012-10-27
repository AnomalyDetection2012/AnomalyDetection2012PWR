#include "IncomingDataController.h"

IncomingDataController::IncomingDataController(QString server, QString dbName, QString username, QString password)
{
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
}

IncomingDataController::~IncomingDataController()
{

}

bool IncomingDataController::checkNewData()
{
    if(!performDatabaseConnection())
        return false;

    long currVersion;
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
            currVersion = query.value(0).toString().toLong();
            qDebug() << "Current version: " << currVersion;
        }
    }

    return currVersion > lastVersionID;
}

long IncomingDataController::getLastVersionID()
{
	return lastVersionID;
}

DataRecord *IncomingDataController::getNewData()
{
    if(!performDatabaseConnection())
        return new DataRecord[0];

    return new DataRecord[0];
	
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
