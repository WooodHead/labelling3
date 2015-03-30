#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSettings>
#include <QColor>
#include <QSqlDatabase>
#include "QtAwesome.h"

class Global
{
public:
    static QString Authority;
    static QString DisplayName;

    // Database
    static QString Hostname;
    static QString Database;
    static QString Username;
    static QString Passwd;

    // LeftWindow Color
    static QColor  LabelledColor;
    static QColor  UnLabelledColor;

    // Path for Seg Results and Mask
    static QString PathImage;
    static QString PathResult;
    static QString PathMask;
    static QString ExtResult;
    static QString ExtMask;

    static QString NewName;
    static QString MoliId;

    // Icons
    static QtAwesome *Awesome;

    static void initialize();
    static bool createConnection(QSqlDatabase &db);

    static QSettings *settings;

private:
    static bool Init;
};

#endif // GLOBAL_H
