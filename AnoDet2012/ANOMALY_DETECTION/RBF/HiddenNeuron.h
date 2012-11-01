#pragma once
#include "Cluster.h"
#include <vector>
class HiddenNeuron
{
public:
	double lastCalculatedRadialFunction;
	Cluster* cluster;
	std::vector<double>* input;
	HiddenNeuron(Cluster* cluster, std::vector<double>* input);
	~HiddenNeuron(void);
	virtual double calculateRadialFunction();
};

