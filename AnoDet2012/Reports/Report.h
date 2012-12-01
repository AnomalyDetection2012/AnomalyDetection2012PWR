#ifndef REPORT_H
#define REPORT_H
#include "ReportGenerator.h"
#include "connectortracker.h"
#include <vector>

class Report
{
public:
    Report(QSqlDatabase* dbConnection);

    ReportGenerator* generator;

    ConnectorTracker* ct;
    QSqlDatabase dbConnection;

    void initialiseConnectors(ConnectorTracker* connectorTracker);

    void reportFromDatabase(QString outputAbsolutePath);
    //void reportFromObject(int objectID, QString outputAbsolutePath);
    void reportFromObjectNewRecords(int objectID, QString outputAbsPath);

};

#endif // REPORT_H
