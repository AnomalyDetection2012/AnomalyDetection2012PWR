#include "Dataset.h"

using namespace std;

Dataset::Dataset(string name, vector<vector<string>> &infoTablesHeaders, vector<string> &dataNames, vector<string> &noninformativeDataNames)
{
	this->name = name;
	vector<vector<string>>::iterator headersIter;
	for(headersIter = infoTablesHeaders.begin(); headersIter != infoTablesHeaders.end(); ++headersIter){
		vector<string> headers;
		vector<string>::iterator iter;
		for(iter = (headersIter->begin())+1;iter != headersIter->end(); ++iter){
			headers.push_back(*iter);
		}
		string title = *(headersIter->begin());
		infoTables.push_back(*(new InfoTable(title,headers)));
	}
	dataTable = new DataRecordTable(dataNames, noninformativeDataNames, infoTables);
}


Dataset::~Dataset(void)
{
}

int Dataset::newInfo(string tableName, vector<string> &infoRecord){
	vector<InfoTable>::iterator iter;
	for(iter = infoTables.begin(); iter != infoTables.end(); ++iter){
		if(iter->tableName.compare(tableName) == 0){
			iter->add(infoRecord);
			break;
		}
	}
}

vector<vector<double>> Dataset::getUncheckedData(){
	return dataTable->getUncheckedRecordsData();
}

void Dataset::saveResults(vector<bool> &anomalies){
	dataTable->saveAnomalies(anomalies);
}