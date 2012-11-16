#include "DatasetConnector.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"

using namespace std;

DatasetConnector::DatasetConnector(void)
{
}


DatasetConnector::~DatasetConnector(void)
{
}

void DatasetConnector::initialiseConnectors(ConnectorTracker *con){
    anomalyDetection = con->anomalyDetection;
}

void DatasetConnector::createDatasetControler(string name, vector<vector<string> > &infoTablesHeaders, vector<string> &dataNames, vector<string> &noninformativeDataNames){
	datasetControler = new DatasetControler(name, infoTablesHeaders, dataNames, noninformativeDataNames);
}

int DatasetConnector::newInfo(string tableName, int id, vector<string> &infoRecord){
	if(datasetControler != NULL){
		return datasetControler->newInfo(tableName, id, infoRecord);
	}
    return NULL;
}

int DatasetConnector::newRecord(time_t time, vector<double> &data, vector<double> &noninformativeData, vector<int> &infos, bool isAnomaly){
	if(datasetControler != NULL){
		return datasetControler->newRecord(time, data, noninformativeData, infos, isAnomaly);
	}
    return NULL;
}

void DatasetConnector::checkNewData(){
	if(datasetControler != NULL){
        datasetControler->checkNewData(anomalyDetection);
	}
}

void DatasetConnector::checkAllData(){
	if(datasetControler != NULL){
        datasetControler->checkAllData(anomalyDetection);
	}
}

void DatasetConnector::checkData(int begin, int end){
    if(datasetControler != NULL){
        datasetControler->checkData(begin, end, anomalyDetection);
    }
}

void DatasetConnector::teachData(int begin, int end){
    if(datasetControler != NULL){
        datasetControler->teachData(begin, end, anomalyDetection);
    }
}

void DatasetConnector::setMethodId(int id){
    if(datasetControler != NULL){
        datasetControler->setMethodId(id);
    }
}

void DatasetConnector::setMinMax(double* min, double* max){//TODO TEMPORARY
    if(datasetControler != NULL){
        datasetControler->setMinMax(min, max);
    }
}

QString DatasetConnector::getMeasurementName(int type_id)
{
    if(datasetControler != NULL)
    {
        return datasetControler->getMeasurementName(type_id);
    }
    return NULL;
}

double DatasetConnector::getMinValue(int type_id)
{
    if(datasetControler != NULL)
    {
        return datasetControler->getMinValue(type_id);
    }
    return NULL;
}

double DatasetConnector::getMaxValue(int type_id)
{
    if(datasetControler != NULL)
    {
        return datasetControler->getMaxValue(type_id);
    }
    return NULL;
}

void DatasetConnector::setMeasurementsInfo(MeasurementInfo *measurementsInfos)
{
    if(datasetControler != NULL){
        datasetControler->setMeasurementsInfo(measurementsInfos);
    }
}

void DatasetConnector::setAnomaly(int id, bool isAnomaly){
    if(datasetControler != NULL){
        datasetControler->setAnomaly(id, isAnomaly);
    }
}
