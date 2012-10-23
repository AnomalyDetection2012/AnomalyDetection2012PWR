#include "InfoTable.h"
#include "DataRecordTable.h"

class Dataset
{
public:
/*
	infoTablesHeaders - naglowki kolumn dla tabel informacyjnych (min. 2 - jako pierwsza nazwa tabeli)
	dataNames - nazwy pomiarow bioracych udzial w wykrywaniu anomalii
	noninformativeDataNames - nazwy pomiarow nie bioracych udzialu w wykrywaniu anomalii
*/
	Dataset(std::string name, std::vector<std::vector<std::string>> &infoTablesHeaders, std::vector<std::string> &dataNames, std::vector<std::string> &noninformativeDataNames);
	~Dataset(void);
	std::string name;
	std::vector<InfoTable> infoTables;
	DataRecordTable *dataTable;
	int newInfo(std::string tableName, int id, std::vector<std::string> &infoRecord);
	int newRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
	std::vector<std::vector<double>> getUncheckedData();
	std::vector<std::vector<double>> getAllData();
	void setCheckingStartingPoint(int x);
	void saveResults(std::vector<bool> &anomalies);
};

