#include "smsmessagebuilder.h"

SMSMessageBuilder::SMSMessageBuilder(ConfigurationHandler *config)
{
    this->config = config;
    this->templateMessage = new SMSMessageBuilder::SMSMessage();
    initialize();
    loadConfiguration();
}

SMSMessageBuilder::~SMSMessageBuilder()
{

}

void SMSMessageBuilder::initialize()
{
    QString location = QString("");
    QString source = QString("");

    location += (config->getPropertyValue("SMS", "TemplateLocation")).toString();
    source += location;
    source += QString("/");
    source += (config->getPropertyValue("SMS", "TemplateDescription")).toString();

    // Opens a file and sets a document content
    QFile f_document(source);
    QDomDocument d_document;
    QString errorMessage;
    int errorLine;
    int errorColumn;
    d_document.setContent(&f_document, &errorMessage, &errorLine, &errorColumn);

    // Gets the root node
    QDomElement d_description = d_document.namedItem("Description").toElement();

    // Gets a file which contains a message body
    QDomElement d_body = d_description.firstChildElement("body");

    // Reads the message body
    QFile f_body(location + QString("/") + d_body.text().trimmed());
    if (f_body.open(QIODevice::ReadOnly))
    {
        QByteArray buffer = f_body.readAll();
        body = new QString(buffer);
    }

    // Reads tags
    QHash<QString, QString> tags;
    QDomElement d_tags = d_description.firstChildElement("tags");
    QDomNode d_tag = d_tags.firstChild();
    for ( ; !d_tag.isNull(); d_tag = d_tag.nextSibling())
    {
        QDomElement tmp = d_tag.toElement();
        tags[tmp.attribute("key")] = tmp.attribute("value");
    }

    // Replaces tags within the message body
    QList<QString> keys = tags.keys();
    QList<QString>::iterator i;
    for (i = keys.begin(); i != keys.end(); ++i)
    {
        body->replace(QRegExp(*i), tags[*i]);
    }
}

void SMSMessageBuilder::loadConfiguration()
{
    QString category = QString("SMS");
    templateMessage->username = (config->getPropertyValue(category, "User")).toString();
    templateMessage->password = (config->getPropertyValue(category, "Password")).toString();
    templateMessage->flash = (config->getPropertyValue(category, "Flash")).toString();
    templateMessage->test = (config->getPropertyValue(category, "Test")).toString();
    templateMessage->details = (config->getPropertyValue(category, "Details")).toString();
    templateMessage->eco = (config->getPropertyValue(category, "ECO")).toString();
    templateMessage->fast = (config->getPropertyValue(category, "Fast")).toString();
    templateMessage->from = (config->getPropertyValue(category, "SenderField")).toString();
    templateMessage->to = QString("Invalid");
    templateMessage->message = QString("Invalid");
}

void SMSMessageBuilder::reloadConfiguration()
{
    loadConfiguration();
}

// TODO: use additional parameters
SMSMessageBuilder::SMSMessage * SMSMessageBuilder::build(QVector<Subscriber> *subscribers, QString *object, QVector<QString> *labels, QVector<double> *values)
{
    QString to = QString("");
    QVector<Subscriber>::iterator i;
    for (i = subscribers->begin(); i != subscribers->end(); ++i)
    {
        switch ((*i).notification)
        {
            case SMS:
            case Both:
                to += (*i).phone;
                to += QString(",");
            default:
                continue;
        }
    }
    to.resize(to.length() - 1);

    SMSMessageBuilder::SMSMessage *msg = new SMSMessageBuilder::SMSMessage();
    *msg = *templateMessage;
    msg->to = to;
    msg->message = *body;

    return msg;
}
