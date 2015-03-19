#include "Setting.h"
#include "ui_Setting.h"

#include "Global.h"
#include "Connection.h"
#include <QFileInfo>

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    connect(this, SIGNAL(showLogin()), parent, SLOT(showLogin()));

    ui->_lineEditIP->setText(Global::Hostname);
    ui->_lineEditDatabaseName->setText(Global::Database);
    ui->_lineEditUsername->setText(Global::Username);
    ui->_lineEditPasswd->setText(Global::Passwd);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on__pushButtonTest_clicked()
{
    QSqlDatabase db;
    if(!createConnection(db))
    {
        ui->_connectStatus->setText(tr("连接失败!"));
    }
    else
    {
        ui->_connectStatus->setText(tr("连接成功!"));
    }
}

void Setting::on__pushButtonOK_clicked()
{
    if(Global::settings)
    {
        Global::settings->setValue("MYSQL/databasename", ui->_lineEditDatabaseName->text());
        Global::settings->setValue("MYSQL/hostname", ui->_lineEditIP->text());
        Global::settings->setValue("MYSQL/username", ui->_lineEditUsername->text());
        Global::settings->setValue("MYSQL/passwd", ui->_lineEditPasswd->text());

        Global::Database = Global::settings->value("MYSQL/databasename", Global::Database).toString();
        Global::Hostname = Global::settings->value("MYSQL/hostname", Global::Hostname).toString();
        Global::Username = Global::settings->value("MYSQL/username", Global::Username).toString();
        Global::Passwd   = Global::settings->value("MYSQL/passwd", Global::Passwd).toString();
    }

    this->close();
    emit showLogin();
}

void Setting::on__pushButtonCancle_clicked()
{
    this->close();
    emit showLogin();
}
