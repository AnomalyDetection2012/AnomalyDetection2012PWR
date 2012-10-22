#include "neuron.h"
#include <cmath>
#include <QTime>
#include<stdio.h>

Neuron::Neuron(int idx, int inputs, double x, double y) :
    index(idx), numInputs(inputs), Xpos(x), Ypos(y)
{
    for(int i=0;i<numInputs;i++)
    {
        //this->weights.push_back(qrand() / static_cast<double>( RAND_MAX ));
        this->weights.push_back((qrand()%255) / (double)255);
    }

}

double Neuron::calcEuclidDistance(std::vector<double> inputs)
{
    double distance = 0.0;

    for (int i=0; i<numInputs; ++i)
    {
        distance += (inputs.at(i) - weights.at(i)) * (inputs.at(i) - weights.at(i));
    }


    return sqrt(distance);
}

double Neuron::calcEuclidDistanceOnMap(Neuron neuron)
{
    double distance = 0.0;

    distance += (this->Xpos - neuron.Xpos)*(this->Xpos - neuron.Xpos) + (this->Ypos - neuron.Ypos)*(this->Ypos - neuron.Ypos);

    return sqrt(distance);

}

