#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QAction>
#include <fstream>
#include "ANOMALY_DETECTION/RANDOM/randommethod.h"
#include "ANOMALY_DETECTION/SOM/somnetwork.h"
#include "ANOMALY_DETECTION/NEIGHBOUR/nearest_neighbor.h"
#include "ANOMALY_DETECTION/BAYES/naive_bayes.h"
#include "ANOMALY_DETECTION/DENSITY/densitymethod.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "connectortracker.h"
#include "Dataset/DatasetConnector.h"
#include "ConfigurationHandler/configurationhandler.h"

using namespace std;

namespace Ui {
class GUIMainWindow;
}

class GUIMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GUIMainWindow(QWidget *parent = 0);
    ~GUIMainWindow();
    void randomTest(int size);//TODO temporary method
    void drawResult();
    void drawScale(double *mins, double *maxs);
    void newTest();
    void initMethods();
    void initDimensions(vector<QString> &dimNames);
    void getMinMax();
    void newMethodTitle(string name);

    //TEST
    ConnectorTracker *ct;
    DatasetConnector *dataset;
    AlgorithmControler *algorithm;
    int selectedMethodId;
    void drawNewFormat();
    
private:
    Ui::GUIMainWindow *ui;
    Method *selectedMethod;
    vector< vector<double> > values;
    vector<bool> anomalies;
    vector<QPen> pens;
    vector<QColor> colors;
    int setSize,setDimensions,choosenDimension;
    double *minimals;
    double *maximals;

private slots:
    void readLearnFile();
    void readTestFile();
    void chooseDimension(int n);

    //METODY
    void chooseRandom();
    void chooseSOM();
    void chooseBAYES();
    void chooseNEIGHBOUR();
    void chooseDENSITY();
    void testSlotA();
    void testSlotB();
    void testSlotC();
};

#endif // GUIMAINWINDOW_H
