#ifndef PROPERTYNAMEDLG_H
#define PROPERTYNAMEDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlRecord>
#include "Global.h"

namespace Ui {
class ProPertyNameDlg;
}

class QPushButton;

class ProPertyNameDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProPertyNameDlg(QWidget *parent = 0);
    ProPertyNameDlg(QWidget *parent ,QString propertyname);
    ~ProPertyNameDlg();
    
signals:
    void setpropertyName(QString propertyName);
    
private slots:
    
    void on_OkButton_clicked();
    
    void on_cancelButton_clicked();
    
private:
    void trriger(QString text);
    
private:
    Ui::ProPertyNameDlg *ui;
};

#endif // PROPERTYNAMEDLG_H
