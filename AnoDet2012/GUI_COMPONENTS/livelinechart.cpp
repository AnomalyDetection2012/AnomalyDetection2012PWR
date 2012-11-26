#include "livelinechart.h"

LiveLineChart::LiveLineChart(QWidget *parent) :
    QWebView(parent)
{
    this->logScale = false;
    this->liveMode = true;
    this->width = 660;
    this->height = 350;
}

LiveLineChart::LiveLineChart(QWidget *parent, int width, int height, Dataset *dataset) :
    QWebView(parent), width(width), height(height), dataset(dataset)
{
    this->logScale = false;
    this->liveMode = true;
    this->width = 660;
    this->height = 350;
}

void LiveLineChart::setDataset(Dataset *dataset)
{
    this->dataset = dataset;
}

void LiveLineChart::initFilter(int size)
{
    this->filter.resize(size);
    for(unsigned i=0;i<size;i++)
        this->filter[i] = true; //show all
}

void LiveLineChart::setLogScale()
{
    this->logScale = true;
    this->loadData();
}

void LiveLineChart::setLinearScale()
{
    this->logScale = false;
    this->loadData();
}

void LiveLineChart::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void LiveLineChart::loadData()
{
    if(dataset != NULL)
    {
        if(this->liveMode)
        {
            int recordsNum = dataset->getDataRecordsAmmount();
            int showLastRecords = 50;   // show last 50 records on chart
            std::vector <std::vector <double> > values = dataset->getData(recordsNum - showLastRecords < 0 ? 0 : recordsNum - showLastRecords, recordsNum);
            std::vector <QString> dataNames = dataset->dataTable->dataNames;
            std::vector <int> programMeasurementIds = dataset->dataTable->programPomiarIds;

            if(!this->filter.size())
                initFilter(dataNames.size());

            std::vector <bool> anomalies = dataset->getAnomalies(recordsNum - showLastRecords < 0 ? 0 : recordsNum - showLastRecords, recordsNum);

            QString url = "qrc:///googleChart/Line.html?&logscale=";
            url = url.append(this->logScale?"true":"false");

            url = url.append(QString("&width=%1&height=%2").arg(this->width,0,10).arg(this->height,0,10));

            url = url.append("&labels=");
            for(int i=0;i<dataNames.size();i++)
            {
                if(this->filter[i])
                    url = url.append(dataNames[i]).append("[").append(dataset->dataTable->measurementsInfos->getUnit(programMeasurementIds[i])).append("];");
            }

            url = url.append("&values=");
            for(int i=0;i<values.size() && i < showLastRecords;i++)
            {

                if(!anomalies[i])
                {
                    url = url.append(" ,");

                    for(int j=0;j<values[0].size();j++)
                    {
                        if(this->filter[j])
                            url = url.append(QString::number(values[i][j])).append(',');
                    }

                    url = url.append(" , ;");
                }
                else //if anomaly
                {
                    url = url.append("anomalia,");

                    for(int j=0;j<values[0].size();j++)
                    {
                        if(this->filter[j])
                            url = url.append(QString::number(values[i][j])).append(',');
                    }

                    url = url.append("anomalia,wykryto anomaliê;");
                }

            }

            //qDebug() << url;
            this->load(QUrl(url));

        }
        else if(this->begin < this->end && this->begin >= 0)// if live mode disabled and beging and are correct
        {
            std::vector <std::vector <double> > values = dataset->getData(this->begin,this->end);
            std::vector <QString> dataNames = dataset->dataTable->dataNames;
            std::vector <int> programMeasurementIds = dataset->dataTable->programPomiarIds;

            if(!this->filter.size())
                initFilter(dataNames.size());

            std::vector <bool> anomalies = dataset->getAnomalies(begin,end);
            std::vector <bool> databaseAnomalies = dataset->getDatabaseAnomalies(begin, end);

            QString url = "qrc:///googleChart/Line.html?&logscale=";
            url = url.append(this->logScale?"true":"false");

            url = url.append(QString("&width=%1&height=%2").arg(this->width,0,10).arg(this->height,0,10));

            url = url.append("&labels=");
            for(int i=0;i<dataNames.size();i++)
            {
                if(this->filter[i])
                    url = url.append(dataNames[i]).append("[").append(dataset->dataTable->measurementsInfos->getUnit(programMeasurementIds[i])).append("];");
            }

            url = url.append("&values=");
            for(int i=0;i<values.size();i++)
            {

                if(!anomalies[i] && !databaseAnomalies[i])
                {
                    url = url.append(" ,");

                    for(int j=0;j<values[0].size();j++)
                    {
                        if(this->filter[j])
                            url = url.append(QString::number(values[i][j])).append(',');
                    }

                    url = url.append(" , ;");
                }
                else //if anomaly
                {
                    url = url.append("anomalia,");

                    for(int j=0;j<values[0].size();j++)
                    {
                        if(this->filter[j])
                            url = url.append(QString::number(values[i][j])).append(',');
                    }

                    if(anomalies[i] && databaseAnomalies[i])
                        url = url.append("zgodnoœæ,pokrywaj¹ca siê wykryta anomalia z BD;");
                    else if(anomalies[i])
                        url = url.append("anomalia,wykryto anomaliê;");
                    else
                        url = url.append("anomalia(BD),anomalia pobrana z BD;");

                }

            }
            //qDebug() << url;
            this->load(QUrl(url));
        }
    }
    else
    {
        qDebug() << "[ERROR]: LiveLineChart: dataset not defined";
    }
}

void LiveLineChart::reloadData()
{
    this->loadData();
}

void LiveLineChart::setFilter(std::vector<bool> filter)
{
    this->filter = filter;
}

void LiveLineChart::removeFilter()
{
    this->filter.clear();
}

std::vector<bool> LiveLineChart::getFilter()
{
    return this->filter;
}

void LiveLineChart::setInterval(int begin, int end)
{
    this->begin = begin;
    this->end = end;
}

void LiveLineChart::setLiveMode(bool mode)
{
    this->liveMode = mode;
}
