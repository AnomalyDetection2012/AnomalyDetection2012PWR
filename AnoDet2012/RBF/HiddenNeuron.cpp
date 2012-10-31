#include "HiddenNeuron.h"
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

HiddenNeuron::HiddenNeuron(Cluster* cluster, vector<double>* input):cluster(cluster),input(input)
{
}


HiddenNeuron::~HiddenNeuron(void)
{
}

double HiddenNeuron::calculateRadialFunction(){
	//cout<<"Norm:"<<this->cluster->toCenterSquare(this->input)<<"-"<<this->cluster->sigmaSquare<<endl;
	return this->lastCalculatedRadialFunction = exp(-this->cluster->toCenterSquare(this->input)/(2.0*this->cluster->sigmaSquare));
}