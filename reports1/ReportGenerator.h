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
    QString& pieChart(int height, QList<double> &data, QList<QString> &labels, QList<QString> &legend, QList<QString> &colors);
    QString& pieChart3D(int height, QList<double> &data, QList<QString> &labels, QList<QString> &legend, QList<QString> &colors);

private:
    QMap<QString, QString> vars;
    QList<QString>& convertToQStringList(QList<double> &set);
    QString& assemble(QList<double> &set, QString separator);
    QString& assemble(QList<QString> &set, QString separator);
    
signals:
    
public slots:
    
};

#endif // REPORTGENERATOR_H
