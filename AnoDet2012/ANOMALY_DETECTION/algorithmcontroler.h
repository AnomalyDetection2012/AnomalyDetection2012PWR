#ifndef ALGORITHMCONTROLER_H
#define ALGORITHMCONTROLER_H
#include <map>
#include "method.h"

using namespace std;

class AlgorithmControler
{
public:
    AlgorithmControler();
    ~AlgorithmControler();
    void registerMethod(int i, Method &m);
    Method *getMethod(int i);
    void learn(int method, vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs);
    void learn(int method, vector< vector<double> > &set, vector<bool> &target);// po normalizacji
    vector<bool> test(int method, vector< vector<double> > &set, double *mins, double *maxs);
    vector<bool> test(int method, vector< vector<double> > &set);// po normalizacji

private:
    map<int, Method> methods;
};

#endif // ALGORITHMCONTROLER_H
