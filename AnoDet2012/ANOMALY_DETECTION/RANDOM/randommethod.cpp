#include "randommethod.h"
#include <stdlib.h>

RandomMethod::RandomMethod()
{
}

vector<bool> RandomMethod::test(vector< vector<double> > &set, double *mins, double *maxs){
    int setSize = set.size();
    vector<bool> error;
    for(int i=0;i<setSize;++i){
            error.push_back( rand()%100 >= 95 ? true : false );
    }
    return error;
}

vector<bool> RandomMethod::test(vector< vector<double> > &set){
    int setSize = set.size();
    vector<bool> error;
    for(int i=0;i<setSize;++i){
            error.push_back( rand()%100 >= 95 ? true : false );
    }
    return error;
}

void RandomMethod::learn(vector< vector<double> > &set, vector<bool> &target, double *mins, double *maxs){

}
