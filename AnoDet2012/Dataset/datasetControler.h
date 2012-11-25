#ifndef DATASETCONTROLER_H
#define DATASETCONTROLER_H
#include "Dataset.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"

class DatasetControler
{
public:
    DatasetControler(std::string name, std::vector<std::vector<std::string> > &infoTablesHeaders, std::vector<QString> &dataNames, std::vector<QString> &noninformativeDataNames);
	~DatasetControler(void);
	Dataset *dataset;
	int newInfo(std::string tableName, int id, std::vector<std::string> &infoRecord);
	int newRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
    void checkNewData(AlgorithmControler *ac);
    void checkAllData(AlgorithmControler *ac);
    void checkData(int begin, int end, AlgorithmControler *ac);
    void teachData(int begin, int end, AlgorithmControler *ac);
    void setMethodId(int id);
    double* getMinimals();
    double* getMaximals();
    void setMinMax(double* min, double* max);
    void setMeasurementsInfo(MeasurementInfo *measurementsInfos);
    void setAnomaly(int id, bool isAnomaly);
    QString getMeasurementName(int type_id);
    QString getUnit(int type_id);
    double getMinValue(int type_id);
    double getMaxValue(int type_id);
    std::vector<std::vector<double> > getData(int begin, int end);

private:
    int methodId;
    int lastCheckedId;
    double* mins;
    double* maxs;
};

#endif // DATASETCONTROLER_H
