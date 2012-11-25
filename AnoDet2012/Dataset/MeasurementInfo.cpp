#include "MeasurementInfo.h"



MeasurementInfo::MeasurementInfo()
{
}

MeasurementInfo::MeasurementInfo(QHash<int, QHash<QString, QVariant> > &measurementInfo)
{
    this->measurementInfo = measurementInfo;
}

MeasurementInfo::~MeasurementInfo()
{
}

QString MeasurementInfo::getMeasurementName(int type_id)
{
    return ((QVariant)(measurementInfo.value(type_id).value("MeasurementName"))).toString();
}

QString MeasurementInfo::getUnit(int type_id)
{
    return ((QVariant)(measurementInfo.value(type_id).value("Unit"))).toString();
}

double MeasurementInfo::getMinValue(int type_id)
{
    return ((QVariant)(measurementInfo.value(type_id).value("MinValue"))).toDouble();
}

double MeasurementInfo::getMaxValue(int type_id)
{
    return ((QVariant)(measurementInfo.value(type_id).value("MaxValue"))).toDouble();
}
