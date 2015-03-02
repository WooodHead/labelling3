#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

static bool createConnection(QSqlDatabase &db)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("bookdata");
    db.setUserName("root");
    db.setPassword("a13011598286");

    if (!db.open()) {
 /*       QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection."),
                              QMessageBox::Cancel);*/
        return false;
    }
    return true;
}
class helper
{
public:
    static QString userid;
    static int flag;
    static QString tran;
};
//extern QString userid;
#endif // CONNECTION_H
