#ifndef INCOMINGDATACONTROLLER_H
#define INCOMINGDATACONTROLLER_H

#include "Dataset\DataRecord.h"
#include <QtSql>
#include <QString>
#include <QDebug>

using namespace QSql;

class IncomingDataController
{
public:
	IncomingDataController(QString server, QString dbName, QString username, QString password);
	~IncomingDataController();

	bool checkNewData();
	long getLastVersionID();

    DataRecord *getNewData();

	

private:
    bool performDatabaseConnection();
	long lastVersionID;
    QSqlDatabase db;
	
};

#endif // INCOMINGDATACONTROLLER_H
