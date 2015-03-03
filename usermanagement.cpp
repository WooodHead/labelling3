#include "usermanagement.h"
#include "ui_usermanagement.h"

#include "connection.h"
#include "useradd.h"

#include <QMessageBox>
#include <QDebug>

UserManagement::UserManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManagement)
{
    ui->setupUi(this);

    setWindowTitle(tr("用户管理"));

    _model = 0;

    initTableView();
}

UserManagement::~UserManagement()
{
    if(_model) delete _model;

    delete ui;
}

void UserManagement::initTableView()
{
    QSqlDatabase db;
    if(createConnection(db))
    {
        if(!_model) _model = new QSqlTableModel();
        _model->setTable("user");
        _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        _model->database().transaction();
        _model->select();

        _model->setHeaderData(0, Qt::Horizontal, tr("序号"));
        _model->setHeaderData(1, Qt::Horizontal, tr("用户名"));
        _model->setHeaderData(2, Qt::Horizontal, tr("邮箱"));
        _model->setHeaderData(3, Qt::Horizontal, tr("手机号码"));
        _model->setHeaderData(4, Qt::Horizontal, tr("密码"));
        _model->setHeaderData(5, Qt::Horizontal, tr("权限"));

        ui->_userTableView->setModel(_model);
    }
}

void UserManagement::on__deleteUser_clicked()
{


    if(ui->_userTableView->selectionModel()->selection().count()<=0)
    {
        QMessageBox::warning(this, tr(""), tr("当前未选中任何记录!"), QMessageBox::Close);
        return;
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认要删除当前行?"), QMessageBox::Yes | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            QSqlDatabase db;
            if(createConnection(db))
            {
                QModelIndex index = ui->_userTableView->currentIndex();
                int curRow = index.row();

                _model->removeRow(curRow);

                if(_model->submitAll())
                {
                    _model->database().commit();
                }
                else
                {
                    _model->database().rollback();
                }
            }
            db.close();
        }
        else if(reply == QMessageBox::Cancel)
        {

        }
    }
}

void UserManagement::on__addUser_clicked()
{
    (new UserAdd)->show();
}
