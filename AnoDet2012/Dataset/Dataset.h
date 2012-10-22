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
	int newInfo(std::string tableName, std::vector<std::string> &infoRecord);
	int newRecord(); // TODO w jaki sposob beda przychodzic dane o powiazaniach pomiar - informacje
	std::vector<std::vector<double>> getUncheckedData();
	void saveResults(std::vector<bool> &anomalies);
};

