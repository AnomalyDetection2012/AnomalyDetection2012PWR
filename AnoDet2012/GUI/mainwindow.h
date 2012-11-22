#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectortracker.h"
#include "ANOMALY_DETECTION/RANDOM/randommethod.h"
#include "ANOMALY_DETECTION/SOM/somnetwork.h"
#include "ANOMALY_DETECTION/NEIGHBOUR/nearest_neighbor.h"
#include "ANOMALY_DETECTION/BAYES/naive_bayes.h"
#include "ANOMALY_DETECTION/DENSITY/densitymethod.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void redrawDataset();
    
private:
    Ui::MainWindow *ui;
    ConnectorTracker *ct;
    int choosenObjectId;
    std::vector<int> objectIDs;

private slots:
    void chooseObject();
    void connectDatabase();
    void loadDataStandard();
    void loadDataDate();
    void learnData();
    void testData();
    void on_filterValuesBtn_clicked();
    void loadAllObjectRecords();
    void showAnomaliesFromDB();
    void startLivelog();
    void stopLivelog();
};

#endif // MAINWINDOW_H
