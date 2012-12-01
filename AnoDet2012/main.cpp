#include <QtGui/QApplication>
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "DataLoader/dataloader.h"
#include "Dataset/MeasurementInfo.h"
#include "GUI_COMPONENTS/livelinechart.h"
#include "GUI/mainwindow.h"
#include "Reports/Report.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    qsrand(time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
