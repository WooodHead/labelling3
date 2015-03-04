#ifndef USERINFO_H
#define USERINFO_H

#include <QString>

class UserInfo
{
public:
    UserInfo();
    UserInfo(QString userName,QString pasword,QString email,QString telephone,int priority);
    
    QString getUserName();
    QString getPasword();
    QString getEmail();
    QString getTelePhone();
    int getPriority();
    
private:
    QString _userName;
    QString _pasword;
    QString _email;
    QString _telephone;
    int     _priority;
};

#endif // USERINFO_H
