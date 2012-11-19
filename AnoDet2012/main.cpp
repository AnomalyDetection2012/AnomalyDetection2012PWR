#include <QtGui/QApplication>
#include "guimainwindow.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "DataLoader/dataloader.h"
#include "Dataset/MeasurementInfo.h"
#include "GUI_COMPONENTS/livelinechart.h"
#include "GUI/mainwindow.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));
    qsrand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


//    GUIMainWindow w;
//    w.show();

//    IncomingDataController *incControl = new IncomingDataController("RAFAL-PC\\SQLEXPRESS","SCSWin","sa","tajnehaslo");
//    IncomingDataController *incControl = new IncomingDataController("BalutKomputer","SCSWin","sa","qwe123!");
//    incControl->startListening();
//    DataLoader* dl = new DataLoader(3, "RAFAL-PC\\SQLEXPRESS", "SCSWin", "sa", "tajnehaslo");
//    dl->initDataRecordTable();
//    dl->loadRecords(2000);
//    dl->setAlarmFlagToRecords();
//    dl->printRecords();
//    dl->removeAllRecords();
//    dl->loadMeasurementInfo();
//    qDebug() << "liczba zaczytanych: " << dl->measurementInfo.size();

//    qDebug() <<"pomiar 10: " << dl->measurementInfo.value(10).value("MeasurementName").toString();




    return a.exec();
}
