#include "livelinechart.h"

LiveLineChart::LiveLineChart(QWidget *parent) :
    QWebView(parent)
{
}

LiveLineChart::LiveLineChart(QWidget *parent, int width, int height, Dataset *dataset) :
    QWebView(parent), width(width), height(height), dataset(dataset)
{

}

void LiveLineChart::setDataset(Dataset *dataset)
{
    this->dataset = dataset;
}

void LiveLineChart::loadData()
{
    if(dataset != NULL)
    {
        std::vector <std::vector <double> > values = dataset->getData(0,200);
        std::vector <std::string> dataNames = dataset->dataTable->dataNames;

        int dimension = values[0].size();
        std::vector <bool> anomalies = dataset->getAnomalies(0,200);
qDebug() << "dimension: " << dimension;
qDebug() << "daataanames: " << dataNames.size();
        QString url = "GUI_COMPONENTS/Line.html?&labels=";

        for(int i=0;i<dataNames.size();i++)
        {
            url = url.append(QString::fromStdString(dataNames[i]).append(";"));
        }
        url = url.append("&values=");

        for(int i=0;i<values.size() && i < 10;i++)
        {
            if(!anomalies[i])
            {
                url = url.append(" ,");

                for(int j=0;j<values[0].size();j++)
                {
                    url = url.append(QString::number(values[i][j])).append(',');
                }

                url = url.append(" , ;");
            }
            else //if anomaly
            {
                url = url.append("anomaly,");

                for(int j=0;j<values[0].size();j++)
                {
                    url = url.append(QString::number(values[i][j])).append(',');
                }

                url = url.append("anomaly,possible anomaly!;");
            }

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
