#include "DatasetConnector.h"

using namespace std;

DatasetConnector::DatasetConnector(void)
{
}


DatasetConnector::~DatasetConnector(void)
{
}

void DatasetConnector::createDatasetControler(string name, vector<vector<string> > &infoTablesHeaders, vector<string> &dataNames, vector<string> &noninformativeDataNames){
	datasetControler = new DatasetControler(name, infoTablesHeaders, dataNames, noninformativeDataNames);
}

int DatasetConnector::newInfo(string tableName, int id, vector<string> &infoRecord){
	if(datasetControler != NULL){
		return datasetControler->newInfo(tableName, id, infoRecord);
	}
}

int DatasetConnector::newRecord(time_t time, vector<double> &data, vector<double> &noninformativeData, vector<int> &infos, bool isAnomaly){
	if(datasetControler != NULL){
		return datasetControler->newRecord(time, data, noninformativeData, infos, isAnomaly);
	}
}

void DatasetConnector::checkNewData(){
	if(datasetControler != NULL){
		datasetControler->checkNewData();
	}
}

void DatasetConnector::checkAllData(){
	if(datasetControler != NULL){
		datasetControler->checkAllData();
	}
}
