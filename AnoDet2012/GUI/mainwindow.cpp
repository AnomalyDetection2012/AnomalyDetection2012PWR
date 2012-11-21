#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConfigurationHandler/configurationhandler.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "DataLoader/dataloader.h"
#include "Dataset/DatasetConnector.h"
#include "GUI_COMPONENTS/guicontroller.h"
#include "GUI/dialogfilter.h"
#include <QListWidgetItem>

#define CHECKBOX_CHECKED 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    objectsData[0][0]  = 3;
    objectsData[1][0]  = 4;
    objectsData[2][0]  = 6;
    objectsData[3][0]  = 9;
    objectsData[4][0]  = 12;
    objectsData[5][0]  = 17;
    objectsData[6][0]  = 19;
    objectsData[7][0]  = 21;
    objectsData[8][0]  = 22;
    objectsData[9][0]  = 24;
    objectsData[10][0] = 27;
    objectsData[11][0] = 29;
    objectsData[12][0] = 30;
    objectsData[13][0] = 33;
    objectsData[14][0] = 36;
    objectsData[15][0] = 38;
    objectsData[16][0] = 39;
    objectsData[0][1]  = 43318;
    objectsData[1][1]  = 40024;
    objectsData[2][1]  = 41908;
    objectsData[3][1]  = 118619;
    objectsData[4][1]  = 40281;
    objectsData[5][1]  = 69183;
    objectsData[6][1]  = 76968;
    objectsData[7][1]  = 40391;
    objectsData[8][1]  = 43629;
    objectsData[9][1]  = 40325;
    objectsData[10][1] = 54374;
    objectsData[11][1] = 25749;
    objectsData[12][1] = 26658;
    objectsData[13][1] = 28101;
    objectsData[14][1] = 24781;
    objectsData[15][1] = 18924;
    objectsData[16][1] = 14830;

    ct = new ConnectorTracker();
    ct->initialise();
    choosenObjectId = ct->configuration->getObjectId();

    // USTAWIENIA P�L
    ui->comboBox_2->setCurrentIndex(choosenObjectId);
    ui->label_12->setText(ui->comboBox_2->currentText());
    ui->lineEdit->setText(ct->configuration->getPropertyValue("Database", "Server").toString());
    ui->lineEdit_2->setText(ct->configuration->getPropertyValue("Database", "Source").toString());
    ui->lineEdit_3->setText(ct->configuration->getPropertyValue("Database", "UserName").toString());
    ui->lineEdit_4->setText(ct->configuration->getPropertyValue("Database", "UserPassword").toString());
    // KONIEC USTAWIEN P�L

    // TODO TEMP METODY DETEKCJI
    AlgorithmControler* algorithm = ct->anomalyDetection;
    QString *name;
    Method *met = new RandomMethod();
    algorithm->registerMethod(0, met);
    name = new QString("SOM");
    TopologyMap map(this,20,20,10);
    met = new SOMNetwork(ct->configuration->getAlgorithmParameter(*name, "Width").toInt(),
                                      ct->configuration->getAlgorithmParameter(*name, "Height").toInt(),
                                      ct->configuration->getAlgorithmParameter(*name, "Inputs").toInt(),
                                      ct->configuration->getAlgorithmParameter(*name, "MaxRadius").toDouble(),
                                      ct->configuration->getAlgorithmParameter(*name, "MaxAlpha").toDouble(),
                                      ct->configuration->getAlgorithmParameter(*name, "MaxIterations").toDouble(),
                                      map);
    algorithm->registerMethod(1, met);
    name = new QString("BAYES");
    met = new NaiveBayes(ct->configuration->getAlgorithmParameter(*name, "FeaturesCount").toInt());
    algorithm->registerMethod(2, met);
    name = new QString("NEIGHBOUR");
    met = new NearestNeighbor(ct->configuration->getAlgorithmParameter(*name, "FeaturesCount").toInt(),
                              ct->configuration->getAlgorithmParameter(*name, "K").toInt());
    algorithm->registerMethod(3, met);
    name = new QString("DENSITY");
    met = new DensityMethod(ct->configuration->getAlgorithmParameter(*name, "LOFResultDeviation").toDouble(),
                            ct->configuration->getAlgorithmParameter(*name, "Neighbors").toInt());
    algorithm->registerMethod(4, met);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseObject(){
    choosenObjectId = ui->comboBox_2->currentIndex();
    ct->configuration->setPropertyValue("SelectedObject", "Object_ID", choosenObjectId );
    ui->label_12->setText(ui->comboBox_2->currentText());
}

void MainWindow::connectDatabase(){
    ct->configuration->setPropertyValue("Database", "Server", ui->lineEdit->text());
    ct->configuration->setPropertyValue("Database", "Source", ui->lineEdit_2->text() );
    ct->configuration->setPropertyValue("Database", "UserName", ui->lineEdit_3->text() );
    ct->configuration->setPropertyValue("Database", "UserPassword", ui->lineEdit_4->text() );

    qDebug() << QString::number(this->objectsData[choosenObjectId][0]);

    ct->createConnection(this->objectsData[choosenObjectId][0]);
}

void MainWindow::loadDataStandard(){// TODO balut a gdzie sa metody do tego?
    int begin = ui->spinBox->value();
    int end = ui->spinBox_2->value();
/*
    DataLoader* dl = ct->loader;
    QProgressDialog progress("Pobieranie rekord�w dla wybranego obiektu...", "Anuluj", 0, 40024, this);
    progress.setWindowModality(Qt::WindowModal);
    dl->progessBar = &progress;
    dl->initDataRecordTable();
    dl->loadAllRecords();
    dl->setAlarmFlagToRecords();
*/
    // TODO TEMP
    int size = ct->dataset->datasetControler->dataset->dataTable->records.size();
    double* min = new double[size];
    double* max = new double[size];
    for(int i=0;i<size;++i){
        min[i]=-1000;
        max[i]=1000;
    }
    ct->dataset->setMinMax(min, max);

    ui->webView->setDataset(ct->dataset->datasetControler->dataset);
    ct->guiController->setLiveLineChart(ui->webView);
    ct->guiController->refreshLiveLineChart();
   // ui->webView->reloadData();
    redrawDataset();
}

void MainWindow::loadDataDate(){// TODO balut a gdzie sa metody do tego?

    ui->webView->setDataset(ct->dataset->datasetControler->dataset);
    redrawDataset();
}

void MainWindow::learnData(){
    int selectedMethodId = ui->comboBox->currentIndex();
    int begin = ui->spinBox_4->value();
    int end = ui->spinBox_3->value();

    ct->dataset->setMethodId(selectedMethodId);
    ct->dataset->teachData(begin, end);
}

void MainWindow::testData(){
    int selectedMethodId = ui->comboBox->currentIndex();
    int begin = ui->spinBox_4->value();
    int end = ui->spinBox_3->value();

    ct->dataset->setMethodId(selectedMethodId);
    ct->dataset->checkData(begin, end);
    redrawDataset();
}

void MainWindow::redrawDataset(){
    //ui->webView->loadData();
    ct->guiController->refreshLiveLineChart();
    //qDebug()<<ui->webView->url();
}

void MainWindow::on_filterValuesBtn_clicked()
{
    DialogFilter dialogFilter;

    std::vector<QListWidgetItem *> items;

    //std::vector<QString> *dataNames = & ct->dataset->datasetControler->dataset->dataTable->dataNames;

    vector<bool> currFilter = ct->guiController->liveLineChart->getFilter();
    QListWidgetItem *item;
    for(unsigned i=0;i<ct->dataset->datasetControler->dataset->dataTable->dataNames.size();i++)
    {
        item = new QListWidgetItem;
        item->setData( Qt::DisplayRole, ct->dataset->datasetControler->dataset->dataTable->dataNames[i] );
        item->setData( Qt::CheckStateRole, currFilter[i]?Qt::Checked : Qt::Unchecked );

        items.push_back(item);
    }


    dialogFilter.addCheckboxesToList(items);
    dialogFilter.setModal(true);

    if(dialogFilter.exec())
    {
        vector<bool> filter(items.size());
        for(unsigned i=0;i<items.size();i++)
            filter[i] = items[i]->checkState() == CHECKBOX_CHECKED;

        ct->guiController->liveLineChart->setFilter(filter);
        ct->guiController->refreshLiveLineChart();
    }


}

void MainWindow::loadAllObjectRecords()
{
    DataLoader* dl = ct->loader;
    QProgressDialog progress("Pobieranie rekord�w dla wybranego obiektu...", "Anuluj", 0, this->objectsData[choosenObjectId][1], this);
    progress.setWindowModality(Qt::WindowModal);
    dl->progessBar = &progress;
    dl->initDataRecordTable();
    dl->loadAllRecords();
    dl->setAlarmFlagToRecords();
}
