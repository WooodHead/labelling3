#ifndef PROPERTYNAMEDLG_H
#define PROPERTYNAMEDLG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ProPertyNameDlg;
}

class ProPertyNameDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProPertyNameDlg(QWidget *parent = 0);
    ~ProPertyNameDlg();
    
signals:
    void setpropertyName(QString propertyName);
    
private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::ProPertyNameDlg *ui;
};

#endif // PROPERTYNAMEDLG_H
