#pragma once
#include <QList>
#include <vector>

class Cluster
{
public:
	int pointSize;
	double sigmaSquare, minSigmaSquare, maxSigmaSquare;
	std::vector<double> center;
    QList<std::vector<double>* > neighbours;

	Cluster(int pointSize, double minSigmaSquare, double maxSigmaSquare);
	~Cluster(void);

	void calculateCenter();
	void calculateSigmaSquare();
	double toCenter(std::vector<double> *neighbour); // Euclidean length
	double toCenterSquare(std::vector<double> *neighbour); // Euclidean length square
	void printNeighbours();
private:
	void randomizeCenter(int precision);
};

