#ifndef CLASS_VARIABLE_H
#define CLASS_VARIABLE_H

#include <vector>
#include <string>

class ClassVariable
{

private:
    static int ALL_RECORDS_COUNT;

    int featuresCount;

    bool recomputeFlag;
    std::vector< std::vector<double> > records;
    std::vector<double> means;
    std::vector<double> variances;

    //

    void mean();

    void variance();

public:
    bool isAnomaly;

    //

    ClassVariable();

    ClassVariable(bool m_isAnomaly, int m_featuresCount);

    int getFeaturesCount();

    int getRecordsCountInClass();

    int getRecordsCountInTotal();

    void addRecord(const std::vector<double> &m_features);

    double posterior(const std::vector<double> &m_features);

};

#endif // CLASS_VARIABLE_H
