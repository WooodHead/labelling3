#ifndef USERADD_H
#define USERADD_H

#include <QDialog>

namespace Ui {
class UserAdd;
}

class UserAdd : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserAdd(QWidget *parent = 0);
    ~UserAdd();
    
private slots:
    void on__cancel_clicked();

    void on__add_clicked();

private:
    Ui::UserAdd *ui;
};

#endif // USERADD_H
