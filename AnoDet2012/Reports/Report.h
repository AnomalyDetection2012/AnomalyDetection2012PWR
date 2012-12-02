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
    void reportFromDataRange(int begin, int end, QString objectName, QString outputAbsolutePath);

};

#endif // REPORT_H
