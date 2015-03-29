#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QCloseEvent>

#include "def.h"

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

    void on__comboBoxMovepartID_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartName_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartType_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartMohe_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartPlaneID_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartPlaneType_editTextChanged(const QString &arg1);

    void on__dateEditMovepartBegin_dateChanged(const QDate &date);

    void on__dateEditMovepartEnd_dateChanged(const QDate &date);

    void on__editMovepartHours_textChanged(const QString &arg1);

    void on__comboBoxMovepartServiceID_editTextChanged(const QString &arg1);

    void on__comboBoxMovepartServiceMovepartType_editTextChanged(const QString &arg1);

    void on__dateEditMovepartServiceDate_dateChanged(const QDate &date);

    void on__editMovepartServiceNumber_textChanged(const QString &arg1);

    void on__comboBoxMovepartServiceUnit_editTextChanged(const QString &arg1);

    void on__editMovepartServiceReson_textChanged(const QString &arg1);

    void on__editMovepartServiceContent_textChanged(const QString &arg1);

    void on__editMovepartServiceInfo_textChanged(const QString &arg1);

    void on__comboBoxOilSamplePlaneType_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleMonitorPart_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleMonitorPartID_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleSamplePointID_editTextChanged(const QString &arg1);

    void on__editOilSampleHours_textChanged(const QString &arg1);

    void on__editOilSampleMount_textChanged(const QString &arg1);

    void on__editOilSampleReason_textChanged(const QString &arg1);

    void on__comboBoxOilSampleSampleUnit_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleSampleGuy_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleID_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleUnitID_editTextChanged(const QString &arg1);

    void on__comboBoxOilSampleSampleMethod_editTextChanged(const QString &arg1);

    void on__editOilSampleSampleMount_textChanged(const QString &arg1);

    void on__editOilSampleInfo_textChanged(const QString &arg1);

    void on__comboBoxOilSampleSendGuy_editTextChanged(const QString &arg1);

    void on__dateEditOilSampleSendDate_dateChanged(const QDate &date);

    void on__timeEditOilSampleSendTime_timeChanged(const QTime &date);

    void on__comboBoxOilSampleSituation_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeOilSampleID_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeUnitName_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeSendUnit_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeSendGuy_editTextChanged(const QString &arg1);

    void on__editOilAnalyzeReason_textChanged(const QString &arg1);

    void on__dateEditOilAnalyzeReceiveDate_dateChanged(const QDate &date);

    void on__comboBoxOilAnalyzeReceiveGuy_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzePollutionLevelMethod_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzePollutionLevelGuy_editTextChanged(const QString &arg1);

    void on__dateEditOilAnalyzePollutionDate_dateChanged(const QDate &date);

    void on__comboBoxOilAnalyzePolluteLevelEquip_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzePolluteReportID_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeLightMethod_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeLightGuy_editTextChanged(const QString &arg1);

    void on__dateEditOilAnalyzeLightDate_dateChanged(const QDate &date);

    void on__comboBoxOilAnalyzeLightEquip_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeLightEquipID_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeMentalMethod_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeMentalGuy_editTextChanged(const QString &arg1);

    void on__dateEditOilAnalyzeMentalDate_dateChanged(const QDate &date);

    void on__comboBoxOilAnalyzeMentalEquip_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeMentalEquipID_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeLihuaMethod_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeLihuaGuy_editTextChanged(const QString &arg1);

    void on__dateEditOilAnalyzeLihuaDate_dateChanged(const QDate &date);

    void on__comboBoxOilAnalyzeLihuaEquip_editTextChanged(const QString &arg1);

    void on__comboBoxOilAnalyzeLihuaEquipID_editTextChanged(const QString &arg1);

    void on__comboBoxMentalInstrumentType_editTextChanged(const QString &arg1);

    void on__comboBoxMentalID_editTextChanged(const QString &arg1);

    void on__comboBoxMentalReportID_editTextChanged(const QString &arg1);

    void on__editMentalMount_textChanged(const QString &arg1);

    void on__comboBoxMentalMethod_editTextChanged(const QString &arg1);

    void on__comboBoxMentalGuy_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleImageID_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleID_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleReportID_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleMicroType_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleSamplerType_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleLightType_editTextChanged(const QString &arg1);

    void on__editMentalSampleEnlarger_textChanged(const QString &arg1);

    void on__comboBoxMentalSampleArea_editTextChanged(const QString &arg1);

    void on__comboBoxMentalSampleGuy_editTextChanged(const QString &arg1);

    void on__editMentalSampleAnalysis_textChanged(const QString &arg1);

    void on__comboBoxMoliID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliImageID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliPianID_editTextChanged(const QString &arg1);

    void on__comboBoxMoliReportID_editTextChanged(const QString &arg1);

    void toChangeId();

private:
    Ui::ImageProperties *ui;

    QSqlTableModel* _models[TABLE_N];
    QString _filename;

    QString _originalImagePath;
    QString _resultImagePath;
    QString _maskImagePath;

    bool _bSaved[TABLE_N];
    bool _bCommited;

    void load();

    QStringList getItems(QSqlTableModel* model, int col);
    QStringList getItems(QSqlTableModel* model, QString fieldName);

    bool isValid();
    bool isValid(int index);

    QString generateTiepupianID();
    QString generateOilSampleID();
    QString generateTieputupianID();

    QString copyOrgImage(QString id, QString org);

    bool isExistID(QString id);
    void setupUiAgain();

signals:
    void flush();
    void closeViewer();
    void syncFilePathStr(QString fName);

public:
    void showDlg(QString filename);
    void commitLabelledResult(QString resultImagePath, QString maskImagePath);
};

#endif // IMAGEPROPERTIES_H
