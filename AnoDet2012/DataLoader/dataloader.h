#ifndef DATALOADER_H
#define DATALOADER_H

#include "../Dataset/DataRecord.h"
#include "../Dataset/DataRecordTable.h"

#include <vector>

#include <QString>
#include <QtSql>


class DataLoader
{
public:
    int objectId, recordSize;
    QSqlDatabase db;
    DataRecordTable* dataRecordTable;

    DataLoader(int objectId, QString server, QString dbName, QString username, QString password);

    void initDataRecordTable();
    void loadRecords(unsigned long limit = 0);
    void printRecords();

private:
    std::vector<double> programAlarmIds;
    bool performDatabaseConnection();
};

#endif // DATALOADER_H
