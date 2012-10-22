#include "Dataset.h"

class DatasetControler
{
public:
	DatasetControler(std::string name, std::vector<std::vector<std::string>> &infoTablesHeaders, std::vector<std::string> &dataNames, std::vector<std::string> &noninformativeDataNames);
	~DatasetControler(void);
	Dataset *dataset;
	int newInfo(std::string tableName, std::vector<std::string> &infoRecord);
	int newRecord(); // TODO w jaki sposob beda przychodzic dane o powiazaniach pomiar - informacje
	void checkNewData();
};

