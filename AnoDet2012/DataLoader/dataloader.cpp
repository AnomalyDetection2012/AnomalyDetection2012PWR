#include "dataloader.h"
#include "Dataset/DatasetConnector.h"

using namespace std;

DataLoader::DataLoader(int objectId, QString server, QString dbName, QString username, QString password)
{
    this->objectId = objectId;

    QString connectionString("Driver={SQL Server};Server=");
    connectionString.append(server).append(";Database=").append(dbName);

    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName(connectionString);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open())
    {
        qDebug() << "An error was encountered: "<< QSqlError(db.lastError()).text();
    }
    else
    {
        qDebug() << "The database connection was established successfully.";
    }

}

DataLoader::DataLoader(int objectId, QSqlDatabase *dbConn)
{
    this->objectId = objectId;
    this->db = *dbConn;
}

void DataLoader::initDataRecordTable()
{
    if(this->performDatabaseConnection())
    {
        this->recordSize = 0;
        this->programPomiarIds = *(new vector<int>());
        this->recordIds = *(new vector<int>());
        vector<QString> dataNames;

        QString statement("Select Program_pomiar.Program_pomiar_ID, Program_pomiar.Nazwa_pomiaru from Program_pomiar where Program_pomiar.Konfiguracja_ID = "+QString::number(this->objectId)+" order by Program_pomiar_ID");
        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            while (query.next()) {
                ++this->recordSize;
                this->programPomiarIds.push_back(query.value(0).toInt());
                dataNames.push_back(query.value(1).toString());
            }
            vector<QString> vcEmpty;
            vector<vector<string> > vvcEmpty;
            dataset->createDatasetControler("", vvcEmpty, dataNames, vcEmpty);
        }
    }
}


void DataLoader::initialiseConnectors(ConnectorTracker *con){
    dataset = con->dataset;
}

void DataLoader::loadMeasurementInfo()
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT [Program_pomiar_ID], [Nazwa_pomiaru], [AlertMin], [AlertMax] FROM  [Program_pomiar];");
        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive())
        {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        }
        else
        {
            while (query.next())
            {
                QHash <QString, QVariant> m;
                //m = new QHash <QString, QVariant>();
                m.insert("MeasurementName",query.value(1));
                m.insert("MinValue",query.value(2));
                m.insert("MaxValue", query.value(3));

                this->measurementInfo.insert(query.value(0).toInt(), m);
            }
            dataset->setMeasurementsInfo(new MeasurementInfo(measurementInfo)); // add to DataRecordTable
        }

    }
}

void DataLoader::loadAllRecords()
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT Rekord_ID, Data");
        for(int a = 0; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programPomiarIds[a])+"]");
        }
        statement.append("from (Select TOP 1600000 Wyniki_pomiar.Rekord_ID, Program_pomiar_ID, Wartosc, Data From Wyniki_pomiar left join Rekord on Rekord.Rekord_ID = Wyniki_pomiar.Rekord_ID where Rekord.Obiekt_ID = "+QString::number(this->objectId)+" order by Rekord_ID) p PIVOT( min(Wartosc) FOR Program_pomiar_ID IN (["+QString::number(this->programPomiarIds[0])+"]");
        for(int a = 1; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programPomiarIds[a])+"]");
        }
        statement.append(")) as pvt order by pvt.Rekord_ID");

        //qDebug() << statement;

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            while (query.next())
            {

                vector<double> data(this->recordSize);
                this->recordIds.push_back(query.value(0).toInt());
                for(int a = 0; a < this->recordSize; ++a)
                {
                    data[a] = query.value(a+2).toDouble();
                }
                dataset->newRecord(query.value(1).toDateTime().toTime_t(), data, *(new vector<double>(0)), this->programPomiarIds,false);
                this->progessBar->setValue(this->progessBar->value()+1);

                if(this->progessBar->wasCanceled())
                {
                    break;
                }
            }
        }
    }
}

vector<QPair<int, QString> >* DataLoader::loadAllObjectsData()
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT TOP 17 [Obiekt_ID], Nazwa_obiektu FROM  [Obiekt] where Nazwa_obiektu like '%Monitoring'");

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            vector<QPair<int, QString> >* objectsData = new vector<QPair<int, QString> >(17);
            int currentObject=0;
            while (query.next())
            {
                objectsData->at(currentObject) = *(new QPair<int, QString>(query.value(0).toInt(), query.value(1).toString()));
                currentObject++;
                this->progessBar->setValue(this->progessBar->value()+1);
            }
            return objectsData;
        }
    }
    return NULL;
}

int DataLoader::getAmountOfObjectRecords(int objectID)
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT COUNT(*) FROM  [Rekord] where Obiekt_ID = "+QString::number(objectID)+" group by Obiekt_ID");

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            while (query.next()) {
                return query.value(0).toInt();
            }
        }
    }
    return 0;
}

void DataLoader::loadRecords(int from, int to)
{
    if(this->performDatabaseConnection())
    {
        if(to<from){ to=from+1; }
        if(from<1){ from=1; }
        if(to<1){ to=1; }

        int firstRecordID = this->getRecordIDOfFirstRecord(from);

        QString statement("SELECT Rekord_ID, Data");
        for(int a = 0; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programPomiarIds[a])+"]");
        }
        statement.append("from (Select TOP "+QString::number((to-from+1)*this->recordSize)+" Wyniki_pomiar.Rekord_ID, Program_pomiar_ID, Wartosc, Data From Wyniki_pomiar left join Rekord on Rekord.Rekord_ID = Wyniki_pomiar.Rekord_ID where Rekord.Obiekt_ID = "+QString::number(this->objectId)+" and Rekord.Rekord_ID>="+QString::number(firstRecordID)+" order by Rekord_ID) p PIVOT( min(Wartosc) FOR Program_pomiar_ID IN (["+QString::number(this->programPomiarIds[0])+"]");
        for(int a = 1; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programPomiarIds[a])+"]");
        }
        statement.append(")) as pvt where Rekord_ID>="+QString::number(firstRecordID)+" order by pvt.Rekord_ID");

        //qDebug() << statement;

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            while (query.next()) {
                vector<double> data;
                this->recordIds.push_back(query.value(0).toInt());
                for(int a = 0; a < this->recordSize; ++a)
                {
                    data.push_back(query.value(a+2).toDouble());
                }
                dataset->newRecord(query.value(1).toDateTime().toTime_t(), data, *(new vector<double>(0)), *(new vector<int>(0)),false);
            }
        }
    }
}

void DataLoader::setAlarmFlagToRecords()
{
    if(this->performDatabaseConnection())
    {
        //QString statement("select distinct Wyniki_alarm.Rekord_ID from Wyniki_alarm left join Program_alarm on Program_alarm.Program_alarm_ID = Wyniki_alarm.Program_alarm_ID where Wyniki_alarm.Status>0 and (Program_alarm.Typ_alarmu_ID=0 or Program_alarm.Typ_alarmu_ID=3 or Program_alarm.Typ_alarmu_ID=5) and Program_alarm.Konfiguracja_ID = "+QString::number(this->objectId)+" and Wyniki_alarm.Rekord_ID >= "+QString::number(this->recordIds[0])+" and Wyniki_alarm.Rekord_ID <= "+QString::number(this->recordIds.back())+" order by Wyniki_alarm.Rekord_ID");
        QString statement("select distinct Wyniki_alarm.Rekord_ID from Wyniki_alarm left join Program_alarm on Program_alarm.Program_alarm_ID = Wyniki_alarm.Program_alarm_ID where Wyniki_alarm.Status>0 and (Program_alarm.Typ_alarmu_ID=0 or Program_alarm.Typ_alarmu_ID=3 or Program_alarm.Typ_alarmu_ID=5) and Program_alarm.Konfiguracja_ID = "+QString::number(this->objectId)+" order by Wyniki_alarm.Rekord_ID");
        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive())
        {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        }
        else
        {
            vector<int>::iterator recordIdsIt = this->recordIds.begin();
            int currentRecord = 0;

            while (query.next())
            {
                int qRecordId = query.value(0).toInt();

                while(recordIdsIt != recordIds.end())
                {
                    if((*recordIdsIt)==qRecordId)
                    {
                        dataset->setAnomaly(currentRecord, true);
                        ++recordIdsIt;
                        ++currentRecord;
                        break;
                    }
                    else
                    {
                        ++recordIdsIt;
                        ++currentRecord;
                    }
                }

            }
        }
    }
}

void DataLoader::removeAllRecords()
{
    this->recordIds.clear();
}

bool DataLoader::performDatabaseConnection()
{
    if(!db.isOpen())
    {
        if(!db.open())
        {
            qDebug() << "An error was encountered: "<< QSqlError(db.lastError()).text();
            return false;
        }
    }

    return true;
}
int DataLoader::getRecordIDOfFirstRecord(int begin)
{
    if(this->performDatabaseConnection())
    {
        QString statement("Select Top 1 rows.Rekord_ID From (Select Top "+QString::number(begin)+" ROW_NUMBER() over(Order by Rekord_ID) as RowNr,  Rekord_ID From Rekord where Rekord.Obiekt_ID = "+QString::number(this->objectId)+") as rows order by rows.RowNr desc");

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            while (query.next()) {
                qDebug() << query.value(0).toString();
                return query.value(0).toInt();
            }
        }
    }
    return -1;
}
