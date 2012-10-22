#include "DataRecord.h"

using namespace std;

DataRecord::DataRecord(time_t time, vector<double> &data, vector<double> &noninformativeData, vector<int> &infos, bool isAnomaly)
{
	this->time = time;
	this->data = data;
	this->noninformativeData = noninformativeData;
	this->infoAddress = infos;
	this->isAnomaly = isAnomaly;
}


DataRecord::~DataRecord(void)
{
}
