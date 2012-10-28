#ifndef DENSITYMETHOD_H
#define DENSITYMETHOD_H

#include "QList"
#include "neighbor.h"
#include "densitydetection.h"

class DensityMethod : public DensityDetection
{
private:
    QList<QList<double> > dataSet;
    int dataSetSize,dimensions;
    void kNearestNeighbors(QList<Neighbor> &retVal, int point, int neighborsNumber);
    double twoPointsDistance(int first, int second);
    void insertNeighbor(QList<Neighbor> &n, Neighbor newN, int lastIndex);
    double lrd(int pIndex, int k);
    void init(int dimensions);

public:
    double LOFResultDeviation;
    int neighbors;
    DensityMethod(double LOFResultDeviation, int neighbors);
    double LOF(int pIndex, int k);
    int getDataSetSize();
    virtual void learn(vector< vector<double> > &set, vector<bool> &target);
    virtual vector<bool> test(vector< vector<double> > &set);
};

#endif // DENSITYMETHOD_H
