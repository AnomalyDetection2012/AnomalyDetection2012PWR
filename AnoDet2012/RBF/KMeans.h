#pragma once
#include "Cluster.h"
#include <vector>
class KMeans
{
public:
	int clustersNumber, iterations, pointSize;
	double stepImprovement, minSigmaSquare, maxSigmaSquare;
	QList<Cluster*> clusters;
    std::vector<std::vector<double> >& neighbours;
    KMeans(int clustersNumber, double minSigmaSquare, double maxSigmaSquare, int pointSize, int iterations, int stepImprovement, std::vector<std::vector<double> >& neighbours);
	~KMeans(void);
	void initAlgorithm();
	void runAlgorithm();
	void drawClusters();
	void printClustersParams();
private:
	double calculateQuantizationError();
};

