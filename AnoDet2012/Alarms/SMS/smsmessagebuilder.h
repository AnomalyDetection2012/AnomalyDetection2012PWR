#ifndef SMSMESSAGEBUILDER_H
#define SMSMESSAGEBUILDER_H

#include <QVector>
#include <QHash>
#include <QList>
#include <QRegExp>
#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include "Alarms/subscriber.h"
#include "ConfigurationHandler/configurationhandler.h"

class SMSSender;

class SMSMessageBuilder
{
public:

    // See http://www.smsapi.pl/media/page/docs/SMSAPI_http.pdf to get a whole
    // set of parameters or read information about them. SMSMessage contains
    // only selected parameters.

    struct SMSMessage {
        // Fields stored in the configuration file
        QString username;
        QString password;
        QString flash;
        QString test;
        QString details;
        QString eco;
        QString fast;
        QString from;
        // User must fill fields below
        QString to;
        QString message;
    };

    SMSMessageBuilder(ConfigurationHandler *config);
    ~SMSMessageBuilder();
    SMSMessageBuilder::SMSMessage * build(QVector<Subscriber> *subscribers, QString *object, QVector<QString> *labels, QVector<double> *values);


private:
    void initialize();
    void loadConfiguration();


public:
    void reloadConfiguration();


private:
    ConfigurationHandler *config;

    SMSMessageBuilder::SMSMessage *templateMessage;

    QString *body;


};

#endif // SMSMESSAGEBUILDER_H
