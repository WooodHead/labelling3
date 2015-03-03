#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSettings>

class Global
{
public:
    static QString Authority;
    static QString DisplayName;

    static QString Hostname;
    static QString Database;
    static QString Username;
    static QString Passwd;

    static void initialize();

private:
    static QSettings *settings;
    static bool Init;
};

#endif // GLOBAL_H
