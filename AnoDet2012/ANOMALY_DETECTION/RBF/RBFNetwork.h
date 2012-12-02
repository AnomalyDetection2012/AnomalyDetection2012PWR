#pragma once
#include "KMeans.h"
#include "OutputNeuron.h"
#include "HiddenNeuron.h"
#include "../method.h"
#include <vector>

class RBFNetwork : public Method
{
public:
    double learnFactor, randomWeightScaleFactor, kmeansMinSigmaSquare, kmeansMaxSigmaSquare, kmeansStepImprovement, sigmoidalActivationFunctionBeta, quantizationErrorThreshold;
    int generations, pointSize, kmeansIterations, clustersNumber;
	KMeans* kmeans;
	std::vector<double>* inputLayer;
	QList<HiddenNeuron*> hiddenLayer;
	QList<OutputNeuron*> outputLayer;
    std::vector<std::vector<double> >& neighbours;

    RBFNetwork(double learnFactor, double sigmoidalActivationFunctionBeta, double quantizationErrorThreshold, int generations, double randomWeightScaleFactor, int clustersNumber, int pointSize, double kmeansMinSigmaSquare, double kmeansMaxSigmaSquare, double kmeansStepImprovement, int kmeansIterations, std::vector<std::vector<double> >& neighbours);
    RBFNetwork(double learnFactor, double sigmoidalActivationFunctionBeta, double quantizationErrorThreshold, int generations, double randomWeightScaleFactor, int clustersNumber, double kmeansMinSigmaSquare, double kmeansMaxSigmaSquare, double kmeansStepImprovement, int kmeansIterations);
    ~RBFNetwork(void);

    void init(std::vector<std::vector<double> > &data);

	void addOutput();
	void calculateOutputs();
	void calculateOutputs(std::vector<double>& input);
    double getLastOutputValue(int outputIndex);
	void learnOutput(std::vector<double>& input, double setpoint, OutputNeuron* outputNeuron);
    void learnNetwork(std::vector<std::vector<double> >& inputs, std::vector<std::vector<double> >& setpointsForEachOutputNeuron);
	void setInput(std::vector<double>& input);
	double calculateQuantizationError(std::vector<double>& setpoints);
	void printOutputs();

    void learn(vector< vector<double> > &set, vector<bool> &target);
    vector<bool> test(vector< vector<double> > &set);
};

