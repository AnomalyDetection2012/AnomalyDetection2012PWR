#include "KMeans.h"
#include "time.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <limits>

using namespace std;

KMeans::KMeans(int clustersNumber, double minSigmaSquare, double maxSigmaSquare, int pointSize, int iterations, int quantizationError, std::vector<std::vector<double> >& neighbours)
	: clustersNumber(clustersNumber),minSigmaSquare(minSigmaSquare),maxSigmaSquare(maxSigmaSquare),pointSize(pointSize),iterations(iterations),stepImprovement(stepImprovement), neighbours(neighbours)
{
}


KMeans::~KMeans(void)
{
}

void KMeans::initAlgorithm(){
	for(int a=0; a<clustersNumber; ++a){
		this->clusters.push_back(new Cluster(this->pointSize, this->minSigmaSquare, this->maxSigmaSquare));
	}
	srand((unsigned)time(NULL));
    vector<vector<double> >::iterator neighboursIt;
	for(neighboursIt=this->neighbours.begin(); neighboursIt!=this->neighbours.end(); ++neighboursIt){
		this->clusters.at(rand()%this->clustersNumber)->neighbours.push_back(&(*neighboursIt));
	}
	QList<Cluster*>::iterator clustersIt;
	for(clustersIt=this->clusters.begin();clustersIt!=this->clusters.end();++clustersIt){
		(*clustersIt)->calculateCenter();
	}
}

void KMeans::runAlgorithm(){
    double previousQuantizationError = numeric_limits<double>().max();
	double currentQuantizationError=0.0;
	double shortestDistance=0.0;
	double possibleClusterDistance=0.0;
	Cluster* chosenCluster;
	QList<Cluster*>::iterator currentClusterIt;
	QList<Cluster*>::iterator possibleClusterIt;
	QList<vector<double>*>::iterator neighboursIt;
	for(int iteration=0; iteration<this->iterations;++iteration){
		//cout<<iteration<<endl;
		for(currentClusterIt=this->clusters.begin();currentClusterIt!=this->clusters.end();++currentClusterIt){
			for(neighboursIt=(*currentClusterIt)->neighbours.begin(); neighboursIt!=(*currentClusterIt)->neighbours.end();){
				shortestDistance = (*currentClusterIt)->toCenter((*neighboursIt));
				chosenCluster = (*currentClusterIt);
				for(possibleClusterIt=this->clusters.begin();possibleClusterIt!=this->clusters.end();++possibleClusterIt){
					if(*currentClusterIt!=*possibleClusterIt){
						if((possibleClusterDistance = (*possibleClusterIt)->toCenter(*neighboursIt))<shortestDistance){
							shortestDistance = possibleClusterDistance;
							chosenCluster = (*possibleClusterIt);
						}
					}
				}
				if(chosenCluster!=(*currentClusterIt)){
					//cout<<"Przeniesiono!"<<endl;
					chosenCluster->neighbours.push_back((*neighboursIt));
					neighboursIt = (*currentClusterIt)->neighbours.erase(neighboursIt);
				}else{
					++neighboursIt;
				}
			}
		}
		for(currentClusterIt=this->clusters.begin();currentClusterIt!=this->clusters.end();++currentClusterIt){
			(*currentClusterIt)->calculateCenter();
		}
		currentQuantizationError = this->calculateQuantizationError();
		if((previousQuantizationError-currentQuantizationError)/currentQuantizationError<stepImprovement){
			break;
		}else{
			previousQuantizationError = currentQuantizationError;
		}
	}
	for(currentClusterIt=this->clusters.begin();currentClusterIt!=this->clusters.end();++currentClusterIt){
		(*currentClusterIt)->calculateSigmaSquare();
		//cout<<(*currentClusterIt)->sigmaSquare<<endl;
	}
}

double KMeans::calculateQuantizationError(){
	QList<Cluster*>::iterator clustersIt;
	QList<vector<double>*>::iterator neighboursIt;
	double qError=0.0;
	for(clustersIt=this->clusters.begin();clustersIt!=this->clusters.end();++clustersIt){
		Cluster* curCluster = (*clustersIt);
		for(neighboursIt=curCluster->neighbours.begin(); neighboursIt!=curCluster->neighbours.end(); ++neighboursIt){
			qError+=curCluster->toCenter(*neighboursIt);
		}
	}
	return qError;
}
void KMeans::drawClusters(){
	QList<Cluster*>::iterator clIt;
	QList<vector<double>*>::iterator ngbrIt;
	int tab[100][100];
	for(int a=0;a<100;++a){
		for(int b=0;b<100;++b){
			tab[a][b]=0;
		}
	}
	for(clIt = this->clusters.begin();clIt!=this->clusters.end();++clIt){
		Cluster* cluster = *clIt;
		tab[(int)(cluster->center[0]*100)][(int)(cluster->center[1]*100)] = 1;
		for(ngbrIt=cluster->neighbours.begin();ngbrIt!=cluster->neighbours.end();++ngbrIt){
			vector<double>* ngbr = *ngbrIt;
			tab[(int)(ngbr->at(0)*100)][(int)(ngbr->at(1)*100)] = 2;
		}
	}
	for(int a=0;a<100;++a){
		for(int b=0;b<100;++b){
			if(tab[a][b]==0){
				cout<<"-";
			}else if(tab[a][b]==1){
				cout<<"C";
			}else if(tab[a][b]==2){
				cout<<"s";
			}
		}
		cout<<endl;
	}
}
void KMeans::printClustersParams(){
	QList<Cluster*>::iterator clustersIt;
	vector<double>::iterator it;
	for(clustersIt = this->clusters.begin(); clustersIt!=this->clusters.end(); ++clustersIt){
		cout<<"sigmaSquare: "<<(*clustersIt)->sigmaSquare<<", center:";
		for(it=(*clustersIt)->center.begin(); it!=(*clustersIt)->center.end();++it){
			cout<<*it<<", ";
		}
		cout<<"neighbours: "<<(*clustersIt)->neighbours.size()<<endl;
	}
}
