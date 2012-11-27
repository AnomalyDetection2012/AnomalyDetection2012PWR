#include <QtGui/QApplication>
#include "INCOMING_DATA_TRACKING/IncomingDataController.h"
#include "DataLoader/dataloader.h"
#include "Dataset/MeasurementInfo.h"
#include "GUI_COMPONENTS/livelinechart.h"
#include "GUI/mainwindow.h"


#include "ConfigurationHandler/configurationhandler.h"
#include "Alarms/notificationsender.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));
    qsrand(time(NULL));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    /*QApplication a(argc, argv);

    ConfigurationHandler *config = new ConfigurationHandler();
    NotificationSender *sender = new NotificationSender(config);

    QString name = "obiekt";
    QDateTime dateTime = QDateTime::currentDateTime();
    std::vector<QString> dataNames;
        dataNames.push_back(QString("Temperatura"));
    std::vector<QString> units;
        units.push_back(QString("C"));
    std::vector<double> values;
        values.push_back(44.5);
    std::vector<double> mins;
        mins.push_back(-90.0);
    std::vector<double> maxs;
        maxs.push_back(90.0);

    sender->sendNotifications(name, dateTime, dataNames, values, units, mins, maxs);*/

    return a.exec();
}
