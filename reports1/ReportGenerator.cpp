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

QString& ReportGenerator::lineChart(int width, int height, vector<vector<double> > &data, vector<QString> &labels, vector<QString> &colors, QString additionalParameters)
{
    int records = data.size();
    int recordSize = data[0].size();

    QString* url = new QString("<img src='https://chart.googleapis.com/chart?");
    url->append("cht=lc");
    url->append("&chs="+QString::number(width)+"x"+QString::number(height));

    vector<QString> valueSets;
    for(int recordValue=0; recordValue<recordSize; ++recordValue)
    {
        valueSets.push_back(*(new QString("")));
        for(int currentRecord=0; currentRecord<records; ++currentRecord)
        {
            valueSets[recordValue].append(QString::number(data[currentRecord][recordValue]));
            if(currentRecord<(records-1))
            {
                valueSets[recordValue].append(QString(","));
            }
        }
    }
    url->append("&chd=t:"+this->assemble(valueSets, QString("|")));

    url->append("&chxt=x,y&chxl=0:|"+this->assemble(labels, QString("|")));

    url->append("&chco="+this->assemble(colors, QString(",")));

    url->append("&chds=a");

    url->append(additionalParameters+"' />");

    return *url;
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
