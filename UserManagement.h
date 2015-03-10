#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>

#include "UserAdd.h"
#include "UserInfo.h"
#include "UserEdit.h"

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

    void addUser(UserInfo *userInfo);
    void editUser(UserInfo *userInfo);
    
    void on__editUser_clicked();
    
    void edit();
    
    void on__searchUser_clicked();

signals:
    void closeAddDialog();

private:
    Ui::UserManagement *ui;

    QSqlTableModel *_model;

    void initTableView();
    UserAdd *_userAddDialog;
    useredit* _userEditDialog;
    QSqlDatabase db;

    int _selectedRow;

    UserInfo* getSelectedUserInfo();
};

#endif // USERMANAGEMENT_H
