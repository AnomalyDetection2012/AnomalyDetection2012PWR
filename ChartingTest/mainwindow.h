#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsWebView>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void setGraphicsScene(QGraphicsScene *scene);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    QTimer *timer;
public slots:
   void processNewData();  // process new data (if exists)
};

#endif // MAINWINDOW_H
