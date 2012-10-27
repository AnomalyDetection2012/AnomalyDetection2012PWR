#include "datasetControler.h"

using namespace std;

DatasetControler::DatasetControler(string name, vector<vector<string> > &infoTablesHeaders, vector<string> &dataNames, vector<string> &noninformativeDataNames)
{
	dataset = new Dataset(name, infoTablesHeaders, dataNames, noninformativeDataNames);
}


DatasetControler::~DatasetControler(void)
{
}

int DatasetControler::newInfo(string tableName, int id, vector<string> &infoRecord){
	return dataset->newInfo(tableName,id,infoRecord);
}

int DatasetControler::newRecord(time_t time, vector<double> &data, vector<double> &noninformativeData, vector<int> &infos, bool isAnomaly){
	return dataset->newRecord(time, data, noninformativeData, infos, isAnomaly);
}

void DatasetControler::checkNewData(){
    vector<vector<double> > data = dataset->getUncheckedData();
	vector<bool> results;
	// TODO wysylanie do algorytmu
	dataset->saveResults(results);
}

void DatasetControler::checkAllData(){
    vector<vector<double> > data = dataset->getAllData();
	vector<bool> results;
	// TODO wysylanie do algorytmu
	dataset->setCheckingStartingPoint(0);
	dataset->saveResults(results);
}
