#include "dialogfilter.h"
#include "ui_dialogfilter.h"
#include <QStandardItem>

DialogFilter::DialogFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFilter)
{
    ui->setupUi(this);

//    QListWidgetItem *item = new QListWidgetItem;
//    item->setData( Qt::DisplayRole, "text" );
//    item->setData( Qt::CheckStateRole, Qt::Checked );
//    ui->listWidget->addItem(item);
//    model = new QStandardItemModel();
}

DialogFilter::~DialogFilter()
{
    delete ui;
}


void DialogFilter::addCheckboxesToList(std::vector<QListWidgetItem *> &items)
{
    for(unsigned i=0; i<items.size(); i++)
        ui->listWidget->addItem(items[i]);
}
