#include "datasetControler.h"

class DatasetConnector
{
public:
	DatasetConnector(void);
	~DatasetConnector(void);
	DatasetControler *datasetControler;
	void createDatasetControler(std::string name, std::vector<std::vector<std::string>> &infoTablesHeaders, std::vector<std::string> &dataNames, std::vector<std::string> &noninformativeDataNames);
	int newInfo(std::string tableName, int id, std::vector<std::string> &infoRecord);
	int newRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
	void checkNewData();
	void checkAllData();
};

