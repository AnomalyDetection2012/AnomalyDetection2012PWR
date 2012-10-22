#ifndef DATARECORD_H
#define DATARECORD_H

#include <vector>

class DataRecord
{
public:
	DataRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
	~DataRecord(void);
	bool isAnomaly;
	time_t time;
	std::vector<double> data;
	std::vector<double> noninformativeData;
	std::vector<int> infoAddress;
};

#endif // DATARECORD_H