#ifndef TOPOLOGYMAP_H
#define TOPOLOGYMAP_H

#include <QWidget>


class TopologyMap : public QWidget
{
    Q_OBJECT
public:
    explicit TopologyMap(QWidget *parent = 0,int columns = 0,int rows = 0,int s = 5);
    
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *event);

public:
    int cols; // number of columns
    int rows;  // number of rows;
    int sizeOfSingleSquare; // size of single square of map; total num. of squares=width * height; default=5[px]
    QVector<QRectF> squares; // squares of map
    QVector<QColor> colors;
    QVector<QRectF> sq;
    QVector<QPoint> neuronPoints;

    
};

#endif // TOPOLOGYMAP_H
