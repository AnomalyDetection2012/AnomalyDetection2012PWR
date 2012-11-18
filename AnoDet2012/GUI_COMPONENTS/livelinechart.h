#ifndef LIVELINECHART_H
#define LIVELINECHART_H

#include <QWebView>
#include <Dataset/Dataset.h>
#include <QDebug>

#include <vector>

class LiveLineChart : public QWebView
{
    Q_OBJECT
public:
    explicit LiveLineChart(QWidget *parent = 0);
    LiveLineChart(QWidget *parent, int width, int height, Dataset *dataset);
    void setDataset(Dataset *dataset);

    void loadData();
    void reloadData();
    void setLogScale();
    void setLinearScale();


    int width;
    int height;

private:
    Dataset *dataset;
    bool logScale;
    
signals:
    
public slots:
    
};

#endif // LIVELINECHART_H
