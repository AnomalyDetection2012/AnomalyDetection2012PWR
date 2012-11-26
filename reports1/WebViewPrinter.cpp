#include "WebViewPrinter.h"
#include <QFileDialog>
#include <QPrinter>


WebViewPrinter::WebViewPrinter(QWidget *parent) :
    QWebView(parent)
{
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(drawDocument()));
}

void WebViewPrinter::drawDocument()
{
    this->print(this->printer);
}
