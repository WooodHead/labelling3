#include "login.h"
#include "ui_login.h"

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>

#include "global.h"
#include "connection.h"
#include "imagecompletionui.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    setWindowTitle( tr("用户登录") );

    this->loadDesign("default");
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());


    connect(ui->_login, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

Login::~Login()
{
    delete ui;
}

void Login::login()
{
    _username = ui->_editUsername->text();
    _passwd   = ui->_editPasswd->text();

    if(_username.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("用户名不能为空!"), QMessageBox::Close);
        return;
    }
    if(_passwd.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("密码不能为空!"), QMessageBox::Close);
        return;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("数据库连接失败!"),
                              QMessageBox::Cancel);
    }

    QString sql = "select * from user where name = ? and passwd = ?";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0, _username);
    query.bindValue(1, _passwd);

    if(query.exec())
    {
        if(query.size() == 0)
        {
            QMessageBox::warning(this, tr("失败"), tr("登录失败,用户名或密码不正确!"), QMessageBox::Close);
        }
        else
        {
            if(query.next())
            {
                int no1 = query.record().indexOf("authority");
                _authority = query.value(no1).toString();

                QString connection = db.connectionName();
                db.close();
                QSqlDatabase::removeDatabase(connection);
                return getDataForMainform();
            }
        }
    }
    else  {
        QMessageBox::warning(this,tr("失败"),tr("查询用户信息失败"),QMessageBox::Close);
        return;
    }
}

void Login::getDataForMainform()
{
    Global::Authority = _authority;

    (new ImageCompletionUI)->show();
    this->deleteLater();
}

void Login::loadDesign(QString strDesign)
{
    // set stylesheet
    QFile file(QString("design/%1/Login/form.css").arg(strDesign));
    file.open(QFile::ReadOnly);
    this->ui->centralWidget->setStyleSheet(file.readAll());
}



