#include "DataRecord.h"

using namespace std;

DataRecord::DataRecord(time_t time, vector<double> &data, bool isAnomaly, vector<double> &noninformativeData, vector<int> &infos)
{
	this->time = time;
	this->data = data;
	this->noninformativeData = noninformativeData;
    this->infoAddress = infos;
    this->isAnomaly = isAnomaly;
    this->isDatabaseAnomaly = isAnomaly;
}

DataRecord::DataRecord(time_t time, vector<double> &data, bool isAnomaly)
{
    this->time = time;
    this->data = data;
    this->isAnomaly = false;
    this->isDatabaseAnomaly = isAnomaly;
}

DataRecord::~DataRecord(void)
{
}
