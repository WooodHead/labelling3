#ifndef ADVANCESEARCHDLG_H
#define ADVANCESEARCHDLG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QSqlRecord>
#include <QSqlField>
#include <QFileDialog>

#include "Connection.h"
#include "propertynamedlg.h"
#include "Global.h"

namespace Ui {
class AdvanceSearchDlg;
}

class AdvanceSearchDlg : public QDialog
{
    Q_OBJECT
    
public:
     AdvanceSearchDlg(QWidget *parent = 0);
    ~AdvanceSearchDlg();
    
private slots:
     void setpropertyName(QString propertyname);
     
     void on_queryBtn_clicked();

     void on_PlaneIdChkBox_clicked();
     
     void on_planeidCbBox_currentIndexChanged(int index);
     
     void on_planeTypeChkBox_clicked();
     
     void on_planeTypeCbBox_currentIndexChanged(int index);
     
     void on_departIdChkBox_clicked();  
     
     void on_departIdCbBox_currentIndexChanged(int index);
     
     void on_runHourChkBox_clicked();
     
     void on_runHourCbBox_currentIndexChanged(int index);
     
     void on_runStageChkBox_clicked();
     
     void on_runStageCbBox_currentIndexChanged(int index);
     
     void on_repairTimeChkBox_clicked();
     
     void on_repairTimeCbBox_currentIndexChanged(int index);
     
     void on_addtoBtn_clicked();
     
     void on_movepartIdChkBox_clicked();
     
     void on_movepartIdCbBox_currentIndexChanged(int index);
     
     void on_movepartNameChkBox_clicked();
     
     void on_movepartNameCbBox_currentIndexChanged(int index);
     
     void on_movepartTypeChkBox_clicked();
     
     void on_movepartTypeCbBox_currentIndexChanged(int index);
     
     void on_partrunHourChkBox_clicked();
     
     void on_partrunHourCbBox_currentIndexChanged(int index);
     
     void on_movepartStartDateEdit_dateChanged(const QDate &date);
     
     void on_movepartStartDataChkBox_clicked();
     
     void on_movepartEndDateEdit_dateChanged(const QDate &date);
     
     void on_movepartEndDataChkBox_clicked();
     
     void on_exportBtn_clicked();
     
     void on_importBtn_clicked();
     
     void on_movepartRepairIdChkBox_clicked();
     
     void on_movepartRepairIdCbBox_currentIndexChanged(int index);
     
     void on_repairDepartChkBox_clicked();
     
     void on_repairDepartCbBox_currentIndexChanged(int index);
     
     void on_repairrepairTimeChkBox_clicked();
     
     void on_repairrepairTimeCbBox_currentIndexChanged(int index);
     
     void on_repairDateDateEdit_dateChanged(const QDate &date);
     
     void on_repairDateChkBox_clicked();
     
     void on_samplestuffChkBox_clicked();
     
     void on_samplestuffLineEdit_textChanged(const QString &arg1);
     
     void on_samplevolumeChkBox_clicked();
     
     void on_samplevolumeLineEdit_textChanged(const QString &arg1);
     
     void on_sampledataChkBox_clicked();
     
     void on_sampledateDateEdit_dateChanged(const QDate &date);
     
     void on_sampletimeChkBox_clicked();
     
     void on_sampletimeTimeEdit_timeChanged(const QTime &date);
     
     void on_senddateChkBox_clicked();
     
     void on_sendtimeTimeEdit_timeChanged(const QTime &date);
     
     void on_oiladditionChkBox_clicked();
     
     void on_oiladditionLineEdit_textChanged(const QString &arg1);
     
     void on_oilworktimeChkBox_clicked();
     
     void on_oilworktimeLineEdit_textChanged(const QString &arg1);
     
     void on_monitorpartidChkBox_clicked();
     
     void on_monitorpartidLineEdit_textChanged(const QString &arg1);
     
     void on_monitorpartnameChkBox_clicked();
     
     void on_monitorpartnameLineEdit_textChanged(const QString &arg1);
     
     void on_oilsampleidChkBox_clicked();
     
     void on_oilsampleidLineEdit_textChanged(const QString &arg1);
     
     void on_samplesituationChkBox_clicked();

     void on_samplesituationLineEdit_textChanged(const QString &arg1);

     void on_sampleidChkBox_clicked();

     void on_sampleidLineEdit_textChanged(const QString &arg1);

     void on_samplemethodChkBox_clicked();

     void on_samplemethodLineEdit_textChanged(const QString &arg1);

     void on_sampledepartidChkBox_clicked();

     void on_sampledepartidlineEdit_textChanged(const QString &arg1);

     void on_sampledepartnameChkBox_clicked();

     void on_sampledepartnameLineEdit_textChanged(const QString &arg1);

     void on_sendstuffChkBox_clicked();

     void on_sendstuffLineEdit_textChanged(const QString &arg1);

     void on_senddateDateEdit_dateChanged(const QDate &date);

     void on_sendtimeChkBox_clicked();

private:
     QString generateSql(QMap<QString,QString> conditionMap,QStringList conditionField,QString tableName);
     void setModelHeaderData(QString tablename);
     void createTableView();
     void createTableNames();
     void createListWidget();
     void initCbBox();
     void initpropertylistName();
     void resetConditions();
     void query();
     bool copyFiles(QString fromDir,QString toDir,bool convertIfExits = false);
     bool copyFiles(QString fromDir,QString toDir,QStringList filenames,bool convertIfExist = false);
     bool importDB(const QString &path);
     bool exportDB(const QSqlQueryModel *model,const QString &tablename,const QString &path);
     bool importDB(const QSqlQueryModel &model);

private slots:
     void useproperty();
     void manageproperty();

private:
    Ui::AdvanceSearchDlg *ui;
    
    QSqlDatabase db;
    ProPertyNameDlg *ppnDlg;
    QString propertyName;
    
    QMap<QString,QString> tableNames;
    QSqlQueryModel *_eqmInfoModel;
    QSqlQueryModel *_abmInfoModel;
    QSqlQueryModel *_fegInfoModel;
    QSqlQueryModel *_fegpInfoModel;
    QSqlQueryModel *_mpInfoModel;
    QSqlQueryModel *_mprInfoModel;
    QSqlQueryModel *_oiaInfoModel;
    QSqlQueryModel *_oisInfoModel;

    QSqlQueryModel *propertymodel;
    QAction *usepropertyAction;
    QAction *managepropertyAction;

    QStringList _eqmCdtField;          // zhuang bei xin xi biao condition Field
    QStringList _abmCdtField;          // mo li biao ji xin xi biao condition Field
    QStringList _fegCdtField;          // tie pu zhi pu xin xi biao condition Field
    QStringList _fegpCdtField;         // tie pu tu pian cai ji xin xi biao condition Field
    QStringList _mpCdtField;           // dong bu jian xin xi biao condtion Field
    QStringList _mprCdtField;          // dong bu jian wei xiu xin xi biao condition Field
    QStringList _oiaCdtField;          // you yang fen xi xin xi biao condition Field
    QStringList _oisCdtField;          // you yang xin xi biao condition Field

    QMap<QString,QString> _eqmCdtMap;          // zhuang bei xin xi biao condition Map
    QMap<QString,QString> _abmCdtMap;          // mo li biao ji xin xi biao condition Map
    QMap<QString,QString> _fegCdtMap;          // tie pu zhi pu xin xi biao condition Map
    QMap<QString,QString> _fegpCdtMap;         // tie pu tu pian cai ji xin xi biao condition Map
    QMap<QString,QString> _mpCdtMap;           // dong bu jian xin xi biao condtion Map
    QMap<QString,QString> _mprCdtMap;          // dong bu jian wei xiu xin xi biao condition Map
    QMap<QString,QString> _oiaCdtMap;          // you yang fen xi xin xi biao condition Map
    QMap<QString,QString> _oisCdtMap;          // you yang xin xi biao condition Map
    
    enum{
        eqm_planeid,
        eqm_planetype,
        eqm_deoartid,
        eqm_runhour,
        eqm_runstage,
        eqm_repairtime
    };
    
    enum{
        mp_movepartid,
        mp_moveparttype,
        mp_movepartname,
        mp_runhour,
        mp_runstage,
        mp_planeid,
        mp_planetype,
        mp_startdat,
        mp_enddate
    };
    
    enum{
        mpr_movepartrepairid,
        mpr_movepartid,
        mpr_moveparttype,
        mpr_repairreason,
        mpr_repairdate,
        mpr_repairtime,
        mpr_repairdepart,
        mpr_repaircontent,
        mpr_repaircondition
    };
    
    enum{
        feg_ferrographysheetid,
        feg_ferrographyreportid,
        feg_oilsampleid,
        feg_ferrographyanalyertype,
        feg_ferrographymakeoilconsumption,
        feg_ferrographymakemethod,
        feg_ferrographymakestuff
    };
    
    enum{
        fegp_ferrographypicid,
        fegp_ferrographysheetid,
        fegp_ferrographyreportid,
        fegp_microscopictype,
        fegp_imageacquisitiontype,
        fegp_lightsourcetype,
        fegp_magnification,
        fegp_imageacquisitionarea,
        fegp_imageacquisitionstuff,
        fegp_ferrographypicpath,
        fegp_imagerecognitioninfoanalysis,
        fegp_imagesymbol
    };
    
    enum{
        oia_oilsampleid,
        oia_analyzedepartname,
        oia_senddepart,
        oia_sendreason,
        oia_sendstuff,
        oia_receivedate,
        oia_receivestuff,
        oia_contaminationanalyzemethod,
        oia_contaminationanalyzestuff,
        oia_contaminationanalyzedate,
        oia_contaminationanalyzeequipment,
        oia_contaminationanalyzereportid,
        oia_spectroscopymethod,
        oia_spectroscopystuff,
        oia_spectroscopydate,
        oia_spectroscopyequipment,
        oia_spectroscopyreportid,
        oia_ferrographymethod,
        oia_ferrographystuff,
        oia_ferrographydate,
        oia_ferrographyequipment,
        oia_ferrographyreportid,
        oia_physicochemicalmethod,
        oia_physicochemicalstuff,
        oia_physicochemicaldate,
        oia_physicochemicalequipment,
        oia_physicochemicalreportid
    };
    
    enum{
        ois_oilsampleid,
        ois_sampledepartid,
        ois_planetype,
        ois_planeid,
        ois_monitorpartname,
        ois_monitorpartid,
        ois_sampleid,
        ois_oilworktime,
        ois_oiladdition,
        ois_samplereason,
        ois_sampledepartname,
        ois_samplestuff,
        ois_sampledate,
        ois_sampletime,
        ois_samplesituation,
        ois_samplemethod,
        ois_samplevolume,
        ois_sampleinstruction,
        ois_sendstuff,
        ois_senddate,
        ois_sendtime
    };
    
    enum{
        abm_abrasiveid,
        abm_ferrographypicid,
        abm_ferrographysheetid,
        abm_ferrographyreportid,
        abm_abrasivemarkstuff,
        abm_abrasivepicpath,
        abm_abrasivematerial,
        abm_abrasiveposition,
        abm_abrasivesize,
        abm_abrasivesperimeter,
        abm_abrasiveshape,
        abm_abrasivecolor,
        abm_abrasivesurfacetexturetype,
        abm_abrasiveweartype,
        abm_abrasivedamagetype,
        abm_abrasivemechanismtype,
        abm_abrasivefaultinformationreflection,
        abm_abrasivetypical
    };
};

#endif // ADVANCESEARCHDLG_H
