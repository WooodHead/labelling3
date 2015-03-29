#include "UserEdit.h"
#include "ui_useredit.h"

#include <QMessageBox>

useredit::useredit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::useredit)
{
    ui->setupUi(this);
    setWindowTitle(tr("编辑用户"));
    setModal(true);

    ui->_edit->setIcon(Global::Awesome->icon(edit));
    ui->_cancel->setIcon(Global::Awesome->icon(remove_));

    this->ui->_editUsername->setEnabled(false);

    ui->_authorityCombo->addItem(tr("普通用户"));
    ui->_authorityCombo->addItem(tr("管理员"));

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    connect(this->ui->_edit, SIGNAL(clicked()), this, SLOT(on_edit_clicked()));
    connect(this,SIGNAL(editUser(UserInfo*)),parent,SLOT(editUser(UserInfo*)));
}

useredit::~useredit()
{
    delete ui;
}

void useredit::showEdit(UserInfo *userInfo)
{
    if(!userInfo) return;

    show();

    this->ui->_editUsername->setText( userInfo->getUserName() );
    this->ui->_editEmail->setText(userInfo->getEmail());
    this->ui->_editPhone->setText(userInfo->getTelePhone());
    this->ui->_editPasswd->setText(userInfo->getPasword());
    this->ui->_editConfirmPasswd->setText(userInfo->getPasword());
    this->ui->_authorityCombo->setCurrentIndex(userInfo->getPriority());
}

void useredit::on_edit_clicked()
{
    emit editUser(getInfo());
}

UserInfo *useredit::getInfo()
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

void useredit::on__cancel_clicked()
{
    close();
}

