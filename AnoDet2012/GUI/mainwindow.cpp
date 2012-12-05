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
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDesktopServices>
#include "GUI/datasettableview.h"
#include "Reports/Report.h"
#include <QProgressBar>
#include <GUI/dialogbusy.h>

#define CHECKBOX_CHECKED 2
#define CHART_RECORDS_LIMIT 1000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ct = new ConnectorTracker();
    ct->initialise();

    choosenObjectId = ct->configuration->getObjectId();
    this->statusOfDatabaseConnection = false;
    this->statusOfObjectDataLoad = false;

    ui->webView->setLiveMode(false);
    ui->livelogChart->setLiveMode(true);

    // USTAWIENIA PÓL
    //ui->comboBox_2->setCurrentIndex(choosenObjectId);
    //ui->label_12->setText(ui->comboBox_2->currentText());
    ui->lineEdit->setText(ct->configuration->getPropertyValue("Database", "Server").toString());
    ui->lineEdit_2->setText(ct->configuration->getPropertyValue("Database", "Source").toString());
    ui->lineEdit_3->setText(ct->configuration->getPropertyValue("Database", "UserName").toString());
    ui->lineEdit_4->setText(ct->configuration->getPropertyValue("Database", "UserPassword").toString());
	
	QStringList headers;
    headers.append("");
    headers.append("");
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setColumnWidth(0, 196);
    ui->treeWidget->setColumnWidth(1, 128);

    ui->comboBox_3->addItem(QString::fromUtf8("poczta elektroniczna"));
    ui->comboBox_3->addItem(QString::fromUtf8("wiadomość SMS"));
    ui->comboBox_3->addItem(QString::fromUtf8("poczta oraz SMS"));
    ui->comboBox_3->addItem(QString::fromUtf8("nie powiadamiaj"));

    createSubscribersTree();
    selectedSubscriber = NULL;
    // KONIEC USTAWIEN PÓL

    // TODO TEMP METODY DETEKCJI
    AlgorithmControler* algorithm = ct->anomalyDetection;
    QString *name;
    Method *met = new RandomMethod();
    algorithm->registerMethod(0, met);
    name = new QString("SOM");
    TopologyMap map(this,ct->configuration->getAlgorithmParameter(*name, "Width").toInt(),
                    ct->configuration->getAlgorithmParameter(*name, "Height").toInt(),
                    10);
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

    name = new QString("RBF");
    met = new RBFNetwork(ct->configuration->getAlgorithmParameter(*name, "LearnFactor").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "SigmoidalActivationFunctionBeta").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "QuantizationErrorThreshold").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "Generations").toInt(),
                         ct->configuration->getAlgorithmParameter(*name, "RandomWeightScaleFactor").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "ClustersNumber").toInt(),
                         ct->configuration->getAlgorithmParameter(*name, "MinSigmaSquare").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "MaxSigmaSquare").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "StepImprovement").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "Iterations").toInt()
                         );
    algorithm->registerMethod(5, met);

    reloadParams();
    ct->mainWindow = this;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(updateSize()));

    /*int w = ui->webView->geometry().width() * 1.11;
    int h = ui->webView->geometry().height() * 1.72;

    ui->webView->setSize(w,h);*/
    //updateSize();

    //qDebug() << ui->webView->geometry().height();
    ui->anomalyDetectionTab->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseObject(){
    choosenObjectId = ui->comboBox_2->currentIndex();
    ct->configuration->setPropertyValue("SelectedObject", "Object_ID", choosenObjectId);
    ui->label_12->setText(ui->comboBox_2->currentText());
}

void MainWindow::connectDatabase(){
    ct->configuration->setPropertyValue("Database", "Server", ui->lineEdit->text());
    ct->configuration->setPropertyValue("Database", "Source", ui->lineEdit_2->text() );
    ct->configuration->setPropertyValue("Database", "UserName", ui->lineEdit_3->text() );
    ct->configuration->setPropertyValue("Database", "UserPassword", ui->lineEdit_4->text() );

    this->statusOfDatabaseConnection = ct->createConnection(4);

    if(this->statusOfDatabaseConnection)
    {
        DataLoader* dl = ct->loader;

        QProgressDialog progress(QString::fromUtf8("Pobieranie informacji o obiektach..."), "Anuluj", 0, 17, this);
        progress.setWindowModality(Qt::WindowModal);
        dl->progessBar = &progress;
        progress.setCancelButton(NULL);
        progress.show();
        this->objectsData = *(ct->loader->loadAllObjectsData());

        vector<QPair<int, QString> >::iterator objectsDataIt;
        for(objectsDataIt = this->objectsData.begin(); objectsDataIt!=this->objectsData.end(); ++objectsDataIt)
        {
            ui->comboBox_2->addItem((*objectsDataIt).second);
        }
        ui->comboBox_2->setCurrentIndex(choosenObjectId);
        ui->label_12->setText(ui->comboBox_2->currentText());
        ui->selectObjectBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);

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

void MainWindow::loadDataStandard(){
    int begin = ui->loadFromSpinBox->value();
    int end = ui->loadToSpinBox->value();

    ui->webView->setDataset(ct->dataset->datasetControler->dataset);
    ui->webView->setInterval(begin, end);

    int count = end - begin;
    if(count > CHART_RECORDS_LIMIT)
    {
        QMessageBox msgBox;;
        msgBox.setInformativeText(QString::fromUtf8("Wybrany przedział zawiera ") + QString::number(count) + QString::fromUtf8(" rekordów.\nMaksymalna liczba rekordów do wyświetlenia to ") + QString::number(CHART_RECORDS_LIMIT) + ".");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(QString::fromUtf8("Przekroczono limit"));

        // workaround for not working setMinimumWidth:
        QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();
    }
    else
    {
        ui->webView->setDataset(ct->dataset->datasetControler->dataset);
        ui->webView->setInterval(begin, end);

        QPair<QDateTime,QDateTime> interval = ct->dataset->getDateTimeRecordInterval(begin, end);

        ui->loadFromDateTime->setDateTime(interval.first);
        ui->loadToDateTime->setDateTime(interval.second);

        ui->fromProcessSpinBox->setValue(begin);
        ui->toProcessSpinBox->setValue(end);

        redrawDataset();
    }


}

void MainWindow::loadDataDate(){
    QDateTime beginDate = ui->loadFromDateTime->dateTime();
    QDateTime endDate = ui->loadToDateTime->dateTime();

    QPair<int,int> interval = ct->dataset->getIndexRecordInterval(beginDate, endDate);


    int count = interval.second - interval.first;
    if(count > CHART_RECORDS_LIMIT)
    {
        QMessageBox msgBox;;
        msgBox.setInformativeText(QString::fromUtf8("Wybrany przedział zawiera ") + QString::number(count) + QString::fromUtf8(" rekordów.\nMaksymalna liczba rekordów do wyświetlenia to ") + QString::number(CHART_RECORDS_LIMIT) + ".");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(QString::fromUtf8("Przekroczono limit"));

        // workaround for not working setMinimumWidth:
        QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();
    }
    else
    {
        ui->webView->setDataset(ct->dataset->datasetControler->dataset);
        ui->webView->setInterval(interval.first, interval.second);

        ui->loadFromSpinBox->setValue(interval.first);
        ui->loadToSpinBox->setValue(interval.second);

        ui->fromProcessSpinBox->setValue(interval.first);
        ui->toProcessSpinBox->setValue(interval.second);

        redrawDataset();
    }

}

void MainWindow::learnData(){
    int selectedMethodId = ui->comboBox->currentIndex();
    int begin = ui->fromProcessSpinBox->value();
    int end = ui->toProcessSpinBox->value();

    ct->dataset->setMethodId(selectedMethodId);
    ct->dataset->teachData(begin, end);
}

void MainWindow::testData(){
    int selectedMethodId = ui->comboBox->currentIndex();
    int begin = ui->fromProcessSpinBox->value();
    int end = ui->toProcessSpinBox->value();

    ct->dataset->setMethodId(selectedMethodId);
    ct->dataset->checkData(begin, end);
    redrawDataset();
}

void MainWindow::redrawDataset(){
    updateSize();
}

void MainWindow::newRecords(unsigned num)
{
    ui->newRecordsNumLabel->setText(QString::number(ui->newRecordsNumLabel->text().toInt() + num));
}

void MainWindow::newAnomalies(unsigned num)
{
    ui->newAnomaliesNumLabel->setText(QString::number(ui->newAnomaliesNumLabel->text().toInt() + num));
}

int MainWindow::getSelectedMethodId()
{
    return ui->comboBox->currentIndex();
}

QString& MainWindow::getSelectedObjectName()
{
    return objectsData[choosenObjectId].second;
}

int MainWindow::getSelectedObjectId()
{
    return objectsData[choosenObjectId].first;
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
    int amountOfObjectRecords = dl->getAmountOfObjectRecords(objectsData[choosenObjectId].first);
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
        ct->loader->objectId = objectsData[choosenObjectId].first;
        QProgressDialog progress(QString::fromUtf8("Pobieranie rekordów dla wybranego obiektu..."), "Anuluj", 0, amountOfObjectRecords, this);
        progress.setWindowModality(Qt::WindowModal);
        dl->progessBar = &progress;
        progress.show();
        progress.setValue(0);
        dl->initDataRecordTable();
        dl->loadAllRecords();

        if(progress.wasCanceled())
            return;

        progress.setLabelText(QString::fromUtf8("Pobieranie danych anomalii dla wybranego obiektu..."));
        progress.setCancelButtonText("Anuluj");
        progress.setMinimum(0);
        progress.setValue(0);

        dl->loadMeasurementInfo();
        dl->setAlarmFlagToRecords();
        ct->dataset->setMinMaxFromDataset();

        if(!progress.wasCanceled())
        {
            this->statusOfObjectDataLoad = true;
            ui->livelogTab->setEnabled(true);
            ui->anomalyDetectionTab->setEnabled(true);

            ui->dataOverviewTab->setEnabled(true);
            ui->refreshDatabaseTable->setEnabled(true);
            ui->datasetTableView->setEnabled(true);

            ui->reportsTab->setEnabled(true);
            ui->subscriptionTab->setEnabled(true);

            ui->reportDataRangeFrom->setMaximum(this->ct->dataset->datasetControler->dataset->dataTable->getLength()-2);
            ui->reportDataRangeTo->setMaximum(this->ct->dataset->datasetControler->dataset->dataTable->getLength()-1);
            ui->reportDataRangeTo->setValue(this->ct->dataset->datasetControler->dataset->dataTable->getLength()-1);


            QMessageBox msgBoxOK;
            msgBoxOK.setInformativeText(QString::fromUtf8("Dane zostały pobrane."));
            msgBoxOK.setStandardButtons(QMessageBox::Ok);
            msgBoxOK.setDefaultButton(QMessageBox::Ok);
            msgBoxOK.setIcon(QMessageBox::Information);
            msgBoxOK.setWindowTitle(QString::fromUtf8("Pobrano"));

            // workaround for not working setMinimumWidth:
            QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
            QGridLayout* layout = (QGridLayout*)msgBoxOK.layout();
            layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

            msgBoxOK.exec();
        }

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
    //ct->guiController->refreshLiveLineChart();
    ct->incomingData->startListening();

    ui->livelogStatusLabel->setText(QString::fromUtf8("<b><font color='green'>Uruchomiony</font></b>"));
    ui->livelogStartTimeLabel->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    ui->filterValuesLivelogBtn->setEnabled(true);
    ui->refreshIntervalLabel->setText(QString::number(ct->incomingData->getRefreshInterval()/1000)+ " " + ui->refreshIntervalSpin->suffix());
    ui->newAnomaliesNumLabel->setText("0");
    ui->newRecordsNumLabel->setText("0");

    updateSize();
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

void MainWindow::updateSize()
{
    int w1 = ui->webView->geometry().width() * 1.30;
    int h1 = ui->webView->geometry().height() * 1.0;

    ui->webView->setSize(w1,h1);
    ui->webView->reloadData();

    if(ui->filterValuesLivelogBtn->isEnabled()) //if livelog is running
    {
        int w2 = ui->livelogChart->geometry().width() * 1.30;
        int h2 = ui->livelogChart->geometry().height() * 1.0;

        ui->livelogChart->setSize(w2,h2);
        ui->livelogChart->reloadData();
    }
}


void MainWindow::resizeEvent(QResizeEvent * event)
{
    timer->stop();
    timer->setInterval(100);
    timer->setSingleShot(true);
    timer->start();
}

void MainWindow::rightClicked()
{
    int begin = ui->loadFromSpinBox->value() + ui->chartStepInterval->value();
    int end = ui->loadToSpinBox->value() + ui->chartStepInterval->value();

    QPair<QDateTime,QDateTime> interval = ct->dataset->getDateTimeRecordInterval(begin, end);
    ui->loadFromDateTime->setDateTime(interval.first);
    ui->loadToDateTime->setDateTime(interval.second);

    if(end <= ct->dataset->datasetControler->dataset->getDataRecordsAmmount() && begin >= 0)
    {
        ui->loadFromSpinBox->setValue(begin);
        ui->loadToSpinBox->setValue(end);
        ui->fromProcessSpinBox->setValue(begin);
        ui->toProcessSpinBox->setValue(end);
        ui->webView->setInterval(begin, end);
        redrawDataset();
    }
}

void MainWindow::leftClicked()
{
    int begin = ui->loadFromSpinBox->value() - ui->chartStepInterval->value();
    int end = ui->loadToSpinBox->value() - ui->chartStepInterval->value();

    QPair<QDateTime,QDateTime> interval = ct->dataset->getDateTimeRecordInterval(begin, end);
    ui->loadFromDateTime->setDateTime(interval.first);
    ui->loadToDateTime->setDateTime(interval.second);

    if(end <= ct->dataset->datasetControler->dataset->getDataRecordsAmmount() && begin >= 0)
    {
        ui->loadFromSpinBox->setValue(begin);
        ui->loadToSpinBox->setValue(end);
        ui->fromProcessSpinBox->setValue(begin);
        ui->toProcessSpinBox->setValue(end);
        ui->webView->setInterval(begin, end);
        redrawDataset();
    }
}

void MainWindow::refreshDatabaseTable(){
    DataRecordTable *dataTable = ct->dataset->datasetControler->dataset->dataTable;
    int columns = dataTable->dataNames.size();
    int rows = dataTable->records.size();
    DatasetTableView *model = new DatasetTableView(rows, columns+1, this);
    model->setDataRecords(dataTable);
    QString *dataCzas = new QString("Data i czas");
    model->setHorizontalHeaderItem(0, new QStandardItem(*dataCzas));
    for(int i=1; i<=columns; ++i){
        model->setHorizontalHeaderItem(i, new QStandardItem(dataTable->dataNames[i-1]));
    }

    ui->datasetTableView->setModel(model);
}

void MainWindow::changeMethodParams(){
    int openedTab = ui->tabWidget_3->currentIndex();
    switch(openedTab){
    case 0:
        changeSOMParams();
        break;
    case 1:
        changeRBFParams();
        break;
    case 2:
        changeBayesParams();
        break;
    case 3:
        changeNeighbourParams();
        break;
    case 4:
        changeDensityParams();
        break;
    default:
        break;
    }

    QMessageBox msgBox;
    msgBox.setInformativeText(QString::fromUtf8("Nowe parametry dla wybranej metody zostały zastosowane."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(QString::fromUtf8("Parametry"));

    // workaround for not working setMinimumWidth:
    QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    msgBox.exec();
}

void MainWindow::openMethodsHelpWindow(){
    ui->webView->settings()->setDefaultTextEncoding("iso-8859-2");
    ui->webView->load(QUrl("qrc:///help/methodParametersHelp.html"));
}

void MainWindow::reloadParams(){
    //SOM
    QString *name;
    name = new QString("SOM");
    ui->som_width->setValue(ct->configuration->getAlgorithmParameter(*name, "Width").toInt());
    ui->som_height->setValue(ct->configuration->getAlgorithmParameter(*name, "Height").toInt());
    ui->som_inputs->setValue(ct->configuration->getAlgorithmParameter(*name, "Inputs").toInt());
    ui->som_radius->setValue(ct->configuration->getAlgorithmParameter(*name, "MaxRadius").toDouble());
    ui->som_alpha->setValue(ct->configuration->getAlgorithmParameter(*name, "MaxAlpha").toDouble());
    ui->som_iters->setValue(ct->configuration->getAlgorithmParameter(*name, "MaxIterations").toDouble());

    //RBF
    name = new QString("RBF");
    ui->rbf_alpha->setValue(ct->configuration->getAlgorithmParameter(*name, "LearnFactor").toDouble());
    ui->rbf_beta->setValue(ct->configuration->getAlgorithmParameter(*name, "SigmoidalActivationFunctionBeta").toDouble());
    ui->rbf_iterations->setValue(ct->configuration->getAlgorithmParameter(*name, "Generations").toInt());
    ui->rbf_randomWeightsDeviation->setValue(ct->configuration->getAlgorithmParameter(*name, "RandomWeightScaleFactor").toDouble());
    ui->rbf_clusters->setValue(ct->configuration->getAlgorithmParameter(*name, "ClustersNumber").toInt());
    ui->rbf_minSquare->setValue(ct->configuration->getAlgorithmParameter(*name, "MinSigmaSquare").toDouble());
    ui->rbf_maxSquare->setValue(ct->configuration->getAlgorithmParameter(*name, "MaxSigmaSquare").toDouble());
    ui->rbf_minStepImprovement->setValue(ct->configuration->getAlgorithmParameter(*name, "StepImprovement").toDouble());


    //Bayes
    name = new QString("BAYES");
    ui->bayes_features->setValue(ct->configuration->getAlgorithmParameter(*name, "FeaturesCount").toInt());


    //Neighbour
    name = new QString("NEIGHBOUR");
    ui->neighbour_features->setValue(ct->configuration->getAlgorithmParameter(*name,"FeaturesCount").toInt());
    ui->neighbour_k->setValue(ct->configuration->getAlgorithmParameter(*name,"K").toInt());


    //Density
    name = new QString("DENSITY");
    ui->density_deviation->setValue(ct->configuration->getAlgorithmParameter(*name,"LOFResultDeviation").toDouble());
    ui->density_neighbours->setValue(ct->configuration->getAlgorithmParameter(*name,"Neighbors").toInt());
}

void MainWindow::changeBayesParams(){
    QString *name = new QString("Algorithm.BAYES");
    ct->configuration->setPropertyValue(*name,"FeaturesCount",ui->bayes_features->value());

    Method *met = new NaiveBayes(ct->configuration->getPropertyValue(*name, "FeaturesCount").toInt());
    ct->anomalyDetection->registerMethod(2, met);
}

void MainWindow::changeDensityParams(){
    QString *name = new QString("Algorithm.DENSITY");
    ct->configuration->setPropertyValue(*name,"LOFResultDeviation",ui->density_deviation->value());
    ct->configuration->setPropertyValue(*name,"Neighbors",ui->density_neighbours->value());

    Method *met = new DensityMethod(ct->configuration->getPropertyValue(*name, "LOFResultDeviation").toDouble(),
                            ct->configuration->getPropertyValue(*name, "Neighbors").toInt());
    ct->anomalyDetection->registerMethod(4, met);
}

void MainWindow::changeNeighbourParams(){
    QString *name = new QString("Algorithm.NEIGHBOUR");
    ct->configuration->setPropertyValue(*name,"FeaturesCount",ui->neighbour_features->value());
    ct->configuration->setPropertyValue(*name,"K",ui->neighbour_k->value());

    Method *met = new NearestNeighbor(ct->configuration->getPropertyValue(*name, "FeaturesCount").toInt(),
                              ct->configuration->getPropertyValue(*name, "K").toInt());
    ct->anomalyDetection->registerMethod(3, met);
}

void MainWindow::changeRBFParams(){
    QString* name = new QString("RBF");
    ct->configuration->setPropertyValue(*name, "LearnFactor", ui->rbf_alpha->value());
    ct->configuration->setPropertyValue(*name, "SigmoidalActivationFunctionBeta", ui->rbf_beta->value());
    ct->configuration->setPropertyValue(*name, "Generations", ui->rbf_iterations->value());
    ct->configuration->setPropertyValue(*name, "RandomWeightScaleFactor", ui->rbf_randomWeightsDeviation->value());
    ct->configuration->setPropertyValue(*name, "ClustersNumber", ui->rbf_clusters->value());
    ct->configuration->setPropertyValue(*name, "MinSigmaSquare", ui->rbf_minSquare->value());
    ct->configuration->setPropertyValue(*name, "MaxSigmaSquare", ui->rbf_maxSquare->value());
    ct->configuration->setPropertyValue(*name, "StepImprovement", ui->rbf_minStepImprovement->value());

    Method* met = new RBFNetwork(ct->configuration->getAlgorithmParameter(*name, "LearnFactor").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "SigmoidalActivationFunctionBeta").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "QuantizationErrorThreshold").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "Generations").toInt(),
                         ct->configuration->getAlgorithmParameter(*name, "RandomWeightScaleFactor").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "ClustersNumber").toInt(),
                         ct->configuration->getAlgorithmParameter(*name, "MinSigmaSquare").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "MaxSigmaSquare").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "StepImprovement").toDouble(),
                         ct->configuration->getAlgorithmParameter(*name, "Iterations").toInt()
                         );
    ct->anomalyDetection->registerMethod(5, met);
}

void MainWindow::changeSOMParams(){
    QString *name = new QString("Algorithm.SOM");
    ct->configuration->setPropertyValue(*name,"Width",ui->som_width->value());
    ct->configuration->setPropertyValue(*name,"Height",ui->som_height->value());
    ct->configuration->setPropertyValue(*name,"Inputs",ui->som_inputs->value());
    ct->configuration->setPropertyValue(*name,"MaxRadius",ui->som_radius->value());
    ct->configuration->setPropertyValue(*name,"MaxAlpha",ui->som_alpha->value());
    ct->configuration->setPropertyValue(*name,"MaxIterations",ui->som_iters->value());

    TopologyMap map(this,ct->configuration->getAlgorithmParameter(*name, "Width").toInt(),
                    ct->configuration->getAlgorithmParameter(*name, "Height").toInt(),
                    10);
    Method *met = new SOMNetwork(ct->configuration->getPropertyValue(*name, "Width").toInt(),
                                      ct->configuration->getPropertyValue(*name, "Height").toInt(),
                                      ct->configuration->getPropertyValue(*name, "Inputs").toInt(),
                                      ct->configuration->getPropertyValue(*name, "MaxRadius").toDouble(),
                                      ct->configuration->getPropertyValue(*name, "MaxAlpha").toDouble(),
                                      ct->configuration->getPropertyValue(*name, "MaxIterations").toDouble(),
                                      map);
    ct->anomalyDetection->registerMethod(1, met);
}

void MainWindow::generateReportFromDatabase()
{
    QString absolutePath = QFileDialog::getSaveFileName(this,"Zapisz raport", QString(), tr("Pdf document (*.pdf)"));

    if(absolutePath.trimmed().isEmpty())
            return;

    DialogBusy dialogBusy;
    dialogBusy.setModal(true);
    dialogBusy.setLabelText("Trwa generowanie raportu...");
    dialogBusy.show();

    this->ct->reports->reportFromDatabase(absolutePath);

    dialogBusy.close();

    QMessageBox msgBox;
    msgBox.setInformativeText(QString::fromUtf8("Raport został pomyślnie wygenerowany."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(QString::fromUtf8("Raport"));

    // workaround for not working setMinimumWidth:
    QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    msgBox.exec();
}

void MainWindow::generateReportFromDataRange()
{
    QString absolutePath = QFileDialog::getSaveFileName(this,"Zapisz raport", QString(), tr("Pdf document (*.pdf)"));

    if(absolutePath.trimmed().isEmpty())
        return;

    DialogBusy dialogBusy;
    dialogBusy.setModal(true);
    dialogBusy.setLabelText("Trwa generowanie raportu...");
    dialogBusy.show();

    this->ct->reports->reportFromDataRange(ui->reportDataRangeFrom->value(), ui->reportDataRangeTo->value(), this->getSelectedObjectName(), absolutePath);

    dialogBusy.close();

    QMessageBox msgBox;
    msgBox.setInformativeText(QString::fromUtf8("Raport został pomyślnie wygenerowany."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(QString::fromUtf8("Raport"));

    // workaround for not working setMinimumWidth:
    QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

    msgBox.exec();
}

void MainWindow::discardChanges()
{
    if (selectedSubscriber != NULL)
    {
        readSubscriberInfo();
    }
}

void MainWindow::applyChanges()
{
    if (selectedSubscriber != NULL)
    {
        QString identifier = ui->lineEdit_5->text();
        QString mail = ui->lineEdit_6->text();
        QString phone = ui->lineEdit_7->text();
        NotificationType notification = None;
        if ((ui->comboBox_3->currentText()).compare(QString::fromUtf8("poczta elektroniczna")) == 0)
            notification = Mail;
        else if ((ui->comboBox_3->currentText()).compare(QString::fromUtf8("wiadomość SMS")) == 0)
            notification = SMS;
        else if ((ui->comboBox_3->currentText()).compare(QString::fromUtf8("poczta oraz SMS")) == 0)
            notification = Both;
        else
            notification = None;

        //
        //
        //

        Subscriber *sub = ct->sender->getSubscribersHandler()->getSubscriber(selectedSubscriber->text(0));
        if (sub != NULL)
        {
            sub->identifier = identifier;
            sub->mail = mail;
            sub->phone = phone;
            sub->notification = notification;
        }
        else
        {
            ct->sender->getSubscribersHandler()->addSubscriber(identifier, mail, phone, notification);
        }

        //
        //
        //

        ui->treeWidget->clear();
        createSubscribersTree();

        ui->lineEdit_5->setText("");
        ui->lineEdit_6->setText("");
        ui->lineEdit_7->setText("");
        ui->comboBox_3->setCurrentIndex(3);

        selectedSubscriber = NULL;
        ct->sender->getSubscribersHandler()->saveData();
    }
}

void MainWindow::collapseSubscribers()
{
    ui->treeWidget->collapseAll();
}

void MainWindow::expandSubscribers()
{
    ui->treeWidget->expandAll();
}

void MainWindow::deleteSubscriber()
{
    if (selectedSubscriber != NULL)
    {
        Subscriber *sub = ct->sender->getSubscribersHandler()->getSubscriber(selectedSubscriber->text(0));
        if (sub != NULL)
        {
            ct->sender->getSubscribersHandler()->removeSubscriber(sub);
        }

        ui->treeWidget->clear();
        createSubscribersTree();

        ui->lineEdit_5->setText("");
        ui->lineEdit_6->setText("");
        ui->lineEdit_7->setText("");
        ui->comboBox_3->setCurrentIndex(3);

        selectedSubscriber = NULL;
        ct->sender->getSubscribersHandler()->saveData();
    }
}

void MainWindow::addSubscriber()
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it)
    {
        (*it)->setSelected(false);
        (*it)->setExpanded(false);
        ++it;
    }

    QTreeWidgetItem *sub = new QTreeWidgetItem(ui->treeWidget);
        sub->setText(0, QString("Brak identyfikatora #") + QString::number(ui->treeWidget->model()->rowCount()));
    QTreeWidgetItem *id = new QTreeWidgetItem(sub);
        id->setText(0, "#:");
        id->setText(1, QString::number(ui->treeWidget->model()->rowCount()));
    QTreeWidgetItem *mail = new QTreeWidgetItem(sub);
        mail->setText(0, "E-mail:");
        mail->setText(1, "brak");
    QTreeWidgetItem *phone = new QTreeWidgetItem(sub);
        phone->setText(0, "Telefon:");
        phone->setText(1, "brak");
    QTreeWidgetItem *notification = new QTreeWidgetItem(sub);
        notification->setText(0, "Typ powiadomienia:");
        notification->setText(1, "brak");

    sub->setSelected(true);
    sub->setExpanded(true);

    ui->treeWidget->insertTopLevelItem(0, sub);
    ui->treeWidget->scrollToBottom();

    selectedSubscriber = NULL;
}

void MainWindow::createSubscribersTree()
{
    std::vector<Subscriber> *subscribers = ct->sender->getSubscribersHandler()->getSubscribers();
    std::vector<Subscriber>::iterator i;
    for (i = subscribers->begin(); i != subscribers->end(); ++i)
    {
        QTreeWidgetItem *sub = new QTreeWidgetItem(ui->treeWidget);
            sub->setText(0, (*i).identifier);
        QTreeWidgetItem *id = new QTreeWidgetItem(sub);
            id->setText(0, "#:");
            id->setText(1, QString::number(ui->treeWidget->model()->rowCount()));
        QTreeWidgetItem *mail = new QTreeWidgetItem(sub);
            mail->setText(0, "E-mail:");
            mail->setText(1, (*i).mail);
        QTreeWidgetItem *phone = new QTreeWidgetItem(sub);
            phone->setText(0, "Telefon:");
            phone->setText(1, (*i).phone);
        QTreeWidgetItem *notification = new QTreeWidgetItem(sub);
            notification->setText(0, "Typ powiadomienia:");
            switch ((*i).notification)
            {
                case Mail:
                    notification->setText(1, QString::fromUtf8("poczta elektroniczna"));
                    break;
                case SMS:
                    notification->setText(1, QString::fromUtf8("wiadomość SMS"));
                    break;
                case Both:
                    notification->setText(1, QString::fromUtf8("poczta oraz SMS"));
                    break;
                default:
                    notification->setText(1, QString::fromUtf8("nie powiadamiaj"));
            }
    }
}

QTreeWidgetItem * MainWindow::getSelectedSubscriber()
{
    return selectedSubscriber;
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    while (item->parent() != NULL)
    {
        item = item->parent();
    }
    selectedSubscriber = item;

    readSubscriberInfo();
}

void MainWindow::readSubscriberInfo()
{
    QTreeWidgetItem *item = getSelectedSubscriber();

    ui->lineEdit_5->setText(item->text(0));
    ui->lineEdit_6->setText(item->child(1)->text(1));
    ui->lineEdit_7->setText(item->child(2)->text(1));

    int index = -1;
    if ((item->child(3)->text(1)).compare(QString::fromUtf8("poczta elektroniczna")) == 0)
        index = 0;
    else if ((item->child(3)->text(1)).compare(QString::fromUtf8("wiadomość SMS")) == 0)
        index = 1;
    else if ((item->child(3)->text(1)).compare(QString::fromUtf8("poczta oraz SMS")) == 0)
        index = 2;
    else
        index = 3;
    ui->comboBox_3->setCurrentIndex(index);
}

void MainWindow::on_actionWczytaj_konfiguracj_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Wskaż plik konfiguracji"), QString(), tr("Pliki konfiguracji (*.ini)"));
    if(fileName != NULL)
    {
        ct->configuration->importSettings(fileName);

        QMessageBox msgBox;
        msgBox.setInformativeText(QString::fromUtf8("Konfiguracja została wczytana."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(QString::fromUtf8("Konfiguracja"));

        // workaround for not working setMinimumWidth:
        QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();
    }
}

void MainWindow::on_actionZapisz_konfiguracj_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString::fromUtf8("Zapisz plik konfiguracji"), QString(),  tr("Pliki konfiguracji (*.ini)"));
    if(fileName != NULL)
    {
        ct->configuration->exportSettings(fileName);

        QMessageBox msgBox;
        msgBox.setInformativeText(QString::fromUtf8("Aktualna konfiguracja została zapisana."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(QString::fromUtf8("Plik konfiguracji"));

        // workaround for not working setMinimumWidth:
        QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();
    }
}
