#include "OutputNeuron.h"
#include "HiddenNeuron.h"
#include "BIASNeuron.h"
#include <vector>
#include <random>
#include <time.h>
#include <iostream>
#include <math.h>

using namespace std;

OutputNeuron::OutputNeuron(double randomWeightScaleFactor):randomWeightScaleFactor(randomWeightScaleFactor)
{
	this->addHiddenNeuron(new BIASNeuron());
	srand(time(NULL));
}
OutputNeuron::OutputNeuron(double randomWeightScaleFactor, QList<HiddenNeuron*>& hiddenLayer):randomWeightScaleFactor(randomWeightScaleFactor)
{
	this->addHiddenNeuron(new BIASNeuron());
	srand(time(NULL));
	QList<HiddenNeuron*>::iterator hiddenLayerIt;
	for(hiddenLayerIt = hiddenLayer.begin();hiddenLayerIt!=hiddenLayer.end();++hiddenLayerIt){
		this->addHiddenNeuron(*hiddenLayerIt);
	}
}


OutputNeuron::~OutputNeuron(void)
{
}

void OutputNeuron::addHiddenNeuron(HiddenNeuron* hiddenNeuron){
	this->connectedNeurons.push_back(hiddenNeuron);
	this->weights.push_back(this->randomWeightScaleFactor*((static_cast<double>(rand()%2001)/1000.0)-1.0));
}
double OutputNeuron::calculateOutput(){
	double outputValue = 0.0;
	QList<HiddenNeuron*>::iterator connectedNeuronsIt;
	vector<double>::iterator weightsIt;
	int a=1;
	for(connectedNeuronsIt = this->connectedNeurons.begin(), weightsIt = this->weights.begin(); connectedNeuronsIt!=this->connectedNeurons.end(); ++connectedNeuronsIt, ++weightsIt){
		//cout<<a<<". wOut: "<<(*weightsIt)<<endl;
		//cout<<a++<<". RF: "<<(*connectedNeuronsIt)->calculateRadialFunction()<<endl;
		outputValue+=(*connectedNeuronsIt)->calculateRadialFunction()*(*weightsIt);
	}
	this->lastOutputValue = outputValue;
	return outputValue;
}

void OutputNeuron::learn(double setpoint, double learnFactor, double sigmoidalActivationFunctionBeta){
	QList<HiddenNeuron*>::iterator connectedNeuronsIt;
	vector<double>::iterator weightsIt;
	for(connectedNeuronsIt = this->connectedNeurons.begin(), weightsIt = this->weights.begin(); connectedNeuronsIt!=this->connectedNeurons.end(); ++connectedNeuronsIt, ++weightsIt){
		(*weightsIt)+=
			learnFactor
			*(setpoint-this->sigmoidalActivationFunction(this->lastOutputValue, sigmoidalActivationFunctionBeta))
			*(*connectedNeuronsIt)->lastCalculatedRadialFunction;
	}
}

double OutputNeuron::sigmoidalActivationFunction(double x, double beta){
	return 1.0/(1.0+exp(-beta*x));
}
