#include "class_variable.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

#define M_E 2.71828182845904523536
#define M_PI 3.14159265358979323846

int ClassVariable::ALL_RECORDS_COUNT = 0;

ClassVariable::ClassVariable()
{

}

ClassVariable::ClassVariable(bool m_isAnomaly, int m_featuresCount)
{
    isAnomaly = m_isAnomaly;
    featuresCount = m_featuresCount;

    recomputeFlag = true;
    means.assign(featuresCount, 0.0);
    variances.assign(featuresCount, 0.0);
}

int ClassVariable::getFeaturesCount()
{
    return featuresCount;
}

int ClassVariable::getRecordsCountInClass()
{
    return records.size();
}

int ClassVariable::getRecordsCountInTotal()
{
    return ClassVariable::ALL_RECORDS_COUNT;
}

void ClassVariable::addRecord(const std::vector<double> &m_features)
{
    if (featuresCount != m_features.size()) {
        exit(EXIT_FAILURE);
    }

    //

    recomputeFlag = true;
    records.push_back(m_features);
    ClassVariable::ALL_RECORDS_COUNT++;
}

void ClassVariable::mean()
{
    double * results = new double[featuresCount];

    for (int i = 0; i < records.size(); i++) {
        for (int j = 0; j < featuresCount; j++) {
            results[j] += records[i][j];
        }
    }

    for (int i = 0; i < featuresCount; i++) {
        means.at(i) = (results[i] / records.size());
    }

    delete [] results;
}

void ClassVariable::variance()
{
    double * results = new double[featuresCount];

    for (int i = 0; i < records.size(); i++) {
        for (int j = 0; j < featuresCount; j++) {
            results[j] += pow((records[i][j] - means[j]), 2.0);
        }
    }

    for (int i = 0; i < featuresCount; i++) {
        variances.at(i) = (results[i] / (records.size() - 1));
    }

    delete [] results;
}

double ClassVariable::posterior(const std::vector<double> &m_features)
{
    if (featuresCount != m_features.size()) {
        exit(EXIT_FAILURE);
    }

    //

    if (recomputeFlag) {
        recomputeFlag = false;
        mean();
        variance();
    }

    double result = ((double) records.size()) / ((double) ClassVariable::ALL_RECORDS_COUNT);
    double temp = -1.0;
    for (int i = 0; i < featuresCount; i++) {
        temp = pow((m_features[i] - means[i]), 2.0);
        temp /= 2.0 * variances[i];
        temp = pow(M_E, -temp);
        temp *= 1.0 / sqrt(2.0 * M_PI * variances[i]);
        result *= temp;
    }

    return result;
}
