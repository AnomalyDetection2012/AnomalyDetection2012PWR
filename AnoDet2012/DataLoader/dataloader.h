#ifndef DATALOADER_H
#define DATALOADER_H

#include "../Dataset/DataRecord.h"
#include "../Dataset/DataRecordTable.h"
#include "../Dataset/MeasurementInfo.h"
#include "connectortracker.h"

#include <vector>

#include <QString>
#include <QtSql>
#include <QHash>
#include <QProgressDialog>


class DataLoader
{
public:
    int objectId, recordSize;
    QSqlDatabase db;
    QHash<int, QHash <QString, QVariant> > measurementInfo;
    DatasetConnector* dataset;
    QProgressDialog* progessBar;


    DataLoader(int objectId, QString server, QString dbName, QString username, QString password);
    DataLoader(int objectId, QSqlDatabase *dbConn);

    void initialiseConnectors(ConnectorTracker *con);
    void initDataRecordTable();
    void loadMeasurementInfo();
    void loadAllRecords();
    std::vector<int> loadAllObjectIDs();
    int getAmountOfObjectRecords(int objectID);
    void loadRecords(int from, int to);
    //void loadRecords(QDateTime from, QDateTime to);
    void setAlarmFlagToRecords();
    void removeAllRecords();

private:
    std::vector<double> programAlarmIds;
    std::vector<int> recordIds;
    bool performDatabaseConnection();
    int getRecordIDOfFirstRecord(int begin);
    //int getRecordIDOfFirstRecord(QDateTime begin);
};

#endif // DATALOADER_H
