#ifndef MIMEMESSAGEBUILDER_H
#define MIMEMESSAGEBUILDER_H

#include <QVector>
#include <QHash>
#include <QList>
#include <QRegExp>
#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QDateTime>
#include "Alarms/Mail/mimemessage.h"
#include "Alarms/Mail/mimehtml.h"
#include "Alarms/Mail/mimeinlinefile.h"
#include "Alarms/subscriber.h"
#include "ConfigurationHandler/configurationhandler.h"

class MimeMessageBuilder
{
public:
    MimeMessageBuilder(ConfigurationHandler *config);
    ~MimeMessageBuilder();
    MimeMessage * build(std::vector<Subscriber> &subscribers, QString &name, QDateTime &dateTime, std::vector<QString> &dataNames, std::vector<double> &values, std::vector<QString> &units, std::vector<double> &mins, std::vector<double> &maxs);


private:
    void initialize();
    void loadConfiguration();


public:
    void reloadConfiguration();


private:
    ConfigurationHandler *config;

    QString *senderAddress;
    QString *senderField;
    QString *messageTopic;
    QString *body;
    QVector<QString> *imageFiles;
    QVector<QString> *imageIdentifiers;
    QVector<QString> *imageTypes;


};

#endif // MIMEMESSAGEBUILDER_H
