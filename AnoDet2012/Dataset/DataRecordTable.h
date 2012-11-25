#ifndef DATARECORDTABLE_H
#define DATARECORDTABLE_H
#include "DataRecord.h"
#include "InfoTable.h"
#include "MeasurementInfo.h"

class DataRecordTable
{
public:
    DataRecordTable(std::vector<QString> &dataNames, std::vector<QString> &noninformativeDataNames, std::vector<InfoTable> &infoTables);
	~DataRecordTable(void);
    std::vector<QString> dataNames;
    std::vector<QString> noninformativeDataNames;
    std::vector<InfoTable> infoTables;
    std::vector<DataRecord> records;
    MeasurementInfo *measurementsInfos;
    int getLength();
	int addRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
    void deleteBefore(time_t time);
    std::vector<std::vector<double> > getData(int begin, int end);
    std::vector<bool> getAnomalies(int begin, int end);
    void saveResults(std::vector<bool> &anomalies, int begin);
    void setMeasurementsInfo(MeasurementInfo *measurementsInfos);
    void setAnomaly(int id, bool isAnomaly);
    void setDatabaseAnomaly(int id, bool isAnomaly);

};

#endif // DATARECORDTABLE_H

