#include "useradd.h"
#include "ui_useradd.h"

#include "connection.h"

#include <QSqlQuery>
#include <QMessageBox>

UserAdd::UserAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserAdd)
{
    ui->setupUi(this);
    
    ui->_authorityCombo->addItem(tr("普通用户"));
    ui->_authorityCombo->addItem(tr("管理员"));
 
    connect(this,SIGNAL(addUser(UserInfo*)),parent,SLOT(addUser(UserInfo*)));
}

UserAdd::~UserAdd()
{
    delete ui;
}

void UserAdd::on__cancel_clicked()
{
    close();
}

void UserAdd::on__add_clicked()
{
    QString username, passwd, confirmPasswd, email, phone, authority;
    int authority2;

    username = ui->_editUsername->text();
    passwd = ui->_editPasswd->text();
    confirmPasswd = ui->_editConfirmPasswd->text();
    email = ui->_editEmail->text();
    phone = ui->_editPhone->text();
    authority2 = ui->_authorityCombo->currentIndex();
    authority = ui->_authorityCombo->itemText(authority2);

    if(username.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("用户名不能为空!"), QMessageBox::Close);
        return;
    }
    else if(passwd.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("密码不能为空!"), QMessageBox::Close);
        return;
    }
    else if(confirmPasswd.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("确认密码不能为空!"), QMessageBox::Close);
        return;
    }
    else if(passwd != confirmPasswd)
    {
        QMessageBox::warning(this, tr(""), tr("两次输入密码不一致!"), QMessageBox::Close);
        return;
    }
    else if(email.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("邮箱不能为空!"), QMessageBox::Close);
        return;
    }
    else if(authority.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("权限不能为空!"), QMessageBox::Close);
        return;
    }
    //added by zhyn
    userInfo = new UserInfo(username,passwd,email,phone,authority2);
    emit addUser(userInfo);
//    QSqlDatabase db;
//    if(!createConnection(db))
//    {
//        QMessageBox::critical(0, qApp->tr("Cannot open database"),
//                              qApp->tr("数据库连接失败!"),
//                              QMessageBox::Cancel);
//    }

//    QSqlQuery query(db);
//    bool ret = query.exec(QString("insert row into user (name, passwd, email, phone, authority) values (%1, %2, %3, %4, %5)").arg(username).arg(passwd).arg(email).arg(phone).arg(authority2));
//    if(ret == false)
//    {
//        QMessageBox::warning(this, tr(""), tr("添加失败!"), QMessageBox::Close);
//        db.close();
//        return;
//    }
//    else
//    {
//        db.close();
//        close();
//    }
}
