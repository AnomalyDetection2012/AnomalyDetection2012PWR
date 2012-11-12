#ifndef METHOD_H
#define METHOD_H

#include <time.h>
#include <vector>

using namespace std;

class Method
{
public:
    Method();
    // rozszerzamy tylko jedna metode z kazdej pary - zaleznie czy potrzebujemy dane po normalizacji czy przed
    virtual void learn(vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs);
    virtual void learn(vector< vector<double> > &set, vector<bool> &target);// po normalizacji
    virtual vector<bool> test(vector< vector<double> > &set, double *mins, double *maxs);
    virtual vector<bool> test(vector< vector<double> > &set);// po normalizacji
    //TODO metoda zwracajaca interfejs wyboru parametrów
    int methodId;

private:
    double normalize(double x, double min, double max);
};

#endif // METHOD_H
