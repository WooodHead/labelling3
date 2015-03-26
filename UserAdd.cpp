#include "UserAdd.h"
#include "ui_useradd.h"

#include "Connection.h"

#include <QSqlQuery>
#include <QMessageBox>

UserAdd::UserAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserAdd)
{
    ui->setupUi(this);
    setModal(true);

    ui->_add->setIcon(Global::Awesome->icon(userplus));
    ui->_cancel->setIcon(Global::Awesome->icon(remove_));

    ui->_authorityCombo->addItem(tr("普通用户"));
    ui->_authorityCombo->addItem(tr("管理员"));

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

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
    emit addUser(getInfo());
}

UserInfo *UserAdd::getInfo()
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
        QMessageBox::warning(this, tr("提示"), tr("用户名不能为空!"), QMessageBox::Close);
        return NULL;
    }
    else if(passwd.isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("密码不能为空!"), QMessageBox::Close);
        return NULL;
    }
    else if(confirmPasswd.isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("确认密码不能为空!"), QMessageBox::Close);
        return NULL;
    }
    else if(passwd != confirmPasswd)
    {
        QMessageBox::warning(this, tr("提示"), tr("两次输入密码不一致!"), QMessageBox::Close);
        return NULL;
    }
    else if(email.isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("邮箱不能为空!"), QMessageBox::Close);
        return NULL;
    }
    else if(authority.isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("权限不能为空!"), QMessageBox::Close);
        return NULL;
    }

    return new UserInfo(username, passwd, email, phone, authority2);
}

