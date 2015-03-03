#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>

static QStringList mysqlSettings()
{
    QStringList values;

    QSettings s("Config.ini", QSettings::IniFormat);
    s.beginGroup("MYSQL");
    QStringList childKeys = s.childKeys();

    foreach(const QString &childKey, childKeys)
    {
        values << s.value(childKey).toString();
    }

    s.endGroup();

    return values;
}


static bool createConnection(QSqlDatabase &db)
{
    QStringList mysqlConfigs = mysqlSettings();
    if(mysqlConfigs.isEmpty()) return false;

    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(mysqlConfigs[1]);
    db.setDatabaseName(mysqlConfigs[0]);
    db.setUserName(mysqlConfigs[3]);
    db.setPassword(mysqlConfigs[2]);

    if(db.isOpen())
    {
        db.close();
    }

    if(!db.open()){
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
