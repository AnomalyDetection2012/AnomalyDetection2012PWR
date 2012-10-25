#include <QDebug>
#include <QtSql>

using namespace QSql;

//
// SQL Database Drivers:
// http://doc.qt.digia.com/latest/sql-driver.html
//
// QtSql Module:
// http://qt-project.org/doc/qt-4.8/qtsql.html
//
// Change Tracking:
// http://www.solidq.com/sqj/Pages/Relational/Microsoft-SQL-Server-2008-R2-Introducing-Change-Tracking-Part-1-of-3.aspx
// http://www.solidq.com/sqj/Pages/Relational/Microsoft-SQL-Server-2008-R2-Introducing-Change-Tracking-Part-2-of-3.aspx
// http://www.solidq.com/sqj/Pages/Relational/Microsoft-SQL-Server-2008-R2-Introducing-Change-Tracking-Part-3-of-3.aspx
//
// SQL Syntax Reference:
// http://www.w3schools.com/sql/sql_quickref.asp
//

int main(int argc, char *argv[]) {
    // Prints a list of the available database drivers
    qDebug() << "All available drivers: " << QSqlDatabase::drivers();


    // Opens the database connection
    // You can use the default driver - QODBC or choose a newer version explicitly - eg. QODBC3.
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    // driver - your datasource type
    // server - your computer name
    // database - the database name
    db.setDatabaseName("Driver={SQL Server};Server=VIRTUALBOX-PC;Database=scswin");
    // You have to use a username who can log on to the database via pure SQL server authentication method.
    // Therefore your SQL server instance must provide that authentication method (you can change that behaviour via Management Studio).
    // Also remember about setting proper roles and user mappings for the user of your choice (Management Studio).
    db.setUserName("scswin_admin");
    db.setPassword("qwe123!");

    if (!db.open()) {
        qDebug() << "An error was encountered: "<< QSqlError(db.lastError()).text();
    } else {
        qDebug() << "The database connection was established successfully.";
    }

    // Checks the database driver features
    QSqlDriver *driver = db.driver();
    qDebug() << "The selected database driver features...";
    qDebug() << "+ Transactions: " << driver -> hasFeature(QSqlDriver::Transactions);
    qDebug() << "+ Query size: " << driver -> hasFeature(QSqlDriver::QuerySize);
    qDebug() << "+ Prepared queries: " << driver -> hasFeature(QSqlDriver::PreparedQueries);
    qDebug() << "+ Named placeholders: " << driver -> hasFeature(QSqlDriver::NamedPlaceholders);
    qDebug() << "+ Last insert id: " << driver -> hasFeature(QSqlDriver::LastInsertId);
    qDebug() << "+ Batch operations: " << driver -> hasFeature(QSqlDriver::BatchOperations);
    qDebug() << "+ Event notifications: " << driver -> hasFeature(QSqlDriver::EventNotifications);


    // SELECT
    {
        QString statement = "SELECT * FROM Jednostka";
        QSqlQuery query(statement, db);
        query.setForwardOnly(true);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            qDebug() << "The SELECT statement was executed successfully. The result is...";
            while (query.next()) {
                // Gets a value by providing the column name
                int index = query.record().indexOf("Jednostka_ID");
                int id = query.value(index).toInt();
                // Gets a value by simply providing the column index
                QString name = query.value(1).toString();
                qDebug() << "ID: " << id << ", Nazwa: " << name;
            }
        }
    }


    // UPDATE
    {
        QString statement = "UPDATE Jednostka SET Nazwa=:name WHERE Jednostka_ID=0";
        QSqlQuery query(db);
        query.prepare(statement);
        query.bindValue(":name", "A");
        bool result = query.exec();

        if (!result) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            qDebug() << "The UPDATE statement was executed successfully.";
        }
    }


    // INSERT
    // If you are going to insert a lot of data you may be interested in a batch execution.
    // For more details see: http://qt-project.org/doc/qt-4.8/qsqlquery.html#execBatch.
    {
        QString statement = "INSERT INTO Jednostka (Jednostka_ID, Nazwa) VALUES (:id, :name)";
        QSqlQuery query(db);
        query.prepare(statement);
        query.bindValue(":id", "100");
        query.bindValue(":name", "test");
        bool result = query.exec();

        if (!result) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            qDebug() << "The INSERT statement was executed successfully.";
        }
    }


    // DELETE
    {
        QString statement = "DELETE FROM Jednostka WHERE Jednostka_ID>=100";
        QSqlQuery query(statement, db);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            qDebug() << "The DELETE statement was executed successfully.";
        }
    }


    // CHANGETABLE
    {
        QString statement = "SELECT C.* from CHANGETABLE(CHANGES Jednostka, NULL) as C";
        QSqlQuery query(statement, db);

        if (!query.isActive()) {
            qDebug() << "An error was encountered: "<< QSqlError(query.lastError()).text();
        } else {
            qDebug() << "The CHANGETABLE statement was executed successfully.";
        }
    }
}
