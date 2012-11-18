#include "configurationhandler.h"

ConfigurationHandler::ConfigurationHandler(QString organization, QString application)
{
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, organization, application);
    initialize();
}

ConfigurationHandler::~ConfigurationHandler()
{
    saveChanges();
}

void ConfigurationHandler::initialize()
{
    if (!isWritable())
    {
        qDebug() << "The main configuration file is not writable. Please check its file permissions.";
    }

    if (isUninitialized())
    {
        qDebug() << "Uninitialized configuration file was found. Application settings were initialized.";

        // Database
        settings->beginGroup("Database");
        settings->setValue("Server", "RAFAL-PC\\SQLEXPRESS");
        settings->setValue("Source", "SCSWin");
        settings->setValue("UserName", "sa");
        settings->setValue("UserPassword", "tajnehaslo");
        settings->endGroup();

        // Selected object
        settings->beginGroup("SelectedObject");
        settings->setValue("Object_ID", "3");
        settings->endGroup();

        // Algorithm - SOM
        // {
        //     int width
        //     int height
        //     int inputs
        //     double maxRad
        //     double maxAlpha
        //     double maxIter
        //     TopologyMap &m - missing!
        // }
        settings->beginGroup("Algorithm.SOM");
        settings->setValue("Width", 20);
        settings->setValue("Height", 20);
        settings->setValue("Inputs", 4);
        settings->setValue("MaxRadius", 50.0);
        settings->setValue("MaxAlpha", 0.4);
        settings->setValue("MaxIterations", 1000.0);
        settings->endGroup();

        // Algorithm - RBF
        // {
        //     double learnFactor
        //     double sigmoidalActivationFunctionBeta
        //     double quantizationErrorThreshold
        //     int generations
        //     double randomWeightScaleFactor
        //     double clustersNumber
        //     int pointSize
        //     double kmeansMinSigmaSquare
        //     double kmeansMaxSigmaSquare
        //     double kmeansStepImprovement
        //     int kmeansIterations
        //     vector< vector<double> > &neighbours - missing!
        // }
        settings->beginGroup("Algorithm.RBF");
        settings->setValue("LearnFactor", 0.1);
        settings->setValue("SigmoidalActivationFunctionBeta", 2.0);
        settings->setValue("QuantizationErrorThreshold", 0.001);
        settings->setValue("Generations", 100);
        settings->setValue("RandomWeightScaleFactor", 0.1);
        settings->setValue("ClustersNumber", 27.0);
        settings->setValue("PointSize", 4);
        settings->setValue("MinSigmaSquare", 0.0001);
        settings->setValue("MaxSigmaSquare", 0.1);
        settings->setValue("StepImprovement", 0.001);
        settings->setValue("Iterations", 100);
        settings->endGroup();

        // Algorithm - NEIGHBOUR
        // {
        //     int m_featuresCount,
        //     int m_k,
        //     double (*m_spaceFunction)(const vector<double> &v1, const vector<double> &v2) - missing!
        // }
        settings->beginGroup("Algorithm.NEIGHBOUR");
        settings->setValue("FeaturesCount", 4);
        settings->setValue("K", 5);
        settings->endGroup();

        // Algorithm - DENSITY
        // {
        //     double LOFResultDeviation
        //     int neighbors
        // }
        settings->beginGroup("Algorithm.DENSITY");
        settings->setValue("LOFResultDeviation", 0.5);
        settings->setValue("Neighbors", 50);
        settings->endGroup();

        // Algorithm - BAYES
        // {
        //     int m_featuresCount
        // }
        settings->beginGroup("Algorithm.BAYES");
        settings->setValue("FeaturesCount", 4);
        settings->endGroup();
    }
}

void ConfigurationHandler::saveChanges()
{
    if (isWritable())
    {
        settings->sync();
    }
}

bool ConfigurationHandler::isUninitialized()
{
    return settings->allKeys().isEmpty();
}

bool ConfigurationHandler::isWritable()
{
    return settings->isWritable();
}

QStringList ConfigurationHandler::getAllProperties()
{
    return settings->allKeys();
}

void ConfigurationHandler::removeAllProperties()
{
    settings->clear();
}

// Removes all properties which belongs to the given category, e.g. Database
void ConfigurationHandler::removeProperties(const QString &category)
{
    settings->beginGroup(category);
    settings->remove("");
    settings->endGroup();
}

void ConfigurationHandler::removeProperty(const QString &key)
{
    settings->remove(key);
}

QVariant ConfigurationHandler::getPropertyValue(const QString &key)
{
    return settings->value(key);
}

QVariant ConfigurationHandler::getPropertyValue(const QString &category, const QString &key)
{
    return getPropertyValue(category + QString("/") + key);
}

void ConfigurationHandler::setPropertyValue(const QString &key, const QVariant &value)
{
    return settings->setValue(key, value);
}

void ConfigurationHandler::setPropertyValue(const QString &category, const QString &key, const QVariant &value)
{
    setPropertyValue(category + QString("/") + key, value);
}

bool ConfigurationHandler::contains(const QString &key)
{
    return settings->contains(key);
}

QString ConfigurationHandler::getDatabaseServer()
{
    return getPropertyValue("Database", "Server").toString();
}

QString ConfigurationHandler::getDatabaseSource()
{
    return getPropertyValue("Database", "Source").toString();
}

QString ConfigurationHandler::getDatabaseUserName()
{
    return getPropertyValue("Database", "UserName").toString();
}

QString ConfigurationHandler::getDatabaseUserPassword()
{
    return getPropertyValue("Database", "UserPassword").toString();
}

QVariant ConfigurationHandler::getAlgorithmParameter(const QString &algorithm, const QString &key)
{
    return getPropertyValue(QString("Algorithm.") + algorithm, key);
}
