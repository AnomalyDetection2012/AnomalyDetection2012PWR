#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QDebug>
#include "ConfigurationHandler/configurationhandler.h"
#include "Alarms/Mail/smtpclient.h"
#include "Alarms/Mail/mimemessagebuilder.h"

class MailSender
{
public:
    MailSender(ConfigurationHandler *config);
    ~MailSender();
    bool sendMail(MimeMessage *message);
    MimeMessageBuilder * getMessageBuilder();


private:
    void initialize();


public:



private:
    ConfigurationHandler *config;

    QString host;
    int port;
    QString user;
    QString password;

    MimeMessageBuilder *builder;


};

#endif // MAILSENDER_H
