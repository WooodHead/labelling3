#ifndef IMPDLG_H
#define IMPDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class ImpDlg;
}

class ImpDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImpDlg(QWidget *parent = 0);
    ~ImpDlg();
    
private slots:
    void on_pushButton_clicked();
    
    void on_okPushButton_clicked();
    
    void on_cancelPushButton_clicked();
    
signals:
    void sendPackgePath(QString path);
    
private:
    Ui::ImpDlg *ui;
    QString packgePath;
};

#endif // IMPDLG_H
