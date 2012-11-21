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
    QString host = (config->getPropertyValue(category, "Host")).toString();
    int port = (config->getPropertyValue(category, "Port")).toInt();
    QString user = (config->getPropertyValue(category, "User")).toString();
    QString password = (config->getPropertyValue(category, "Password")).toString();

    sender = new SmtpClient(host, port, SmtpClient::SslConnection);
    sender->setUser(user);
    sender->setPassword(password);
}

MimeMessageBuilder * MailSender::getMessageBuilder()
{
    return builder;
}

bool MailSender::sendMail(MimeMessage *message)
{
    sender->connectToHost();
    sender->login();
    sender->sendMail(*message);
    sender->quit();

    return true;
}
