#ifndef ADVANCESEARCHDLG_H
#define ADVANCESEARCHDLG_H

#include <QDialog>

namespace Ui {
class AdvanceSearchDlg;
}

class AdvanceSearchDlg : public QDialog
{
    Q_OBJECT
    
public:
     AdvanceSearchDlg(QWidget *parent = 0);
    ~AdvanceSearchDlg();
    
private:
    Ui::AdvanceSearchDlg *ui;
};

#endif // ADVANCESEARCHDLG_H
