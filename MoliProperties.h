#ifndef MOLIPROPERTIES_H
#define MOLIPROPERTIES_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlRecord>
#include <QBuffer>

#include "ImageOperate.h"

namespace Ui {
class MoliProperties;
}

class MoliProperties : public QDialog
{
    Q_OBJECT
    
public:
    explicit MoliProperties(QWidget *parent = 0);
    ~MoliProperties();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on__comboBoxMoliGivenInfo_editTextChanged(const QString &arg1);

    void on__comboBoxMoliID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliImageID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliPianID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliReportID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliGuy_editTextChanged(const QString &arg1);

    void on__editMoliPath_textChanged(const QString &arg1);

    void on__comboBoxMoliProperty_editTextChanged(const QString &arg1);

    void on__comboBoxMoliPosition_editTextChanged(const QString &arg1);

    void on__editMoliSize_textChanged(const QString &arg1);

    void on__editMoliLength_textChanged(const QString &arg1);

    void on__comboBoxMoliShape_editTextChanged(const QString &arg1);

    void on__comboBoxMoliColor_editTextChanged(const QString &arg1);

    void on__comboBoxMoliSurface_editTextChanged(const QString &arg1);

    void on__comboBoxMoliErodeType_editTextChanged(const QString &arg1);

    void on__comboBoxMoliErodePart_editTextChanged(const QString &arg1);

    void on__comboBoxMoliErodeReason_editTextChanged(const QString &arg1);

    void on__comboBoxMoliTypical_editTextChanged(const QString &arg1);

signals:
    void next();
    void saveImages();
    void flushBottom();
    void flushLeft(QString path, QString label);

private:
    Ui::MoliProperties *ui;

    QSqlTableModel* _model;
    QSqlDatabase _db;

    QString _originalImagePath;
    QImage _result;
    QImage _result2;
    QImage _mask;

    bool _bDirty;

    QStringList getItems(QSqlTableModel *model, QString fieldName);
    void computeMoliInfo(double imageScale, double& perimeter, double& maxLength, double& maxWidth);


public:
    void showDlg(QString imagePath, const QImage& result, const QImage& result2, const QImage& mask, const double imageScale);
};

#endif // MOLIPROPERTIES_H
