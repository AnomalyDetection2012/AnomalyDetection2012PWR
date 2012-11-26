#ifndef REPORT_H
#define REPORT_H
#include "ReportGenerator.h"
#include <vector>

class Report
{
public:
    Report();

    ReportGenerator* generator;
    QString outputAbsolutePath;

    void reportFromDatabase(QString outputAbsolutePath);
    //void reportFromObject(int objectID, QString outputAbsolutePath);
    void reportFromObjectNewRecords(int objectID, QString outputAbsPath);

};

#endif // REPORT_H
