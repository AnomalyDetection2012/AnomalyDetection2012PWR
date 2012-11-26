#ifndef WEBVIEWPRINTER_H
#define WEBVIEWPRINTER_H

#include <QWebView>
#include <QLabel>
#include <QPrinter>

class WebViewPrinter : public QWebView
{
    Q_OBJECT
public:
    explicit WebViewPrinter(QWidget *parent = 0);

    QPrinter* printer;
    
signals:
    
public slots:
    void drawDocument();
};

#endif // WEBVIEWPRINTER_H
