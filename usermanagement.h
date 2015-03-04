#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>

#include "useradd.h"
#include "userinfo.h"

namespace Ui {
class UserManagement;
}

class UserManagement : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserManagement(QWidget *parent = 0);
    ~UserManagement();
    
private slots:
    void on__deleteUser_clicked();

    void on__addUser_clicked();
    // added by zhyn
    void addUser(UserInfo *userInfo);
    
    void on__editUser_clicked();
    
    void modifyData();
    
signals:
    void closeAddDialog();

private:
    Ui::UserManagement *ui;

    QSqlTableModel *_model;

    void initTableView();
    // added by zhyn
    UserAdd *_userAddDialog;
    QSqlDatabase db;
    QAction *modifyAction;
};

#endif // USERMANAGEMENT_H
