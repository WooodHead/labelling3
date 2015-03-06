#ifndef PROPERTYNAMEDLG_H
#define PROPERTYNAMEDLG_H

#include <QDialog>

namespace Ui {
class ProPertyNameDlg;
}

class ProPertyNameDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProPertyNameDlg(QWidget *parent = 0);
    ~ProPertyNameDlg();
    
private:
    Ui::ProPertyNameDlg *ui;
};

#endif // PROPERTYNAMEDLG_H
