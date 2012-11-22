#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConfigurationHandler/configurationhandler.h"
#include "ANOMALY_DETECTION/algorithmcontroler.h"
#include "DataLoader/dataloader.h"
#include "Dataset/DatasetConnector.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "GUI_COMPONENTS/guicontroller.h"
#include "GUI/dialogfilter.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include <QSqlError>
#include <QDateTime>
#include <QSpacerItem>
#include <QGridLayout>

#define CHECKBOX_CHECKED 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ct = new ConnectorTracker();
    ct->initialise();
    choosenObjectId = ct->configuration->getObjectId();

    ui->webView->setLiveMode(false);
    ui->livelogChart->setLiveMode(true);

    // USTAWIENIA PÓL
    ui->comboBox_2->setCurrentIndex(choosenObjectId);
    ui->label_12->setText(ui->comboBox_2->currentText());
    ui->lineEdit->setText(ct->configuration->getPropertyValue("Database", "Server").toString());
    ui->lineEdit_2->setText(ct->configuration->getPropertyValue("Database", "Source").toString());
    ui->lineEdit_3->setText(ct->configuration->getPropertyValue("Database", "UserName").toString());
    ui->lineEdit_4->setText(ct->configuration->getPropertyValue("Database", "UserPassword").toString());
    // KONIEC USTAWIEN PÓL

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


    ct->mainWindow = this;
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

    bool status = ct->createConnection(4);

    if(status)
    {
        this->objectIDs = ct->loader->loadAllObjectIDs();
        ui->connectionStatusLabel->setText(QString::fromUtf8("<font color='green'>Połączono</font>"));
        ui->statusBar->showMessage(QString::fromUtf8("   Połączenie ze wskazaną bazą danych zostało nawiązane"),10000);
    }
    else
    {
        QString msg(QSqlError(ct->dbConnection->lastError()).text());
        ui->connectionStatusLabel->setText(QString::fromUtf8("<font color='red'>Błąd połączenia </font>"));

        ui->statusBar->showMessage(msg,10000);
    }

}

void MainWindow::loadDataStandard(){// TODO balut a gdzie sa metody do tego?
    int begin = ui->spinBox->value();
    int end = ui->spinBox_2->value();
/*
    DataLoader* dl = ct->loader;
    QProgressDialog progress("Pobieranie rekordów dla wybranego obiektu...", "Anuluj", 0, 40024, this);
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
    ui->webView->setInterval(begin, end);

    ui->webView->reloadData();
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
    ui->webView->reloadData();
    //qDebug()<<ui->webView->url();
}

void MainWindow::newRecords(unsigned num)
{
    ui->newRecordsNumLabel->setText(QString::number(ui->newRecordsNumLabel->text().toInt() + num));
}

void MainWindow::newAnomalies(unsigned num)
{
    ui->newAnomaliesNumLabel->setText(QString::number(ui->newAnomaliesNumLabel->text().toInt() + num));
}

void MainWindow::on_filterValuesBtn_clicked()
{
    DialogFilter dialogFilter;

    std::vector<QListWidgetItem *> items;

    std::vector<bool> currFilter = ui->webView->getFilter();
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

        ui->webView->setFilter(filter);
        ui->webView->reloadData();
    }


}

void MainWindow::loadAllObjectRecords()
{
    DataLoader* dl = ct->loader;

    QMessageBox msgBox;
    int amountOfObjectRecords = dl->getAmountOfObjectRecords(objectIDs[choosenObjectId]);
    msgBox.setInformativeText(QString::fromUtf8("Czy chcesz załadować ") + QString::number(amountOfObjectRecords) + QString::fromUtf8(" rekordów?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(QString::fromUtf8("Pobieranie danych"));

    // workaround for not working setMinimumWidth:
    QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    if(msgBox.exec()==QMessageBox::Yes)
    {
        ct->loader->objectId = objectIDs[choosenObjectId];
        QProgressDialog progress(QString::fromUtf8("Pobieranie rekordów dla wybranego obiektu..."), "Anuluj", 0, amountOfObjectRecords, this);
        progress.setWindowModality(Qt::WindowModal);
        dl->progessBar = &progress;
        dl->initDataRecordTable();
        dl->loadAllRecords();
    }
}

void MainWindow::showAnomaliesFromDB()
{
    ct->loader->setAlarmFlagToRecords();
    ui->webView->reloadData();
}

void MainWindow::startLivelog()
{
    ui->livelogChart->setDataset(ct->dataset->datasetControler->dataset);
    ct->guiController->setLiveLineChart(ui->livelogChart);
    ct->guiController->refreshLiveLineChart();
    ct->incomingData->startListening();

    ui->livelogStatusLabel->setText(QString::fromUtf8("<b><font color='green'>Uruchomiony</font></b>"));
    ui->livelogStartTimeLabel->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    ui->filterValuesLivelogBtn->setEnabled(true);
    ui->refreshIntervalLabel->setText(QString::number(ct->incomingData->getRefreshInterval()/1000)+ " " + ui->refreshIntervalSpin->suffix());
    ui->newAnomaliesNumLabel->setText("0");
    ui->newRecordsNumLabel->setText("0");
}

void MainWindow::stopLivelog()
{
    ct->incomingData->stopListening();
    ui->livelogStatusLabel->setText(QString::fromUtf8("<b>Zatrzymany</b>"));
    ui->livelogStartTimeLabel->setText("-");
    ui->filterValuesLivelogBtn->setEnabled(false);
    ui->refreshIntervalLabel->setText("-");
}

void MainWindow::filterValuesLivelog()
{
    DialogFilter dialogFilter;

    std::vector<QListWidgetItem *> items;

    std::vector<bool> currFilter =ct->guiController->liveLineChart->getFilter();
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


void MainWindow::setRefreshInterval()
{
    ct->incomingData->setRefreshInterval(1000 * ui->refreshIntervalSpin->value());
    ui->refreshIntervalLabel->setText(QString::number(ui->refreshIntervalSpin->value()) + " " + ui->refreshIntervalSpin->suffix());
}
