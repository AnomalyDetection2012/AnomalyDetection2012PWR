#include "guimainwindow.h"
#include "ui_guimainwindow.h"
#include <iostream>

#define YSIZE 350
#define VERTS 11
#define MAXDOUBLE 9999999
#define XSPACE 15
#define SCALEWIDTH 79

GUIMainWindow::GUIMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUIMainWindow)
{
    ui->setupUi(this);
    initMethods();
    selectedMethod = new RandomMethod();
    pens.push_back( QPen(QColor("black")) ); colors.push_back(QColor("black"));
    pens.push_back( QPen(QColor("green")) ); colors.push_back(QColor("green"));
    pens.push_back( QPen(QColor("yellow")) ); colors.push_back(QColor("yellow"));
    pens.push_back( QPen(QColor("cyan")) ); colors.push_back(QColor("cyan"));
    pens.push_back( QPen(QColor("magneta")) ); colors.push_back(QColor("magneta"));

    vector<QPen>::iterator iter;
    for(iter = pens.begin(); iter != pens.end();iter++){
        iter->setWidth(2);
    }
}

GUIMainWindow::~GUIMainWindow()
{
    delete ui;
    delete selectedMethod;
    values.clear();
    anomalies.clear();
}

void GUIMainWindow::newTest(){
    anomalies.clear();
    values.clear();
    choosenDimension = 0;
    ui->dimensionChoiceBox->clear();
}

void GUIMainWindow::getMinMax(){
    minimals = new double[setDimensions];
    maximals = new double[setDimensions];
    for(int i=0;i<setDimensions;++i){
        minimals[i]=MAXDOUBLE;
        maximals[i]=0;
    }
    vector< vector<double> >::iterator setIter;
    vector<double>::iterator iter;
    int j; double dtemp;
    for(setIter = values.begin();setIter != values.end();setIter++){
        j=0;
        for(iter = setIter->begin();iter != setIter->end();iter++){
            dtemp = *iter;
            if( dtemp < minimals[j] ){
                minimals[j] = dtemp;
            }
            if( dtemp > maximals[j] ){
                maximals[j] = dtemp;
            }
            j++;
        }
    }
    minimals[0]=0.0;
    minimals[1]=100.0;
    minimals[2]=10.0;
    minimals[3]=5.0;
    maximals[0]=1.0;
    maximals[1]=300.0;
    maximals[2]=400.0;
    maximals[3]=50.0;
}

void GUIMainWindow::drawResult(){
    drawScale(minimals,maximals);
    int lastY,temp;
    QGraphicsScene *resultScene = new QGraphicsScene(this);
    QPen blue(QColor("blue"));
    QPen red(QColor("red"));
    red.setWidth(3);
    QPen *pen;
    for(int i=0;i<=VERTS;++i){
        temp = i*YSIZE/VERTS;
        resultScene->addLine( 0, temp, (setSize-1)*XSPACE, temp, blue);
    }

    for(int i=0;i<setDimensions;++i){
        pen = &( pens.at( i % pens.size() ) );
        lastY = YSIZE - (((values.at(0).at(i) - minimals[i]) / (maximals[i]-minimals[i])) * YSIZE);
        for(int j=1;j<setSize;++j){
            if( i == 0 && anomalies.at(j)){
                resultScene->addLine( j*XSPACE , 0 , j*XSPACE , YSIZE, red);
            }
            temp = YSIZE - (((values.at(j).at(i) - minimals[i]) / (maximals[i]-minimals[i])) * YSIZE);
            resultScene->addLine( (j-1)*XSPACE , lastY , j*XSPACE , temp, *pen);
            lastY = temp;
        }
    }
    QGraphicsTextItem *b;
    b = new QGraphicsTextItem(QString(" "));
    b->setPos(0,YSIZE+6);
    resultScene->addItem(b);
    ui->resultView->setScene(resultScene);
}

void GUIMainWindow::drawScale(double *mins, double *maxs){
    QGraphicsScene *scaleScene = new QGraphicsScene(this);
    QGraphicsTextItem *text;
    QPen pen(QColor("blue"));
    int temp; double val;
    for(int i=0;i<=VERTS;++i){
        temp = i*YSIZE/VERTS;
        scaleScene->addLine( 0, temp, SCALEWIDTH, temp, pen);
        val = maxs[choosenDimension]-i*(maxs[choosenDimension]-mins[choosenDimension])/VERTS;
        text = new QGraphicsTextItem(QString::number(val,'g'));
        text->setPos(0,temp);
        text->setDefaultTextColor(colors.at( choosenDimension % colors.size() ));
        scaleScene->addItem(text);
    }
    ui->scaleView->setScene(scaleScene);
}

void GUIMainWindow::randomTest(int size){
    newTest();
    setSize = size;
    setDimensions = 1;
    for(int i=0;i<size;++i){
        vector<double> vec;
        vec.push_back(rand()%1000);
        values.push_back(vec);
    }
    selectedMethod = new RandomMethod();
    vector<QString> names;
    names.push_back(QString("zero"));
    initDimensions(names);
    getMinMax();
    anomalies = selectedMethod->test(values,minimals,maximals);
    drawResult();
}

void GUIMainWindow::readLearnFile(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Open data set file"));
    fstream file;
    double temp;
    file.open(fileName.toStdString().c_str(), fstream::in);
    file>>setSize;
    file>>setDimensions;
    newTest();

    vector<bool> target;
    for(int i=0;i<setSize;++i){
        vector<double> vec;
        for(int j=0;j<setDimensions;++j){
            file>>temp;
            vec.push_back(temp);
        }
        values.push_back(vec);
        file>>temp;
        target.push_back( temp >= 0.5 ? true : false);
    }
    file.close();

    for(int i=0;i<values.size();i++)
    {
        std::cout<<"\n";
        for(int j=0;j<values[i].size();j++)
            std::cout<<values[i][j]<<" ";
    }

    getMinMax();
    selectedMethod->learn(values,target,minimals,maximals);
}

void GUIMainWindow::readTestFile(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Open data set file"));
    fstream file;
    double temp;
    file.open(fileName.toStdString().c_str(), fstream::in);
    file>>setSize;
    file>>setDimensions;
    newTest();
    string name;

    vector<QString> names;
    for(int i=0;i<setDimensions;++i){
        file>>name;
        names.push_back(QString::fromStdString(name));
    }

    for(int i=0;i<setSize;++i){
        vector<double> vec;
        for(int j=0;j<setDimensions;++j){
            file>>temp;
            vec.push_back(temp);
        }
        values.push_back(vec);
    }
    file.close();



    initDimensions(names);
    getMinMax();
    anomalies = selectedMethod->test(values,minimals,maximals);
    drawResult();
}

void GUIMainWindow::initDimensions(vector<QString> &dimNames){
    for(int i=0;i<setDimensions;++i){
        ui->dimensionChoiceBox->addItem(dimNames.at(i));
    }
}

void GUIMainWindow::chooseDimension(int n){
    choosenDimension = n;
    drawScale(minimals,maximals);
}

//WYBOR METOD

void GUIMainWindow::newMethodTitle(string name){
    this->setWindowTitle(QString::fromStdString("Wykrywanie Anomalii - "+name));
}

void GUIMainWindow::initMethods(){
    QAction *action;
    //BEGIN - RANDOM
    action = new QAction(tr("Metoda losowa"),this);
    connect(action, SIGNAL(triggered()), this, SLOT(chooseRandom()));
    ui->menuMetody->addAction(action);
    //END - RANDOM

    //BEGIN - SOM
    action = new QAction(tr("Siec SOM"),this);
    connect(action, SIGNAL(triggered()), this, SLOT(chooseSOM()));
    ui->menuMetody->addAction(action);
    //END - SOM

    //BEGIN - BAYES
    action = new QAction(tr("Klasyfikator Bayesa"),this);
    connect(action, SIGNAL(triggered()), this, SLOT(chooseBAYES()));
    ui->menuMetody->addAction(action);
    //END - BAYES

    //BEGIN - NEIGHBOUR
    action = new QAction(tr("Najblizsze sasiedztwo"),this);
    connect(action, SIGNAL(triggered()), this, SLOT(chooseNEIGHBOUR()));
    ui->menuMetody->addAction(action);
    //END - NEIGHBOUR

    //BEGIN - DENSITY
    action = new QAction(tr("Metoda gestosciowa"),this);
    connect(action, SIGNAL(triggered()), this, SLOT(chooseDENSITY()));
    ui->menuMetody->addAction(action);
    //END - DENSITY
}

void GUIMainWindow::chooseRandom(){
    delete selectedMethod;
    newMethodTitle("Metoda losowa");
    selectedMethod = new RandomMethod();
}

void GUIMainWindow::chooseSOM(){
    delete selectedMethod;
    newMethodTitle("Siec SOM");
    TopologyMap map(this,20,20,10);
    selectedMethod = new SOMNetwork(20,20,4,50,0.4,1000,map);
}

void GUIMainWindow::chooseBAYES(){
    delete selectedMethod;
    newMethodTitle("Klasyfikator Bayesa");
    selectedMethod = new NaiveBayes(4);
}

void GUIMainWindow::chooseNEIGHBOUR(){
    delete selectedMethod;
    newMethodTitle("Metoda najblizszego sasiedztwa");
    selectedMethod = new NearestNeighbor(4,5);
}

void GUIMainWindow::chooseDENSITY(){
    delete selectedMethod;
    newMethodTitle("Metoda gestosciowa");
    selectedMethod = new DensityMethod(0.5,50);
}
