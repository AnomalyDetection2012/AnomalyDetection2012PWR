#include "RBFNetwork.h"
#include "OutputNeuron.h"
#include "HiddenNeuron.h"
#include <vector>
#include <iostream>

using namespace std;

RBFNetwork::RBFNetwork(
	double learnFactor, double sigmoidalActivationFunctionBeta, double quantizationErrorThreshold, int generations, double randomWeightScaleFactor, double clustersNumber, int pointSize, double kmeansMinSigmaSquare, 
    double kmeansMaxSigmaSquare, double kmeansStepImprovement, int kmeansIterations, vector<vector<double> >& neighbours)
	:learnFactor(learnFactor),  generations(generations), clustersNumber(clustersNumber), pointSize(pointSize), 
	 kmeansMinSigmaSquare(kmeansMinSigmaSquare), kmeansMaxSigmaSquare(kmeansMaxSigmaSquare), kmeansStepImprovement(kmeansStepImprovement), 
	 kmeansIterations(kmeansIterations), neighbours(neighbours), randomWeightScaleFactor(randomWeightScaleFactor),
	 sigmoidalActivationFunctionBeta(sigmoidalActivationFunctionBeta), quantizationErrorThreshold(quantizationErrorThreshold)
{
    this->inputLayer = new vector<double>(this->pointSize);

    this->kmeans = new KMeans(this->clustersNumber, this->kmeansMinSigmaSquare, this->kmeansMaxSigmaSquare,
                              this->pointSize, this->kmeansIterations, this->kmeansStepImprovement, this->neighbours);
    this->kmeans->initAlgorithm();

	QList<Cluster*>::iterator clustersIt;

    for(clustersIt = this->kmeans->clusters.begin();clustersIt != this->kmeans->clusters.end();++clustersIt)
    {
		this->hiddenLayer.push_back(new HiddenNeuron(*clustersIt, this->inputLayer));
	}

	this->kmeans->runAlgorithm();
}


RBFNetwork::~RBFNetwork(void)
{
}


void RBFNetwork::addOutput()
{
	this->outputLayer.push_back(new OutputNeuron(this->randomWeightScaleFactor, this->hiddenLayer));
}

void RBFNetwork::setInput(vector<double>& input)
{
    vector<double>::iterator inputIt, inputLayerIt;
    for(inputIt=input.begin(), inputLayerIt=this->inputLayer->begin(); inputLayerIt != this->inputLayer->end()
        ;++inputIt, ++inputLayerIt)
    {
		*inputLayerIt = *inputIt;
    }
}

void RBFNetwork::calculateOutputs()
{
	QList<OutputNeuron*>::iterator outputLayerIt;

    for(outputLayerIt=this->outputLayer.begin();outputLayerIt!=this->outputLayer.end();++outputLayerIt)
    {
		(*outputLayerIt)->calculateOutput();
	}
}
void RBFNetwork::calculateOutputs(vector<double>& input)
{
	this->setInput(input);

	QList<OutputNeuron*>::iterator outputLayerIt;

    for(outputLayerIt=this->outputLayer.begin();outputLayerIt!=this->outputLayer.end();++outputLayerIt)
    {
		(*outputLayerIt)->calculateOutput();
	}
}

double RBFNetwork::getLastOutputValue(int outputIndex)
{
    return this->outputLayer[outputIndex]->lastOutputValue;
}

void RBFNetwork::learnOutput(vector<double>& input, double setpoint, OutputNeuron* outputNeuron)
{
	this->setInput(input);
	outputNeuron->calculateOutput();
	outputNeuron->learn(setpoint, this->learnFactor, this->sigmoidalActivationFunctionBeta);
}

void RBFNetwork::learnNetwork(vector<vector<double> >& inputs, vector<vector<double> >& setpointsForEachOutputNeuron)
{
    vector<vector<double> >::iterator inputsIt, setpointsIt;
	vector<double>::iterator setpointIt;

    for(int generation=0; generation < this->generations; ++generation)
    {
        for(inputsIt=inputs.begin(), setpointsIt=setpointsForEachOutputNeuron.begin(); inputsIt != inputs.end()
            ;++inputsIt, ++setpointsIt)
        {
			vector<double> setpointForEachOutputNeuron = (*setpointsIt);
            int currentOutputNeuronIndex = 0;

            for(setpointIt = setpointForEachOutputNeuron.begin(); setpointIt!=setpointForEachOutputNeuron.end();++setpointIt, ++currentOutputNeuronIndex){
                this->learnOutput(*inputsIt, *setpointIt, this->outputLayer[currentOutputNeuronIndex]);
			}
            if(this->calculateQuantizationError(setpointForEachOutputNeuron)<=this->quantizationErrorThreshold){
				generation = this->generations;
				break;
			}
		}
	}
}

double RBFNetwork::calculateQuantizationError(vector<double>& setpoints)
{
	QList<OutputNeuron*>::iterator outputLayerIt;
	vector<double>::iterator setpointsIt;
	double qError=0.0;

    for(outputLayerIt=this->outputLayer.begin(), setpointsIt = setpoints.begin();outputLayerIt!=this->outputLayer.end()
        ;++outputLayerIt, ++setpointsIt)
    {
		qError+=pow((*setpointsIt)-(*outputLayerIt)->lastOutputValue, 2.0);
	}

	return 0.5*qError;
}

void RBFNetwork::printOutputs()
{
	QList<OutputNeuron*>::Iterator outputsIt;
	QList<HiddenNeuron*>::Iterator hiddensIt;
	vector<double>::iterator wIt;
	int index=1;

    for(outputsIt = this->outputLayer.begin(); outputsIt!=this->outputLayer.end(); ++outputsIt, ++index)
    {
		cout<<index<<". "<<(*outputsIt)->sigmoidalActivationFunction((*outputsIt)->lastOutputValue, this->sigmoidalActivationFunctionBeta)<<endl;
		cout<<index<<". "<<(*outputsIt)->lastOutputValue<<endl;
		
		for(wIt = (*outputsIt)->weights.begin(); wIt!=(*outputsIt)->weights.end();++wIt){
			cout<<*wIt<<", ";
		}
		cout<<endl;
	}
	index=1;
	for(hiddensIt = this->hiddenLayer.begin(); hiddensIt!=this->hiddenLayer.end(); ++hiddensIt, ++index){
		cout<<index<<". "<<(*hiddensIt)->lastCalculatedRadialFunction<<endl;
	}
}

void RBFNetwork::learn(vector<vector<double> > &set, vector<bool> &target)
{
    this->outputLayer.clear();
    this->addOutput();
    vector<vector<double> > setpointsSet;
    vector<bool>::iterator targetIt;

    for(targetIt = target.begin(); targetIt != target.end(); ++targetIt)
    {
        vector<double> setpoint = *(new vector<double>(0));
        setpoint.push_back(*targetIt?1.0:0.0);
        setpointsSet.push_back(setpoint);
    }

    this->learnNetwork(set, setpointsSet);
}
vector<bool> RBFNetwork::test(vector<vector<double> > &set)
{
    vector<bool> result = *(new vector<bool>(0));
    vector<vector<double> >::iterator setIt;

    for(setIt=set.begin(); setIt != set.end(); ++setIt)
    {
        this->calculateOutputs(*setIt);
        result.push_back(this->getLastOutputValue(0)>0.8);
    }

    return result;
}
