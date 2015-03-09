#include "Login.h"
#include "ui_login.h"

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>

#include "Global.h"
#include "Connection.h"
#include "imagecompletionui.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    _moving = false;

    setWindowTitle( tr("用户登录") );
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    //this->loadDesign("default");
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());

    ui->_icon->resize(30, 30);
    ui->_icon->setStyleSheet("background: url(:/new/prefix1/icons/login.png);");

    ui->_editUsername->setPlaceholderText("用户名");
    ui->_editUsername->setStyleSheet("border: 3px solid #eee;height: 30px;");

    ui->_editPasswd->setPlaceholderText("密码");
    ui->_editPasswd->setStyleSheet("border: 3px solid #eee;height: 30px;");

    ui->_login->setFocus();
    ui->_login->setStyleSheet("padding:5px 0;border:0px;background-color: #26b9a5;color:white;");
    ui->_cancel->setStyleSheet("padding:5px 0;border:0px;background-color: #26b9a5;color:white;");

    ui->_login->setAutoDefault(false);
    ui->_cancel->setAutoDefault(false);


    connect(ui->_login, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

Login::~Login()
{
    delete ui;
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(_moving)
    {
        this->move(event->globalPos() - _dpos);
    }
}

void Login::mousePressEvent(QMouseEvent *event)
{
    _moving = true;

    _pos = this->pos();
    _mousePos = event->globalPos();
    _dpos = _mousePos - _pos;
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    _moving = false;
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
        QMessageBox::critical(0, qApp->tr("提示"),
                              qApp->tr("数据库连接失败!"),
                              QMessageBox::Cancel);
        return;
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



