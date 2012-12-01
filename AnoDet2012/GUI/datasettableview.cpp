#include "datasettableview.h"

DatasetTableView::DatasetTableView(QObject *parent) :
    QStandardItemModel(parent)
{
}

DatasetTableView::DatasetTableView(int rows, int columns, QObject *parent) :
    QStandardItemModel(rows, columns, parent)
{
}

QVariant DatasetTableView::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    DataRecord *record = &(dataRecords->records[index.row()]);

    switch (role)
    {
    case Qt::DisplayRole:
        if(index.column() > 0)
            return QVariant(QString::number(record->data[index.column()-1]));
        return QVariant(record->time);
    case Qt::BackgroundRole:
        if(record->isAnomaly){
            return QVariant(QColor(Qt::red));
        }
    default:
        return QVariant();
    }
}

void DatasetTableView::setDataRecords(DataRecordTable *records){
    this->dataRecords = records;
}
