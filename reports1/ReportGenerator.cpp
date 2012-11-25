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

ReportGenerator::ReportGenerator(QObject *parent) :
    QObject(parent)
{
}

bool ReportGenerator::generatePDF(QString templateFilename, QString outputAbsolutePath)
{
    QFile templateFile(":/input/"+templateFilename);

    if(!templateFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ql->setText("chuj");
        return false;
    }else
    {
        ql->setText("nie chuj");
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
    wv->ql = this->ql;
    wv->printer = printer;

    wv->setHtml(document.toHtml());

    return true;

}

QString& ReportGenerator::pieChart(int height, QList<double> &data, QList<QString> &labels, QList<QString> &legend, QList<QString> &colors)
{
    QString dataAssemled = this->assemble(data, QString(","));
    QString* url = new QString("https://chart.googleapis.com/chart?");
    url->append("cht=p");
    url->append("&chs="+QString::number(2*height)+"x"+QString::number(height));
    url->append("&chd=t:"+dataAssemled);
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
    return *url;
}

QString& ReportGenerator::pieChart3D(int height, QList<double> &data, QList<QString> &labels, QList<QString> &legend, QList<QString> &colors)
{
    QString dataAssemled = this->assemble(data, QString(","));
    QString* url = new QString("https://chart.googleapis.com/chart?");
    url->append("cht=p3");
    url->append("&chs="+QString::number(2.5*static_cast<double>(height))+"x"+QString::number(height));
    url->append("&chd=t:"+dataAssemled);
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
    return *url;
}

void ReportGenerator::addVariable(QString key, QString value)
{
    this->vars.insert(key, value);
}

QList<QString>& ReportGenerator::convertToQStringList(QList<double> &set)
{
    QList<QString>* result = new QList<QString>();
    QList<double>::iterator setIt;
    for(setIt = set.begin();setIt!=set.end();++setIt)
    {
        result->push_back(QString::number(*setIt));
    }
    return *result;
}

QString& ReportGenerator::assemble(QList<double> &data, QString separator)
{
    return this->assemble(this->convertToQStringList(data), separator);
}

QString& ReportGenerator::assemble(QList<QString> &data, QString separator)
{
    QString* result = new QString("");
    if(!data.empty())
    {
        QList<QString>::iterator dataIt = data.begin();
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
