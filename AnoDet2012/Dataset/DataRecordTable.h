#ifndef DATARECORDTABLE_H
#define DATARECORDTABLE_H
#include "DataRecord.h"
#include "InfoTable.h"

class DataRecordTable
{
public:
	DataRecordTable(std::vector<std::string> &dataNames, std::vector<std::string> &noninformativeDataNames, std::vector<InfoTable> &infoTables);
	~DataRecordTable(void);
	std::vector<std::string> dataNames;
	std::vector<std::string> noninformativeDataNames;
	std::vector<InfoTable> infoTables;
	std::vector<DataRecord> records;
	int lastChecked;
	int addRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
	void deleteBefore(time_t time);
    std::vector<std::vector<double> > getUncheckedRecordsData();
    std::vector<std::vector<double> > getAllRecordsData();
	void saveResults(std::vector<bool> &anomalies);
};

#endif // DATARECORDTABLE_H

