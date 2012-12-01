#include "Report.h"
#include "connectortracker.h"
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
    }

    this->generator->generatePDF("reportFromDatabase.html", outputAbsolutePath);
}

void Report::reportFromObjectNewRecords(int objectID, QString outputAbsPath)
{
    vector<vector<double> > data;
    for(int a=0; a<100; ++a)
    {
        data.push_back(*(new vector<double>(0)));

        for(int b=0;b<33;++b)
        {
            data[a].push_back(static_cast<double>(rand()%100)/10.0);
        }
    }

    vector<QString> headers;
    for(int a=0;a<20;++a)
    {
        headers.push_back(QString::number(a));
    }

    vector<double> data1;
    data1.push_back(45);
    data1.push_back(5);

    vector<QString> labels;
    labels.push_back("21/10/2012");
    labels.push_back("22/10/2012");
    labels.push_back("23/10/2012");
    labels.push_back("24/10/2012");

    vector<QString> legend;
    legend.push_back("Nie anomalie");
    legend.push_back("Anomalie");

    vector<QString> colors;
    colors.push_back("000000");
    colors.push_back("111111");
    colors.push_back("222222");
    colors.push_back("333333");
    colors.push_back("444444");
    colors.push_back("555555");
    colors.push_back("666666");
    colors.push_back("777777");
    colors.push_back("888888");
    colors.push_back("999999");
    colors.push_back("aaaaaa");
    colors.push_back("bbbbbb");
    colors.push_back("cccccc");
    colors.push_back("dddddd");
    colors.push_back("eeeeee");
    colors.push_back("0000ff");
    colors.push_back("00ffff");
    colors.push_back("ff0000");
    colors.push_back("ffff00");
    colors.push_back("00ff00");

    this->generator->addVariable("OBJECTNAME", "Obiekt 01 - Monitoring");
    this->generator->addVariable("RECORDSSUM", QString::number(data.size()));
    this->generator->addVariable("ALARMSSUM", "5");
    this->generator->addVariable("LINECHART", this->generator->lineChart(600, 300, data, labels, colors));

    qDebug() << this->generator->lineChart(600, 300, data, labels, colors);

    this->generator->addVariable("PIECHART3D",
                                 this->generator->pieChart3D(300, data1, legend, legend, colors));
    this->generator->addVariable("TABLE", this->generator->table(data, &headers));

    this->generator->generatePDF("newRecords.html", outputAbsPath);
}
