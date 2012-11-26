#include "subscribershandler.h"

SubscribersHandler::SubscribersHandler(ConfigurationHandler *config)
{
    this->config = config;
    this->subscribers = new std::vector<Subscriber>();
    initialize();
}

SubscribersHandler::~SubscribersHandler()
{

}

void SubscribersHandler::initialize()
{
    QString location = (config->getPropertyValue("Subscribers", "Location")).toString();
    QString source = (config->getPropertyValue("Subscribers", "File")).toString();
    data = new QFile(location + "/" + source);
}

void SubscribersHandler::loadData()
{
    // Opens a file and sets a document content
    QDomDocument d_document;
    QString errorMessage;
    int errorLine;
    int errorColumn;
    d_document.setContent(data, &errorMessage, &errorLine, &errorColumn);

    // Closes used file
    data->close();

    // Gets the root node
    QDomElement d_subscribers = d_document.namedItem("Subscribers").toElement();

    // Reads subscribers
    QDomNode d_entry = d_subscribers.firstChild();
    for ( ; !d_entry.isNull(); d_entry = d_entry.nextSibling())
    {
        Subscriber *sub = new Subscriber();
        sub->identifier = d_entry.namedItem("identifier").toElement().text();
        sub->mail = d_entry.namedItem("mail").toElement().text();
        sub->phone = d_entry.namedItem("phone").toElement().text();

        QString notification = d_entry.namedItem("notification").toElement().text();;
        if (notification.compare("Mail") == 0)
            sub->notification = Mail;
        else if (notification.compare("SMS") == 0)
            sub->notification = SMS;
        else if (notification.compare("Both") == 0)
            sub->notification = Both;
        else
            sub->notification = None;

        subscribers->push_back(*sub);
    }
}

void SubscribersHandler::saveData()
{
    // Creates a document structure
    QDomDocument d_document;

    QDomProcessingInstruction instr = d_document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    d_document.appendChild(instr);

    QDomElement d_subscribers = d_document.createElement("Subscribers");
    d_document.appendChild(d_subscribers);

    std::vector<Subscriber>::iterator i;
    for (i = subscribers->begin(); i != subscribers->end(); ++i)
    {
        QDomElement d_entry = d_document.createElement("subscriber");

            QDomElement de_identifier = d_document.createElement("identifier");
            QDomText dt_identifier = d_document.createTextNode((*i).identifier);
            de_identifier.appendChild(dt_identifier);
            d_entry.appendChild(de_identifier);


            QDomElement de_mail = d_document.createElement("mail");
            QDomText dt_mail = d_document.createTextNode((*i).mail);
            de_mail.appendChild(dt_mail);
            d_entry.appendChild(de_mail);

            QDomElement de_phone = d_document.createElement("phone");
            QDomText dt_phone = d_document.createTextNode((*i).phone);
            de_phone.appendChild(dt_phone);
            d_entry.appendChild(de_phone);

            QDomElement de_notification = d_document.createElement("notification");
            QString value;
            if ((*i).notification == Mail)
                value = "Mail";
            else if ((*i).notification == SMS)
                value = "SMS";
            else if ((*i).notification == Both)
                value = "Both";
            else
                value = "None";
            QDomText dt_notification = d_document.createTextNode(value);
            de_notification.appendChild(dt_notification);
            d_entry.appendChild(de_notification);

        d_subscribers.appendChild(d_entry);
    }

    // Saves data to a file
    if(data->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(data);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        stream << d_document.toString();

        data->close();
    }
}

std::vector<Subscriber> * SubscribersHandler::getSubscribers()
{
    return subscribers;
}

Subscriber * SubscribersHandler::getSubscriber(const QString &identifier)
{
    std::vector<Subscriber>::iterator i;
    for (i = subscribers->begin(); i != subscribers->end(); ++i)
    {
        if (!(*i).identifier.compare(identifier))
        {
            return &(*i);
        }
    }

    return NULL;
}

bool SubscribersHandler::addSubscriber(const QString &identifier, const QString &mail, const QString &phone, NotificationType notification)
{
    std::vector<Subscriber>::iterator i;
    for (i = subscribers->begin(); i != subscribers->end(); ++i)
    {
        if (!(*i).identifier.compare(identifier))
        {
            return false;
        }
    }

    Subscriber *sub = new Subscriber();
    sub->identifier = identifier;
    sub->mail = mail;
    sub->phone = phone;
    sub->notification = notification;
    subscribers->push_back(*sub);
    return true;
}

bool SubscribersHandler::removeSubscriber(const QString &identifier)
{
    std::vector<Subscriber>::iterator i;
    for (i = subscribers->begin(); i != subscribers->end(); ++i)
    {
        if (!(*i).identifier.compare(identifier))
        {
            Subscriber *sub = &(*i);
            subscribers->erase(i);
            delete sub;
            return true;
        }
    }

    return false;
}

bool SubscribersHandler::removeSubscriber(Subscriber *subscriber)
{
    return removeSubscriber(subscriber->identifier);
}
