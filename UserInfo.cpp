#include "UserInfo.h"

UserInfo::UserInfo()
{
}
UserInfo::UserInfo(QString userName, QString pasword, QString email, QString telephone, int priority)
{
    this->_email = email;
    this->_pasword = pasword;
    this->_priority = priority;
    this->_telephone = telephone;
    this->_userName = userName;
}

QString UserInfo::getEmail()
{
    return this->_email;
}

QString UserInfo::getPasword()
{
    return this->_pasword;
}

int UserInfo::getPriority()
{
    return this->_priority;
}

QString UserInfo::getTelePhone()
{
    return this->_telephone;
}

QString UserInfo::getUserName()
{
    return this->_userName;
}
