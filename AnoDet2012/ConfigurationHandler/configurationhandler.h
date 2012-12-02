#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include <QDebug>
#include <QSettings>
#include <QStringList>

class ConfigurationHandler
{
public:
    ConfigurationHandler(QString organization = "AnomalyDetection", QString application = "settings");
    ~ConfigurationHandler();
    void saveChanges();
    QStringList getAllProperties();
    void removeAllProperties();
    void removeProperties(const QString &category);
    void removeProperty(const QString &key);
    QVariant getPropertyValue(const QString &key);
    QVariant getPropertyValue(const QString &category, const QString &key);
    void setPropertyValue(const QString &key, const QVariant &value);
    void setPropertyValue(const QString &category, const QString &key, const QVariant &value);
    bool contains(const QString &key);

    // Below you can see higher level methods added for simplicity's sake
    QString getDatabaseServer();
    QString getDatabaseSource();
    QString getDatabaseUserName();
    QString getDatabaseUserPassword();
    int getObjectId();

    QVariant getAlgorithmParameter(const QString &algorithm, const QString &key);

    QString getSettingsFileLocation();
    void importSettings(const QString &path);
    void exportSettings(const QString &path);


private:
    void initialize();
    bool isWritable();
    bool isUninitialized();


public:



private:
    QSettings *settings;


};

#endif // CONFIGURATIONHANDLER_H
