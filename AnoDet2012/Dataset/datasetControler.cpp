#include "datasetControler.h"

using namespace std;

DatasetControler::DatasetControler(string name, vector<vector<string>> &infoTablesHeaders, vector<string> &dataNames, vector<string> &noninformativeDataNames)
{
	dataset = new Dataset(name, infoTablesHeaders, dataNames, noninformativeDataNames);
}


DatasetControler::~DatasetControler(void)
{
}

int DatasetControler::newInfo(string tableName, vector<string> &infoRecord){
	dataset->newInfo(tableName,infoRecord);
}

void DatasetControler::checkNewData(){
	vector<vector<double>> data = dataset->getUncheckedData();
	vector<bool> results;
	// TODO wysylanie do algorytmu
	dataset->saveResults(results);
}