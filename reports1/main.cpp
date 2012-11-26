#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ReportGenerator.h"
#include "WebViewPrinter.h"
#include "Report.h"
#include "QFileDialog"
#include <QDebug>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Report* r = new Report();
    //r->reportFromDatabase(QString("C:/Users/Balut/Desktop/cos.pdf"));
    r->reportFromObjectNewRecords(11, "C:/Users/Balut/Desktop/cos1.pdf");

    w.show();
    return a.exec();
}
