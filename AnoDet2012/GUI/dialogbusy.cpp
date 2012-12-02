#include "dialogbusy.h"
#include "ui_dialogbusy.h"
#include <QPixmap>
#include <QBitmap>
#include <QMovie>
#include <QDebug>

DialogBusy::DialogBusy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBusy)
{
    ui->setupUi(this);
}

DialogBusy::~DialogBusy()
{
    delete ui;
}

void DialogBusy::updateProgressBar()
{
    ui->progressBar->setValue(-1);
    ui->progressBar->repaint();
}

void DialogBusy::setLabelText(QString text)
{
    ui->label->setText(text);
}
