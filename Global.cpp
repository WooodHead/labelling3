#include "Global.h"

#include <QCoreApplication>
#include <QApplication>
#include <QFileInfo>
#include <QDir>

QString Global::Authority = "2";
QString Global::DisplayName = "";

QString Global::Database = "bookdata";
QString Global::Hostname = "localhost";
QString Global::Username = "root";
QString Global::Passwd   = "123456";

QString Global::PathMask = "";
QString Global::PathResult = "";
QString Global::ExtMask = "";
QString Global::ExtResult = "";

QColor Global::LabelledColor = QColor(255, 255, 0);
QColor Global::UnLabelledColor = QColor(255, 0, 0);

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

    Global::PathMask = Global::settings->value("IMAGE/pathMask", Global::PathMask).toString();
    Global::PathResult = Global::settings->value("IMAGE/pathResult", Global::PathResult).toString();

    QDir dirMask(Global::PathMask);
    QDir dirResult(Global::PathResult);
    if(!dirMask.exists()) dirMask.mkdir(Global::PathMask);
    if(!dirResult.exists()) dirResult.mkdir(Global::PathResult);

    Global::ExtMask = Global::settings->value("IMAGE/extMask", Global::ExtMask).toString();
    Global::ExtResult = Global::settings->value("IMAGE/extResult", Global::ExtResult).toString();

    Global::Init = true;
}
