#include "dialoghelp.h"
#include "ui_dialoghelp.h"

DialogHelp::DialogHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogHelp)
{
    ui->setupUi(this);
}

DialogHelp::~DialogHelp()
{
    delete ui;
}


void DialogHelp::setContentUrl(QString url)
{
    ui->webViewHelp->settings()->setDefaultTextEncoding("iso-8859-2");
    ui->webViewHelp->load(QUrl(url));
}
