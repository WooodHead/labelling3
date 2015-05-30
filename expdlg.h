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
    ExpDlg(QWidget *parent,QString _sourcePicPath,QString resultPath, QString maskPicPath);
    ~ExpDlg();
    
private slots:
    void on_CancelPushButton_clicked();
    
    void on_OKPushButton_clicked();
    
    void on_picPushButton_clicked();
    
    void on_resultPushButton_clicked();
    
    void on_packgePushButton_clicked();
    
    void on_maskPushButton_clicked();

signals:
    void initPathParams(QString sourcePicPath,QString resultPicPath, QString maskPicPath, QString _packgePath);
    
private:
    Ui::ExpDlg *ui;
    
    QString _sourcePicPath;
    QString _resultPicPath;
    QString _maskPicPath;
    QString _packgePath;
};

#endif // EXPDLG_H
