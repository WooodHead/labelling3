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
    
    if(!createConnection(db))
    {
        QMessageBox::warning(this,tr("提示"),tr("不能打开数据库链接"),QMessageBox::Close);
        return;
    }
    else
        initTableView();
}

UserManagement::~UserManagement()
{
    if(_model) 
        delete _model;
    db.close();
    delete ui;
}

void UserManagement::initTableView()
{
    if(!_model) 
        _model = new QSqlTableModel(this);
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
    // added by zhyn
    ui->_userTableView->setAlternatingRowColors(true);
    ui->_userTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->_userTableView->setSelectionMode(QTableView::SingleSelection);
    //        ui->_userTableView->resizeColumnsToContents();
    ui->_userTableView->resizeColumnToContents(0);
    ui->_userTableView->resizeColumnToContents(5);
    ui->_userTableView->resizeColumnToContents(2);
    ui->_userTableView->verticalHeader()->setVisible(false);
    ui->_userTableView->setEditTriggers(QTableView::NoEditTriggers);
    
    modifyAction = new QAction(tr("修改"),ui->_userTableView);
    this->ui->_userTableView->addAction(modifyAction);
    this->ui->_userTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(modifyAction,SIGNAL(triggered()),this,SLOT(modifyData()));
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
        QModelIndex index = ui->_userTableView->currentIndex();
        int curRow = index.row();
        _model->removeRow(curRow);
        
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认要删除当前行?"), QMessageBox::Yes | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            _model->submitAll();
            db.commit();
        }
        else if(reply == QMessageBox::Cancel)
        {
            _model->revertAll();
            db.rollback();
        }
    }
}

void UserManagement::on__addUser_clicked()
{
    //added by zhyn
    this->_userAddDialog = new UserAdd(this);
    connect(this,SIGNAL(closeAddDialog()),this->_userAddDialog,SLOT(close()));
    this->_userAddDialog->show();
}

void UserManagement::addUser(UserInfo *userInfo)
{
    QString email     = userInfo->getEmail();
    QString pasword   = userInfo->getPasword();
    int priority      = userInfo->getPriority();
    QString telephone = userInfo->getTelePhone();
    QString username  = userInfo->getUserName();
    delete userInfo;
    QSqlRecord record = _model->record();
    record.setValue(1,username);
    record.setValue(2,email);
    record.setValue(3,telephone);
    record.setValue(4,pasword);
    record.setValue(5,priority);
    _model->insertRecord(-1,record);
    if(_model->submitAll())
    {
        db.commit();
        QMessageBox::warning(this->_userAddDialog,tr("提示"),tr("添加新用户成功"),QMessageBox::Close);
    }
    else
    {
        _model->revert();
        db.rollback();
        QMessageBox::warning(this->_userAddDialog,tr("提示"),tr("添加新用户失败"),QMessageBox::Close);
    }
    emit closeAddDialog();
}

void UserManagement::on__editUser_clicked()
{
    if(ui->_userTableView->selectionModel()->selection().count()<=0)
    {
        QMessageBox::warning(this, tr(""), tr("当前未选中任何记录!"), QMessageBox::Close);
        return;
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认要修改当前用户信息?"), QMessageBox::Yes | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            _model->submitAll();
            db.commit();
        }
        else if(reply == QMessageBox::Cancel)
        {
            _model->revertAll();
            db.rollback();
        }
    }
    ui->_userTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->_userTableView->setEditTriggers(QTableView::NoEditTriggers);
}

void UserManagement::modifyData()
{
    ui->_userTableView->setSelectionBehavior(QTableView::SelectItems);
    ui->_userTableView->setEditTriggers(QTableView::AllEditTriggers);
}
