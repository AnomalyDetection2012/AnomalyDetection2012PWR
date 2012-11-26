#ifndef DATARECORD_H
#define DATARECORD_H

#include <vector>
#include <QDateTime>

class DataRecord
{
public:
    DataRecord(time_t time, std::vector<double> &data, bool isAnomaly, std::vector<double> &noninformativeData, std::vector<int> &infos);
    DataRecord(time_t time, std::vector<double> &data, bool isAnomaly = false);
	~DataRecord(void);
	bool isAnomaly;
    bool isDatabaseAnomaly;
	time_t time;
	std::vector<double> data;
	std::vector<double> noninformativeData;
	std::vector<int> infoAddress;
};

#endif // DATARECORD_H
