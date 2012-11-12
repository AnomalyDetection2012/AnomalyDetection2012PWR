#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->webView->load(QUrl("Line.html?&l=Wilgotnosc&values='',170,null,null,null,null;'',700,null,null,null,null;'',660,null,null,null,null;'anomaly', 1000, 500, 1500,'anomaly','possible anomaly!';'', 200, 100, 300,null,null"));

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(processNewData()));
    timer->start(5000);


}

void MainWindow::setGraphicsScene(QGraphicsScene *scene)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processNewData()
{
    ui->webView->load(QUrl("Line.html?&l=Wilgotnosc&values='',170,null,null,null,null;'',700,null,null,null,null;'',660,null,null,null,null;'anomaly', 1000, 500, 1500,'anomaly','possible anomaly!';'', 200, 100, 300,null,null;'',820,null,null,null,null"));
}
