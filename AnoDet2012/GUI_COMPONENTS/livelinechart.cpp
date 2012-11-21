#include "livelinechart.h"

LiveLineChart::LiveLineChart(QWidget *parent) :
    QWebView(parent)
{
    this->logScale = false;
}

LiveLineChart::LiveLineChart(QWidget *parent, int width, int height, Dataset *dataset) :
    QWebView(parent), width(width), height(height), dataset(dataset)
{
    this->logScale = false;
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

void LiveLineChart::loadData()
{
    if(dataset != NULL)
    {
        int recordsNum = dataset->getDataRecordsAmmount();
        int showLastRecords = 50;   // show last 50 records on chart
        std::vector <std::vector <double> > values = dataset->getData(recordsNum - showLastRecords < 0 ? 0 : recordsNum - showLastRecords, recordsNum);
        std::vector <QString> dataNames = dataset->dataTable->dataNames;

        if(!this->filter.size())
            initFilter(dataNames.size());

        int dimension = values[0].size();
        std::vector <bool> anomalies = dataset->getAnomalies(recordsNum - showLastRecords < 0 ? 0 : recordsNum - showLastRecords, recordsNum);

        QString url = "GUI_COMPONENTS/Line.html?&logscale=";
        url = url.append(this->logScale?"true":"false");

        url = url.append("&labels=");
        for(int i=0;i<dataNames.size();i++)
        {
            if(this->filter[i])
                url = url.append(dataNames[i].append(";"));
        }

        url = url.append("&values=");
        for(int i=0;i<values.size() && i < 50;i++)
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
                url = url.append("anomaly,");

                for(int j=0;j<values[0].size();j++)
                {
                    if(this->filter[j])
                        url = url.append(QString::number(values[i][j])).append(',');
                }

                url = url.append("anomaly,possible anomaly!;");
            }

        }

        //qDebug() << url;
        this->load(QUrl(url));
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
