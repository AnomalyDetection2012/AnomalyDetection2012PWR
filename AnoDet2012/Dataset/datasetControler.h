#ifndef DATASETCONTROLER_H
#define DATASETCONTROLER_H
#include "Dataset.h"

class DatasetControler
{
public:
    DatasetControler(std::string name, std::vector<std::vector<std::string> > &infoTablesHeaders, std::vector<std::string> &dataNames, std::vector<std::string> &noninformativeDataNames);
	~DatasetControler(void);
	Dataset *dataset;
	int newInfo(std::string tableName, int id, std::vector<std::string> &infoRecord);
	int newRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
	void checkNewData();
	void checkAllData();
};

#endif // DATASETCONTROLER_H
