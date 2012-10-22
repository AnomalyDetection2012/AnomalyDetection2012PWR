#include "nearest_neighbor.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <map>

NearestNeighbor::NearestNeighbor(int m_featuresCount, int m_k, double (*m_spaceFunction)(const std::vector<double> &v1, const std::vector<double> &v2))
{
    featuresCount = m_featuresCount;
    k = m_k;
    spaceFunction = m_spaceFunction;
}

void NearestNeighbor::train(const std::vector<double> &m_features, bool m_isAnomaly)
{
    if (featuresCount != m_features.size()) {
        exit(EXIT_FAILURE);
    }

    //

    variable var = {m_features, m_isAnomaly};
    variables.push_back(var);
}

bool NearestNeighbor::classify(const std::vector<double> &m_features, bool m_appendData)
{
    if (featuresCount != m_features.size()) {
        exit(EXIT_FAILURE);
    }

    //

    std::vector<distanceN> distances;
    distanceN comparator;
    for (int i = 0; i < variables.size(); i++) {
        distanceN tmp;
        tmp.var = variables[i];
        tmp.dst = (*spaceFunction)(m_features, variables[i].features);
        distances.push_back(tmp);
    }
    std::sort(distances.begin(), distances.end(), comparator);

    int invalidDataCount = 0;
    int validDataCount = 0;
    for (int i = 0; i < k; i++) {
        if (distances[i].var.isAnomaly) {
            invalidDataCount++;
        } else {
            validDataCount++;
        }
    }

    if (m_appendData) train(m_features, (invalidDataCount > validDataCount));

    return (invalidDataCount > validDataCount);
}

void NearestNeighbor::learn(std::vector<std::vector<double> > &set, std::vector<bool> &target)
{
    for (int i = 0; i < set.size(); i++) train(set[i], target[i]);
}

std::vector<bool> NearestNeighbor::test(std::vector<std::vector<double> > &set)
{
    std::vector<bool> results;
    for (int i = 0; i < set.size(); i++) results.push_back(classify(set[i], false));
    return results;
}
