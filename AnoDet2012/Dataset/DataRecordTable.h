#ifndef DATARECORDTABLE_H
#define DATARECORDTABLE_H
#include "DataRecord.h"
#include "InfoTable.h"
#include "MeasurementInfo.h"
#include <QPair>

class DataRecordTable
{
public:
    DataRecordTable(std::vector<QString> &dataNames, std::vector<QString> &noninformativeDataNames, std::vector<InfoTable> &infoTables);
	~DataRecordTable(void);
    std::vector<QString> dataNames;
    std::vector<int> programPomiarIds;
    std::vector<QString> noninformativeDataNames;
    std::vector<InfoTable> infoTables;
    std::vector<DataRecord> records;
    MeasurementInfo *measurementsInfos;
    int getLength();
    QPair<int,int> getIndexRecordInterval(QDateTime begin, QDateTime end);
    QPair<QDateTime,QDateTime> getDateTimeRecordInterval(int begin, int end);
    int addRecord(QDateTime time, std::vector<double> &data, bool isAnomaly, std::vector<double> &noninformativeData, std::vector<int> &infos);
    int addRecord(QDateTime time, std::vector<double> &data, bool isAnomaly = false);
    void deleteBefore(QDateTime time);
    std::vector<std::vector<double> > getData(int begin, int end);
    std::vector<bool> getAnomalies(int begin, int end);
    std::vector<bool> getDatabaseAnomalies(int begin, int end);
    void saveResults(std::vector<bool> &anomalies, int begin);
    void setMeasurementsInfo(MeasurementInfo *measurementsInfos);
    void setAnomaly(int id, bool isAnomaly);
    void setDatabaseAnomaly(int id, bool isAnomaly);
    void setPomiarIds(std::vector<int> ids);
};

#endif // DATARECORDTABLE_H

