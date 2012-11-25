#include "WebViewPrinter.h"
#include <QFileDialog>
#include <QPrinter>


WebViewPrinter::WebViewPrinter(QWidget *parent) :
    QWebView(parent)
{
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(drawDocument()));
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(laduje(int)));
}

void WebViewPrinter::drawDocument()
{
    this->ql->setText("Zaladowano");
    this->print(this->printer);
}

void WebViewPrinter::laduje(int v)
{
    this->ql->setText("laduje sie"+QString::number(v));
}
