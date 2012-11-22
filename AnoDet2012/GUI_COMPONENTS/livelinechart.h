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
    void initFilter(int size);

    void loadData();
    void reloadData();
    void setLogScale();
    void setLinearScale();
    void setFilter(std::vector<bool> filter);
    void removeFilter();
    std::vector<bool> getFilter();
    void setInterval(int begin, int end);

    void setLiveMode(bool mode);


    int width;
    int height;

private:
    Dataset *dataset;
    bool logScale;
    std::vector<bool> filter;
    int begin;
    int end;
    bool liveMode;
    
signals:
    
public slots:
    
};

#endif // LIVELINECHART_H
