#include "Alarms\SMS\smssender.h"

SMSSender::SMSSender(ConfigurationHandler *config)
{
    this->config = config;
    this->url = new QString((config->getPropertyValue("SMS", "URL")).toString());
    this->sender = new QNetworkAccessManager();
    this->builder = new SMSMessageBuilder(config);
    initialize();
}

SMSSender::~SMSSender()
{

}

void SMSSender::initialize()
{

}

SMSMessageBuilder * SMSSender::getMessageBuilder()
{
    return builder;
}

bool SMSSender::sendSMS(SMSMessageBuilder::SMSMessage *message)
{
    if (message != NULL)
    {
        if (!(*message).to.compare("Invalid")) return false;
        if (!(*message).message.compare("Invalid")) return false;

        QUrl *params;
        params = new QUrl(*url);
        params->addQueryItem("username", message->username);
        params->addQueryItem("password", message->password);
        params->addQueryItem("flash", message->flash);
        params->addQueryItem("test", message->test);
        params->addQueryItem("details", message->details);
        params->addQueryItem("eco", message->eco);
        params->addQueryItem("fast", message->fast);
        params->addQueryItem("from", message->from);
        params->addQueryItem("to", message->to);
        params->addQueryItem("message", message->message);
        params->addQueryItem("encoding", "utf-8");  // Forces to use UTF-8 encoding

        int result = -1;
        {
            QNetworkRequest *request = new QNetworkRequest(*params);
            request->setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
            QNetworkReply *reply = sender->post(*request, (*params).encodedQuery());
            result = reply->error();
        }

        delete params;
        return (result == 0);
    }
    else
    {
        return false;
    }
}
