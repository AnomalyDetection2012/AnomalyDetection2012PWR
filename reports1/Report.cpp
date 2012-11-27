#include "Report.h"
#include <stdlib.h>
#include <time.h>
#include <QDebug>
#include <QFile>

using namespace std;

Report::Report()
{
    this->generator = new ReportGenerator();
    srand(time(NULL));
}

void Report::reportFromDatabase(QString outputAbsolutePath){

    vector<vector<double> > data;
    vector<double> d1;
    d1.push_back(56546);
    d1.push_back(156435);
    d1.push_back(36345);
    d1.push_back(19123);
    d1.push_back(56324);
    vector<double> d2;
    d2.push_back(5546);
    d2.push_back(15635);
    d2.push_back(3635);
    d2.push_back(1923);
    d2.push_back(5624);
    data.push_back(d1);
    data.push_back(d2);

    vector<QString> labels;
    labels.push_back("Ob 1");
    labels.push_back("Ob 2");
    labels.push_back("Ob 3");
    labels.push_back("Ob 4");
    labels.push_back("Ob 5");

    vector<QString> colors;
    colors.push_back("888888");
    colors.push_back("cccccc");

    this->generator->addVariable("ALARMSSUM", "5423");
    this->generator->addVariable("RECORDSSUM", "786882");
    this->generator->addVariable("BARCHART",this->generator->verticalStackedBarChart(300, data, labels, colors, 50));

    this->generator->generatePDF("report.html", outputAbsolutePath);
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
