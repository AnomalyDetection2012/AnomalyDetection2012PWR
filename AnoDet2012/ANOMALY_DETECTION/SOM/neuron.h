#ifndef NEURON_H
#define NEURON_H
#include <QVector>

class Neuron
{
public:
    Neuron(int idx, int inputs, double x, double y);
    double calcEuclidDistance(std::vector<double> inputs);
    double calcEuclidDistanceOnMap(Neuron neuron);

public:
    int index;
    int numInputs;
    double Xpos;
    double Ypos;
    std::vector<double> weights;


};

#endif // NEURON_H
