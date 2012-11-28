#include "Dataset.h"

using namespace std;

Dataset::Dataset(string name, vector<vector<string> > &infoTablesHeaders, vector<QString> &dataNames, vector<QString> &noninformativeDataNames)
{
	this->name = name;
    vector<vector<string> >::iterator headersIter;
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

int Dataset::getDataRecordsAmmount(){
    return dataTable->getLength();
}

int Dataset::newInfo(string tableName, int id, vector<string> &infoRecord){
	vector<InfoTable>::iterator iter;
	for(iter = infoTables.begin(); iter != infoTables.end(); ++iter){
		if(iter->tableName.compare(tableName) == 0){
			return iter->add(id, infoRecord);
		}
	}
    return NULL;
}

int Dataset::newRecord(QDateTime time, vector<double> &data, bool isAnomaly, vector<double> &noninformativeData, vector<int> &infos){
    return dataTable->addRecord(time, data, isAnomaly, noninformativeData, infos);
}

int Dataset::newRecord(QDateTime time, vector<double> &data, bool isAnomaly){
    return dataTable->addRecord(time, data, isAnomaly);
}

vector<vector<double> > Dataset::getData(int begin, int end){
    return dataTable->getData(begin, end);
}

vector<bool> Dataset::getAnomalies(int begin, int end){
    return dataTable->getAnomalies(begin, end);
}

vector<bool> Dataset::getDatabaseAnomalies(int begin, int end){
    return dataTable->getDatabaseAnomalies(begin, end);
}

void Dataset::saveResults(vector<bool> &anomalies, int begin){
    dataTable->saveResults(anomalies, begin);
}

void Dataset::setMeasurementsInfo(MeasurementInfo *measurementsInfos)
{
    dataTable->setMeasurementsInfo(measurementsInfos);
}

void Dataset::setAnomaly(int id, bool isAnomaly){
    dataTable->setAnomaly(id, isAnomaly);
}

void Dataset::setDatabaseAnomaly(int id, bool isAnomaly){
    dataTable->setDatabaseAnomaly(id, isAnomaly);
}
