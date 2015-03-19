#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT
    
public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();
    
private slots:
    void on__pushButtonTest_clicked();

    void on__pushButtonOK_clicked();

    void on__pushButtonCancle_clicked();

private:
    Ui::Setting *ui;

signals:
    void showLogin();
};

#endif // SETTING_H
