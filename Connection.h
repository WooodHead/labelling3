#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>

#include "Global.h"

static bool createConnection(QSqlDatabase &db)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(Global::Hostname);
    db.setDatabaseName(Global::Database);
    db.setUserName(Global::Username);
    db.setPassword(Global::Passwd);

    if(!db.open())
    {
        return false;
    }

    return true;
}

#endif // CONNECTION_H
