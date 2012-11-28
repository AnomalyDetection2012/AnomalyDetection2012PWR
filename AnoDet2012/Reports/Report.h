#ifndef REPORT_H
#define REPORT_H
#include "ReportGenerator.h"
#include "connectortracker.h"
#include <vector>

class Report
{
public:
    Report();

    ReportGenerator* generator;

    ConnectorTracker* ct;

    void reportFromDatabase(QString outputAbsolutePath);
    //void reportFromObject(int objectID, QString outputAbsolutePath);
    void reportFromObjectNewRecords(int objectID, QString outputAbsPath);

};

#endif // REPORT_H
