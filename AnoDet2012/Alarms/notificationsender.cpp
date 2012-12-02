#include "notificationsender.h"

NotificationSender::NotificationSender(ConfigurationHandler *config)
{
    this->config = config;
    this->handler = new SubscribersHandler(config);
    this->mailSender = new MailSender(config);
    this->smsSender = new SMSSender(config);
}

NotificationSender::~NotificationSender()
{

}

void NotificationSender::sendNotifications(QString &name, QDateTime &dateTime, std::vector<QString> &dataNames, std::vector<double> &values, std::vector<QString> &units, std::vector<double> &mins, std::vector<double> &maxs)
{
    std::vector<Subscriber> subs = *(handler->getSubscribers());
    mailSender->sendMail(mailSender->getMessageBuilder()->build(subs, name, dateTime, dataNames, values, units, mins, maxs));
    smsSender->sendSMS(smsSender->getMessageBuilder()->build(subs, name, dateTime, dataNames, values, units, mins, maxs));
}

SubscribersHandler * NotificationSender::getSubscribersHandler()
{
    return handler;
}
