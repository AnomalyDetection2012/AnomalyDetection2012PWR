#include "Alarms\Mail\mailsender.h"

MailSender::MailSender(ConfigurationHandler *config)
{
    this->config = config;
    this->builder = new MimeMessageBuilder(config);
    initialize();
}

MailSender::~MailSender()
{

}

void MailSender::initialize(){
    QString category = QString("Mail");
    this->host = (config->getPropertyValue(category, "Host")).toString();
    this->port = (config->getPropertyValue(category, "Port")).toInt();
    this->user = (config->getPropertyValue(category, "User")).toString();
    this->password = (config->getPropertyValue(category, "Password")).toString();
}

MimeMessageBuilder * MailSender::getMessageBuilder()
{
    return builder;
}

bool MailSender::sendMail(MimeMessage *message)
{
    SmtpClient *sender = new SmtpClient(host, port, SmtpClient::SslConnection);
    sender->setUser(user);
    sender->setPassword(password);

    if (message != NULL)
    {
        bool result = sender->connectToHost() &&
                      sender->login() &&
                      sender->sendMail(*message);
        sender->quit();

        return result;
    }
    else
    {
        return false;
    }
}
