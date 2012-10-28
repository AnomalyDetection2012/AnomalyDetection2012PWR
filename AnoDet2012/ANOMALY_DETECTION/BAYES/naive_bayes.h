#ifndef NAIVE_BAYES_H
#define NAIVE_BAYES_H

#include "class_variable.h"
#include <vector>
#include <string>
#include "bayesdetection.h"

class NaiveBayes : public BayesDetection
{

private:
    std::vector<ClassVariable> classVariables;

    //

    void train(const std::vector<double> &m_features, bool m_isAnomaly);

    bool classify(const std::vector<double> &m_features, bool m_appendData);

public:
    NaiveBayes(int m_featuresCount);

    void learn(std::vector< std::vector<double> > &set, std::vector<bool> &target);

    std::vector<bool> test(std::vector< std::vector<double> > &set);

};

#endif // NAIVE_BAYES_H
