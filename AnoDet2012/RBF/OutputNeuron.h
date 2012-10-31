#pragma once
#include <vector>
#include "HiddenNeuron.h"

class OutputNeuron
{
public:
	double randomWeightScaleFactor, lastOutputValue;
	std::vector<double> weights;
	QList<HiddenNeuron*> connectedNeurons;

	OutputNeuron(double randomWeightScaleFactor);
	OutputNeuron(double randomWeightScaleFactor, QList<HiddenNeuron*>& hiddenLayer);
	~OutputNeuron(void);

	void addHiddenNeuron(HiddenNeuron* hiddenNeuron);
	double calculateOutput();
	void learn(double setpoint, double learnFactor, double sigmoidalActivationFunctionBeta);
	double sigmoidalActivationFunction(double x, double beta);
};

