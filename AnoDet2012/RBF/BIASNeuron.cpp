#include "BIASNeuron.h"
#include <vector>

using namespace std;

BIASNeuron::BIASNeuron(void):HiddenNeuron(NULL, NULL){
	this->lastCalculatedRadialFunction = 1.0;
}

BIASNeuron::~BIASNeuron(void){
}

double BIASNeuron::calculateRadialFunction(){
	return 1.0;
}