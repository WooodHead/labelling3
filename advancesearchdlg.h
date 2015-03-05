#ifndef ADVANCESEARCHDLG_H
#define ADVANCESEARCHDLG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QStringList>
#include <QMap>
#include <QDebug>

#include "connection.h"

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
     
private:
     QString generateSql(QMap<QString,QString> conditionMap,QStringList conditionField,QString tableName);
     void setModelHeaderData(QString tablename);
     void createTableView();
     void createTableNames();
     void createListWidget();
     void initCbBox();
     void query();

private:
    Ui::AdvanceSearchDlg *ui;
    
    QSqlDatabase db;
    
    QMap<QString,QString> tableNames;
    QSqlQueryModel *_eqmInfoModel;
    QSqlQueryModel *_abmInfoModel;
    QSqlQueryModel *_fegInfoModel;
    QSqlQueryModel *_fegpInfoModel;
    QSqlQueryModel *_mpInfoModel;
    QSqlQueryModel *_mprInfoModel;
    QSqlQueryModel *_oiaInfoModel;
    QSqlQueryModel *_oisInfoModel;
    

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

};

#endif // ADVANCESEARCHDLG_H
