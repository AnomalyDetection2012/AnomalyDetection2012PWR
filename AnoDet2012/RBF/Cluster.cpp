#include "Cluster.h"
#include <random>
#include "stdlib.h"
#include "time.h"
#include <vector>
#include <iostream>

using namespace std;

Cluster::Cluster(int pointSize, double minSigmaSquare, double maxSigmaSquare) 
	: pointSize(pointSize), minSigmaSquare(minSigmaSquare), maxSigmaSquare(maxSigmaSquare)
{
	srand((unsigned)time(NULL));
}


Cluster::~Cluster(void)
{
}

void Cluster::calculateCenter(void){
	if(!this->neighbours.isEmpty()){
		this->center.clear();
		for(int i=0;i<this->pointSize;++i){
			this->center.push_back(0.0);
		}
		QList<vector<double>*>::iterator neighboursIterator;
		for(neighboursIterator=this->neighbours.begin();neighboursIterator!=this->neighbours.end();++neighboursIterator){
			vector<double> neighbour = **neighboursIterator;
			for(int i=0; i<this->pointSize; ++i){
				this->center[i]+=neighbour[i];
			}
		}
		for(int i=0;i<this->pointSize;++i){
			this->center[i]/=((double)this->neighbours.size());
		}
	}else{
		this->randomizeCenter(1000);
	}
}
void Cluster::randomizeCenter(int precision){
	this->center.clear();
	for(int a=0;a<this->pointSize;++a){
		this->center.push_back(((double)(rand()%(precision+1)))/((double)precision));
	}
}
void Cluster::calculateSigmaSquare(){
	if(!this->neighbours.isEmpty()){
		this->sigmaSquare = std::numeric_limits<double>().max();
		double currentSigma=0.0;
		QList<vector<double>*>::iterator neighboursIterator;
		for(neighboursIterator=this->neighbours.begin();neighboursIterator!=this->neighbours.end();++neighboursIterator){
			vector<double>* neighbour = *neighboursIterator;
			currentSigma = this->toCenterSquare(neighbour);
			if(currentSigma<this->sigmaSquare){
				this->sigmaSquare = currentSigma;
			}
		}
		if(this->sigmaSquare<this->minSigmaSquare){
			this->sigmaSquare = this->minSigmaSquare;
		}else if(this->sigmaSquare>this->maxSigmaSquare){
			this->sigmaSquare = this->maxSigmaSquare;
		}
	}else{
		this->sigmaSquare = std::numeric_limits<double>().min();
	}
}
double Cluster::toCenter(vector<double> *neighbour){
	if(!this->center.empty()){
		double length = 0.0;
		for(int a=0;a<this->pointSize;++a){
			length+=pow(neighbour->at(a)-this->center[a], 2.0);
		}
		return sqrt(length);
	}else{
		return 0.0;
	}
}
double Cluster::toCenterSquare(vector<double> *neighbour){
	if(!this->center.empty()){
		double length = 0.0;
		for(int a=0;a<this->pointSize;++a){
			length+=pow(neighbour->at(a)-this->center[a], 2.0);
		}
		return length;
	}else{
		return 0.0;
	}
}
void Cluster::printNeighbours(){
	/*QList<vector<double>*>::iterator neighboursIterator;
	for(neighboursIterator=this->neighbours.begin();neighboursIterator!=this->neighbours.end();++neighboursIterator){
		vector<double> neighbour = **neighboursIterator;
		cout<<neighbour[0]<<" "<<neighbour[1]<<endl;
	}*/
	cout<<this->center[0]<<" "<<this->center[1]<<endl;
}