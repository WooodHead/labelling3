#ifndef ADVANCESEARCHDLG_H
#define ADVANCESEARCHDLG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
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
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QModelIndex>

#include <vector>

#include "propertynamedlg.h"
#include "Global.h"
#include "expdlg.h"
#include "impdlg.h"

namespace Ui {
class AdvanceSearchDlg;
}

class AdvanceSearchDlg : public QDialog
{
    Q_OBJECT

public:
     AdvanceSearchDlg(QWidget *parent = 0,bool flag = false);
    ~AdvanceSearchDlg();

private:
     bool inornot(QStringList List,QString elem);

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

     void on_sampledateChkBox_clicked();

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

     void on_monitorpartnameChkBox_clicked();

     void on_oilsampleidChkBox_clicked();

     void on_oilsampleidCbBox_currentIndexChanged(int index);

     void on_samplesituationChkBox_clicked();

     void on_sampleidChkBox_clicked();

     void on_samplemethodChkBox_clicked();

     void on_sampledepartidChkBox_clicked();

     void on_sampledepartnameChkBox_clicked();

     void on_sendstuffChkBox_clicked();

     void on_sendstuffLineEdit_textChanged(const QString &arg1);

     void on_senddateDateEdit_dateChanged(const QDate &date);

     void on_sendtimeChkBox_clicked();

     void on_sampleidCbBox_currentIndexChanged(int index);

     void on_samplesituationCbBox_currentIndexChanged(int index);

     void on_samplemethodCbBox_currentIndexChanged(int index);

     void on_sampledepartidCbBox_currentIndexChanged(int index);

     void on_sampledepartnameCbBox_currentIndexChanged(int index);

     void on_monitorpartidCbBox_currentIndexChanged(int index);

     void on_monitorpartnameCbBox_currentIndexChanged(int index);

     void on_oia_sendstuffChkBox_clicked();

     void on_oia_sendstuffLineEdit_textChanged(const QString &arg1);

     void on_oia_senddepartChkBox_clicked();

     void on_oia_senddepartCbBox_currentIndexChanged(int index);

     void on_oia_receivestuffChkBox_clicked();

     void on_oia_receivestuffLineEdit_textChanged(const QString &arg1);

     void on_oia_receivedateChkBox_clicked();

     void on_oia_analyzedepartnameChkBox_clicked();

     void on_oia_analyzedepartnameCbBox_currentIndexChanged(int index);

     void on_oia_contaminationanalyzestuffChkBox_clicked();

     void on_oia_contaminationanalyzestuffChkLineEdit_textChanged(const QString &arg1);

     void on_oia_contaminationanalyzemethodChkBox_clicked();

     void on_oia_contaminationanalyzemethodCbBox_currentIndexChanged(int index);

     void on_oia_contaminationanalyzeequipmentChkBox_clicked();

     void on_oia_contaminationanalyzeequipmentCbBox_currentIndexChanged(int index);

     void on_oia_contaminationanalyzedateChkBox_clicked();

     void on_oia_contaminationanalyzedateDateEdit_dateChanged(const QDate &date);

     void on_oia_receivedateDateEdit_dateChanged(const QDate &date);

     void on_oia_contaminationanalyzereportidChkBox_clicked();

     void on_oia_contaminationanalyzereportidCbBox_currentIndexChanged(int index);

     void on_oia_spectroscopystuffChkBox_clicked();

     void on_oia_spectroscopystuffLineEdit_textChanged(const QString &arg1);

     void on_oia_spectroscopymethodChkBox_clicked();

     void on_oia_spectroscopymethodCbBo_currentIndexChanged(int index);

     void on_oia_spectroscopyequipmentChkBox_clicked();

     void on_oia_spectroscopyequipmentCbBox_currentIndexChanged(int index);

     void on_oia_spectroscopydateChkBox_clicked();

     void on_oia_spectroscopydateDateEdit_dateChanged(const QDate &date);

     void on_oia_spectroscopyreportidChkBox_clicked();

     void on_oia_spectroscopyreportidCbBox_currentIndexChanged(int index);

     void on_oia_physicochemicalstuffChkBox_clicked();

     void on_oia_physicochemicalstuffLineEdit_textChanged(const QString &arg1);

     void on_oia_physicochemicalmethodChkBox_clicked();

     void on_oia_physicochemicalmethodCbBox_currentIndexChanged(int index);

     void on_oia_physicochemicalequipmentChkBox_clicked();

     void on_oia_physicochemicalequipmentCbBox_currentIndexChanged(int index);

     void on_oia_physicochemicaldateChkBox_clicked();

     void on_oia_physicochemicaldateDateEdit_dateChanged(const QDate &date);

     void on_oia_physicochemicalreportidChkBox_clicked();

     void on_oia_physicochemicalreportidCbBox_currentIndexChanged(int index);

     void on_oia_ferrographystuffChkBox_clicked();

     void on_oia_ferrographystuffLineEdit_textChanged(const QString &arg1);

     void on_oia_ferrographymethodChkBox_clicked();

     void on_oia_ferrographymethodCbBox_currentIndexChanged(int index);

     void on_oia_ferrographyequipmentChkBox_clicked();

     void on_oia_ferrographyequipmentCbBox_currentIndexChanged(int index);

     void on_oia_ferrographydateChkBox_clicked();

     void on_oia_ferrographydateDateEdit_dateChanged(const QDate &date);

     void on_oia_ferrographyreportidChkBox_clicked();

     void on_oia_ferrographyreportidCbBox_currentIndexChanged(int index);

     void on_abm_abrasiveidChkBox_clicked();

     void on_abm_abrasiveidCbBox_currentIndexChanged(int index);

     void on_abm_abrasiveshapeChkBox_clicked();

     void on_abm_abrasiveshapeCbBox_currentIndexChanged(int index);

     void on_abm_abrasivepositionChkBox_clicked();

     void on_abm_abrasivepositionCbBox_currentIndexChanged(int index);

     void on_abm_abrasivematerialChkBox_clicked();

     void on_abm_abrasivematerialCbBox_currentIndexChanged(int index);

     void on_abm_abrasivecolorChkBox_clicked();

     void on_abm_abrasivecolorCbBox_currentIndexChanged(int index);

     void on_abm_abrasivesperimeterChkBox_clicked();

     void on_abm_abrasivesperimeterLineEdit_textChanged(const QString &arg1);

     void on_abm_abrasivesizeChkBox_clicked();

     void on_abm_abrasivesizeLineEdit_textChanged(const QString &arg1);

     void on_abm_abrasivemarkstuffChkBox_clicked();

     void on_abm_abrasivemarkstuffLineEdit_textChanged(const QString &arg1);

     void on_abm_abrasivetypicalChkBox_clicked();

     void on_abm_abrasivetypicalLineEdit_textChanged(const QString &arg1);

     void on_abm_abrasivemechanismtypeChkBox__clicked();

     void on_abm_abrasivemechanismtypeLineEdit_textChanged(const QString &arg1);

     void on_abm_abrasivedamagetypeChkBox_clicked();

     void on_abm_abrasivedamagetypeCbBox_currentIndexChanged(int index);

     void on_abm_abrasiveweartypeChkBox_clicked();

     void on_abm_abrasiveweartypeCbBox_currentIndexChanged(int index);

     void on_abm_abrasivesurfacetexturetypeChkBox_clicked();

     void on_abm_abrasivesurfacetexturetypeCbBox_currentIndexChanged(int index);

     void on_abm_ferrographysheetidChkBox_clicked();

     void on_abm_ferrographysheetidCbBox_currentIndexChanged(int index);

     void on_abm_ferrographypicidChkBox_clicked();

     void on_abm_ferrographypicidCbBox_currentIndexChanged(int index);

     void on_abm_ferrographyreportidChkBox_clicked();

     void on_abm_ferrographyreportidCbBox_currentIndexChanged(int index);

     void on_fegp_ferrographypicidChkBox_clicked();

     void on_fegp_ferrographypicidCbBox_currentIndexChanged(int index);

     void on_fegp_imagesymbolChkBox_clicked();

     void on_fegp_imagesymbolCbBox_currentIndexChanged(int index);

     void on_fegp_ferrographyreportidChkBox_clicked();

     void on_fegp_ferrographyreportidCbBox_currentIndexChanged(int index);

     void on_fegp_imageacquisitionareaChkBox_clicked();

     void on_fegp_imageacquisitionareaCbBox_currentIndexChanged(int index);

     void on_fegp_microscopictypeChkBox_clicked();

     void on_fegp_microscopictypeCbBox_currentIndexChanged(int index);

     void on_fegp_lightsourcetypeChkBox_clicked();

     void on_fegp_lightsourcetypeCbBox_currentIndexChanged(int index);

     void on_fegp_magnificationChkBox_clicked();

     void on_fegp_magnificationCbBox_currentIndexChanged(int index);

     void on_fegp_imageacquisitiontypeChkBox_clicked();

     void on_fegp_imageacquisitiontypeCbBox_currentIndexChanged(int index);

     void on_fegp_imageacquisitionstuffChkBox_clicked();

     void on_fegp_imageacquisitionstuffLineEdit_textChanged(const QString &arg1);

     void on_fegp_imagerecognitioninfoanalysisChkBox_clicked();

     void on_fegp_imagerecognitioninfoanalysisLineEdit_textChanged(const QString &arg1);

     void on_feg_ferrographysheetidChkBox_clicked();

     void on_feg_ferrographysheetidCbBox_currentIndexChanged(int index);

     void on_feg_ferrographymakestuffChkBox_clicked();

     void on_feg_ferrographymakestuffLineEdit_textChanged(const QString &arg1);

     void on_feg_ferrographymakemethodChkBox_clicked();

     void on_feg_ferrographymakemethodCbBox_currentIndexChanged(int index);

     void on_feg_ferrographyanalyzertypeChkBox_clicked();

     void on_feg_ferrographyanalyzertypeCbBox_currentIndexChanged(int index);

     void on_feg_ferrographymakeoilconsumptionChkBox_clicked();

     void on_feg_ferrographymakeoilconsumptionLineEdit_textChanged(const QString &arg1);

private:
     QString generateSql(QMap<QString,QString> conditionMap,QString tableName);
     void setModelHeaderData(QString tablename);
     void createTableView();
     void createTableNames();
     void createListWidget();
     void initCbBox();
     void initpropertylistName();
     void resetConditions();
     void reloadConditions(QString propertyname);
     void query();
     bool copyFiles(QString fromDir,QString toDir,bool convertIfExits = false);
     bool copyFiles(QString fromDir,QString toDir,QStringList filenames,bool convertIfExist = false);
     bool importDB(const QString &path);
     bool exportDB(const QStringList &tablenameList,const QString &path);
     bool importDB(const QSqlQueryModel &model);

     bool deletefromtable(QStringList idList,QString tablename, QStringList path = QStringList());

private slots:
     void useproperty();
     void deleteproperty();
     void renameprtperty();

     void deletedata();

//     void on_modifyButton_clicked();

     void setExpPath(QString sourcePicPath, QString resultPicPath, QString maskPicPath, QString packgePath);
     void setImpPath(QString packgePath);

     void on_deletepropertyButton_clicked();

     void on_deleteDataButton_clicked();

     void on_delAllButton_clicked();

signals:
     void showqueryThumbnails(QStringList list);
     void flush();
     void clearAll();
     void flushLeftTree();
     void removeImageSignal(QString fegid);

private:
    Ui::AdvanceSearchDlg *ui;

    QSqlDatabase db;
    ProPertyNameDlg *ppnDlg;
    QString propertyName;

    QString _expSourcePicPath;
    QString _expResultPicPath;
    QString _expMaskPicPath;
    QString _expPackgePath;

    QString _impPackgePath;

    bool deleteflag;

    QStringList ferrographypicpathList;

    QStringList ferrographysheetidList;
    QStringList ferrographypicidList;
    QStringList planeidList;
    QStringList movepartidList;
    QStringList oilsampleidList;

    QMap<QString,QString> tableNames;
    QSqlQueryModel *_eqmInfoModel;
    QSqlQueryModel *_abmInfoModel;
//    QSqlTableModel *_abmInfoModel;
    QSqlQueryModel *_fegInfoModel;
    QSqlQueryModel *_fegpInfoModel;
    QSqlQueryModel *_mpInfoModel;
    QSqlQueryModel *_mprInfoModel;
    QSqlQueryModel *_oiaInfoModel;
    QSqlQueryModel *_oisInfoModel;

    QSqlQueryModel *propertymodel;
    QAction *usepropertyAction;
    QAction *deletepropertyAction;
    QAction *renameprtpertyAction;

//    QAction *deletedataAction;

    QAction *eqmdelAction;
    QAction *mpdelAction;
    QAction *mprdelAction;
    QAction *oisdelAction;
    QAction *oiadelAction;
    QAction *fegdelAction;
    QAction *fegpdelAction;
    QAction *abmdelAction;

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
