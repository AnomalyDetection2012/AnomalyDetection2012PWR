#ifndef SOMNETWORK_H
#define SOMNETWORK_H
#include "neuron.h"
#include "topologymap.h"
#include <vector>
#include "method.h"

class SOMNetwork : public Method
{
public:
    SOMNetwork(int width, int height, int inputs,double maxRad, double maxAlpha,double maxIter, TopologyMap &m);
    int findWinner(std::vector<double> inputs);
    void correctWeights(std::vector<double> inputs);
    void learn(std::vector<std::vector<double> > inputs);
    void findAssignments(std::vector<std::vector<double> > inputs, QStringList names);
    double getQuantizationError(std::vector<double> inputs);
    void calcQuantizationError(std::vector<std::vector<double> > inputs);

    void learn(vector< vector<double> > &set, vector<bool> &target);  // override Method::learn()
    vector<bool> test(vector< vector<double> > &set);  // override Method::test()


private:
    int cols;
    int rows;
    int numInputs;
    double Rmax;
    double R;
    double alphamax;
    double alpha;
    int t;
    int tmax;



    int numNeurons;
    TopologyMap *map;
    std::vector<Neuron> neurons;

public:
    std::vector<std::vector<QString> > assignments;
    double minQuantizationError;
    double maxQuantizationError;
    double avgQuantizationError;


};

#endif // SOMNETWORK_H
