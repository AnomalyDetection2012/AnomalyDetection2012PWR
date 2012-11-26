#include "mimemessagebuilder.h"

MimeMessageBuilder::MimeMessageBuilder(ConfigurationHandler *config)
{
    this->config = config;
    initialize();
    loadConfiguration();
}

MimeMessageBuilder::~MimeMessageBuilder()
{

}

void MimeMessageBuilder::initialize()
{
    QString location = QString("");
    QString source = QString("");

    location += (config->getPropertyValue("Mail", "TemplateLocation")).toString();
    source += location;
    source += QString("/");
    source += (config->getPropertyValue("Mail", "TemplateDescription")).toString();

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

    // Reads images
    imageFiles = new QVector<QString>();
    imageIdentifiers = new QVector<QString>();
    imageTypes = new QVector<QString>();

    QDomElement d_images = d_description.firstChildElement("images");
    QString subfolder = d_images.toElement().attribute("folder");
    QDomNode d_image = d_images.firstChild();
    for ( ; !d_image.isNull(); d_image = d_image.nextSibling())
    {
        QDomElement tmp = d_image.toElement();
        imageFiles->append(location + "/" + subfolder + "/" + tmp.attribute("file"));
        imageIdentifiers->append(tmp.attribute("id"));
        imageTypes->append(tmp.attribute("type"));
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

void MimeMessageBuilder::loadConfiguration()
{
    senderAddress = new QString((config->getPropertyValue("Mail", "SenderAddress")).toString());
    senderField = new QString((config->getPropertyValue("Mail", "SenderField")).toString());
    messageTopic = new QString((config->getPropertyValue("Mail", "MessageTopic")).toString());
}

void MimeMessageBuilder::reloadConfiguration()
{
    loadConfiguration();
}

MimeMessage * MimeMessageBuilder::build(std::vector<Subscriber> &subscribers, QString &name, QDateTime &dateTime, std::vector<QString> &dataNames, std::vector<double> &values, std::vector<QString> &units, std::vector<double> &mins, std::vector<double> &maxs)
{
    MimeMessage *message = new MimeMessage();
    message->setSender(new EmailAddress(*senderAddress, *senderField));
    message->setSubject(*messageTopic);

    std::vector<Subscriber>::iterator i;
    for (i = subscribers.begin(); i != subscribers.end(); ++i)
    {
        switch ((*i).notification)
        {
            case Mail:
            case Both:
                message->addRecipient(new EmailAddress((*i).mail,
                                                       (*i).identifier),
                                                       MimeMessage::Bcc);
            default:
                continue;
        }
    }

    // Adds additional information
    QString content(*body);
    content.replace(QRegExp("{%OBJECT%}"), name);
    content.replace(QRegExp("{%TIME%}"), dateTime.toString("hh:mm:ss dd.MM.yyyy"));

    QString details;
    for (int i = 0; i < values.size(); i++)
    {
        details.append(QString("<tr id='m_highlight'>"));
        details.append(QString("<td id='m_content'>" + dataNames[i] + "</td>"));
        details.append(QString("<td id='m_content'>" + QString::number(values[i]) + units[i] + "</td>"));
        details.append(QString("<td id='m_content'><" + QString::number(mins[i]) + "; " + QString::number(maxs[i]) + "></td>"));
        details.append(QString("</tr>"));
    }
    content.replace(QRegExp("{%DETAILS%}"), details);

    MimeHtml *html = new MimeHtml();
    html->setHtml(content);
    message->addPart(html);

    for (int i = 0; i < (imageFiles->size()); i++)
    {
        MimeInlineFile *image = new MimeInlineFile(new QFile((*imageFiles)[i]));
        image->setContentId((*imageIdentifiers)[i]);
        image->setContentType((*imageTypes)[i]);
        message->addPart(image);
    }

    return message;
}
