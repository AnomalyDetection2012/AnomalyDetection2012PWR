
#include <QtCore/QCoreApplication>
#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include "Cluster.h"
#include "KMeans.h"
#include "RBFNetwork.h"
#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <iostream>

using namespace std;

void printPoint(vector<double> *point, int pointSize){
	for(int a=0;a<pointSize;++a){
		cout<<point->at(a)<<" ";
	}
	cout<<endl;
}
void printDouble(double value){
	cout<<value<<" "<<endl;
}
double n(double value, double min, double max){
	return (value-min)/(max-min);
}
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QFile file("C:/iris.data");
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("B³¹d otwarcia pliku");
        exit(1);
    }
	QTextStream textStream(&file);
	QString line;
	QStringList strList;
    vector<vector<double> > data, setpoints;
	while(!textStream.atEnd()){
		line = textStream.readLine().trimmed();
		strList = line.split(",");
		vector<double>* record = new vector<double>(0);
		vector<double>* setpointsForRecord = new vector<double>(0);
		record->push_back(n(strList.at(0).toDouble(), 4.3, 7.9));
		record->push_back(n(strList.at(1).toDouble(), 2.0, 4.4));
		record->push_back(n(strList.at(2).toDouble(), 1.0, 6.9));
		record->push_back(n(strList.at(3).toDouble(), 0.1, 2.5));
		for(int a=0;a<3;++a){
			setpointsForRecord->push_back(0.0);
		}
		if(strList.at(4).contains("Iris-setosa")){
			setpointsForRecord->at(0) = 1.0;
		}else if(strList.at(4).contains("Iris-versicolor")){
			setpointsForRecord->at(1) = 1.0;
		}else if(strList.at(4).contains("Iris-virginica")){
			setpointsForRecord->at(2) = 1.0;
		}
		data.push_back(*record);
		setpoints.push_back(*setpointsForRecord);
	}
	RBFNetwork* rbf = new RBFNetwork(0.1, 2.0, 0.001, 100, 0.1, 27, 4, 0.0001, 0.1, 0.001, 100, data);
	rbf->addOutput();
	rbf->addOutput();
	rbf->addOutput();
	rbf->learnNetwork(data, setpoints);
	vector<double> input;
	//5.8,2.6,4.0,1.2
	input.push_back(n(6.7, 4.3, 7.9));
	input.push_back(n(3.3, 2.0, 4.4));
	input.push_back(n(5.6, 1.0, 6.9));
	input.push_back(n(2.1, 0.1, 2.5));
	rbf->calculateOutputs(input);
	rbf->printOutputs();
	rbf->kmeans->printClustersParams();
	return a.exec();
}
