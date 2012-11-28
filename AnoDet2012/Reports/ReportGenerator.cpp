#include "ReportGenerator.h"

#include <QFile>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextStream>
#include <QPrinter>
#include <QFileDialog>
#include <QDataStream>
#include <QTextCursor>
#include <QWebView>
#include "WebViewPrinter.h"
#include <vector>

using namespace std;

ReportGenerator::ReportGenerator(QObject *parent) :
    QObject(parent)
{
}

bool ReportGenerator::generatePDF(QString templateFilename, QString outputAbsolutePath)
{
    QFile templateFile(":/input/"+templateFilename);

    if(!templateFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream templateStream(&templateFile);

    QTextDocument document;
    document.setHtml(templateStream.readAll());

    QList<QString> keys = this->vars.keys();
    QList<QString>::iterator keysIt;

    for(keysIt = keys.begin(); keysIt != keys.end(); ++keysIt)
    {
        QTextCursor cursor(&document);
        while(!cursor.isNull())
        {
            cursor = document.find("%"+(*keysIt)+"%", cursor);
            cursor.insertHtml(this->vars[(*keysIt)]);
        }
    }

    QPrinter* printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A4);
    printer->setOutputFormat(QPrinter::PdfFormat);

    printer->setOutputFileName(outputAbsolutePath);

    WebViewPrinter* wv = new WebViewPrinter();
    wv->printer = printer;

    wv->setHtml(document.toHtml());

    return true;

}

QString& ReportGenerator::table(vector<vector<double> > &data, vector<QString> *headers)
{
    QString* table = new QString("<table border='1' cellspacing='0' cellpadding='0' >");

    if(headers!=NULL){
        table->append("<tr>");
        vector<QString>::iterator headersIt;

        for(headersIt = headers->begin(); headersIt!=headers->end(); ++headersIt)
        {
            table->append("<th>"+(*headersIt)+"</th>");
        }
        table->append("</tr>");
    }

    vector<vector<double> >::iterator dataIt;
    vector<double>::iterator dataRecordIt;

    for(dataIt = data.begin(); dataIt!=data.end(); ++dataIt)
    {
        table->append("<tr>");
        for(dataRecordIt = (*dataIt).begin(); dataRecordIt!=(*dataIt).end(); ++dataRecordIt)
        {
            table->append("<td>"+QString::number(*dataRecordIt)+"</td>");
        }
        table->append("</tr>");
    }

    table->append("</table>");

    return *table;
}
QString& ReportGenerator::table(vector<vector<QString> > &data, vector<QString> *headers)
{
    QString* table = new QString("<table border='1' cellspacing='0' cellpadding='0' >");

    if(headers!=NULL){
        table->append("<tr>");
        vector<QString>::iterator headersIt;

        for(headersIt = headers->begin(); headersIt!=headers->end(); ++headersIt)
        {
            table->append("<th>"+(*headersIt)+"</th>");
        }
        table->append("</tr>");
    }

    vector<vector<QString> >::iterator dataIt;
    vector<QString>::iterator dataRecordIt;

    for(dataIt = data.begin(); dataIt!=data.end(); ++dataIt)
    {
        table->append("<tr>");
        for(dataRecordIt = (*dataIt).begin(); dataRecordIt!=(*dataIt).end(); ++dataRecordIt)
        {
            table->append("<td>"+(*dataRecordIt)+"</td>");
        }
        table->append("</tr>");
    }

    table->append("</table>");

    return *table;
}

QString& ReportGenerator::lineChart(int width, int height, vector<vector<double> > &data, vector<QString> &labels, vector<QString> &colors, QString additionalParameters)
{
    int records = data.size();
    int recordSize = data[0].size();

    QString* url = new QString("https://chart.googleapis.com/chart?");

    url->append("cht=lc");
    url->append("&chs="+QString::number(width)+"x"+QString::number(height));

    url->append("&chxt=x,y&chxl=0:|"+this->assemble(labels, QString("|")));

    url->append("&chco="+this->assemble(colors, QString(",")));

    url->append("&chds=a");

    url->append(additionalParameters);

    int urlSize = url->size();

    vector<QString> charts;
    vector<QString> valueSets;
    int record=0;
    while(record<records)
    {
        charts.push_back(*url+"&chd=t:");
        valueSets.clear();
        for(int recordValue = 0; recordValue<recordSize; ++recordValue)
        {
            valueSets.push_back(QString(""));
        }
        for(; record<records;)
        {
            // Counting number of chars in url with new record
            int currentChartUrlLength = urlSize+7+(recordSize-1);
            for(int recordValue = 0; recordValue<recordSize; ++recordValue)
            {
                currentChartUrlLength+=valueSets[recordValue].size();
                currentChartUrlLength+=QString::number(data[record][recordValue]).size();
                currentChartUrlLength+=recordSize;
            }
            if(currentChartUrlLength<=2048)
            {
                // We are trying to use another record, so lets add values to current chart url
                for(int recordValue = 0; recordValue<recordSize; ++recordValue)
                {
                    if(valueSets[recordValue].size()!=0)
                    {
                        valueSets[recordValue].append(","+QString::number(data[record][recordValue]));
                    }else{
                        valueSets[recordValue].append(QString::number(data[record][recordValue]));
                    }
                }
                ++record;
            }else{
                // Because of this record url becomes too large, so we have to break loop
                break;
            }
        }
        charts.back().append(this->assemble(valueSets, QString("|")));
    }

    // TODO przejdü po chartach i dodaj <img
    vector<QString>::iterator chartsIt;
    for(chartsIt=charts.begin(); chartsIt!=charts.end(); ++chartsIt)
    {
        (*chartsIt).prepend("<img src='");
        (*chartsIt).append("' />");
    }

    return this->assemble(charts, QString(""));
}

QString& ReportGenerator::verticalStackedBarChart(int height, vector<vector<double> > &data, vector<QString> &labels, vector<QString> &colors, int barWidth, QString additionalParameters)
{
    int spaceBetweenBars = (10.0/23.0)*barWidth;
    int spaceBetweenGroups = (8.0/23.0)*barWidth;
    int bars = data[0].size();

    QString* url = new QString("<img src='https://chart.googleapis.com/chart?");
    url->append("cht=bvs");
    url->append("&chs="+QString::number(spaceBetweenBars*(bars+1)+bars*barWidth+50)+"x"+QString::number(height));

    vector<QString> valueSets;
    vector<vector<double> >::iterator dataIt;
    for(dataIt = data.begin(); dataIt!=data.end(); ++dataIt)
    {
        vector<double> valueSet = *dataIt;
        valueSets.push_back(this->assemble(this->convertToQStringList(valueSet), QString(",")));
    }
    url->append("&chd=t:"+this->assemble(valueSets, QString("|")));

    url->append("&chxt=x,y&chxl=0:|"+this->assemble(labels, QString("|")));

    url->append("&chco="+this->assemble(colors, QString(",")));

    url->append("&chbh="+QString::number(barWidth)+","+QString::number(spaceBetweenBars)+","+QString::number(spaceBetweenGroups));

    url->append("&chds=a");

    url->append(additionalParameters+"' />");

    return *url;
}

QString& ReportGenerator::pieChart(int height, vector<double> &data, vector<QString> &labels, vector<QString> &legend, vector<QString> &colors)
{
    QString dataAssembled = this->assemble(data, QString(","));
    QString* url = new QString("<img src='https://chart.googleapis.com/chart?");
    url->append("cht=p");
    url->append("&chs="+QString::number(2*height)+"x"+QString::number(height));
    url->append("&chd=t:"+dataAssembled);
    if(!labels.empty())
    {
        QString labelsAssembled = this->assemble(labels, QString("|"));
        url->append("&chl="+labelsAssembled);
    }
    if(!legend.empty())
    {
        QString legendAssembled = this->assemble(legend, QString("|"));
        url->append("&chdl="+legendAssembled);
    }
    if(!colors.empty())
    {
        QString colorsAssembled = this->assemble(colors, QString(","));
        url->append("&chco="+colorsAssembled);
    }

    url->append("' />");

    return *url;
}

QString& ReportGenerator::pieChart3D(int height, vector<double> &data, vector<QString> &labels, vector<QString> &legend, vector<QString> &colors)
{
    QString dataAssembled = this->assemble(data, QString(","));
    QString* url = new QString("<img src='https://chart.googleapis.com/chart?");
    url->append("cht=p3");
    url->append("&chs="+QString::number(2.5*static_cast<double>(height))+"x"+QString::number(height));
    url->append("&chd=t:"+dataAssembled);
    if(!labels.empty())
    {
        QString labelsAssembled = this->assemble(labels, QString("|"));
        url->append("&chl="+labelsAssembled);
    }
    if(!legend.empty())
    {
        QString legendAssembled = this->assemble(legend, QString("|"));
        url->append("&chdl="+legendAssembled);
    }
    if(!colors.empty())
    {
        QString colorsAssembled = this->assemble(colors, QString(","));
        url->append("&chco="+colorsAssembled);
    }

    url->append("' />");

    return *url;
}

void ReportGenerator::addVariable(QString key, QString value)
{
    this->vars.insert(key, value);
}

vector<QString>& ReportGenerator::convertToQStringList(vector<double> &set)
{
    vector<QString>* result = new vector<QString>();
    vector<double>::iterator setIt;
    for(setIt = set.begin();setIt!=set.end();++setIt)
    {
        result->push_back(QString::number(*setIt));
    }
    return *result;
}

QString& ReportGenerator::assemble(vector<double> &data, QString separator)
{
    return this->assemble(this->convertToQStringList(data), separator);
}

QString& ReportGenerator::assemble(vector<QString> &data, QString separator)
{
    QString* result = new QString("");
    if(!data.empty())
    {
        vector<QString>::iterator dataIt = data.begin();
        result->append(*dataIt);
        ++dataIt;
        for(;dataIt!=data.end();++dataIt)
        {
            result->append(separator);
            result->append(*dataIt);
        }
    }
    return *result;
}
