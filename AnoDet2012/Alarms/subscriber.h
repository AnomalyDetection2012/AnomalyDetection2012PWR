#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QString>

enum NotificationType {
    Mail,
    SMS,
    Both,
    None
};

struct Subscriber {
    QString identifier;
    QString mail;
    QString phone;
    NotificationType notification;
};

#endif // SUBSCRIBER_H
