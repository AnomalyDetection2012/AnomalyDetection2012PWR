#pragma once
#include "HiddenNeuron.h"
#include <vector>

class BIASNeuron :
	public HiddenNeuron
{
public:
	BIASNeuron(void);
	~BIASNeuron(void);
	double calculateRadialFunction();
};

