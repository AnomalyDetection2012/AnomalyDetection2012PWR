#include "DataRecordTable.h"

using namespace std;

DataRecordTable::DataRecordTable(vector<string> &dataNames, vector<string> &noninformativeDataNames, vector<InfoTable> &infoTables)
{
	this->dataNames = dataNames;
	this->noninformativeDataNames = noninformativeDataNames;
    this->infoTables = infoTables;
}


DataRecordTable::~DataRecordTable(void)
{
}

int DataRecordTable::getLength(){
    return records.size();
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
}

vector<vector<double> > DataRecordTable::getData(int begin, int end){
    vector<vector<double> > result;
    vector<DataRecord>::iterator iter;
    for(iter = records.begin()+begin; iter != records.end() && end>0; ++iter){
        result.push_back(iter->data);
        --end;
    }
    return result;
}

vector<bool> DataRecordTable::getAnomalies(int begin, int end){
    vector<bool> result;
    vector<DataRecord>::iterator iter;
    for(iter = records.begin()+begin; iter != records.end() && end>0; ++iter){
        result.push_back(iter->isAnomaly);
        --end;
    }
    return result;
}

void DataRecordTable::saveResults(vector<bool> &anomalies, int begin){
    vector<DataRecord>::iterator iter;
    unsigned int n=0;
    for(iter = records.begin()+begin; iter != records.end() && n<anomalies.size(); ++iter){
        iter->isAnomaly = anomalies.at(n);
        ++n;
    }
}

void DataRecordTable::setMeasurementsInfo(MeasurementInfo *measurementsInfos)
{
    this->measurementsInfos = measurementsInfos;
}
