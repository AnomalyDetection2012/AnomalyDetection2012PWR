#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QAction>
#include <fstream>
#include "RANDOM/randommethod.h"
#include "SOM/somnetwork.h"
#include "NEIGHBOUR/nearest_neighbor.h"
#include "BAYES/naive_bayes.h"
#include "DENSITY/densitymethod.h"

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

};

#endif // GUIMAINWINDOW_H