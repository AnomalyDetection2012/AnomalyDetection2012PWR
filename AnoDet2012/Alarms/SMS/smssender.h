#ifndef SMSSENDER_H
#define SMSSENDER_H

#include <QDebug>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "ConfigurationHandler/configurationhandler.h"
#include "Alarms/SMS/smsmessagebuilder.h"

class SMSSender
{
public:
    SMSSender(ConfigurationHandler *config);
    ~SMSSender();
    bool sendSMS(SMSMessageBuilder::SMSMessage *message);
    SMSMessageBuilder * getMessageBuilder();


private:
    void initialize();


public:



private:
    ConfigurationHandler *config;
    QString *url;
    QNetworkAccessManager *sender;

    SMSMessageBuilder *builder;


};

#endif // SMSSENDER_H
