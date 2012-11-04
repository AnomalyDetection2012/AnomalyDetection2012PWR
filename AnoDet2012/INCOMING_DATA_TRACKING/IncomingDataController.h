#ifndef INCOMINGDATACONTROLLER_H
#define INCOMINGDATACONTROLLER_H

#include "Dataset\DataRecord.h"
#include <QtSql>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QtCore/QCoreApplication>


using namespace QSql;

class IncomingDataController : public QObject
{

    Q_OBJECT

public:
	IncomingDataController(QString server, QString dbName, QString username, QString password);
	~IncomingDataController();

    void startListening();  // start tracking database for new data
    void startListening(unsigned long interval);    // start tracking database for new data with indicated interval
    void stopListening();   // stop tracking database for new data

    bool checkNewData();
    int getCurrVersionID();


    int getLastVersionID();
    DataRecord *getNewData();

	

private:
    bool performDatabaseConnection();
    int lastVersionID;
    QSqlDatabase db;
    QTimer *timer;

public slots:
   void processNewData();  // process new data (if exists)

	
};

#endif // INCOMINGDATACONTROLLER_H