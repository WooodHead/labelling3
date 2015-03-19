#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

#define TABLE_N 7

namespace Ui {
class ImageProperties;
}

class ImageProperties : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProperties(QWidget *parent = 0);
    ~ImageProperties();
    
private slots:
    void on__buttonSave_clicked();

    void on__comboBoxEquipPlaneID_textChanged(const QString &arg1);

    void on__comboBoxEquipPlaneType_textChanged(const QString &arg1);

    void on__comboBoxEquipUnitID_textChanged(const QString &arg1);

    void on__comboBoxMovepartID_textChanged(const QString &arg1);

    void on__comboBoxOilSampleID_textChanged(const QString &arg1);

    void on__comboBoxMentalID_textChanged(const QString &arg1);

    void on__comboBoxMentalSampleImageID_textChanged(const QString &arg1);

    void on__buttonNext_clicked();

    void on__tabWidget_currentChanged(int index);

    void on__buttonCancel_clicked();

    void closeEvent(QCloseEvent *);

    void on__comboBoxEquipPlaneID_editTextChanged(const QString &arg1);

    void on__comboBoxEquipPlaneType_editTextChanged(const QString &arg1);

    void on__comboBoxEquipUnitID_editTextChanged(const QString &arg1);

    void on__editEquipHours_textChanged(const QString &arg1);

    void on__comboBoxEquipRuntime_editTextChanged(const QString &arg1);

    void on__editServiceNumber_textChanged(const QString &arg1);

    void on__dateEditOilSampleSampleDate_dateChanged(const QDate &date);

    void on__timeEditOilSampleSampleTime_timeChanged(const QTime &date);

    void on__comboBoxOilSamplePlaneID_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartServiceMovepartID_editTextChanged(const QString &arg1);

    void on__comboBoxMentalOilSampleID_editTextChanged(const QString &arg1);

private:
    Ui::ImageProperties *ui;

    void load();

    QSqlTableModel* _models[TABLE_N];

    QStringList getItems(QSqlTableModel* model, int col);
    QStringList getItems(QSqlTableModel* model, QString fieldName);

    bool isValid();
    bool isValid(int index);
    QString _filename;

    QString _originalImagePath;
    QString _resultImagePath;
    QString _maskImagePath;

    bool _bSaved[TABLE_N];
    bool _bCommited;

    QString generateTiepupianID();

signals:
    void flush();
    void removeImage(QString filename);

public:
    void showDlg(QString filename);
    void commitLabelledResult(QString resultImagePath, QString maskImagePath);
};

#endif // IMAGEPROPERTIES_H
