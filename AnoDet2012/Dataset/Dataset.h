#ifndef DATASET_H
#define DATASET_H
#include "InfoTable.h"
#include "DataRecordTable.h"
#include <iostream>

class Dataset
{
public:
/*
	infoTablesHeaders - naglowki kolumn dla tabel informacyjnych (min. 2 - jako pierwsza nazwa tabeli)
	dataNames - nazwy pomiarow bioracych udzial w wykrywaniu anomalii
	noninformativeDataNames - nazwy pomiarow nie bioracych udzialu w wykrywaniu anomalii
*/
    Dataset(std::string name, std::vector<std::vector<std::string> > &infoTablesHeaders, std::vector<QString> &dataNames, std::vector<QString> &noninformativeDataNames);
	~Dataset(void);
	std::string name;
	std::vector<InfoTable> infoTables;
	DataRecordTable *dataTable;
    int getDataRecordsAmmount();
	int newInfo(std::string tableName, int id, std::vector<std::string> &infoRecord);
	int newRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
    std::vector<std::vector<double> > getData(int begin, int end);
    std::vector<bool> getAnomalies(int begin, int end);
    void saveResults(std::vector<bool> &anomalies, int begin);
    void setMeasurementsInfo(MeasurementInfo *measurementsInfos);
    void setAnomaly(int id, bool isAnomaly);
    void setDatabaseAnomaly(int id, bool isAnomaly);
};

#endif // DATASET_H
