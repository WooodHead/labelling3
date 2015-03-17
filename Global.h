#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSettings>
#include <QColor>
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

    // Icons
    static QtAwesome *Awesome;

    static void initialize();

private:
    static QSettings *settings;
    static bool Init;
};

#endif // GLOBAL_H
