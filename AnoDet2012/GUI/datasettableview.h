#ifndef DATASETTABLEVIEW_H
#define DATASETTABLEVIEW_H

#include <QStandardItemModel>
#include <Dataset/DataRecordTable.h>

class DatasetTableView : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DatasetTableView(QObject *parent = 0);
    explicit DatasetTableView(int rows, int columns, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setDataRecords(DataRecordTable *records);
    
private:
    DataRecordTable *dataRecords;

signals:
    
public slots:
    
};

#endif // DATASETTABLEVIEW_H
