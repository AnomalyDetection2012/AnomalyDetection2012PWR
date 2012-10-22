#ifndef RANDOMMETHOD_H
#define RANDOMMETHOD_H

#include <time.h>
#include <vector>
#include <iostream>
#include "method.h"

using namespace std;

class RandomMethod : public Method
{
public:
    RandomMethod();
    void learn(vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs);
    vector<bool> test(vector< vector<double> > &set, double *mins,double *maxs);
};

#endif // RANDOMMETHOD_H
