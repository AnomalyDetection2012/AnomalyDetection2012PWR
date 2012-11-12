#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    

    QGraphicsScene scene;

         QGraphicsView view(&scene);








    return a.exec();
}
