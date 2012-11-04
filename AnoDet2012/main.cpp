#include <QtGui/QApplication>
#include "guimainwindow.h"
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "DataLoader/dataloader.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));
    qsrand(time(NULL));
    QApplication a(argc, argv);
    GUIMainWindow w;
//    w.randomTest(60);
    w.show();

    //IncomingDataController *incControl = new IncomingDataController("RAFAL-PC\\SQLEXPRESS","SCSWin","sa","tajnehaslo");
    //IncomingDataController *incControl = new IncomingDataController("BalutKomputer","SCSWin","sa","qwe123!");
    //incControl->startListening();
    DataLoader* dl = new DataLoader(4, "BalutKomputer", "SCSWin", "sa", "qwe123!");
    dl->initDataRecordTable();
    dl->loadRecords(1000);
    dl->printRecords();
    return a.exec();
}
