#ifndef CHECKPROPERTIES_H
#define CHECKPROPERTIES_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlRecord>
#include <QBuffer>

#include "ImageOperate.h"
#include "Global.h"

namespace Ui {
class CheckProperties;
}

class CheckProperties : public QDialog
{
    Q_OBJECT
    
public:
    explicit CheckProperties(QWidget *parent = 0);
    ~CheckProperties();
    
private:
    Ui::CheckProperties *ui;

    QSqlTableModel* _model;
    QSqlDatabase _db;

    QString _originalImagePath;
    QImage _result;
    QImage _result2;
    QImage _mask;

    QStringList getItems(QSqlTableModel *model, QString fieldName);
    void computeMoliInfo(double imageScale, double& perimeter, double& maxLength, double& maxWidth);

public:
    void showDlg(QString imagePath, const QImage* result, const QImage* result2, const QImage* mask, const double imageScale);
private slots:
    void on_btnClose_clicked();
};

#endif // CHECKPROPERTIES_H
