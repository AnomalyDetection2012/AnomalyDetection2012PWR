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
#include <QDateTime>
#include <QTextStream>
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
    SMSMessageBuilder::SMSMessage * build(std::vector<Subscriber> &subscribers, QString &name, QDateTime &dateTime, std::vector<QString> &dataNames, std::vector<double> &values, std::vector<QString> &units, std::vector<double> &mins, std::vector<double> &maxs);


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
