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
        statement.append("from (Select "+(limit?"TOP "+QString::number(limit*this->recordSize):"")+" Wyniki_pomiar.Rekord_ID, Program_pomiar_ID, Wartosc, Data From Wyniki_pomiar left join Rekord on Rekord.Rekord_ID = Wyniki_pomiar.Rekord_ID where Rekord.Obiekt_ID = "+QString::number(this->objectId)+") p PIVOT( min(Wartosc) FOR Program_pomiar_ID IN (["+QString::number(this->programAlarmIds[0])+"]");
        for(int a = 1; a < this->recordSize; ++a)
        {
            statement.append(", ["+QString::number(this->programAlarmIds[a])+"]");
        }
        statement.append(")) as pvt order by pvt.Rekord_ID");

        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {

            while (query.next()) {
                int pos = dataRecordTable->addRecord(query.value(1).toDateTime().toTime_t(),*(new vector<double>(0)),*(new vector<double>(0)),*(new vector<int>(0)),false);
                for(int a = 0; a < this->recordSize; ++a)
                {
                    dataRecordTable->records.at(pos).data.push_back(query.value(a+2).toDouble());
                }
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
