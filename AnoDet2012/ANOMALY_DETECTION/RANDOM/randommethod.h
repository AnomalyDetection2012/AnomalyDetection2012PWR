#ifndef RANDOMMETHOD_H
#define RANDOMMETHOD_H

#include <time.h>
#include <vector>
#include <iostream>
#include "randomdetection.h"

using namespace std;

class RandomMethod : public RandomDetection
{
public:
    RandomMethod();
    virtual void learn(vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs);
    virtual vector<bool> test(vector< vector<double> > &set, double *mins, double *maxs);
    virtual vector<bool> test(vector< vector<double> > &set);
};

#endif // RANDOMMETHOD_H
