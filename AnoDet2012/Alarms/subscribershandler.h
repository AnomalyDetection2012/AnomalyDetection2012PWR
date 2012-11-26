#ifndef SUBSCRIBERSHANDLER_H
#define SUBSCRIBERSHANDLER_H

#include <QFile>
#include <QVector>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomNode>
#include <QDomElement>
#include <QDomText>
#include <QTextCodec>
#include "ConfigurationHandler/configurationhandler.h"
#include "Alarms/subscriber.h"

class SubscribersHandler
{
public:
    SubscribersHandler(ConfigurationHandler *config);
    ~SubscribersHandler();

    void loadData();
    void saveData();

    QVector<Subscriber> * getSubscribers();

    Subscriber * getSubscriber(const QString &identifier);
    bool addSubscriber(const QString &identifier, const QString &mail, const QString &phone, NotificationType notification);
    bool removeSubscriber(const QString &identifier);
    bool removeSubscriber(Subscriber *subscriber);


private:
    void initialize();


public:



private:
    ConfigurationHandler *config;

    QFile *data;

    QVector<Subscriber> *subscribers;


};

#endif // SUBSCRIBERSHANDLER_H
