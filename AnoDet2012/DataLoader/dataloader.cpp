#include "dataloader.h"

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

void DataLoader::initDataRecordTable()
{
    if(this->performDatabaseConnection())
    {
        this->recordSize = 0;
        this->programAlarmIds = *(new vector<double>());
        this->recordIds = *(new vector<int>());
        this->dataRecordTable = new DataRecordTable(*(new vector<string>(0)), *(new vector<string>(0)), *(new vector<InfoTable>()));

        QString statement("Select Program_pomiar.Program_pomiar_ID, Program_pomiar.Nazwa_pomiaru from Program_pomiar where Program_pomiar.Konfiguracja_ID = "+QString::number(this->objectId)+" order by Program_pomiar_ID");
        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {

            while (query.next()) {
                ++this->recordSize;
                this->programAlarmIds.push_back(query.value(0).toInt());
                dataRecordTable->dataNames.push_back(query.value(1).toString().toStdString());
            }
        }
    }
}

void DataLoader::loadRecords(unsigned long limit)
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT Rekord_ID, Data");
        for(int a = 0; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programAlarmIds[a])+"]");
        }
        statement.append("from (Select "+(limit?"TOP "+QString::number(limit*this->recordSize):"")+" Wyniki_pomiar.Rekord_ID, Program_pomiar_ID, Wartosc, Data From Wyniki_pomiar left join Rekord on Rekord.Rekord_ID = Wyniki_pomiar.Rekord_ID where Rekord.Obiekt_ID = "+QString::number(this->objectId)+" order by Rekord_ID) p PIVOT( min(Wartosc) FOR Program_pomiar_ID IN (["+QString::number(this->programAlarmIds[0])+"]");
        for(int a = 1; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programAlarmIds[a])+"]");
        }
        statement.append(")) as pvt order by pvt.Rekord_ID");

        //qDebug() << statement;

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {

            while (query.next()) {
                int pos = dataRecordTable->addRecord(query.value(1).toDateTime().toTime_t(),*(new vector<double>(0)),*(new vector<double>(0)),*(new vector<int>(0)),false);
                this->recordIds.push_back(query.value(0).toInt());
                for(int a = 0; a < this->recordSize; ++a)
                {
                    dataRecordTable->records.at(pos).data.push_back(query.value(a+2).toDouble());
                }
            }
        }
    }
}

void DataLoader::setAlarmFlagToRecords()
{
    if(this->performDatabaseConnection())
    {
        QString statement("select distinct Wyniki_alarm.Rekord_ID from Wyniki_alarm left join Program_alarm on Program_alarm.Program_alarm_ID = Wyniki_alarm.Program_alarm_ID where Wyniki_alarm.Status>0 and (Program_alarm.Typ_alarmu_ID=0 or Program_alarm.Typ_alarmu_ID=3 or Program_alarm.Typ_alarmu_ID=5) and Program_alarm.Konfiguracja_ID = "+QString::number(this->objectId)+" and Wyniki_alarm.Rekord_ID >= "+QString::number(this->recordIds[0])+" and Wyniki_alarm.Rekord_ID <= "+QString::number(this->recordIds.back())+" order by Wyniki_alarm.Rekord_ID");
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
                        this->dataRecordTable->records[currentRecord].isAnomaly = true;
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
    this->dataRecordTable->records.clear();
    this->recordIds.clear();
}

void DataLoader::printRecords()
{
    vector<string>::iterator names;
    vector<DataRecord>::iterator drIt;
    vector<double>::iterator dataIt;
    QString record("");
    for(names = this->dataRecordTable->dataNames.begin();names!=this->dataRecordTable->dataNames.end();++names)
    {
        record+=QString::fromStdString(*names)+=", ";
    }
    qDebug() << record;
    record.clear();
    int current = 0;
    for(drIt = this->dataRecordTable->records.begin();drIt!=this->dataRecordTable->records.end();++drIt)
    {
        DataRecord dr = *drIt;
        record+=QString::number(this->recordIds[current++])+" ";
        record+=dr.isAnomaly?"1 -- ":"0 -- ";
        if(dr.data.empty())
        {
            record+="Pusty rekord";
        }else{
            for(dataIt = dr.data.begin();dataIt!=dr.data.end();++dataIt)
            {
                record+=QString::number(*dataIt).append(", ");
            }
        }
        qDebug() << record;
        record.clear();
    }

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
