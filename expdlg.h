#ifndef EXPDLG_H
#define EXPDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class ExpDlg;
}

class ExpDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExpDlg(QWidget *parent = 0);
    ExpDlg(QWidget *parent,QString sourcepicPath,QString resultPath);
    ~ExpDlg();
    
private slots:
    void on_CancelPushButton_clicked();
    
    void on_OKPushButton_clicked();
    
    void on_picPushButton_clicked();
    
    void on_resultPushButton_clicked();
    
    void on_packgePushButton_clicked();
    
signals:
    void initPathParams(QString sourcePicPath,QString resultPicPath,QString packgePath);
    
private:
    Ui::ExpDlg *ui;
    
    QString sourcepicPath;
    QString resultpicPath;
    QString packgePath;
};

#endif // EXPDLG_H
