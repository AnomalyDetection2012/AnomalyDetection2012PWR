#include "livelinechart.h"

LiveLineChart::LiveLineChart(QWidget *parent) :
    QWebView(parent)
{
}

LiveLineChart::LiveLineChart(QWidget *parent, int width, int height, Dataset *dataset) :
    QWebView(parent), width(width), height(height), dataset(dataset)
{

}

void LiveLineChart::loadData()
{
    if(dataset != NULL)
    {
        std::vector <std::vector <double> > values = dataset->getData(0,100);
        //std::vector <std::string> dataNames = dataset->dataTable->dataNames;

        int dimension = values[0].size();
        std::vector <bool> anomalies = dataset->getAnomalies(0,100);

        QString url = "Line.html?&labels=";

       /* for(int i=0;i<dataNames.size();i++)
        {
            url = url.append(QString::fromStdString(dataNames[i]).append(";"));
        }*/

        for(int i=0;i<values.size();i++)
            for(int j=0;j<values[0].size();j++)
            {
                if(!anomalies[i])
                {
                    url = url.append("'',").append(QString::number(values[i][j])).append(",null,null,null,null;");
                }
                else //if anomaly
                {
                    url = url.append("'anomaly',").append(QString::number(values[i][j])).append(",0,100,'anomaly','possible anomaly';");
                }
                break;
            }

        qDebug() << url;
        this->load(QUrl(url));
    }
    else
    {
        qDebug() << "[ERROR]: LiveLineChart: dataset not defined";
    }
}

void LiveLineChart::reloadData()
{

}
