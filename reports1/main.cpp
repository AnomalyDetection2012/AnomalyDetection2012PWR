#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ReportGenerator.h"
#include "WebViewPrinter.h"
#include "QFileDialog"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    ReportGenerator* rg = new ReportGenerator();
    rg->ql = w.getLabel();
    rg->addVariable("INFO", "chujaszek");
    QList<double> d;
    d.push_back(80);
    d.push_back(180);
    d.push_back(60);
    d.push_back(20);
    QList<QString> labels;
    labels.push_back("chuj1");
    labels.push_back("chuj2");
    labels.push_back("chuj3");
    labels.push_back("chuj4");
    QList<QString> colors;
    colors.push_back("00FF00");
    colors.push_back("FF0000");
    rg->addVariable("CHART", "<img src='"+rg->pieChart3D(200, d, labels, labels, colors)+"' />");

    qDebug() << rg->pieChart3D(200, d, labels, labels, colors);

    rg->generatePDF("report.html", "C:/Users/Balut/Desktop/cos.pdf");

    w.show();
    return a.exec();
}
