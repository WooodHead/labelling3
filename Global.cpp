#include "Global.h"

#include <QCoreApplication>
#include <QApplication>
#include <QFileInfo>

QString Global::Authority = "2";
QString Global::DisplayName = "";

QString Global::Database = "bookdata";
QString Global::Hostname = "localhost";
QString Global::Username = "root";
QString Global::Passwd   = "123456";

QSettings* Global::settings = 0;
bool Global::Init = false;

void Global::initialize()
{
    if(Global::Init) return;

    QCoreApplication* app = QApplication::instance();

    QString strConfigFile = QFileInfo(app->applicationFilePath()).baseName() + ".ini";
    if(!QFile::exists(strConfigFile))
    {
        qFatal("配置文件不存在:\r\n%s", qPrintable(strConfigFile));
    }

    Global::settings = new QSettings(strConfigFile, QSettings::IniFormat);

    Global::Database = Global::settings->value("MYSQL/databasename", Global::Database).toString();
    Global::Hostname = Global::settings->value("MYSQL/hostname", Global::Hostname).toString();
    Global::Username = Global::settings->value("MYSQL/username", Global::Username).toString();
    Global::Passwd   = Global::settings->value("MYSQL/passwd", Global::Passwd).toString();

    Global::Init = true;
}
