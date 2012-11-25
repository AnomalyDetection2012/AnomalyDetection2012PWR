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
    QLabel* ql;
    
signals:
    
public slots:
    void drawDocument();
    void laduje(int);
};

#endif // WEBVIEWPRINTER_H
