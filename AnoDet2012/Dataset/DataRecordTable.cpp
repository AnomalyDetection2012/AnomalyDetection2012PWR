#include "DataRecordTable.h"

using namespace std;

DataRecordTable::DataRecordTable(vector<string> &dataNames, vector<string> &noninformativeDataNames, vector<InfoTable> &infoTables)
{
	this->dataNames = dataNames;
	this->noninformativeDataNames = noninformativeDataNames;
	this->infoTables = infoTables;
	this->lastChecked = 0;
}


DataRecordTable::~DataRecordTable(void)
{
}

int DataRecordTable::addRecord(time_t time, vector<double> &data, vector<double> &noninformativeData, vector<int> &infos, bool isAnomaly){
	records.push_back(*(new DataRecord(time, data, noninformativeData, infos, isAnomaly)));
	return records.size()-1;
}

void DataRecordTable::deleteBefore(time_t time){ // TODO co jesli nie sa posortowane wzgledem daty?
	vector<DataRecord>::iterator iter;
	int n=0;
	for(iter = records.begin(); iter != records.end() && iter->time <= time; ++iter){
		++n;
	}
	records.erase(records.begin(),iter);
	lastChecked -= n;
}

vector<vector<double> > DataRecordTable::getUncheckedRecordsData(){
    vector<vector<double> > result;
	vector<DataRecord>::iterator iter;
	for(iter = records.begin()+lastChecked; iter != records.end(); ++iter){
		result.push_back(iter->data);
	}
	return result;
}

vector<vector<double> > DataRecordTable::getAllRecordsData(){
    vector<vector<double> > result;
	vector<DataRecord>::iterator iter;
	for(iter = records.begin(); iter != records.end(); ++iter){
		result.push_back(iter->data);
	}
	return result;
}

void DataRecordTable::saveResults(vector<bool> &anomalies){
	vector<DataRecord>::iterator iter;
    unsigned n=0;
	for(iter = records.begin()+lastChecked; iter != records.end() && n<anomalies.size(); ++iter){
		iter->isAnomaly = anomalies.at(n);
		++n;
	}
	lastChecked += n;
}
