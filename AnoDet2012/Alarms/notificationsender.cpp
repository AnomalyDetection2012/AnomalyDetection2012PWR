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

void NotificationSender::sendNotifications(QString *object, QVector<QString> *labels, QVector<double> *values)
{
    QVector<Subscriber> *subs = handler->getSubscribers();
    mailSender->sendMail(mailSender->getMessageBuilder()->build(subs, object, labels, values));
    smsSender->sendSMS(smsSender->getMessageBuilder()->build(subs, object, labels, values));
}

void NotificationSender::sendNotifications(QString &name, QDateTime &dateTime, std::vector<QString> &dataNames, std::vector<double> &values, std::vector<QString> &units, std::vector<double> &mins, std::vector<double> &maxs)
{
    //TODO: jakby byl problem jakis z pamiecia to usun referencje (przekaz przez wartosc)
}
