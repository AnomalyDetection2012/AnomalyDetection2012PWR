#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ReportGenerator.h"
#include "WebViewPrinter.h"
#include "QFileDialog"
#include <QDebug>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    ReportGenerator* rg = new ReportGenerator();
    rg->ql = w.getLabel();
    rg->addVariable("INFO", "chujaszek");
    vector<vector<double> > d;
    vector<double> d1;
    d1.push_back(80);
    d1.push_back(180);
    d1.push_back(60);
    d1.push_back(20);
    vector<double> d2;
    d2.push_back(80);
    d2.push_back(180);
    d2.push_back(60);
    d2.push_back(20);
    vector<double> d3;
    d3.push_back(10);
    d3.push_back(130);
    d3.push_back(70);
    d3.push_back(420);
    d.push_back(d1);
    d.push_back(d2);
    d.push_back(d3);
    vector<QString> labels;
    labels.push_back("chuj1");
    labels.push_back("chuj2");
    labels.push_back("chuj3");
    labels.push_back("chuj4");
    vector<QString> colors;
    colors.push_back("00FF00");
    colors.push_back("FF0000");
    colors.push_back("0000FF");
    colors.push_back("FFFF00");
    rg->addVariable("CHART", "<img src='"+rg->lineChart(300, 200, d, labels, colors)+"' />");

    qDebug() << rg->lineChart(300, 200, d, labels, colors);

    rg->generatePDF("report.html", "C:/Users/Balut/Desktop/cos.pdf");

    w.show();
    return a.exec();
}
