#include "densitymethod.h"
#include <QList>
#include <math.h>
#include <iostream>

using namespace std;

DensityMethod::DensityMethod(double LOFResultDeviation, int neighbors){
    this->neighbors = neighbors;
    this->LOFResultDeviation = LOFResultDeviation;
}
void DensityMethod::init(int dimensions){
    this->dataSetSize = 0;
    this->dimensions = dimensions;
    this->dataSet.clear();
    for(int a=0;a<dimensions;++a){
        this->dataSet.push_back(QList<double>());
    }
}
double DensityMethod::twoPointsDistance(int first, int second){
    double res=0.0;
    for(int a=0;a<this->dimensions;a++){
        res+=pow(this->dataSet[a][first]-this->dataSet[a][second],2.0);
    }
    return sqrt(res);
}

void DensityMethod::kNearestNeighbors(QList<Neighbor> &retVal, int point, int neighborsNumber){
    int neighborsSize=0;
    double kDistance=0.0;
    double curDistance=0.0;
    for(int a=0;a<dataSetSize;++a){
        if(point!=a){
            if(neighborsSize>=neighborsNumber){
                curDistance = twoPointsDistance(a,point);
                if(curDistance<kDistance){
                    Neighbor* n = new Neighbor();
                    n->index = a;
                    n->length = curDistance;
                    insertNeighbor(retVal,*n,neighborsNumber-1);
                    kDistance = retVal[neighborsNumber-1].length;
                }
            }else{
                Neighbor* n = new Neighbor();
                n->index = a;
                n->length = twoPointsDistance(a, point);
                retVal.push_back(*n);
                insertNeighbor(retVal,*n,neighborsSize-1);
                kDistance = retVal[neighborsSize].length;
                neighborsSize++;
            }
        }
    }
}

void DensityMethod::insertNeighbor(QList<Neighbor> &n, Neighbor newN, int lastIndex){
    int lastIndexBegin = lastIndex;
    while(lastIndex>=0){
        if(newN.length>n[lastIndex].length){
            break;
        }
        --lastIndex;
    }
    for(int a=lastIndexBegin;a>(lastIndex+1);--a){
        n[a]=n[a-1];
    }
    n[lastIndex+1]=newN;
}
double DensityMethod::lrd(int pIndex, int k){
    QList<Neighbor> ngs;
    QList<Neighbor> ngsngs;
    kNearestNeighbors(ngs,pIndex,k);
    double lrd=0.0;
    for(int a=0;a<k;a++){
        kNearestNeighbors(ngsngs,ngs[a].index,k);
        lrd+=max(ngs[a].length, ngsngs[k-1].length);
    }
    return (static_cast<double>(k))/lrd;
}
double DensityMethod::LOF(int pIndex, int k){
    if(k<dataSetSize && k>0){
        double lof = 0.0;
        QList<Neighbor> ngs;
        kNearestNeighbors(ngs,pIndex,k);
        for(int a=0;a<k;a++){
            lof+=lrd(ngs[a].index,k);
        }
        return lof/((static_cast<double>(k))*lrd(pIndex,k));
    }else{
        return 1.0;
    }
}
int DensityMethod::getDataSetSize(){
    return this->dataSetSize;
}

void DensityMethod::learn(vector< vector<double> > &set, vector<bool> &target){
    vector< vector<double> >::iterator setIter = set.begin();
    this->init(setIter->size());
    vector<bool>::iterator targetIter=target.begin();
    for(;setIter!=set.end();setIter++){
        if(!(*targetIter)){
            for(int a=0;a<setIter->size();++a){
                this->dataSet[a]<<setIter->at(a);
            }
            this->dataSetSize++;
        }
        targetIter++;
    }
}
vector<bool> DensityMethod::test(vector< vector<double> > &set){
    vector<bool> errors;
    vector< vector<double> >::iterator setIter;
    for(setIter=set.begin();setIter!=set.end();setIter++){
        for(int d=0;d<setIter->size();++d){
            this->dataSet[d]<<setIter->at(d);
        }
        double lof = this->LOF(dataSetSize,this->neighbors);
        errors.push_back(lof<(1.0-this->LOFResultDeviation) || lof>(1.0+this->LOFResultDeviation));
        for(int d=0;d<setIter->size();++d){
            this->dataSet[d].removeLast();
        }
    }
    return errors;
}
