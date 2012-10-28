#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include "space.h"
#include "neighbourdetection.h"
#include <vector>
#include <string>

struct variable
{
    std::vector<double> features;
    bool isAnomaly;
};

struct distanceN
{
    variable var;
    double dst;

    bool operator ()(const distanceN &d1, const distanceN &d2) { return (d1.dst < d2.dst); }
};

class NearestNeighbor : public NeighbourDetection
{

private:
    int featuresCount;
    int k;
    double (*spaceFunction)(const std::vector<double> &v1, const std::vector<double> &v2);

    std::vector<variable> variables;

    //

    void train(const std::vector<double> &m_features, bool m_isAnomaly);

    bool classify(const std::vector<double> &m_features, bool m_appendData);

public:
    NearestNeighbor(
                    int m_featuresCount,
                    int m_k,
                    double (*m_spaceFunction)(const std::vector<double> &v1, const std::vector<double> &v2) = euclideanSpace
                   );

    void learn(std::vector< std::vector<double> > &set, std::vector<bool> &target);

    std::vector<bool> test(std::vector< std::vector<double> > &set);

};

#endif // NEAREST_NEIGHBOR_H
