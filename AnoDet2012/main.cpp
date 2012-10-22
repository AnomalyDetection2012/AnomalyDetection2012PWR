#include <QtGui/QApplication>
#include "guimainwindow.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));
    qsrand(time(NULL));
    QApplication a(argc, argv);
    GUIMainWindow w;
//    w.randomTest(60);
    w.show();

    return a.exec();
}
