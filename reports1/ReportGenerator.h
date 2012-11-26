#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QObject>
#include <QMap>
#include <QLabel>

class ReportGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ReportGenerator(QObject *parent = 0);

    QLabel* ql;

    bool generatePDF(QString templateFilename, QString outputAbsolutePath);
    void addVariable(QString key, QString value);
    QString& pieChart(int height, std::vector<double> &data, std::vector<QString> &labels, std::vector<QString> &legend, std::vector<QString> &colors);
    QString& pieChart3D(int height, std::vector<double> &data, std::vector<QString> &labels, std::vector<QString> &legend, std::vector<QString> &colors);
    QString& verticalStackedBarChart(int height, std::vector<std::vector<double> > &data, std::vector<QString> &labels, std::vector<QString> &colors, int barWidth = 23, QString additionalParameters = QString(""));
    QString& lineChart(int width, int height, std::vector<std::vector<double> > &data, std::vector<QString> &labels, std::vector<QString> &colors, QString additionalParameters = QString(""));


private:
    QMap<QString, QString> vars;
    std::vector<QString>& convertToQStringList(std::vector<double> &set);
    QString& assemble(std::vector<double> &set, QString separator);
    QString& assemble(std::vector<QString> &set, QString separator);
    
signals:
    
public slots:
    
};

#endif // REPORTGENERATOR_H
