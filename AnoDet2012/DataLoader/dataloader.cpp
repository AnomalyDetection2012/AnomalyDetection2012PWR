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
        this->dataRecordTable = new DataRecordTable(*(new vector<string>(0)), *(new vector<string>(0)), *(new vector<InfoTable>()));

        QString statement("Select Program_pomiar.Nazwa_pomiaru from Program_pomiar where Program_pomiar.Konfiguracja_ID = "+QString::number(this->objectId));
        QSqlQuery query(statement, db);
        //query.prepare(statement);
        //query.bindValue(":objectID", this->objectId);
        query.setForwardOnly(true);
        //query.exec();

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {

            while (query.next()) {
                qDebug() << query.value(0).toString();
                dataRecordTable->dataNames.push_back(query.value(0).toString().toStdString());
            }
        }
    }
}

void DataLoader::loadRecords(unsigned long limit)
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT "+(limit?"TOP "+QString::number(limit):"")+" Rekord.[Rekord_ID],[Data], case when alarms.Rekord_ID IS null then 0 else 1 end FROM [SCSWin].[dbo].[Rekord] left join (select distinct Wyniki_alarm.Rekord_ID from [SCSWin].[dbo].Wyniki_alarm where Wyniki_alarm.Status>0) as alarms on alarms.Rekord_ID=Rekord.Rekord_ID where Rekord.Obiekt_ID = "+QString::number(this->objectId));
        //QString statement("SELECT Rekord.[Rekord_ID],[Data] FROM [SCSWin].[dbo].[Rekord] where Rekord.Obiekt_ID = :objectID");
        QSqlQuery query(statement, db);
        //query.prepare(statement);
        //query.bindValue(":objectID", this->objectId);
        query.setForwardOnly(true);
        //query.exec();

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {

            while (query.next()) {
                int pos = dataRecordTable->addRecord(query.value(1).toDateTime().toTime_t(),*(new vector<double>(0)),*(new vector<double>(0)),*(new vector<int>(0)),query.value(2).toBool());
                this->getValuesForDataRecord(query.value(0).toInt(), dataRecordTable->records[pos]);
            }
        }
    }
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
    for(drIt = this->dataRecordTable->records.begin();drIt!=this->dataRecordTable->records.end();++drIt)
    {
        DataRecord dr = *drIt;
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

void DataLoader::getValuesForDataRecord(int recordID, DataRecord &dataRecord)
{
    if(this->performDatabaseConnection())
    {
        QString statement("SELECT [Program_pomiar_ID], [Wartosc] FROM [SCSWin].[dbo].[Wyniki_pomiar] as pomiary_alarm where pomiary_alarm.Rekord_ID=:recordID");
        QSqlQuery query(statement, db);
        query.prepare(statement);
        query.bindValue(":recordID", recordID);
        query.setForwardOnly(true);
        query.exec();

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {

            while (query.next()) {
                dataRecord.infoAddress.push_back(query.value(0).toInt());
                dataRecord.data.push_back(query.value(1).toDouble());
            }
        }
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
