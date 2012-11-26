#include "datasetControler.h"

using namespace std;

DatasetControler::DatasetControler(string name, vector<vector<string> > &infoTablesHeaders, vector<QString> &dataNames, vector<QString> &noninformativeDataNames)
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

int DatasetControler::newRecord(time_t time, vector<double> &data, bool isAnomaly, vector<double> &noninformativeData, vector<int> &infos){
    return dataset->newRecord(time, data, isAnomaly, noninformativeData, infos);
}

int DatasetControler::newRecord(time_t time, vector<double> &data, bool isAnomaly){
    return dataset->newRecord(time, data, isAnomaly);
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
    if(end > max)
        end = max;
    if(begin < max){
        vector<vector<double> > data = dataset->getData(begin, end);
        vector<bool> results = ac->test(methodId, data, mins, maxs);
        dataset->saveResults(results, begin);
        lastCheckedId = end;
    }
}

void DatasetControler::teachData(int begin, int end, AlgorithmControler *ac){
    vector<vector<double> > data = dataset->getData(begin, end);
    vector<bool> anomalies = dataset->getDatabaseAnomalies(begin, end);
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

void DatasetControler::setMinMaxFromDataset(){
    int size = dataset->dataTable->programPomiarIds.size();
    mins = new double[size];
    maxs = new double[size];
    for(int i=0; i<size; ++i){
        mins[i] = getMinValue(dataset->dataTable->programPomiarIds[i]);
        maxs[i] = getMaxValue(dataset->dataTable->programPomiarIds[i]);
    }
}

QString DatasetControler::getMeasurementName(int type_id)
{
    return this->dataset->dataTable->measurementsInfos->getMeasurementName(type_id);
}

double DatasetControler::getMinValue(int type_id)
{
    return this->dataset->dataTable->measurementsInfos->getMinValue(type_id);
}

double DatasetControler::getMaxValue(int type_id)
{
    return this->dataset->dataTable->measurementsInfos->getMinValue(type_id);
}

QString DatasetControler::getUnit(int type_id)
{
    return this->dataset->dataTable->measurementsInfos->getUnit(type_id);
}

void DatasetControler::setMeasurementsInfo(MeasurementInfo *measurementsInfos)
{
    dataset->setMeasurementsInfo(measurementsInfos);
}

void DatasetControler::setAnomaly(int id, bool isAnomaly){
    dataset->setAnomaly(id, isAnomaly);
}

vector<vector<double> > DatasetControler::getData(int begin, int end){
    return dataset->getData(begin, end);
}

void DatasetControler::setDatabaseAnomaly(int id, bool isAnomaly){
    dataset->setDatabaseAnomaly(id, isAnomaly);
}
