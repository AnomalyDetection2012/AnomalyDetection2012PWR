#ifndef MEASUREMENTINFO_H
#define MEASUREMENTINFO_H

#include <QHash>
#include <QVariant>

class MeasurementInfo
{
public:
    MeasurementInfo();
    MeasurementInfo(QHash<int, QHash<QString, QVariant> > &measurementInfo);

    ~MeasurementInfo();

    QString getMeasurementName(int type_id);
    double getMinValue(int type_id);
    double getMaxValue(int type_id);

private:
    QHash<int, QHash <QString, QVariant> > measurementInfo;

};

#endif // MEASUREMENTINFO_H
