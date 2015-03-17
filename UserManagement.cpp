#include "UserManagement.h"
#include "ui_usermanagement.h"

#include "Connection.h"
#include "UserAdd.h"

#include <QMessageBox>
#include <QDebug>

UserManagement::UserManagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManagement)
{
    ui->setupUi(this);

    setWindowTitle(tr("用户管理"));

    ui->_authorityCombo->addItem(tr(""));
    ui->_authorityCombo->addItem(tr("普通用户"));
    ui->_authorityCombo->addItem(tr("管理员"));

    _model = 0;
    _selectedRow = -1;

    setIcons();
    
    if(!createConnection(db))
    {
        QMessageBox::warning(this, tr("提示"), tr("不能打开数据库链接"), QMessageBox::Close);
        return;
    }
    else initTableView();
}

UserManagement::~UserManagement()
{
    if(_model)
    {
        delete _model;
        _model = 0;
    }
    db.close();
    delete ui;
}

void UserManagement::initTableView()
{
    if(!_model)  _model = new QSqlTableModel(this);
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
    ui->_userTableView->setAlternatingRowColors(true);
    ui->_userTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->_userTableView->setSelectionMode(QTableView::SingleSelection);
    ui->_userTableView->resizeColumnToContents(0);
    ui->_userTableView->resizeColumnToContents(5);
    ui->_userTableView->resizeColumnToContents(2);
    ui->_userTableView->verticalHeader()->setVisible(false);
    ui->_userTableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->_userTableView->horizontalHeader()->setHighlightSections(false);
}

UserInfo *UserManagement::getSelectedUserInfo()
{
    if(ui->_userTableView->selectionModel()->selection().count()<=0) return NULL;

    _selectedRow   = ui->_userTableView->selectionModel()->currentIndex().row();
    QString username = ui->_userTableView->model()->data(ui->_userTableView->model()->index(_selectedRow,1)).toString();
    QString email    = ui->_userTableView->model()->data(ui->_userTableView->model()->index(_selectedRow,2)).toString();
    QString phone    = ui->_userTableView->model()->data(ui->_userTableView->model()->index(_selectedRow,3)).toString();
    QString passwd   = ui->_userTableView->model()->data(ui->_userTableView->model()->index(_selectedRow,4)).toString();
    int authority    = ui->_userTableView->model()->data(ui->_userTableView->model()->index(_selectedRow,5)).toInt();

    return new UserInfo(username, passwd, email, phone, authority);
}

void UserManagement::setIcons()
{
    ui->_addUser->setIcon(Global::Awesome->icon(userplus));
    ui->_deleteUser->setIcon(Global::Awesome->icon(usertimes));
    ui->_searchUser->setIcon(Global::Awesome->icon(search));
    ui->_editUser->setIcon(Global::Awesome->icon(pencilsquareo));
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

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认要删除当前行?"), QMessageBox::Yes | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            _model->removeRow(curRow);
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
    if(!userInfo) return;

    QString email     = userInfo->getEmail();
    QString pasword   = userInfo->getPasword();
    int priority      = userInfo->getPriority();
    QString telephone = userInfo->getTelePhone();
    QString username  = userInfo->getUserName();

    if(userInfo) delete userInfo;

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
        QMessageBox::warning(this->_userAddDialog, tr("提示"), tr("添加成功"), QMessageBox::Close);
    }
    else
    {
        _model->revert();
        db.rollback();
        QMessageBox::warning(this->_userAddDialog, tr("提示"), tr("添加失败"), QMessageBox::Close);
    }
    emit closeAddDialog();
}

void UserManagement::editUser(UserInfo *userInfo)
{
    if(!userInfo) return;
    if(_selectedRow == -1) return;

    QString email     = userInfo->getEmail();
    QString pasword   = userInfo->getPasword();
    int priority      = userInfo->getPriority();
    QString telephone = userInfo->getTelePhone();
    QString username  = userInfo->getUserName();

    if(userInfo) delete userInfo;

    _model->setFilter(QString("name = '%1'").arg(username));
    if(_model->select())
    {
        if(_model->rowCount() == 1)
        {
            QSqlRecord record = _model->record(0);
            record.setValue(1,username);
            record.setValue(2,email);
            record.setValue(3,telephone);
            record.setValue(4,pasword);
            record.setValue(5,priority);

            _model->setRecord(0, record);

            if(_model->submitAll())
            {
                db.commit();
                initTableView();
                QMessageBox::warning(this->_userEditDialog, tr("提示"), tr("修改成功"), QMessageBox::Close);
            }
            else
            {
                _model->revert();
                db.rollback();
                QMessageBox::warning(this->_userEditDialog, tr("提示"), tr("修改失败"), QMessageBox::Close);
            }
        }
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
        reply = QMessageBox::question(this, tr("提示"), tr("确认要修改当前用户信息?"), QMessageBox::Yes | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            this->_userEditDialog = new useredit(this);
            connect(this, SIGNAL(closeAddDialog()), this->_userEditDialog, SLOT(close()));

            this->_userEditDialog->showEdit(getSelectedUserInfo());

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

void UserManagement::edit()
{
    ui->_userTableView->setSelectionBehavior(QTableView::SelectItems);
    ui->_userTableView->setEditTriggers(QTableView::AllEditTriggers);
}

void UserManagement::on__searchUser_clicked()
{
    QString filter = tr("");

    QString username = ui->_editUsername->text();
    QString email = ui->_editEmail->text();
    QString phone = ui->_editPhone->text();
    int authority = ui->_authorityCombo->currentIndex()-1;

    if(username.isEmpty() && email.isEmpty() && phone.isEmpty() && authority == -1)
        return;

    if(!username.isEmpty())
    {
        filter.append(QString("name='%1' and ").arg(username));
    }
    if(!email.isEmpty())
    {
        filter.append(QString("email='%1' and ").arg(email));
    }
    if(!phone.isEmpty())
    {
        filter.append(QString("phone='%1' and ").arg(phone));
    }
    if(authority != -1)
    {
        filter.append(QString("authority=%1 and ").arg(authority));
    }

    filter = filter.left(filter.length()-5);

    _model->setFilter(filter);
    _model->select();
}
