#ifndef DATASETCONNECTOR_H
#define DATASETCONNECTOR_H
#include "datasetControler.h"
#include "connectortracker.h"

class DatasetConnector
{
public:
	DatasetConnector(void);
    ~DatasetConnector(void);
    DatasetControler *datasetControler;
    void initialiseConnectors(ConnectorTracker *con);
    void createDatasetControler(std::string name, std::vector<std::vector<std::string> > &infoTablesHeaders, std::vector<QString> &dataNames, std::vector<QString> &noninformativeDataNames);
	int newInfo(std::string tableName, int id, std::vector<std::string> &infoRecord);
	int newRecord(time_t time, std::vector<double> &data, std::vector<double> &noninformativeData, std::vector<int> &infos, bool isAnomaly);
    void checkNewData();
    void checkAllData();
    void checkData(int begin, int end);
    void teachData(int begin, int end);
    void setMethodId(int id);
    void setMinMax(double *min, double *max);
    double* getMinimals();
    double* getMaximals();
    void setMeasurementsInfo(MeasurementInfo *measurementsInfos);
    QString getMeasurementName(int type_id);
    QString getUnit(int type_id);
    double getMinValue(int type_id);
    double getMaxValue(int type_id);
    void setAnomaly(int id, bool isAnomaly);
    std::vector<std::vector<double> > getData(int begin, int end);

private:
    AlgorithmControler *anomalyDetection;
    int methodId;
};

#endif // DATASETCONNECTOR_H
