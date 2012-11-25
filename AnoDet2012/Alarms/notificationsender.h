#ifndef NOTIFICATIONSENDER_H
#define NOTIFICATIONSENDER_H

#include "ConfigurationHandler/configurationhandler.h"
#include "Alarms/subscribershandler.h"
#include "Alarms/Mail/mailsender.h"
#include "Alarms/SMS/smssender.h"

class NotificationSender
{
public:
    NotificationSender(ConfigurationHandler *config);
    ~NotificationSender();

    void sendNotifications(QString *object, QVector<QString> *labels, QVector<double> *values);


private:



public:



private:
    ConfigurationHandler *config;

    SubscribersHandler *handler;

    MailSender *mailSender;
    SMSSender *smsSender;


};

#endif // NOTIFICATIONSENDER_H
