#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

#define TABLE_N 8

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
    void on__buttonClose_clicked();

    void on__buttonSave_clicked();

    void on__comboBoxEquipPlaneID_textChanged(const QString &arg1);

    void on__comboBoxEquipPlaneType_textChanged(const QString &arg1);

    void on__comboBoxEquipUnitID_textChanged(const QString &arg1);

    void on__comboBoxMovepartID_textChanged(const QString &arg1);

    void on__comboBoxOilSampleID_textChanged(const QString &arg1);

    void on__comboBoxMentalID_textChanged(const QString &arg1);

    void on__comboBoxMentalSampleImageID_textChanged(const QString &arg1);

private:
    Ui::ImageProperties *ui;

    void load();

    QSqlTableModel* _models[TABLE_N];

    QStringList getItems(QSqlTableModel* model, int col);

    bool isValid();
    QString _filename;

public:
    void showDlg(QString filename);
};

#endif // IMAGEPROPERTIES_H