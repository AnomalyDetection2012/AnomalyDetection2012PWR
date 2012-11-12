#include "datasetControler.h"

using namespace std;

DatasetControler::DatasetControler(string name, vector<vector<string> > &infoTablesHeaders, vector<string> &dataNames, vector<string> &noninformativeDataNames)
{
	dataset = new Dataset(name, infoTablesHeaders, dataNames, noninformativeDataNames);
    lastCheckedId=-1;
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

void DatasetControler::checkNewData(AlgorithmControler *ac){
    int end = dataset->getDataRecordsAmmount();
    checkData(lastCheckedId + 1, end, ac);
}

void DatasetControler::checkAllData(AlgorithmControler *ac){
    int end = dataset->getDataRecordsAmmount();
    checkData(0, end, ac);
}

void DatasetControler::checkData(int begin, int end, AlgorithmControler *ac){
    int max = dataset->getDataRecordsAmmount() - 1;
    if(begin < max){
        if(end > max)
            end = max;
        vector<vector<double> > data = dataset->getData(begin, end);
        vector<bool> results = ac->test(methodId, data, mins, maxs);
        dataset->saveResults(results, begin);
        lastCheckedId = end;
    }
}

void DatasetControler::teachData(int begin, int end, AlgorithmControler *ac){
    vector<vector<double> > data = dataset->getData(begin, end);
    vector<bool> anomalies = dataset->getAnomalies(begin, end);
    ac->learn(methodId, data, anomalies, mins, maxs);
}

void DatasetControler::setMethodId(int id){
    methodId = id;
}

double* DatasetControler::getMinimals(){
    return mins;
}

double* DatasetControler::getMaximals(){
    return maxs;
}

void DatasetControler::setMinMax(double* min, double* max){//TODO TEMPORARY
    mins = min;
    maxs = max;
}
