#include "DataRecordTable.h"

using namespace std;

DataRecordTable::DataRecordTable(vector<QString> &dataNames, vector<QString> &noninformativeDataNames, vector<InfoTable> &infoTables)
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

int DataRecordTable::addRecord(QDateTime time, vector<double> &data, bool isAnomaly, vector<double> &noninformativeData, vector<int> &infos){
    records.push_back(*(new DataRecord(time, data, isAnomaly, noninformativeData, infos)));
	return records.size()-1;
}

int DataRecordTable::addRecord(QDateTime time, vector<double> &data, bool isAnomaly){
    records.push_back(*(new DataRecord(time, data, isAnomaly)));
    return records.size()-1;
}

void DataRecordTable::deleteBefore(QDateTime time){ // TODO co jesli nie sa posortowane wzgledem daty?
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
    end-=begin;
    for(iter = records.begin()+begin; iter != records.end() && end>=0; ++iter){
        result.push_back(iter->data);
        --end;
    }
    return result;
}

vector<bool> DataRecordTable::getAnomalies(int begin, int end){
    vector<bool> result;
    vector<DataRecord>::iterator iter;
    end-=begin;
    for(iter = records.begin()+begin; iter != records.end() && end>=0; ++iter){
        result.push_back(iter->isAnomaly);
        --end;
    }
    return result;
}

vector<bool> DataRecordTable::getDatabaseAnomalies(int begin, int end){
    vector<bool> result;
    vector<DataRecord>::iterator iter;
    end-=begin;
    for(iter = records.begin()+begin; iter != records.end() && end>=0; ++iter){
        result.push_back(iter->isDatabaseAnomaly);
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

void DataRecordTable::setAnomaly(int id, bool isAnomaly){
    records[id].isAnomaly = isAnomaly;
}

void DataRecordTable::setDatabaseAnomaly(int id, bool isAnomaly){
    records[id].isDatabaseAnomaly = isAnomaly;
}

void DataRecordTable::setPomiarIds(std::vector<int> ids){
    this->programPomiarIds = ids;
}
