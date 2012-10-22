#include "somnetwork.h"
#include <vector>
#include <QPainter>
#include <QPen>
#include <cmath>
#include <stdio.h>
#include <iostream>

SOMNetwork::SOMNetwork(int width, int height,int inputs, double maxRad, double maxAlpha, double maxIter, TopologyMap &m) :
    cols(width), rows(height),numInputs(inputs), Rmax(maxRad), alphamax(maxAlpha),tmax(maxIter), numNeurons(width*height), map(&m)
{
    this->t = 0;


    double X,Y;

    int cnt = 0;

    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
        {
            X = i*map->sizeOfSingleSquare + map->sizeOfSingleSquare/2;
            Y = j*map->sizeOfSingleSquare + map->sizeOfSingleSquare/2;
            map->sq.push_back(QRectF(X,Y,2,2)); //symbole neuronow
            map->neuronPoints.push_back(QPoint(X,Y));
            Neuron neuron(cnt++,numInputs,X,Y);
            neurons.push_back(neuron);

            //map->colors[cnt-1] = QColor(255.0 * neuron.weights[0],255.0 * neuron.weights[1], 255.0 * neuron.weights[2]);

        }
}


int SOMNetwork::findWinner(std::vector<double> inputs)
{
    int tmp_idx = 0;
    double tmp_d = neurons[0].calcEuclidDistance(inputs);


    for(int i=1;i<numNeurons;i++)
    {
        double distance = neurons[i].calcEuclidDistance(inputs);

        if(distance < tmp_d)
        {
            tmp_d = distance;
            tmp_idx = i;
        }
    }

    return tmp_idx;
}

double SOMNetwork::getQuantizationError(std::vector<double> inputs)
{
    int tmp_idx = 0;
    double tmp_d = neurons[0].calcEuclidDistance(inputs);


    for(int i=1;i<numNeurons;i++)
    {
        double distance = neurons[i].calcEuclidDistance(inputs);

        if(distance < tmp_d)
        {
            tmp_d = distance;
            tmp_idx = i;
        }
    }

    return tmp_d;

}

void SOMNetwork::correctWeights(std::vector<double> inputs)
{
    int winner = findWinner(inputs);

    double d,fnc;

    for(int i=0;i<numNeurons;i++)
    {
        //d = neurons[i].calcEuclidDistance(inputs); // zla odledlosc, ma byc odleglosc of zwyciezkiego
        //d = neurons[i].calcEuclidDistance(neurons[winner].weights);  // ta jest chyba ok, akt: jednak nie ;p
        d = neurons[i].calcEuclidDistanceOnMap(neurons[winner]);


        fnc = exp(-( (d*d) / (2 * R * R) ));


        //if(fnc>0.000001)
        //    map->colors[i] = QColor(0,255,0);

        //if(fnc>0.000001)
            for(int j=0;j<neurons[i].numInputs;j++)
                neurons[i].weights[j] += alpha * fnc * (neurons[winner].weights[j] - neurons[i].weights[j]);
                //neurons[i].weights[j] += alpha * /*fnc * */ (neurons[winner].weights[j] - neurons[i].weights[j]);



    }

    //map->colors[winner] = QColor(inputs[0]*255,inputs[1]*255,inputs[2]*255);
    //map->colors[winner] = QColor(0,0,0);
}



void SOMNetwork::learn(std::vector<std::vector<double> > inputs)
{
    size_t size = inputs.size();

    while(t<=tmax)
    {
        t++;

        /*R = Rmax * pow((0.5 / Rmax),(double)t/tmax);

        alpha = alphamax * pow((0.001 / alphamax),(double)t/tmax);*/


        double timeConst = tmax / log(Rmax);
        R = Rmax * exp(-(double)t/timeConst);

        alpha = alphamax * exp(-(double)t/tmax);


        correctWeights(inputs[t % size]);

        //map->repaint();
    }
}


void SOMNetwork::findAssignments(std::vector<std::vector<double> > inputs, QStringList names)
{

    minQuantizationError = INT_MAX;
    maxQuantizationError = INT_MIN;

    int winner;
    double quantErr;
    double quantErrSum = 0;

    //assignments.clear();
    //assignments.reserve(this->numNeurons);
    std::vector<std::vector<QString> >  assignments(this->numNeurons);

    for(int i=0;i<inputs.size();i++)
    {
        winner = findWinner(inputs[i]);
        assignments[winner].push_back(names[i]);

        quantErr = getQuantizationError(inputs[i]);
        if(quantErr<minQuantizationError)
            minQuantizationError = quantErr;

        if(quantErr>maxQuantizationError)
            maxQuantizationError = quantErr;

        quantErrSum += quantErr;
    }

    avgQuantizationError = quantErrSum / (double)inputs.size();
    QString qst;

    std::vector<QString> v;
    int a=0;
    foreach(v, assignments)
    {
        std::cout<<"\n"<<(a++)<<": ";
        foreach(qst, v)
            std::cout<<qst.toAscii().constData()<<" ";
    }

    this->assignments = assignments;

}

void SOMNetwork::calcQuantizationError(std::vector<std::vector<double> > inputs)
{
    minQuantizationError = INT_MAX;
    maxQuantizationError = INT_MIN;

    double quantErr;
    double quantErrSum = 0;


    for(int i=0;i<inputs.size();i++)
    {

        quantErr = getQuantizationError(inputs[i]);

        if(quantErr<minQuantizationError)
            minQuantizationError = quantErr;

        if(quantErr>maxQuantizationError)
            maxQuantizationError = quantErr;

        quantErrSum += quantErr;
    }

    avgQuantizationError = quantErrSum / (double)inputs.size();

	std::cout<<"\nmin quan err: "<<minQuantizationError;
	std::cout<<"\nmax quan err: "<<maxQuantizationError;
	std::cout<<"\navg quan err: "<<avgQuantizationError ;
}


void SOMNetwork::learn(vector< vector<double> > &set, vector<bool> &target)
{
    std::vector<std::vector<double> > learnVector;
    std::vector<double> vec;

    for(int i=0;i<set.size();i++)
        if(!target[i])
            learnVector.push_back(set[i]);

    learn(learnVector);
    calcQuantizationError(learnVector);

}

std::vector<bool> SOMNetwork::test(vector< vector<double> > &set)
{
    std::vector<bool> detection;

    double quantErr;
    double quantErrSum = 0;

    for(int i=0;i<set.size();i++)
    {
        quantErr = getQuantizationError(set[i]);
	quantErrSum += quantErr;

        if(quantErr > maxQuantizationError)
            detection.push_back(true);
        else
            detection.push_back(false);
    }
std::cout<<"\nsrednia podczas testu: "<<quantErrSum/(double)set.size();
    return detection;
}
