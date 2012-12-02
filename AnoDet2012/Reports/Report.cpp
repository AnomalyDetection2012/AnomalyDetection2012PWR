#include "Report.h"
#include "connectortracker.h"
#include "Dataset/Dataset.h"
#include "Dataset/DatasetConnector.h"
#include "Dataset/datasetControler.h"
#include "Dataset/DataRecord.h"
#include "Dataset/DataRecordTable.h"
#include "DataLoader/dataloader.h"
#include <stdlib.h>
#include <time.h>
#include <QDebug>
#include <QFile>
#include <QtSql>

using namespace std;

Report::Report(QSqlDatabase* dbCon)
{
    this->dbConnection = *dbCon;
    this->generator = new ReportGenerator();
    srand(time(NULL));
}

void Report::initialiseConnectors(ConnectorTracker* connectorTracker)
{
    this->ct = connectorTracker;
}

void Report::reportFromDataRange(int begin, int end, QString objectName, QString outputAbsolutePath)
{
    DataRecordTable *selData = this->ct->dataset->datasetControler->dataset->dataTable;
    vector<vector<double> > selectedData = selData->getData(begin, end);
    vector<bool> selectedDetectedAnomalies = selData->getAnomalies(begin, end);
    vector<bool> selectedDatabaseAnomalies = selData->getDatabaseAnomalies(begin, end);
    double min = selectedData[0][0];
    double max = min;

    vector<vector<double> >::iterator selectedDataIt;
    vector<double>::iterator selectedDataRecordIt;
    for(selectedDataIt = selectedData.begin(); selectedDataIt!= selectedData.end(); ++selectedDataIt)
    {
        for(selectedDataRecordIt = (*selectedDataIt).begin(); selectedDataRecordIt!=(*selectedDataIt).end(); ++selectedDataRecordIt)
        {
            if(*selectedDataRecordIt<min)
            {
                min = *selectedDataRecordIt;
            }else if(*selectedDataRecordIt>max)
            {
                max = *selectedDataRecordIt;
            }
        }
    }


    this->generator->addVariable("ID_PIERWSZY_REKORD", QString::number(this->ct->loader->recordIds[begin]));
    this->generator->addVariable("ID_OSTATNI_REKORD", QString::number(this->ct->loader->recordIds[end]));
    this->generator->addVariable("NAZWA_OBIEKTU", objectName);
    this->generator->addVariable("LICZBA_REKORDOW", QString::number(selectedData.size()));

    this->generator->addVariable("TABELA_POMIAROW", this->generator->table(selectedData));

    vector<bool>::iterator dbAnomaliesIt, detectedAnomaliesIt;
    int amountOfDetectedAnomalies = 0;
    int amountOfDatabaseAnomalies = 0;
    selectedDataIt = selectedData.begin();
    for(dbAnomaliesIt = selectedDatabaseAnomalies.begin(), detectedAnomaliesIt = selectedDetectedAnomalies.begin()
        ;dbAnomaliesIt!=selectedDatabaseAnomalies.end(); ++dbAnomaliesIt, ++detectedAnomaliesIt)
    {
        amountOfDetectedAnomalies+=*detectedAnomaliesIt;
        amountOfDatabaseAnomalies+=*dbAnomaliesIt;
        (*selectedDataIt).insert((*selectedDataIt).begin(), *dbAnomaliesIt?max:min);
        (*selectedDataIt).insert((*selectedDataIt).begin(), *detectedAnomaliesIt?max:min);
        ++selectedDataIt;
        //qDebug() << amountOfDetectedAnomalies;
    }

    this->generator->addVariable("LICZBA_ANOMALII_W_BAZIE", QString::number(amountOfDatabaseAnomalies));
    this->generator->addVariable("LICZBA_WYKRYTYCH_ANOMALII", QString::number(amountOfDetectedAnomalies));

    vector<QString> colors;
    colors.push_back("ff0000");
    colors.push_back("00ff00");
    int recordSize = selectedData[0].size()-2;
    for(int a=0; a<recordSize;++a)
    {
        QString color = QString::number(a*(256/recordSize), 16);
        colors.push_back(color.size()==1?color.repeated(6):color.repeated(3));
    }

    this->generator->addVariable("WYKRESY_POMIAROW", this->generator->lineChart(600, 300, selectedData, *(new vector<QString>()), colors, "&chls="+this->generator->assemble(*(new vector<QString>(recordSize+2, "4")), "|")));

    //qDebug() << this->generator->lineChart(600, 300, selectedData, *(new vector<QString>()), colors);

    this->generator->generatePDF("reportFromDataRange.html", outputAbsolutePath);

}

void Report::reportFromDatabase(QString outputAbsolutePath){

    vector<vector<double> > pieChartsData;
    vector<double> pieChart3DData;
    vector<vector<double> > barChartData;
    barChartData.push_back(*(new vector<double>()));
    double recordsSum = 0.0;

    QString statement("SELECT Obiekt_ID, COUNT(*) as Ilosc FROM [Rekord] Group by Obiekt_ID");

    QSqlQuery query(statement, this->dbConnection);
    query.setForwardOnly(true);

    if (!query.isActive()) {
        qDebug() << "1. An error was encountered: "<< QSqlError(query.lastError()).text();
    } else {

        while(query.next())
        {
            barChartData[0].push_back(query.value(1).toDouble());
            pieChartsData.push_back(*(new vector<double>()));
            recordsSum+=query.value(1).toDouble();
            qApp->processEvents();
        }

    }

    QFile file(":/reports/Reports/queries/countingAlarmsGroupedByTypeForObject.sql");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Nie mozna otworzyæ pliku!";
    }

    QTextStream fileStream(&file);

    QString statement1(fileStream.readAll());

    qDebug() << statement1;

    QSqlQuery query1(statement1, this->dbConnection);
    query1.setForwardOnly(true);

    int objectsAmount = pieChartsData.size();

    if (!query1.isActive()) {
        qDebug() << "2. An error was encountered: "<< QSqlError(query1.lastError()).text();
    } else {

        double currentTypeSum = 0.0;

        while(query1.next())
        {
            currentTypeSum = 0.0;
            double currentTypeValue = 0.0;
            for(int o = 0; o<objectsAmount; ++o)
            {
                currentTypeValue = query1.value(o+1).toString().contains("NULL")?0.0:query1.value(o+1).toDouble();
                currentTypeSum += currentTypeValue;
                pieChartsData[o].push_back(currentTypeValue);
                qApp->processEvents();
            }
            pieChart3DData.push_back(currentTypeSum);
        }

    }


    this->generator->addVariable("RECORDSSUM", QString::number(recordsSum));
    this->generator->addVariable("ALARMSSUM", QString::number(pieChart3DData[0]+pieChart3DData[1]+pieChart3DData[2]));


    vector<QString> labels;
    labels.push_back("Alarm pilny");
    labels.push_back("Alarm krytyczny");
    labels.push_back("Zanik sieci");

    vector<QString> colors;
    colors.push_back("00ff00");
    colors.push_back("ff0000");
    colors.push_back("0000ff");

    double alarmsSum = pieChart3DData[0]+pieChart3DData[1]+pieChart3DData[2];
    vector<QString> legend;
    for(int a = 0; a<3; ++a)
    {
        legend.push_back(labels[a]+" - "+QString::number(pieChart3DData[a]/alarmsSum, 'f', 3)+"%");
    }

    vector<QString> barChartColors;
    barChartColors.push_back("0000aa");

    vector<QString> barChartLabels;
    for(int a=1; a<=objectsAmount; ++a)
    {
        barChartLabels.push_back(QString::number(a));
    }

    this->generator->addVariable("BARCHARTRECORDSFOREACHOBJECT", this->generator->verticalStackedBarChart(300, barChartData, barChartLabels, barChartColors, 30, QString("")));

    this->generator->addVariable("PIECHARTALL",this->generator->pieChart3D(300, pieChart3DData, labels, legend, colors));

    vector<QString> objLegend;

    for(int o=0; o<objectsAmount; ++o)
    {
        objLegend.clear();
        double curObjAlarmSum = pieChartsData[o][0]+pieChartsData[o][1]+pieChartsData[o][2];
        for(int a = 0; a<3; ++a)
        {
            objLegend.push_back(QString::number(pieChartsData[o][a]/curObjAlarmSum, 'f', 3)+"%");
        }
        this->generator->addVariable("PIECHARTOBJECT"+QString::number(o+1), this->generator->pieChart(200, pieChartsData[o], labels, objLegend, colors));
        qDebug() <<"PIECHARTOBJECT"+QString::number(o)<< this->generator->pieChart(200, pieChartsData[o], labels, objLegend, colors);
        this->generator->addVariable("OBJECTNAME"+QString::number(o+1), "Obiekt "+QString::number(o+1));

        qApp->processEvents();
    }

    this->generator->generatePDF("reportFromDatabase.html", outputAbsolutePath);
}
