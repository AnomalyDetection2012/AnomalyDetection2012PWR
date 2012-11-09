#ifndef DATALOADER_H
#define DATALOADER_H

#include "../Dataset/DataRecord.h"
#include "../Dataset/DataRecordTable.h"
#include "../Dataset/MeasurementInfo.h"

#include <vector>

#include <QString>
#include <QtSql>
#include <QHash>


class DataLoader
{
public:
    int objectId, recordSize;
    QSqlDatabase db;
    DataRecordTable* dataRecordTable;
    QHash<int, QHash <QString, QVariant> > measurementInfo;


    DataLoader(int objectId, QString server, QString dbName, QString username, QString password);

    void initDataRecordTable();
    void loadMeasurementInfo();
    void loadRecords(unsigned long limit = 0);
    void setAlarmFlagToRecords();
    void removeAllRecords();
    void printRecords();

private:
    std::vector<double> programAlarmIds;
    std::vector<int> recordIds;
    bool performDatabaseConnection();
};

#endif // DATALOADER_H
