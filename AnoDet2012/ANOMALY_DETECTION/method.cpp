#include "method.h"
#include <iostream>

Method::Method()
{

}

vector<bool> Method::test(vector< vector<double> > &set, double *mins, double *maxs){
    vector< vector<double> > normalized;
    vector< vector<double> >::iterator setIter;
    for(setIter = set.begin();setIter != set.end();setIter++){
        vector<double> temp;
        for(int i=0;i<setIter->size();i++){
            temp.push_back(normalize(setIter->at(i),mins[i],maxs[i]));
        }
        normalized.push_back(temp);
    }
    return test(normalized);
}

vector<bool> Method::test(vector< vector<double> > &set){
    vector<bool> temp;
    return temp;
}

void Method::learn(vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs){
    vector< vector<double> > normalized;
    vector< vector<double> >::iterator setIter;
    for(setIter = set.begin();setIter != set.end();setIter++){
        vector<double> temp;
        for(int i=0;i<setIter->size();i++){
            temp.push_back(normalize(setIter->at(i),mins[i],maxs[i]));
        }
        normalized.push_back(temp);
    }
    learn(normalized,target);
}

void Method::learn(vector< vector<double> > &set, vector<bool> &target){

}

double Method::normalize(double x, double min, double max){
    return (x-min)/(max-min);
}
