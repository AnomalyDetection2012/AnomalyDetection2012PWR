#include "algorithmcontroler.h"
#include "Dataset/DatasetConnector.h"
#include <iostream>

AlgorithmControler::AlgorithmControler()
{
}

AlgorithmControler::~AlgorithmControler()
{
}

void AlgorithmControler::initialiseConnectors(ConnectorTracker *con){
    dataset = con->dataset;
}

void AlgorithmControler::registerMethod(int i, Method *m){
    methods.erase(i);
    methods.insert(make_pair(i, m));
}

Method *AlgorithmControler::getMethod(int i){
    return methods.at(i);
}

void AlgorithmControler::learn(int method, vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs){
    getMethod(method)->learn(set, target, mins, maxs);
}

void AlgorithmControler::learn(int method, vector< vector<double> > &set, vector<bool> &target){
    getMethod(method)->learn(set, target);
}

vector<bool> AlgorithmControler::test(int method, vector< vector<double> > &set, double *mins, double *maxs){
    return getMethod(method)->test(set, mins, maxs);
}

vector<bool> AlgorithmControler::test(int method, vector< vector<double> > &set){
    return getMethod(method)->test(set);
}
