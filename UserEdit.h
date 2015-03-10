#ifndef USEREDIT_H
#define USEREDIT_H

#include <QDialog>

#include "UserInfo.h"
#include "QtAwesome.h"

namespace Ui {
class useredit;
}

class useredit : public QDialog
{
    Q_OBJECT
    
public:
    explicit useredit(QWidget *parent = 0);
    ~useredit();

    void showEdit(UserInfo *_userInfo);

private slots:
    void on_edit_clicked();
    
    void on__cancel_clicked();

signals:
    void editUser(UserInfo *_userInfo);

private:
    Ui::useredit *ui;

    UserInfo *_userInfo;
    int _selectedRow;

    UserInfo* getInfo();

    QtAwesome* _awesome;
};

#endif // USEREDIT_H
