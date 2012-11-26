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

    void sendNotifications(QString &name, QDateTime &dateTime, std::vector<QString> &dataNames, std::vector<double> &values, std::vector<QString> &units, std::vector<double> &mins, std::vector<double> &maxs);


private:



public:



private:
    ConfigurationHandler *config;

    SubscribersHandler *handler;

    MailSender *mailSender;
    SMSSender *smsSender;


};

#endif // NOTIFICATIONSENDER_H
