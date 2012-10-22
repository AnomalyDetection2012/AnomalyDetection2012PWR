#include "topologymap.h"
#include <QPen>
#include <QPainter>

TopologyMap::TopologyMap(QWidget *parent, int columns, int rows, int s) :
    QWidget(parent),cols(columns),rows(rows),sizeOfSingleSquare(s)
{
    this->setGeometry(0,0,this->sizeOfSingleSquare * this->cols, this->sizeOfSingleSquare * this->rows);

    //parent->setGeometry(100,100,this->width()+100,this->height()+40);

    for(int i=0;i<this->rows;i++)
        for(int j=0;j<this->cols;j++)
        {
            squares << QRectF(i*this->sizeOfSingleSquare,j*this->sizeOfSingleSquare,i*this->sizeOfSingleSquare + this->sizeOfSingleSquare, j*this->sizeOfSingleSquare + this->sizeOfSingleSquare);
            colors.push_back(QColor(255,255,255));
        }
}


void TopologyMap::paintEvent(QPaintEvent *event)
{
    QPen pen(Qt::black, 1, Qt::SolidLine);

    QPainter painter(this);
    painter.setPen(pen);

    /*QRadialGradient gradient(this->width()/2, this->height()/2, 100, 100, 50);
        gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));


    for(int i=0;i<this->squares.size();i++)
        painter.fillRect(this->squares.at(i),gradient);*/

    /*for(int i=0;i<this->squares.size();i++)
        painter.fillRect(this->squares.at(i),colors.at(i));*/

    //painter.drawRects(this->squares);

    //painter.drawRects(this->sq);

    for(int i=0;i<neuronPoints.size();i++)
        //painter.drawArc(neuronPoints.at(i).x(),neuronPoints.at(i).y(),10,10,360,1);
        painter.drawEllipse(neuronPoints.at(i),this->sizeOfSingleSquare/2,this->sizeOfSingleSquare/2);

    pen = QPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(pen);



    painter.drawRect(0,0,this->width()-1,this->height()-1); //frame
}
