#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectortracker.h"
#include "ANOMALY_DETECTION/RANDOM/randommethod.h"
#include "ANOMALY_DETECTION/SOM/somnetwork.h"
#include "ANOMALY_DETECTION/NEIGHBOUR/nearest_neighbor.h"
#include "ANOMALY_DETECTION/BAYES/naive_bayes.h"
#include "ANOMALY_DETECTION/DENSITY/densitymethod.h"
#include "ANOMALY_DETECTION/RBF/RBFNetwork.h"
#include "Alarms/subscriber.h"
#include <QResizeEvent>
#include <QProgressDialog>
#include <QFileDialog>
#include <GUI/dialogbusy.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QStringList>
#include <QDebug>

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
    void newRecords(unsigned num);
    void newAnomalies(unsigned num);
    int getSelectedMethodId();
    QString& getSelectedObjectName();
    int getSelectedObjectId();
    
private:
    std::vector<QPair<int, QString> > objectsData;
    Ui::MainWindow *ui;
    ConnectorTracker *ct;
    int choosenObjectId;
    void resizeEvent(QResizeEvent * event);
    QTimer *timer;
    bool statusOfDatabaseConnection;
    bool statusOfObjectDataLoad;
    DialogBusy *dialogBusy;
	QTreeWidgetItem *selectedSubscriber;

    void reloadParams();
    void changeBayesParams();
    void changeDensityParams();
    void changeNeighbourParams();
    void changeRBFParams();
    void changeSOMParams();
	
	void createSubscribersTree();
    QTreeWidgetItem * getSelectedSubscriber();
    void readSubscriberInfo();

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
    void filterValuesLivelog();
    void setRefreshInterval();
    void updateSize();
    void rightClicked();
    void leftClicked();
    void refreshDatabaseTable();
    void changeMethodParams();
    void openMethodsHelpWindow();
    void generateReportFromDatabase();
    void generateReportFromDataRange();
	void discardChanges();
    void applyChanges();
    void collapseSubscribers();
    void expandSubscribers();
    void deleteSubscriber();
    void addSubscriber();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
